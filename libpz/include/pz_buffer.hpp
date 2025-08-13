#ifndef PZ_BUFFER_HPP
#define PZ_BUFFER_HPP

#include <pz_cxx_std.hpp>
#include <pz_error.hpp>
#include <pz_std.hpp>
#include <pz_types.hpp>

class PzStd::PzCore;

namespace PzStd {
class PzBuffer;
enum class PzBufferType;
}; // namespace PzStd

using PzErr = PzError::PzErrorType;
using PzBufferType = PzStd::PzBufferType;

/**
 * @name Storage flag enum class
 * @brief Storage optimization flags for different text processing patterns.
 * @details
 * These flags determine which data structures are actively maintained
 * during text processing operations. Multiple structures can be used
 * simultaneously based on access patterns.
 */
enum class PzStd::PzBufferType : st32 {
  PZ_BUF_TYPE_SEQUENTIAL =
      1, /**< Use vector for linear access (O(1) append, cache-friendly). */
  PZ_BUF_TYPE_FREQUENCY, /**< Track word frequencies (O(1) lookup via
                            `unordered_map`). */
  PZ_BUF_TYPE_UNIQUE,    /**< Store unique words only (O(1) checking via
                            `unordered_set`). */
  PZ_BUF_TYPE_INDEXED, /**< Create word position index (O(log n) via `map`). */
  PZ_BUF_TYPE_SORTED   /**< Maintain sorted order (O(log n) via `set`). */
};

class PzStd::PzBuffer {
private:
  // Create buffer instance via smart pointer.
  static std::unique_ptr<PzBuffer>
  create(PzBufferType flag =
             PzBufferType::PZ_BUF_TYPE_SEQUENTIAL); // Storage optimization
                                                    // (default: SEQUENTIAL).
  // Private constructor ensures only PzCore can create buffers.
  explicit PzBuffer(PzBufferType flag = PzBufferType::PZ_BUF_TYPE_SEQUENTIAL);

  ~PzBuffer() = default; // Default destructor.

  PzBuffer(PzBuffer &&other) noexcept; // Move constructor - efficient resource
                                       // transfer

  PzBuffer &operator=(PzBuffer &&other) noexcept; // Move assignment operator

  PzBuffer(const PzBuffer &) =
      delete; // Deleted copy constructor - force move semantics.

  PzBuffer &operator=(const PzBuffer &) =
      delete; //   Deleted copy assignment - force move semantics.

  // for linear text processing operations.
  std::vector<std::string>
      words_; // Primary word storage - cache-friendly sequential access.

  // for text analysis and word counting operations.
  std::unordered_map<std::string, ut64>
      word_frequencies_; // Word frequency tracking

  // for vocabulary analysis and duplicate detection.
  std::unordered_set<std::string> unique_words_; // Unique word storage

  // position tracking for advanced text indexing.
  std::map<std::string, std::vector<ut64>>
      word_positions_; // Sorted word-to-positions mapping

  // vocabulary processing and range operations.
  std::set<std::string> sorted_words_; // Sorted unique words

  // Tracks starting positions of lines for line-based access.
  std::vector<ut64> line_boundaries_; // Line boundary tracking for structured
                                      // document processing.

  // Tracks batch starts for incremental processing.
  std::vector<ut64>
      batch_boundaries_; // Batch boundary tracking for streaming operations

  friend class PzCore; /**< Friend class to access private constructor. */

public:
};

#endif // PZ_BUFFER_HPP