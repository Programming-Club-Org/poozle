#include <pz_analysis.hpp>
#include <pz_buffer.hpp>
#include <pz_core.hpp>
#include <pz_error.hpp>
#include <pz_std.hpp>

/**
 * @brief Constructs PzCore, empty PzBuffer and PzAnalysis object. Also pass
 * core's shared pointer to them.
 */
PzStd::PzCore::PzCore() {
  this->pz_buffer_sptr = PzStd::PzBuffer::create();

  this->pz_analysis_sptr = PzStd::PzAnalysis::create(shared_from_this());
}

/**
 * @brief Constructs PzCore, existing PzBuffer and PzAnalysis object. Also pass
 * core's shared pointer to them.
 */
PzStd::PzCore::PzCore(const std::shared_ptr<PzBuffer> buffer) {
  this->pz_buffer_sptr = buffer;

  this->pz_analysis_sptr = PzStd::PzAnalysis::create(shared_from_this());
}

PzStd::PzCore::PzCore(PzCore &&other) noexcept {
  // Transfer ownership
  this->pz_buffer_sptr = std::move(other.pz_buffer_sptr);
  this->pz_analysis_sptr = std::move(other.pz_analysis_sptr);
}

PzStd::PzCore &PzStd::PzCore::operator=(PzCore &&other) noexcept {
  if (this != &other) {
    // Release any existing resources (shared_ptr will auto-handle)
    this->pz_buffer_sptr = std::move(other.pz_buffer_sptr);
    this->pz_analysis_sptr = std::move(other.pz_analysis_sptr);
  }
  return *this;
}