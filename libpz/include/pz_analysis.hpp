#ifndef PZ_ANALYSIS_HPP
#define PZ_ANALYSIS_HPP
#include <stdexcept>
#include <pz_std.hpp>
#include <pz_error.hpp>
#include <pz_cxx_std.hpp>
class PzStd::PzCore;
class PzStd::PzBuffer;

/** @brief namespace PzStd */
namespace PzStd {

    class PzAnalysis; /** Main Analysis class */
    class PzAnalysisImpl; /** Abstract base class for analysis implementations */
    class PzAnalysisExact; /** Exact string matching implementation class */
    class PzAnalysisRegex; /** Regex string matching implementation class */  
    enum class PzAnalysisType; /** Enum for types of analysis */
}; 



/** @brief Custom type definitions used in analysis */
using PzCoreSPtr = std::shared_ptr<PzStd::PzCore>;
using PzAnalysisImplUPtr = std::unique_ptr<PzStd::PzAnalysisImpl>;
using PzAnalysisType = PzStd::PzAnalysisType;
using PzErrorType = PzError::PzErrorType;

enum class PzStd::PzAnalysisType { /** Enum for analysis types */
        PZ_ANALYSIS_TYPE_EXACT,
        PZ_ANALYSIS_TYPE_REGEX
};

/** 
 * @brief Abstract base class for analysis implementations.
 * 
 * All specific analysis types should inherit from this and implement analyze().
 */
class PzStd::PzAnalysisImpl {
public:
    virtual ~PzAnalysisImpl() = default;/**< Virtual destructor */
    virtual bool analyze(const std::string& pattern, std::vector<size_t>& results) = 0;/**< Perform analysis on the buffer */

protected:
    PzCoreSPtr core_; /** Shared pointer to core data */
    explicit PzAnalysisImpl(PzCoreSPtr core) : core_(std::move(core)) {} /** Constructor accepting shared pointer to core */
};

/** 
 * @brief Exact string search
 */
class PzStd::PzAnalysisExact : public PzStd::PzAnalysisImpl {
public:
    explicit PzAnalysisExact(PzCoreSPtr core) : PzAnalysisImpl(core) {} //** Constructor */
    bool analyze(const std::string& pattern, std::vector<size_t>& results) override;
};

/** 
 * @brief Regex search
 */
class PzStd::PzAnalysisRegex : public PzStd::PzAnalysisImpl {
public:
    explicit PzAnalysisRegex(PzCoreSPtr core) : PzAnalysisImpl(core) {} //** Constructor */
    bool analyze(const std::string& pattern, std::vector<size_t>& results) override;
};

/** 
 * @brief Main PzAnalysis class for string analysis
 */
class PzStd::PzAnalysis {

private:
    PzCoreSPtr core_; /** Shared pointer to PzCore */
    PzAnalysisImplUPtr impl_; /** Current analysis implementation */
    PzAnalysisType curr_type_ = PzAnalysisType::PZ_ANALYSIS_TYPE_EXACT; /** Default selected type **/
    explicit PzAnalysis(PzCoreSPtr core); /** Construct private PzAnalysis with a core instance.*/

public:
    static PzAnalysis create(PzCoreSPtr core); //** Method to create PzAnalysis objects accessible */

    PzAnalysis(PzAnalysis&& other) noexcept = default; /** Move constructor */
    PzAnalysis& operator=(PzAnalysis&& other) noexcept = default; /** Move assignment operator */
    
    PzAnalysis(const PzAnalysis&) = delete; /** Delete copy constructor to avoid accidental copying */
    PzAnalysis& operator=(const PzAnalysis&) = delete; /** Delete copy assignment operator to avoid accidental copying. */

    bool performAnalysis(PzAnalysisType type, const std::string& pattern, std::vector<size_t>& results); /** Perform analysis of the given pattern using the specified analysis type. */
};
#endif //PZ_ANALYSIS_HPP