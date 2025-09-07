#ifndef PZ_BUFFER_HPP
#define PZ_BUFFER_HPP
#include <pz_cxx_std.hpp>
#include <pz_error.hpp>
#include <pz_std.hpp>
#include <pz_types.hpp>
class PzStd::PzCore;

/** @brief namespace PzStd */
namespace PzStd {
enum class PzBufferType; /** Enum for buffer storage optimization modes */
};                       // namespace PzStd

/** @brief Custom type definitions used in buffer */
using PzErrorType = PzError::PzErrorType;
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
                            unordered_map). */
  PZ_BUF_TYPE_UNIQUE,    /**< Store unique words only (O(1) checking via
                            unordered_set). */
  PZ_BUF_TYPE_INDEXED, /**< Create word position index (O(log n) via `map`). */
  PZ_BUF_TYPE_SORTED   /**< Maintain sorted order (O(log n) via `set`). */
};

/**
 * @brief Main buffer class for text storage and processing
 */
class PzStd::PzBuffer {
private:
  PzBufferType storage_type_; /**< Current storage mode flag */
  ut64 total_characters_ = 0; /**< Total number of characters stored */

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

  friend class PzCore; /**< Friend class to access private constructor. */

  // Input (loading) methods — private, only accessible to PzCore
  bool load_word(const std::string &word,
                 bool needs_build); /** Load a single word */
  bool load_text(std::string_view text,
                 bool needs_build); /** Load a block of text */
  bool
  load_words(const std::vector<std::string>
                 &words); /** Load list of words into the buffer by copying. */
  bool load_words(
      std::vector<std::string> &&
          words); /** Load words into the buffer by moving (more efficient). */
  bool
  load_from_file(const std::string &filename); /** Load contents of a file */
  bool
  load_from_file_chunked(const std::string &filename,
                         ut64 chunk_size = 4096); /** Load file in chunks */

  /**
   * @brief Internal method to trigger processing based on storage mode after
   * loading input.
   */
  void apply_storage_flag();

  // Processing methods
  void build_frequencies();    /** Build word frequencies */
  void build_unique_words();   /** Build unique words */
  void build_word_positions(); /** Build word positions */
  void build_sorted_words();   /** Build sorted words */

public:
  // Accessors - Read-only methods to query stored data
  const std::vector<std::string> &
  get_all_words() const noexcept; /** Get all words */
  std::string_view
  get_word_at(ut64 index) const; /** Get word at a given index */
  ut64 get_word_count(const std::string &word)
      const; /** Get the frequency of a specific word */
  const std::unordered_map<std::string, ut64> &
  get_frequency_map() const noexcept; /** Get frequency map */
  const std::unordered_set<std::string> &
  get_unique_words() const noexcept; /** Get unique words */
  const std::set<std::string> &
  get_sorted_words() const noexcept; /** Get sorted unique words */
  const std::vector<ut64> *get_positions_of(
      const std::string &word) const; /** Get positions of a given word */

  // MetaData Access Methods - To provide quick information about the buffer's
  // content
  ut64 total_words() const noexcept;       /** Get total words count */
  ut64 unique_word_count() const noexcept; /** Get unique word count */
  ut64 total_characters() const noexcept;  /** Get total characters count */
  bool empty() const noexcept;             /** Check if buffer is empty */

  // Other functions
  void swap(PzBuffer &other) noexcept;

  // Method to clear the buffer
  void clear() noexcept;
};

#endif // PZ_BUFFER_HPP