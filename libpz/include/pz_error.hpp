#ifndef PZ_ERROR_HPP
#define PZ_ERROR_HPP
#include <pz_cxx_std.hpp>

namespace PzError {
enum class PzErrorType;
// other error / warning related functions or enums
void report_error(PzErrorType type, const std::string &message) {
  throw std::runtime_error("PzError " + message + " (Code: " +
                           std::to_string(static_cast<int>(type)) + ")");
}
}; // namespace PzError

enum class PzError::PzErrorType {
  PZ_NO_ERROR = 0,
  PZ_WRONG_ARGS,
  PZ_ERROR,
  PZ_INVALID_INPUT,
  PZ_BUFFER_ACCESS_FAILED,
  PZ_ANALYSIS_FAILED,
  PZ_INVALID_ANALYSIS_TYPE,
  PZ_FILE_NOT_FOUND,
};

#endif // PZ_ERROR_HPP