#ifndef __SKETCH_DRIVERLOG__
#define __SKETCH_DRIVERLOG__


#include <sketch.hxx>
#include "../features/boolean_feature.hxx"
#include "../features/numerical_feature.hxx"

namespace aptk {
class BaseElement;

class N_PackagesNotAtGoalLocation : public NumericalFeature {
public:
    N_PackagesNotAtGoalLocation(const BaseSketch* sketch, const std::string &name);
    virtual ~N_PackagesNotAtGoalLocation() = default;
};

class N_TrucksNotAtGoalLocation : public NumericalFeature {
public:
    N_TrucksNotAtGoalLocation(const BaseSketch* sketch, const std::string &name);
    virtual ~N_TrucksNotAtGoalLocation() = default;
};

class N_DriversNotAtGoalLocation : public NumericalFeature {
public:
    N_DriversNotAtGoalLocation(const BaseSketch* sketch, const std::string &name);
    virtual ~N_DriversNotAtGoalLocation() = default;
};

class DriverlogSketch : public BaseSketch {
public:
    DriverlogSketch(const Sketch_STRIPS_Problem *problem);
    virtual ~DriverlogSketch() = default;
};


}
#endif
