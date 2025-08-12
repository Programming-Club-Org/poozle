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
    class PzAnalysisExact;
    class PzAnalysisRegex;
    enum class PzAnalysisType {
        PZ_ANALYSIS_TYPE_EXACT,
        PZ_ANALYSIS_TYPE_REGEX
    };
}; 

/**  
 * @brief Enum for analysis types
 */


using PzCoreSPtr = std::shared_ptr<PzStd::PzCore>;
using PzAnalysisImplUPtr = std::unique_ptr<PzStd::PzAnalysisImpl>;
using PzAnalysisType = PzStd::PzAnalysisType;
using PzErrorType = PzError::PzErrorType;

/** 
 * @brief Abstract base class for analysis implementations
 */
class PzStd::PzAnalysisImpl {
public:
    virtual ~PzAnalysisImpl() = default;
    virtual bool analyze(const std::string& pattern, std::vector<size_t>& results) = 0;
protected:
    PzCoreSPtr core_; 
    //explicit to ensure buffer type remains unchanged
    explicit PzAnalysisImpl(PzCoreSPtr core) : core_(std::move(core)) {}
};

/** 
 * @brief Exact string search
 */
class PzStd::PzAnalysisExact : public PzStd::PzAnalysisImpl {
public:
    explicit PzAnalysisExact(PzCoreSPtr core) : PzAnalysisImpl(core) {}
    
    bool analyze(const std::string& pattern, std::vector<size_t>& results) override {
        PzBuffer* buffer = core_->getBuffer();
        if (!buffer || pattern.empty()) {
            PzError::reportError(PzErrorType::PZ_INVALID_INPUT, "Invalid buffer or empty pattern");
            return false;
        }
        try {
           // code or function declaration
           return true;
           // to catch exceptions, change to whatever is more suited later
        } catch (const std::exception& e) {
            PzError::reportError(PzErrorType::PZ_ANALYSIS_FAILED, "Exact analysis failed: " + std::string(e.what()));
            return false;
        }
    }
};

/** 
 * @brief Regex search
 */
class PzStd::PzAnalysisRegex : public PzStd::PzAnalysisImpl {
public:
    explicit PzAnalysisRegex(PzCoreSPtr core) : PzAnalysisImpl(core) {}
    bool analyze(const std::string& pattern, std::vector<size_t>& results) override;

    bool analyze(const std::string& pattern, std::vector<size_t>& results) override {
        PzBuffer* buffer = core_->getBuffer();
        if (!buffer || pattern.empty()) {
            PzError::reportError(PzErrorType::PZ_INVALID_INPUT, "Invalid buffer or empty pattern");
            return false;
        }
        try {
           // code or function declaration
           return true;
           // to catch exceptions, change to whatever is more suited later
        } catch (const std::regex_error& e) {
            PzError::reportError(PzErrorType::PZ_ANALYSIS_FAILED, "Regex analysis failed: " + std::string(e.what()));
            return false;
        }
    }
};

/** 
 * @brief Main PzAnalysis class
 */
class PzStd::PzAnalysis {

private:
    PzCoreSPtr core_; // Shared pointer to PzCore
    PzAnalysisImplUPtr impl_; // Current analysis implementation
    PzAnalysisType currentType_ = PzAnalysisType::PZ_ANALYSIS_TYPE_EXACT; // Default type for now

public:
    /** 
     * @brief Constructor with move semantics
     */
    explicit PzAnalysis(PzCoreSPtr core)
        : core_(std::move(core)) {
        if (!core_) {
            PzError::reportError(PzErrorType::PZ_INVALID_INPUT, "Null PzCore provided");
        }
    }

    /** 
     * @brief Move constructor
     * @brief Move assignemnt
     */
    PzAnalysis(PzAnalysis&& other) noexcept = default;
    PzAnalysis& operator=(PzAnalysis&& other) noexcept = default;

    /** 
     * @brief Delete copy operations to prevent unnecessary copying
     */
    PzAnalysis(const PzAnalysis&) = delete;
    PzAnalysis& operator=(const PzAnalysis&) = delete;

    /** 
     * @brief Public interface for analysis
     */
    bool performAnalysis(PzAnalysisType type, const std::string& pattern, std::vector<size_t>& results) {
        try {
            /** 
             * Implementation based on type
             */
            if (!impl_ || currentType_ != type) {
                switch (type) {
                    case PzAnalysisType::PZ_ANALYSIS_TYPE_EXACT:
                    {
                        impl_ = std::make_unique<PzAnalysisExact>(core_);
                        break;
                    }
                    case PzAnalysisType::PZ_ANALYSIS_TYPE_REGEX:
                    {
                        impl_ = std::make_unique<PzAnalysisRegex>(core_);
                        break;
                    }
                    default:
                    {
                        PzError::reportError(PzErrorType::PZ_INVALID_ANALYSIS_TYPE, "Unknown analysis type");
                        return false;
                    }
                }
                currentType_ = type;
            }
            return impl_->analyze(pattern, results);
        } catch (const std::exception& e) {
            PzError::reportError(PzErrorType::PZ_ANALYSIS_FAILED, "Analysis failed: " + std::string(e.what())); //reason of failure
            return false;
        }
    }

};
#endif //PZ_ANALYSIS_HPP