#ifndef PZ_STD_HPP
#define PZ_STD_HPP

#include <pz_error.hpp>
#include <pz_cxx_std.hpp>

namespace PzStd {
class PzCore;
class PzBuffer;
class PzAnalysis;

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

using PzErr = PzError::PzErrorType;

class PzStd::PzCore {
private:
  PzStd::PzBufferSPtr pz_buffer_sptr;
  PzStd::PzAnalysisSPtr pz_analysis_sptr;
  PzStd::PzCoreSPtr pz_core_sptr;

  explicit PzCore();
  PzCore(const PzStd::PzBuffer& buffer);

public:
  static PzStd::PzCore create();
  static PzStd::PzCore from_buffer(const PzStd::PzBuffer& buffer);

  bool set_buffer(const PzStd::PzBuffer& buffer);
  // core operations on the object
  // eg. pz_core_input_buffer(const PzStd::PzBuffer& new_buffer);
};

class PzStd::PzBuffer {
private:
  // std::vector<std::string> or equivalent ways of storing buffer
  // number of words, vector of sizes, all that is required for buffer ....

public:
  // I/O buffer, other operations, deleting etc etc
  // eg. pz_buf_push(const std::string& words);
};

#endif PZ_STD_HPP