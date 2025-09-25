#include <pz_buffer.hpp>
#include <pz_cxx_std.hpp>
#include <pz_error.hpp>

namespace PzStd {
/**
 * @brief Constructs a PzBuffer with the specified storage type.
 * @param flag Storage optimization flag.
 */
PzBuffer::PzBuffer(PzBufferType flag) : storage_type_(flag) {}

/**
 * @brief Move constructor.
 * Transfers ownership of resources from another PzBuffer.
 * @param other Buffer to move from.
 */
PzBuffer::PzBuffer(PzBuffer &&other) noexcept
    : storage_type_(other.storage_type_),
      total_characters_(other.total_characters_),
      words_(std::move(other.words_)),
      word_frequencies_(std::move(other.word_frequencies_)),
      unique_words_(std::move(other.unique_words_)),
      word_positions_(std::move(other.word_positions_)),
      sorted_words_(std::move(other.sorted_words_)) {
  // reset other's char count
  other.total_characters_ = 0;
  other.storage_type_ = PzBufferType::PZ_BUF_TYPE_SEQUENTIAL;
}

/**
 * @brief Custom swap function for efficient member-wise swap.
 * @param other Buffer to swap with.
 */
void PzBuffer::swap(PzBuffer &other) noexcept {
  std::swap(storage_type_, other.storage_type_);
  std::swap(total_characters_, other.total_characters_);
  std::swap(words_, other.words_);
  std::swap(word_frequencies_, other.word_frequencies_);
  std::swap(unique_words_, other.unique_words_);
  std::swap(word_positions_, other.word_positions_);
  std::swap(sorted_words_, other.sorted_words_);
}

/**
 * @brief Move assignment operator.
 * Uses move-and-swap idiom for safe resource transfer.
 * @param other Buffer to assign from.
 * @return Reference to this buffer.
 */
PzBuffer &PzBuffer::operator=(PzBuffer &&other) noexcept {
  if (this != &other) {
    PzBuffer temp(std::move(other)); // Move construct temporary
    swap(temp);                      // Swap *this with temp
                // temp is destroyed, releasing old resources
  }
  return *this;
}

/**
 * @brief Factory method to create a buffer instance.
 * @param flag Storage type flag.
 * @return Unique pointer to a new PzBuffer.
 */
std::shared_ptr<PzBuffer> PzBuffer::create(PzBufferType flag) {
  return std::shared_ptr<PzBuffer>(new PzBuffer(flag));
}

// TODO: [Configurable Token Delimiter]
// Currently, buffer storage assumes words are separated by spaces.
// In future, support custom delimiters so users can define what separates words
// (e.g. commas, semicolons, etc). Also support searching that respects the
// chosen delimiter (including phrase searches like "word1 word2"). This will
// require:
//   - Adding a parameter to input methods to specify delimiter.
//   - Storing delimiter info in the buffer.
//   - Updating tokenization and search logic accordingly.
// Input Methods
/**
 * @brief Load a single word into the buffer.
 * @param word Input word.
 * @return true if successful, false otherwise.
 */
bool PzBuffer::load_word(const std::string &word, bool needs_build) {
  if (word.empty()) {
    PzError::reportError(PzErrorType::PZ_INVALID_INPUT, "Empty word input");
    return false;
  }
  words_.push_back(word);
  total_characters_ += words_.back().size();
  if (needs_build) {
    apply_storage_flag();
  }
  return true;
}

/**
 * @brief Load text into the buffer by tokenizing words.
 * @param text Input text block.
 * @return true if successful, false otherwise.
 */
bool PzBuffer::load_text(std::string_view text, bool needs_build) {
  if (text.empty()) {
    PzError::reportError(PzErrorType::PZ_INVALID_INPUT, "Input text is empty");
    return false;
  }
  // tokenize the input text into words
  // need to convert string_view to string for std::istringstream processing
  std::istringstream iss{std::string(text)};
  std::string word;
  while (iss >> word) {
    load_word(word, false);
  }
  if (needs_build) {
    apply_storage_flag();
  }
  return true;
}

/**
 * @brief Load words from a vector (copy).
 * @param words List of words.
 * @return true if successful, false otherwise.
 */
bool PzBuffer::load_words(const std::vector<std::string> &words) {
  if (words.empty()) {
    PzError::reportError(PzErrorType::PZ_INVALID_INPUT,
                         "Input word vector is empty");
    return false;
  }
  // Append all words to the internal storage
  words_.insert(words_.end(), words.begin(), words.end());
  // Update total characters count by adding the characters of new words
  for (const auto &w : words) {
    total_characters_ += w.size();
  }
  apply_storage_flag();
  return true;
}

/**
 * @brief Load words from a vector (move).
 * @param words List of words (rvalue reference).
 * @return true if successful, false otherwise.
 */
bool PzBuffer::load_words(std::vector<std::string> &&words) {
  if (words.empty()) {
    PzError::reportError(PzErrorType::PZ_INVALID_INPUT,
                         "Input moved word vector is empty");
    return false;
  }
  // Reserve memory to avoid reallocations
  words_.reserve(words_.size() + words.size());

  // Append by moving each element
  for (std::string &word : words) {
    total_characters_ += word.size();
    words_.push_back(std::move(word));
  }
  apply_storage_flag();
  return true;
}

/**
 * @brief Load text from a file line by line.
 * @param filename Path to file.
 * @return true if successful, false otherwise.
 */
bool PzBuffer::load_from_file(const std::string &filename) {
  std::ifstream file(filename);
  if (!file) {
    PzError::reportError(PzErrorType::PZ_FILE_NOT_FOUND,
                         "File not found: " + filename);
    return false;
  }
  std::string line;
  // Read file line-by-line and tokenize
  while (std::getline(file, line)) {
    load_text(line, false);
  }
  apply_storage_flag();
  return true;
}

/**
 * @brief Load file contents in fixed-size chunks.
 * @param filename Path to file.
 * @param chunk_size Size of chunks in bytes.
 * @return true if successful, false otherwise.
 */
bool PzBuffer::load_from_file_chunked(const std::string &filename,
                                      ut64 chunk_size) {
  // Add a guard to check for chunk_size > 0
  if (chunk_size == 0) {
    PzError::reportError(PzErrorType::PZ_INVALID_INPUT,
                         "Chunk size cannot be zero");
    return false;
  }

  std::ifstream file(filename, std::ios::binary);
  if (!file) {
    PzError::reportError(PzErrorType::PZ_FILE_NOT_FOUND,
                         "File not found: " + filename);
    return false;
  }
  std::string buffer(chunk_size, '\0');
  std::string carry_over; // Holds partial words from previous chunk
  while (file.read(buffer.data(), chunk_size) || file.gcount() > 0) {
    std::string chunk = buffer.substr(0, file.gcount());
    std::string combined_chunk = carry_over + chunk;

    // Find the last space to determine if there's a partial word
    size_t last_space = combined_chunk.find_last_of(" \n\r\t");

    // Reset carry_over to be empty for this iteration
    carry_over.clear();

    if (last_space == std::string::npos) {
      // No whitespace in combined_chunk, so keep whole thing as carry_over
      carry_over = std::move(combined_chunk);
      continue; // read next chunk and prepend carry_over
    }

    if (last_space < combined_chunk.size() - 1) {
      // if a word is split, take the last word as the carry_over
      carry_over = combined_chunk.substr(last_space + 1);
      combined_chunk = combined_chunk.substr(0, last_space + 1);
    }

    load_text(combined_chunk, false);
  }

  // After the loop, process any remaining content in carry_over
  if (!carry_over.empty()) {
    load_text(carry_over, false);
  }
  apply_storage_flag();
  return true;
}

// Processing
/**
 * @brief Apply storage flag to trigger appropriate processing.
 */
void PzBuffer::apply_storage_flag() {
  switch (storage_type_) {
  case PzBufferType::PZ_BUF_TYPE_FREQUENCY:
    build_frequencies();
    break;
  case PzBufferType::PZ_BUF_TYPE_UNIQUE:
    build_unique_words();
    break;
  case PzBufferType::PZ_BUF_TYPE_INDEXED:
    build_word_positions();
    break;
  case PzBufferType::PZ_BUF_TYPE_SORTED:
    build_sorted_words();
    break;
  case PzBufferType::PZ_BUF_TYPE_SEQUENTIAL:
  default:
    // No additional processing required
    break;
  }
}

/** @brief Build word frequency map. */
void PzBuffer::build_frequencies() {
  word_frequencies_.clear();
  for (const auto &word : words_) {
    ++word_frequencies_[word];
  }
}

/** @brief Build set of unique words. */
void PzBuffer::build_unique_words() {
  unique_words_.clear();
  for (const auto &word : words_) {
    unique_words_.insert(word);
  }
}

/** @brief Build word-to-position index. */
void PzBuffer::build_word_positions() {
  word_positions_.clear();
  for (ut64 pos = 0; pos < words_.size(); ++pos) {
    word_positions_[words_[pos]].push_back(pos);
  }
}

/** @brief Build sorted set of words. */
void PzBuffer::build_sorted_words() {
  sorted_words_.clear();
  for (const auto &word : words_) {
    sorted_words_.insert(word);
  }
}

// Access Methods
/**
 * @brief Get all stored words.
 * @return Vector of words.
 */
const std::vector<std::string> &PzBuffer::get_all_words() const noexcept {
  return words_;
}

/**
 * @brief Get word at a given index.
 * @param index Position in buffer.
 * @return Word view, or empty view if out of range.
 */
std::string_view PzBuffer::get_word_at(ut64 index) const {
  if (index >= words_.size())
    return {}; // return an empty std::string_view when the index is out of
               // range
  return words_[index];
}

/**
 * @brief Get frequency of a word.
 * @param word Target word.
 * @return Count of word occurrences.
 */
ut64 PzBuffer::get_word_count(const std::string &word) const {
  auto it = word_frequencies_.find(word);
  return it != word_frequencies_.end() ? it->second : 0;
}

/**
 * @brief Get frequency map of all words.
 * @return Unordered map of word -> frequency.
 */
const std::unordered_map<std::string, ut64> &
PzBuffer::get_frequency_map() const noexcept {
  return word_frequencies_;
}

/**
 * @brief Get set of unique words.
 * @return Unordered set of unique words.
 */
const std::unordered_set<std::string> &
PzBuffer::get_unique_words() const noexcept {
  return unique_words_;
}

/**
 * @brief Get sorted set of words.
 * @return Sorted set of words.
 */
const std::set<std::string> &PzBuffer::get_sorted_words() const noexcept {
  return sorted_words_;
}

/**
 * @brief Get all positions of a given word.
 * @param word Word to look up.
 * @return Pointer to positions vector, or nullptr if not found.
 */
const std::vector<ut64> *
PzBuffer::get_positions_of(const std::string &word) const {
  auto it = word_positions_.find(word);
  return (it != word_positions_.end()) ? &(it->second) : nullptr;
}

// Metadata Access
/** @brief Get total number of words. */
ut64 PzBuffer::total_words() const noexcept {
  return static_cast<ut64>(words_.size());
}

/** @brief Get unique word count. */
ut64 PzBuffer::unique_word_count() const noexcept {
  return static_cast<ut64>(unique_words_.size());
}

/** @brief Get total number of characters. */
ut64 PzBuffer::total_characters() const noexcept { return total_characters_; }

/** @brief Check if buffer is empty. */
bool PzBuffer::empty() const noexcept { return words_.empty(); }

/**
 * @brief Clears the buffer contents, resetting all internal data structures.
 * The object's state is preserved but its contents are removed.
 */
void PzBuffer::clear() noexcept {
  words_.clear();
  word_frequencies_.clear();
  unique_words_.clear();
  word_positions_.clear();
  sorted_words_.clear();
  total_characters_ = 0;
  storage_type_ = PzBufferType::PZ_BUF_TYPE_SEQUENTIAL;
}

} // namespace PzStd