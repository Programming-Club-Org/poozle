#ifndef PZ_ANALYSIS_HPP
#define PZ_ANALYSIS_HPP

#include <memory>
#include <string>
#include <vector>
#include <variant>
#include <regex>
#include <map>
#include <stdexcept>
#include <pz_std.hpp>
#include <pz_error.hpp>

class PzStd::PzCore;
class PzStd::PzBuffer;

/** 
 *  @brief namespace PzStd
 */
namespace PzStd {
class PzAnalysis;
class PzAnalysisImpl;
enum class PzAnalysisType;
class PzAnalysisExact;
class PzAnalysisRegex;
}; 

/**  
 * @brief Enum for analysis types
 */
enum class PzStd::PzAnalysisType{
    EXACT,
    REGEX
};

/** 
 * @brief Abstract base class for analysis implementations
 */
class PzStd::PzAnalysisImpl {
public:
    virtual ~PzAnalysisImpl() = default;
    virtual bool analyze(const std::string& pattern, std::vector<size_t>& results) = 0;
protected:
    PzStd::PzBuffer* buffer_;
    //explicit to ensure buffer type remains unchanged
    explicit PzAnalysisImpl(PzStd::PzBuffer* buffer) : buffer_(buffer) {}
};

/** 
 * @brief Exact string search
 */
class PzStd::PzAnalysisExact : public PzStd::PzAnalysisImpl {
public:
    explicit PzAnalysisExact(PzStd::PzBuffer* buffer) : PzAnalysisImpl(buffer) {}
    
    bool analyze(const std::string& pattern, std::vector<size_t>& results) override {
        if (!buffer_ || pattern.empty()) {
            PzError::reportError(PzError::PzErrorType::PZ_INVALID_INPUT, "Invalid buffer or empty pattern");
            return false;
        }
        try {
           // code or function declaration
           return true;
           // to catch exceptions, change to whatever is more suited later
        } catch (const std::exception& e) {
            PzError::reportError(PzError::PzErrorType::PZ_ANALYSIS_FAILED, "Exact analysis failed: " + std::string(e.what()));
            return false;
        }
    }
};

/** 
 * @brief Regex search
 */
class PzStd::PzAnalysisRegex : public PzStd::PzAnalysisImpl {
public:
    explicit PzAnalysisRegex(PzStd::PzBuffer* buffer) : PzAnalysisImpl(buffer) {}
    
    bool analyze(const std::string& pattern, std::vector<size_t>& results) override {
        if (!buffer_ || pattern.empty()) {
            PzError::reportError(PzError::PzErrorType::PZ_INVALID_INPUT, "Invalid buffer or empty pattern");
            return false;
        }
        try {
           // code or function declaration
           return true;
           // to catch exceptions, change to whatever is more suited later
        } catch (const std::regex_error& e) {
            PzError::reportError(PzError::PzErrorType::PZ_ANALYSIS_FAILED, "Regex analysis failed: " + std::string(e.what()));
            return false;
        }
    }
};

/** 
 * @brief Main PzAnalysis class
 */
class PzStd::PzAnalysis {

private:
    std::shared_ptr<PzStd::PzCore> core_; // Shared pointer to PzCore
    std::unique_ptr<PzStd::PzAnalysisImpl> impl_; // Current analysis implementation
    PzAnalysisType currentType_ = PzStd::PzAnalysisType::EXACT; // Default type for now

    // To access buffer from PzCore
    PzStd::PzBuffer* getBuffer() {
        if (!core_) {
            PzError::reportError(PzError::PzErrorType::PZ_BUFFER_ACCESS_FAILED, "No valid PzCore instance");
            return NULL;
        }
        PzStd::PzBuffer* buffer = core_->getBuffer();
        if (!buffer) {
            PzError::reportError(PzError::PzErrorType::PZ_BUFFER_ACCESS_FAILED, "Failed to access PzBuffer");
        }
        return buffer;
    }

public:
    /** 
     * @brief Constructor with move semantics
     */
    explicit PzAnalysis(std::shared_ptr<PzStd::PzCore> core)
        : core_(std::move(core)) {
        if (!core_) {
            PzError::reportError(PzError::PzErrorType::PZ_INVALID_INPUT, "Null PzCore provided");
        }
    }

    /** 
     * @brief Move constructor
     * @brief Move assignemnt
     */
    PzAnalysis(PzStd::PzAnalysis&& other) noexcept = default;
    PzStd::PzAnalysis& operator=(PzStd::PzAnalysis&& other) noexcept = default;

    /** 
     * @brief Delete copy operations to prevent unnecessary copying
     */
    PzAnalysis(const PzStd::PzAnalysis&) = delete;
    PzStd::PzAnalysis& operator=(const PzStd::PzAnalysis&) = delete;

    /** 
     * @brief Public interface for analysis
     */
    bool performAnalysis(PzStd::PzAnalysisType type, const std::string& pattern, std::vector<size_t>& results) {
        try {
            /** 
             * Implementation based on type
             */
            if (!impl_ || currentType_ != type) {
                switch (type) {
                    case PzStd::PzAnalysisType::EXACT:
                        impl_ = std::make_unique<PzAnalysisExact>(getBuffer());
                        break;
                    case PzStd::PzAnalysisType::REGEX:
                        impl_ = std::make_unique<PzAnalysisRegex>(getBuffer());
                        break;
                    default:
                        PzError::reportError(PzError::PzErrorType::PZ_INVALID_ANALYSIS_TYPE, "Unknown analysis type");
                        return false;
                }
                currentType_ = type;
            }
            return impl_->analyze(pattern, results);
        } catch (const std::exception& e) {
            PzError::reportError(PzError::PzErrorType::PZ_ANALYSIS_FAILED, "Analysis failed: " + std::string(e.what())); //reason of failure
            return false;
        }
    }

};
#endif PZ_ANALYSIS_HPP