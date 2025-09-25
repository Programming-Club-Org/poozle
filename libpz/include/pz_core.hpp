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

  PzCore(const PzCore &) =
      delete; // Deleted copy constructor - force move semantics.
  PzCore &operator=(const PzCore &) =
      delete; //   Deleted copy assignment - force move semantics.

  PzCore(PzCore &&other) noexcept;
  PzCore &operator=(PzCore &&other) noexcept;

public:
};

#endif // POOZLE_PZ_CORE_H