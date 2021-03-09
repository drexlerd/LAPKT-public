
#ifndef __PHILOSOPHERS_SKETCH__
#define __PHILOSOPHERS_SKETCH__

#include <sketch.hxx>
#include "../features/numerical_feature.hxx"

namespace aptk {

class N_NotInState3 : public NumericalFeature {
public:
    N_NotInState3(const BaseSketch* sketch, const std::string &name);
    virtual ~N_NotInState3() = default;
};

class PhilosophersSketch : public BaseSketch {
public:
    PhilosophersSketch(const Sketch_STRIPS_Problem *problem);
    virtual ~PhilosophersSketch() = default;
};

}

#endif
