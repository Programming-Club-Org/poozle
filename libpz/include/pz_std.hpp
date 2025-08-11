#ifndef PZ_STD_HPP
#define PZ_STD_HPP

#include <pz_error.hpp>


namespace PzStd {
class PzCore;
class PzBuffer;
}; // namespace PzStd

using PzErr = PzError::PzErrorType;

class PzStd::PzCore {
private:
  PzErr pz_error = PzErr::PZ_NO_ERROR;
  PzStd::PzBuffer buffer;

public:
  // core operations on the object
  // eg. pz_core_input_buffer(const PzStd::PzBuffer& new_buffer);
};

/**
 * @name Storage flag enum class
 * @brief Storage optimization flags for different text processing patterns.
 * @headerfile pz_std.hpp "include/pz_std.hpp"
 * 
 * @details
 * These flags determine which data structures are actively maintained
 * during text processing operations. Multiple structures can be used
 * simultaneously based on access patterns.
 */
enum class PzStorageFlag : int {
    SEQUENTIAL = 1,      /**< Use vector for linear access (O(1) append, cache-friendly). */
    FREQUENCY = 2,       /**< Track word frequencies (O(1) lookup via `unordered_map`). */
    UNIQUE = 3,          /**< Store unique words only (O(1) checking via `unordered_set`). */
    INDEXED = 4,         /**< Create word position index (O(log n) via `map`). */
    SORTED = 5           /**< Maintain sorted order (O(log n) via `set`). */
};
class PzStd::PzBuffer {
private:

    /**
     * @brief Create buffer instance via smart pointer.
     * @param [in] flag Storage optimization mode (default: SEQUENTIAL).
     * @return Unique pointer to new PzBuffer instance.
     * 
     * @details
     * Static factory method that creates and returns buffer via unique_ptr.
     * Ensures proper initialization and smart pointer management.
     * 
     * **Performance:** O(1) allocation, smart pointer initialization.
     */
    static std::unique_ptr<PzBuffer> create(PzStorageFlag flag = PzStorageFlag::SEQUENTIAL);
    

 /**
     * @name Constructors & Destructor
     * @{
     */
    
    /**
     * @brief Constructor with storage flag configuration.
     * @param [in] flag Storage optimization mode (default: SEQUENTIAL).
     * 
     * @details
     * Private constructor ensures only PzCore can create buffers.
     * Configures internal data structures based on storage flag.
     */
    explicit PzBuffer(PzStorageFlag flag = PzStorageFlag::SEQUENTIAL);
    
    /**
     * @brief Default destructor.
     */
    ~PzBuffer() = default;
    
    /**
     * @brief Move constructor - efficient resource transfer.
     * @param [in] other Buffer to move from.
     */
    PzBuffer(PzBuffer&& other) noexcept;
    
    /**
     * @brief Move assignment operator - efficient resource transfer.
     * @param [in] other Buffer to move from.
     * @return Reference to this buffer.
     */
    PzBuffer& operator=(PzBuffer&& other) noexcept;
    
    /**
     * @brief Deleted copy constructor - force move semantics.
     */
    PzBuffer(const PzBuffer&) = delete; 
    
    /**
     * @brief Deleted copy assignment - force move semantics.
     */
    PzBuffer& operator=(const PzBuffer&) = delete;

    /**
     * @}
     */

 /**
     * @name Data Structures
     * 
     * Optimized containers for different access patterns.
     * @{
     */
    
    /**
     * @brief Primary word storage - cache-friendly sequential access.
     * 
     * @details
     * Vector provides O(1) append and excellent cache locality
     * for linear text processing operations. Uses smart pointers
     * for automatic memory management.
     */
    std::unique_ptr<std::vector<std::string>> words_;
    
    /**
     * @brief Word frequency tracking - O(1) statistical lookups.
     * 
     * @details
     * `unordered_map` enables constant-time frequency queries
     * for text analysis and word counting operations. Managed
     * via smart pointer for automatic cleanup.
     */
    std::unique_ptr<std::unordered_map<std::string, size_t>> word_frequencies_;
    
    /**
     * @brief Unique word storage - O(1) deduplication.
     * 
     * @details
     * `unordered_set` provides constant-time uniqueness checking
     * for vocabulary analysis and duplicate detection. Smart
     * pointer ensures automatic memory management.
     */
    std::unique_ptr<std::unordered_set<std::string>> unique_words_;
    
    /**
     * @brief Sorted word-to-positions mapping - O(log n) range queries.
     * 
     * @details
     * `map` enables logarithmic-time sorted access and
     * position tracking for advanced text indexing. Position
     * vectors also managed via smart pointers.
     */
    std::unique_ptr<std::map<std::string, std::unique_ptr<std::vector<size_t>>>> word_positions_;
    
    /**
     * @brief Sorted unique words - O(log n) ordered iteration.
     * 
     * @details
     * `set` maintains alphabetical order for sorted
     * vocabulary processing and range operations. Smart
     * pointer provides automatic resource management.
     */
    std::unique_ptr<std::set<std::string>> sorted_words_;
    
    /**
     * @brief Line boundary tracking for structured document processing.
     * 
     * @details
     * Tracks starting positions of lines for line-based access.
     * Smart pointer ensures proper cleanup of boundary data.
     */
    std::unique_ptr<std::vector<size_t>> line_boundaries_;
    
    /**
     * @brief Batch boundary tracking for streaming operations.
     * 
     * @details
     * Tracks batch starts for incremental processing. Managed
     * via smart pointer for exception safety.
     */
    std::unique_ptr<std::vector<size_t>> batch_boundaries_;
    
    /**
     * @}
     */

     friend class PzCore;  /**< Friend class to access private constructor. */

    
  
public:
  
};

#endif PZ_STD_HPP