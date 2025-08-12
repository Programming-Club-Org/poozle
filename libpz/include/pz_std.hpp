#ifndef PZ_STD_HPP
#define PZ_STD_HPP

#include <pz_buffer.hpp>
#include <pz_cxx_std.hpp>
#include <pz_error.hpp>
#include <pz_types.hpp>

namespace PzStd {
class PzCore;
class PzBuffer;
}; // namespace PzStd

using PzErr = PzError::PzErrorType;
class PzStd::PzCore {
private:
  PzErr pz_error = PzErr::PZ_NO_ERROR;

public:
  // core operations on the object
  // eg. pz_core_input_buffer(const PzStd::PzBuffer& new_buffer);
};

#endif PZ_STD_HPP
<<<<<<< HEAD
=======
=======
/**
 * @name Storage flag enum class
 * @brief Storage optimization flags for different text processing patterns.
 * @details
 * These flags determine which data structures are actively maintained
 * during text processing operations. Multiple structures can be used
 * simultaneously based on access patterns.
 */
enum class PzBType: int {
    PZ_BUF_TYPE_SEQUENTIAL = 1,      /**< Use vector for linear access (O(1) append, cache-friendly). */
    PZ_BUF_TYPE_FREQUENCY ,       /**< Track word frequencies (O(1) lookup via `unordered_map`). */
    PZ_BUF_TYPE_UNIQUE ,          /**< Store unique words only (O(1) checking via `unordered_set`). */
    PZ_BUF_TYPE_INDEXED ,         /**< Create word position index (O(log n) via `map`). */
    PZ_BUF_TYPE_SORTED            /**< Maintain sorted order (O(log n) via `set`). */
};

class PzStd::PzBuffer {
private:

     //Create buffer instance via smart pointer.
    static std::unique_ptr<PzBuffer> create( PzBType flag =  PzBType::PZ_BUF_TYPE_SEQUENTIAL);  //Storage optimization mode (default: SEQUENTIAL).
  
     //Private constructor ensures only PzCore can create buffers.
   explicit PzBuffer( PzBType flag =  PzBType::PZ_BUF_TYPE_SEQUENTIAL); 
  
    ~PzBuffer() = default;  //Default destructor.
    
    PzBuffer(PzBuffer&& other) noexcept;    //Move constructor - efficient resource transfer
     
    PzBuffer& operator=(PzBuffer&& other) noexcept;  // Move assignment operator 
    
    PzBuffer(const PzBuffer&) = delete;   // Deleted copy constructor - force move semantics.
    
    PzBuffer& operator=(const PzBuffer&) = delete;   //   Deleted copy assignment - force move semantics.


     //for linear text processing operations.
    std::vector<std::string> words_;   //Primary word storage - cache-friendly sequential access.
    
     //for text analysis and word counting operations.
    std::unordered_map<std::string, ut64> word_frequencies_;   // Word frequency tracking
    
     //for vocabulary analysis and duplicate detection. 
    std::unordered_set<std::string> unique_words_;    // Unique word storage 
    
     //position tracking for advanced text indexing.
    std::map<std::string, std::vector<ut64>> word_positions_;   // Sorted word-to-positions mapping 
    
     //vocabulary processing and range operations.
    std::set<std::string> sorted_words_;    //Sorted unique words
    
     //Tracks starting positions of lines for line-based access.
    std::vector<ut64> line_boundaries_;    //Line boundary tracking for structured document processing.
  
     //Tracks batch starts for incremental processing.
    std::vector<ut64> batch_boundaries_;     // Batch boundary tracking for streaming operations
  
     friend class PzCore;  /**< Friend class to access private constructor. */


  // Input Methods

  // Load a single word into the buffer
  PzErr load_word(std::string&& word);
  PzErr load_word(const std::string& word);

  // Load a block of text into the buffer
  PzErr load_text(std::string_view text);
  PzErr load_text(std::string&& text);

  // Load a list of words into the buffer
  PzErr load_words(const std::vector<std::string>& words);
  PzErr load_words(std::vector<std::string>&& words);

  // Load contents of a file into the buffer
  PzErr load_from_file(const std::string& filename);

  // Load contents of a large file in chunks, tokenizing each chunk (Useful for large files)
  PzErr load_from_file_chunked(const std::string& filename, ut64 chunk_size = 4096);

  // Processing Methods
  
  void build_frequencies(); // Build word_frequencies_ from buffer by counting occurrences of each word
  void build_unique_words();  // Build unique_words_ from buffer containing all distinct words
  void build_word_positions(); // Build word_positions_ mapping each word to the list of positions it appears in the buffer
  void build_sorted_words();  // Build sorted_words_ from the buffer, containing all unique words in sorted order
  
};

#endif // PZ_STD_HPP
<<<<<<< HEAD
>>>>>>> 602fa85 (Add constructors, destructors and data structures.)
=======
>>>>>>> edd9730 (Added Some Input Methods)
>>>>>>> 225bf7c (Rename Input Methods, Add Some Processing Methods, Update Comments)
