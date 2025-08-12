#ifndef PZ_STD_HPP
#define PZ_STD_HPP

#include <pz_error.hpp>

namespace PzStd {
class PzCore;
class PzBuffer;
class PzAnaylysis;

using PzCoreSPtr = std::shared_ptr<PzCore>;
using PzBufferSPtr = std::shared_ptr<PzBuffer>;
using PzAnalysisSPtr = std::shared_ptr<PzAnalysis>;
}; // namespace PzStd

using PzErr = PzError::PzErrorType;

class PzStd::PzCore {
private:
  PzErr pz_error = PzErr::PZ_NO_ERROR;
  PzStd::PzBufferSPtr pz_buffer;
  PzStd::PzAnalysisSPtr pz_analysis;

  explicit PzStd::PzCore();

public:
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