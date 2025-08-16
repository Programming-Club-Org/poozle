#include <pz_buffer.hpp>
#include <pz_error.hpp>
#include <pz_std.hpp>
#include<string>
namespace PzStd{
    // Constructor and Move Semantics
    PzBuffer::PzBuffer(PzBType flag) : storage_type_(flag) {}

    PzBuffer::PzBuffer(PzBuffer&& other) noexcept
        :   storage_type_(other.storage_type_),
            last_error_(other.last_error_),
            total_characters_(other.total_characters_),
            words_(std::move(other.words_)),
            word_frequencies_(std::move(other.word_frequencies_)),
            unique_words_(std::move(other.unique_words_)),
            word_positions_(std::move(other.word_positions_)),
            sorted_words_(std::move(other.sorted_words_)),
            line_boundaries_(std::move(other.line_boundaries_)),
            batch_boundaries_(std::move(other.batch_boundaries_)) {
                // reset other's error and char count
                other.last_error_ = PzErr::PZ_NO_ERROR;
                other.total_characters_ = 0;
            }

    PzBuffer& PzBuffer::operator=(PzBuffer&& other) noexcept {
        if (this != &other) {
            PzBuffer temp(std::move(other));
            std::swap(*this, temp);
        }
        return *this;
    }

    // Factory Method
    std::unique_ptr<PzBuffer> PzBuffer::create(PzBType flag){
        return std::unique_ptr<PzBuffer>(new PzBuffer(flag));
    }

    // Input Methods

    PzErr PzBuffer::load_word(std::string&& word){
        if(word.empty()){
            last_error_ = PzErr::PZ_INVALID_INPUT;
            return last_error_;
        }
        words_.push_back(std::move(word));
        total_characters_ += words_.back().size();
        last_error_ = PzErr::PZ_NO_ERROR;
        return last_error_;
    }

    PzErr PzBuffer::load_word(const std::string& word){
        if (word.empty())
        {
            last_error_ = PzErr::PZ_INVALID_INPUT;
            return last_error_;
        }
        words_.push_back(word);
        total_characters_ += word.size();
        apply_storage_flag();
        last_error_ = PzErr::PZ_NO_ERROR;
        return last_error_;
    }

    PzErr PzBuffer::load_text(std::string_view text){
        if (text.empty())
        {
            last_error_ = PzErr::PZ_INVALID_INPUT;
            return last_error_;
        }
        // tokenize the input text into words
        // need to convert string_view to string for std::istringstream processing
        std::istringstream iss(std::string(text));
        std::string word;
        while (iss >> word)
        {
            load_word(std::move(word));
        }
        apply_storage_flag();
        last_error_ = PzErr::PZ_NO_ERROR;
        return last_error_;
    }

    PzErr PzBuffer::load_words(const std::vector<std::string>& words){
        if (words.empty())
        {
            last_error_ = PzErr::PZ_INVALID_INPUT;
            return last_error_;
        }
        for(const std::string& w : words){
            load_word(w);
        }
        apply_storage_flag();
        last_error_ = PzErr::PZ_NO_ERROR;
        return last_error_;
    }

    PzErr PzBuffer::load_words(std::vector<std::string>&& words){
        if (words.empty())
        {
            last_error_ = PzErr::PZ_INVALID_INPUT;
            return last_error_;
        }
        for(std::string& w : words){
            load_word(std::move(w));
        }
        apply_storage_flag();
        last_error_ = PzErr::PZ_NO_ERROR;
        return last_error_;
    }

    PzErr PzBuffer::load_from_file(const std::string& filename){
        std::ifstream file(filename);
        if(!file){
            last_error_ = PzErr::PZ_FILE_NOT_FOUND;
            return last_error_;
        }
        std::string line;
        // Read file line-by-line to handle large inputs and tokenize line-wise
        while (std::getline(file, line)) {
            load_text(line);
        }
        apply_storage_flag();
        last_error_ = PzErr::PZ_NO_ERROR;
        return last_error_;
    }

    // Processing

    void PzBuffer::apply_storage_flag(){
        switch (storage_type_) {
        case PzBType::PZ_BUF_TYPE_FREQUENCY:
            build_frequencies();
            break;
        case PzBType::PZ_BUF_TYPE_UNIQUE:
            build_unique_words();
            break;
        case PzBType::PZ_BUF_TYPE_INDEXED:
            build_word_positions();
            break;
        case PzBType::PZ_BUF_TYPE_SORTED:
            build_sorted_words();
            break;
        case PzBType::PZ_BUF_TYPE_SEQUENTIAL:
        default:
            // No additional processing required
            break;
        }
    }

    void PzBuffer::build_frequencies() {
        word_frequencies_.clear();
        for (const auto& word : words_) {
        ++word_frequencies_[word];
        }
    }

    void PzBuffer::build_unique_words() {
        unique_words_.clear();
        for (const auto& word : words_) {
            unique_words_.insert(word);
        }
    }

    void PzBuffer::build_word_positions() {
        word_positions_.clear();
        for (ut64 pos = 0; pos < words_.size(); ++pos) {
            word_positions_[words_[pos]].push_back(pos);
        }
    }

    void PzBuffer::build_sorted_words() {
        sorted_words_.clear();
        for (const auto& word : words_) {
            sorted_words_.insert(word);
        }
    }

    // Access Methods
    const std::vector<std::string>& PzBuffer::get_all_words() const noexcept {
        return words_;
    }

};