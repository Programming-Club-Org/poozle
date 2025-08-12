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

/** @brief namespace PzStd */
namespace PzStd {

    class PzAnalysis; /**< Main Analysis class */
    class PzAnalysisImpl; /**< Abstract base class for analysis implementations */
    class PzAnalysisExact; /**< Exact string matching implementation class */
    class PzAnalysisRegex; /**< Regex string matching implementation class */  
    enum class PzAnalysisType { /**< Enum for analysis types */
        PZ_ANALYSIS_TYPE_EXACT,
        PZ_ANALYSIS_TYPE_REGEX
    };
}; 

/** @brief Custom type definitions used in analysis */
using PzCoreSPtr = std::shared_ptr<PzStd::PzCore>;
using PzAnalysisImplUPtr = std::unique_ptr<PzStd::PzAnalysisImpl>;
using PzAnalysisType = PzStd::PzAnalysisType;
using PzErrorType = PzError::PzErrorType;

/** 
 * @brief Abstract base class for analysis implementations.
 * 
 * All specific analysis types should inherit from this and implement analyze().
 */
class PzStd::PzAnalysisImpl {
public:
    /** @brief Virtual destructor */
    virtual ~PzAnalysisImpl() = default;

    /**
     * @brief Perform analysis on the buffer.
     * @param pattern Pattern string to search for.
     * @param results Vector to store search results (positions).
     * @return True if analysis was successful, false otherwise.
     */

    virtual bool analyze(const std::string& pattern, std::vector<size_t>& results) = 0;

protected:
    /** @brief Shared pointer to core data */
    PzCoreSPtr core_;  

    /**
     * @brief Constructor accepting shared pointer to core
     * @param core Shared pointer to PzCore
     */
    explicit PzAnalysisImpl(PzCoreSPtr core) : core_(std::move(core)) {}
};

/** 
 * @brief Exact string search
 */
class PzStd::PzAnalysisExact : public PzStd::PzAnalysisImpl {
public:
    explicit PzAnalysisExact(PzCoreSPtr core) : PzAnalysisImpl(core) {} //**< Constructor */
    bool analyze(const std::string& pattern, std::vector<size_t>& results) override;
};

/** 
 * @brief Regex search
 */
class PzStd::PzAnalysisRegex : public PzStd::PzAnalysisImpl {
public:
    explicit PzAnalysisRegex(PzCoreSPtr core) : PzAnalysisImpl(core) {} //**< Constructor */
    bool analyze(const std::string& pattern, std::vector<size_t>& results) override;
};

/** 
 * @brief Main PzAnalysis class for string analysis
 */
class PzStd::PzAnalysis {

private:
    PzCoreSPtr core_; /**< Shared pointer to PzCore */
    PzAnalysisImplUPtr impl_; /**< Current analysis implementation */
    PzAnalysisType currentType_ = PzAnalysisType::PZ_ANALYSIS_TYPE_EXACT; /**< Default selected type **/

public:
    /**
     * @brief Construct PzAnalysis with a core instance.
     * @param core Shared pointer to PzCore
     */
    explicit PzAnalysis(PzCoreSPtr core) : core_(std::move(core)) {
        if (!core_) {
            PzError::reportError(PzErrorType::PZ_INVALID_INPUT, "Null PzCore provided");
        }
    }

    /** 
     * @brief Move constructor
     */
    PzAnalysis(PzAnalysis&& other) noexcept = default;

    /**
     * @brief Move assignment operator
     */
    PzAnalysis& operator=(PzAnalysis&& other) noexcept = default;

    /**
     * @brief Delete copy constructor to avoid accidental copying
     */
    PzAnalysis(const PzAnalysis&) = delete;

    /**
     * @brief Delete copy assignment operator to avoid accidental copying.
     */
    PzAnalysis& operator=(const PzAnalysis&) = delete;

    /**
     * @brief Perform analysis of the given pattern using the specified analysis type.
     * @param type The type of analysis.
     * @param pattern The pattern string to search for.
     * @param results Vector to hold the result positions.
     * @return True if analysis succeeded, false otherwise.
     */
    bool performAnalysis(PzAnalysisType type, const std::string& pattern, std::vector<size_t>& results);
};
#endif //PZ_ANALYSIS_HPP