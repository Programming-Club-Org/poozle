#include <pz_error.hpp>
#include <pz_std.hpp>options

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

/**
 * @brif Factory method to create PzCore instance (empty default buffer).
 * @return PzCore A new PzCore object.
 */
static PzStd::PzCore PzStd::PzCore::create() { return PzStd::PzCore(); }

/**
 * @brif Factory method to create PzCore instance (empty specified buffer).
 * @return PzCore A new PzCore object.
 */
static PzStd::PzCore PzStd::PzCore::create(PzStd::PzBufferType flag) {
  PzStd::PzBuffer buffer_obj = PzStd::PzBuffer::create(flag);
  return PzStd::PzCore(buffer_obj);
}

/**
 * @brif Factory method to create PzCore instance from an existing buffer.
 * @return PzCore A new PzCore object.
 */
PzStd::PzCore::from_buffer(const PzStd::PzBuffer &buffer) {
  if (buffer == nullptr) {
    PzError::reportError(PzErrorType::PZ_INVALID_INPUT, "Invalid buffer");
  }
  return PzStd::PzCore(buffer);
}

/**
 * @brief Get shared pointer to current buffer
 * @return shared pointer to buffer
 */
PzStd::PzCore::get_buffer() { return this->buffer_sptr; }

/**
 * @brif Reset the buffer with a new existing buffer.
 * @return true if operation was successful, false otherwise.
 */
bool ::PzCore::set_buffer(const PzStd::PzBuffer &buffer) {
  if (buffer == nullptr) {
    PzError::reportError(PzErrorType::PZ_INVALID_INPUT, "Invalid buffer");
    return false;
  }
  this->buffer_sptr = std::shared_ptr<PzStd::PzBuffer>(buffer);
  return true;
}

/**
 * @brif Perform analysis and store results in the given vector reference
 * @return true if operation was successful, false otherwise.
 */
bool PzStd::PzCore::performAnalysis(PzAnalysisType type,
                                    const std::string &pattern,
                                    std::vector<size_t> &results,
                                    const unsigned num_threads) {
  // TODO: multithread
  return this->analysis_sptr->performAnalysis(type, pattern, results);
}

/**
 * @brief get size of buffer
 * @return size of buffer
 */
std::option<size_t> PzStd::PzCore::buffer_size() const {
  return this->buffer ? this->buffer->size() : nullopt;
}
