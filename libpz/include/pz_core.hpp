#ifndef POOZLE_PZ_CORE_H
#define POOZLE_PZ_CORE_H

#include <pz_std.hpp>

enum class PzStd::PzOptions {
  CASE_SENSITIVE, // C and c will match C or c
  SEARCH_BY_LINE, // include whole line as a result if match present in line
  WHOLE_WORDS, // matches only whole words, "there" does not match "the"
}

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

public:
  static PzStd::PzCore create();
  static explicit PzStd::PzCore create(PzStd::PzBufferType flag);
  static PzStd::PzCore from_buffer(const PzStd::PzBuffer &buffer);

  PzBufferSPtr get_buffer();
  bool set_buffer(const PzStd::PzBuffer &buffer);
  bool perform_analysis(PzAnalysisType type, const std::string &pattern,
                        std::vector<size_t> &results,
                        const unsigned num_threads = 0);
  size_t buffer_size() const;
};

#endif // POOZLE_PZ_CORE_H