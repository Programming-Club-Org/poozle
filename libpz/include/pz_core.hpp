#ifndef POOZLE_PZ_CORE_H
#define POOZLE_PZ_CORE_H

#include <pz_std.hpp>

class PzStd::PzCore {
private:
    PzStd::PzBufferSPtr pz_buffer_sptr;
    PzStd::PzAnalysisSPtr pz_analysis_sptr;
    PzStd::PzCoreSPtr pz_core_sptr;

    PzCore();
    explicit PzCore(const PzStd::PzBuffer &buffer);
    ~PzCore() = default; // Default destructor.

    PzCore(const PzCore &) = delete; // Deleted copy constructor - force move semantics.
    PzCore &operator=(const PzCore &) = delete; //   Deleted copy assignment - force move semantics.

public:
    static PzStd::PzCore create();
    static explicit PzStd::PzCore create(PzStd::PzBufferType flag);
    static PzStd::PzCore from_buffer(const PzStd::PzBuffer &buffer);

    bool set_buffer(const PzStd::PzBuffer &buffer);
    bool performAnalysis(PzAnalysisType type, const std::string &pattern, std::vector<size_t> &results);
};

#endif //POOZLE_PZ_CORE_H