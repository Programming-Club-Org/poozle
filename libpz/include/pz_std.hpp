#ifndef PZ_STD_HPP
#define PZ_STD_HPP

#include <pz_buffer.hpp>
#include <pz_cxx_std.hpp>
#include <pz_error.hpp>
#include <pz_types.hpp>

namespace PzStd {
class PzCore;
class PzBuffer;
}; // namespace PzStd

using PzErr = PzError::PzErrorType;
class PzStd::PzCore {
private:
  PzErr pz_error = PzErr::PZ_NO_ERROR;

public:
  // core operations on the object
  // eg. pz_core_input_buffer(const PzStd::PzBuffer& new_buffer);
};

#endif PZ_STD_HPP