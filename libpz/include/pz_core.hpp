#ifndef POOZLE_PZ_CORE_H
#define POOZLE_PZ_CORE_H

#include <pz_std.hpp>

namespace PzStd {
class PzCore : public std::enable_shared_from_this<PzCore> {
private:
  PzStd::PzBufferSPtr pz_buffer_sptr;
  PzStd::PzAnalysisSPtr pz_analysis_sptr;

  PzCore();
  explicit PzCore(const PzStd::PzBuffer &buffer);
  explicit PzCore(const std::shared_ptr<PzBuffer> buffer);

  PzCore(const PzCore &) =
      delete; // Deleted copy constructor - force move semantics.
  PzCore &operator=(const PzCore &) =
      delete; //   Deleted copy assignment - force move semantics.

  PzCore(PzCore &&other) noexcept;
  PzCore &operator=(PzCore &&other) noexcept;

  PzBufferSPtr get_buffer();

public:
  // TODO : Need a workaround for minimum friend declaration
  // probably through getters
  friend class PzAnalysis;
  friend class PzAnalysisExact;
  friend class PzAnalysisRegex;
};
} // namespace PzStd

#endif // POOZLE_PZ_CORE_H