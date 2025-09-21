#include <pz_core.hpp>
#include <pz_error.hpp>
#include <pz_std.hpp>

/**
 * @brief Constructs PzCore, empty PzBuffer and PzAnalysis object. Also pass
 * core's shared pointer to them.
 */
PzStd::PzCore::PzCore() {
  this->pz_core_sptr = std::make_shared<PzStd::PzCore>(this);

  PzStd::PzBuffer buffer_obj = PzStd::PzBuffer::create();
  this->buffer_sptr = std::shared_ptr<PzStd::PzBuffer>(buffer_obj);

  PzStd::PzAnalysis analysis_obj = PzAnalysis::create(this->pz_core_sptr);
  this->analysis_sptr = std::shared_ptr<PzStd::PzAnalysis>(analysis_obj);
}

/**
 * @brief Constructs PzCore, existing PzBuffer and PzAnalysis object. Also pass
 * core's shared pointer to them.
 */
PzStd::PzCore::PzCore(const PzStd::PzBuffer &buffer) {
  this->pz_core_sptr = std::make_shared<PzStd::PzCore>(this);

  this->buffer_sptr = std::shared_ptr<PzStd::PzBuffer>(buffer);

  PzStd::PzAnalysis analysis_obj = PzAnalysis::create(this->pz_core_sptr);
  this->analysis_sptr = std::shared_ptr<PzStd::PzAnalysis>(analysis_obj);
}

PzStd::PzCore::PzCore(PzCore &&other) noexcept {
  // Transfer ownership
  this->pz_core_sptr = std::make_shared<PzStd::PzCore>(this);
  this->buffer_sptr = std::move(other.buffer_sptr);
  this->analysis_sptr = std::move(other.analysis_sptr);
}

PzStd::PzCore &PzStd::PzCore::operator=(PzCore &&other) noexcept {
  if (this != &other) {
    // Release any existing resources (shared_ptr will auto-handle)
    this->buffer_sptr = std::move(other.buffer_sptr);
    this->analysis_sptr = std::move(other.analysis_sptr);
  }
  return *this;
}