#include <pz_std.hpp>
#include <pz_error.hpp>

/**
* @brief Constructs PzCore, PzBuffer and PzAnalysis object. Also pass core's
* shared pointer to them.
*/
PzStd::PzCore::PzCore() {
    this->pz_core_sptr = std::make_shared<PzStd::PzCore>(this);

    PzBuffer buffer_obj = PzBuffer::create();
    this->buffer_sptr = std::shared_ptr<PzStd::PzBuffer>(buffer_obj);

    PzAnalysis analysis_obj = PzAnalysis::create(this->pz_core_sptr);
    this->analysis_sptr = std::shared_ptr<PzStd::PzAnalysis>(analysis_obj);
}

PzStd::PzCore::PzCore(const PzStd::PzBuffer& buffer) {
    this->pz_core_sptr = std::make_shared<PzStd::PzCore>(this);

    this->buffer_sptr = std::shared_ptr<PzStd::PzBuffer>(buffer);

    PzAnalysis analysis_obj = PzAnalysis::create(this->pz_core_sptr);
    this->analysis_sptr = std::shared_ptr<PzStd::PzAnalysis>(analysis_obj);
}

/**
* @brif Factory method to create PzCore instance.
* @return PzCore A new PzCore object.
*/
PzStd::PzCore::create() {
    return PzStd::PzCore();
}

/**
* @brif Factory method to create PzCore instance from an existing buffer.
* @return PzCore A new PzCore object.
*/
PzStd::PzCore::from_buffer(const PzStd::PzBuffer& buffer) {
    if (buffer == nullptr) {
        PzError::reportError(PzErrorType::PZ_INVALID_INPUT,
                         "Invalid buffer");
    }
    return PzStd::PzCore(buffer);
}

/**
* @brif Reset the buffer with a new existing buffer.
* @return true if operation was successful, false otherwise.
*/
PzStd::PzCore::set_buffer(const PzStd::PzBuffer& buffer) {
    if (buffer == nullptr) {
        PzError::reportError(PzErrorType::PZ_INVALID_INPUT,
                         "Invalid buffer");
        return false;
    }
    this->buffer_sptr = std::shared_ptr<PzStd::PzBuffer>(buffer);
    return true;
}