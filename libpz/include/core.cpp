#include <pz_std.hpp>

PzStd::PzCore::PzCore() {
    // will be friend classes, so can use private constructors
    PzBuffer buffer_obj = new PzBuffer();
    this->buffer = make_shared<PzBuffer>(buffer_obj);
    PzAnalysis analysis_obj = new PzAnalysis();
    this->analysis = make_shared<PzAnalysis>(analysis_obj);
}