#ifndef PZ_BUFFER_HPP
#define PZ_BUFFER_HPP

#include <pz_cxx_std.hpp>
#include <pz_error.hpp>
<<<<<<< HEAD
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
=======
#include <pz_types.hpp>

namespace PzStd {
    class PzBuffer;
    enum class PzBufferTypes;
}; // namespace PzStd

using PzBType = PzStd::PzBufferTypes;
using PzErr = PzError::PzErrorType;

/**
 * @brief Enum defining storage optimization modes for PzBuffer.
 */
enum class PzStd::PzBufferTypes : int {
    SEQUENTIAL,   /**< Use vector for fast linear access and appending */
    FREQUENCY,    /**< Track frequency of each word using a hash map */
    UNIQUE,       /**< Store only unique words via hash set */
    INDEXED,      /**< Maintain a map indexing words to their positions */
    SORTED        /**< Keep words sorted using a tree-based set */
};

/**
 * @class PzBuffer
 * @brief Text buffer supporting various storage and indexing strategies.
 *
 * Stores and processes textual data with different internal representations
 * selectable via storage flags for optimized access patterns.
 */
class PzStd::PzBuffer {
private:
    PzBType storage_type_;              /**< Current storage mode flag */
    PzErr last_error_ = PzErr::PZ_NO_ERROR; /**< Last error code encountered */
    ut64 total_characters_ = 0;         /**< Total number of characters stored */

    // Storage containers
    std::vector<std::string> words_;               /**< Words stored sequentially */
    std::unordered_map<std::string, ut64> word_frequencies_; /**< Frequency counts */
    std::unordered_set<std::string> unique_words_; /**< Set of unique words */
    std::map<std::string, std::vector<ut64>> word_positions_; /**< Word positions index */
    std::set<std::string> sorted_words_;           /**< Sorted unique words */
    std::vector<ut64> line_boundaries_;             /**< Line start positions */
    std::vector<ut64> batch_boundaries_;            /**< Batch boundary positions */

    friend class PzCore;  /**< PzCore is allowed full access to private members */

    /**
     * @brief Private constructor to enforce creation by PzCore only.
     * @param flag Storage mode to use, defaults to SEQUENTIAL.
     */
    explicit PzBuffer(PzBType flag = PzBType::SEQUENTIAL);
    ~PzBuffer() = default;  /**< Default destructor */

    // Move semantics
    PzBuffer(PzBuffer&& other) noexcept;
    PzBuffer& operator=(PzBuffer&& other) noexcept;

    // Disable copy semantics
    PzBuffer(const PzBuffer&) = delete;
    PzBuffer& operator=(const PzBuffer&) = delete;

    /**
     * @brief Factory function to create unique pointer instances.
     * @param flag Storage mode to use, defaults to SEQUENTIAL.
     * @return Unique pointer to new PzBuffer.
     */
    static std::unique_ptr<PzBuffer> create(PzBType flag = PzBType::SEQUENTIAL);

    // Input (loading) methods — private, only accessible to PzCore

    PzErr load_word(std::string&& word);
    PzErr load_word(const std::string& word);
    PzErr load_text(std::string_view text);
    PzErr load_text(std::string&& text);
    PzErr load_words(const std::vector<std::string>& words);
    PzErr load_words(std::vector<std::string>&& words);
    PzErr load_from_file(const std::string& filename);
    PzErr load_from_file_chunked(const std::string& filename, ut64 chunk_size = 4096);

    /**
     * @brief Internal method to trigger processing based on storage mode after loading input.
     */
    void apply_storage_flag();

    // Processing methods

    void build_frequencies();
    void build_unique_words();
    void build_word_positions();
    void build_sorted_words();

public:
    // Accessors - Read-only methods to query stored data
    /** Get all words */
    const std::vector<std::string>& get_all_words() const noexcept;

    /** Get word at a given index */
    std::string_view get_word_at(ut64 index) const;

    /** Get the frequency of a specific word */
    ut64 get_word_count(const std::string& word) const;

    /** Get frequency map */
    const std::unordered_map<std::string, ut64>& get_frequency_map() const noexcept;

    /** Get unique words */
    const std::unordered_set<std::string>& get_unique_words() const noexcept;

    /** Get sorted unique words */
    const std::set<std::string>& get_sorted_words() const noexcept;

    /** Get positions of a given word */
    const std::vector<ut64>* get_positions_of(const std::string& word) const;

    // MetaData Access Methods - To provide quick information about the buffer's content
    /** Get total words count */
    ut64 total_words() const noexcept;

    /** Get unique word count */
    ut64 unique_word_count() const noexcept;

    /** Get number of lines */
    ut64 line_count() const noexcept;

    /** Get number of batches */
    ut64 batch_count() const noexcept;

    /** Get total characters count */
    ut64 total_characters() const noexcept;

    /** Check if buffer is empty */
    bool empty() const noexcept;

};

#endif // PZ_BUFFER_HPP
>>>>>>> ff5f06c (Moved the PzBuffer Class Declaration from pz_std.hpp to pz_buffer.hpp, Added Doxygen Style Comments in pz_buffer.hpp, Added Access Methods to query Stored Data and MetaData Access Methods)
