#include <pz_analysis.hpp>  
#include <pz_std.hpp>      
#include <pz_error.hpp> 
using PzBufferSPtr = std::shared_ptr<PzStd::PzBuffer>;   
namespace PzStd {

/**
 * @brief Performs exact pattern search on the buffer.
 * @param pattern The exact string pattern to search for.
 * @param results Vector to store the positions where pattern is found or results as per implementation.
 * @return true if search was successful, false otherwise.
 */
bool PzAnalysisExact::analyze(const std::string& pattern, std::vector<size_t>& results) {
    PzBufferSPtr* buffer = core_ ? core_->getBuffer() : nullptr;
    if (buffer == nullptr || pattern.empty()) {
        PzError::reportError(PzErrorType::PZ_INVALID_INPUT, "Invalid buffer or empty pattern");
        return false;
    }

    try {
        // TODO: Implement actual exact matching 
        return true; 
    } catch (const std::exception& e) {
        PzError::reportError(PzErrorType::PZ_ANALYSIS_FAILED, "Exact analysis failed: " + std::string(e.what()));
        return false;
    }
}

/**
 * @brief Performs regex pattern search on the buffer.
 * @param pattern The regex pattern to search for.
 * @param results Vector to store the positions where pattern matches or results as per implementation.
 * @return true if search was successful, false otherwise.
 */
bool PzAnalysisRegex::analyze(const std::string& pattern, std::vector<size_t>& results) {
    PzBufferSPtr buffer = core_ ? core_->getBuffer() : nullptr;
    if (buffer == nullptr || pattern.empty()) {
        PzError::reportError(PzErrorType::PZ_INVALID_INPUT, "Invalid buffer or empty pattern");
        return false;
    }

    try {
        // TODO: Implement actual regex search
        return true; 
    } catch (const std::regex_error& e) {
        PzError::reportError(PzErrorType::PZ_ANALYSIS_FAILED,"Regex analysis failed: " + std::string(e.what()));
        return false;
    }
}

/**
 * @brief Constructs a PzAnalysis object with the given shared pointer to PzCore.
 * This constructor takes ownership of the provided core pointer. If the pointer is null, it reports an error.
 * @param core Shared pointer to a PzCore instance.
 */
PzAnalysis::PzAnalysis(PzCoreSPtr core): core_(std::move(core)) {
    if (core_ == nullptr) {
        PzError::reportError(PzErrorType::PZ_INVALID_INPUT, "Null PzCore provided");
    }
}

/**
 * @brief Factory method to create a PzAnalysis instance.
 * This static method creates and returns a PzAnalysis object by taking ownership of the given shared pointer to PzCore.
 * @param core Shared pointer to a PzCore instance.
 * @return PzAnalysis A new PzAnalysis object initialized with the given core.
 */
PzAnalysis PzAnalysis::create(PzCoreSPtr core) {
    return PzAnalysis(std::move(core));
}

/**
 * @brief Performs analysis by selecting the appropriate implementation based on the analysis type.
 * @param type The type of analysis to perform (exact or regex).
 * @param pattern The pattern string to search for.
 * @param results Vector to store the results of the analysis.
 * @return true if analysis was successful, false otherwise.
 */
bool PzAnalysis::performAnalysis(PzAnalysisType type, const std::string& pattern, std::vector<size_t>& results) {
    try {
        if (impl_==nullptr || curr_type_ != type) {
            switch (type) {
                case PzAnalysisType::PZ_ANALYSIS_TYPE_EXACT:
                    impl_ = std::make_unique<PzAnalysisExact>(core_);
                    break;
                case PzAnalysisType::PZ_ANALYSIS_TYPE_REGEX:
                    impl_ = std::make_unique<PzAnalysisRegex>(core_);
                    break;
                default:
                    PzError::reportError(PzErrorType::PZ_INVALID_ANALYSIS_TYPE, "Unknown analysis type");
                    return false;
            }
            curr_type_ = type;
        }
        return impl_->analyze(pattern, results);
    } catch (const std::exception& e) {
        PzError::reportError(PzErrorType::PZ_ANALYSIS_FAILED, "Analysis failed: " + std::string(e.what()));
        return false;
    }
}

} // namespace PzStd
