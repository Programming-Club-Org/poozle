#include <pz_buffer.hpp>
#include <pz_error.hpp>
#include <pz_std.hpp>

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
            storage_type_ = other.storage_type_;
            last_error_ = other.last_error_;
            total_characters_ = other.total_characters_;

            words_ = std::move(other.words_);
            word_frequencies_ = std::move(other.word_frequencies_);
            unique_words_ = std::move(other.unique_words_);
            word_positions_ = std::move(other.word_positions_);
            sorted_words_ = std::move(other.sorted_words_);
            line_boundaries_ = std::move(other.line_boundaries_);
            batch_boundaries_ = std::move(other.batch_boundaries_);

            // reset other's error and char count
            other.last_error_ = PzErr::PZ_NO_ERROR;
            other.total_characters_ = 0;
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
};