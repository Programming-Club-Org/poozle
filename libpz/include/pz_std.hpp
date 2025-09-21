#ifndef PZ_STD_HPP
#define PZ_STD_HPP

#include <pz_analysis.hpp>
#include <pz_buffer.hpp>
#include <pz_cxx_std.hpp>
#include <pz_error.hpp>
#include <pz_types.hpp>

namespace PzStd {
class PzCore;
class PzBuffer;
class PzAnalysis;

enum class PzOptions;

/**
 * @brief unique_ptr and shared_ptr common typedefs
 */
using PzCoreUPtr = std::unique_ptr<PzCore>;
using PzBufferUPtr = std::unique_ptr<PzBuffer>;
using PzAnalysisUPtr = std::unique_ptr<PzAnalysis>;

using PzCoreSPtr = std::shared_ptr<PzCore>;
using PzBufferSPtr = std::shared_ptr<PzBuffer>;
using PzAnalysisSPtr = std::shared_ptr<PzAnalysis>;
}; // namespace PzStd

#endif PZ_STD_HPP