#ifndef __SKETCH_DRIVERLOG__
#define __SKETCH_DRIVERLOG__


#include <sketch.hxx>

namespace aptk {
class BaseElement;

class N_PackagesNotAtGoalLocation : public NumericalFeature {
private:
    BaseElement* m_packages_not_at_goal_location;
public:
    N_PackagesNotAtGoalLocation(const BaseSketch* sketch, const std::string &name);
    virtual ~N_PackagesNotAtGoalLocation() = default;

    virtual void evaluate(const State* state) override;
};

class N_TrucksNotAtGoalLocation : public NumericalFeature {
private:
    BaseElement* m_trucks_not_at_goal_location;
public:
    N_TrucksNotAtGoalLocation(const BaseSketch* sketch, const std::string &name);
    virtual ~N_TrucksNotAtGoalLocation() = default;

    virtual void evaluate(const State* state) override;
};

class N_DriversNotAtGoalLocation : public NumericalFeature {
private:
    BaseElement* m_drivers_not_at_goal_location;
public:
    N_DriversNotAtGoalLocation(const BaseSketch* sketch, const std::string &name);
    virtual ~N_DriversNotAtGoalLocation() = default;

    virtual void evaluate(const State* state) override;
};

class DriverlogSketch : public BaseSketch {
public:
    DriverlogSketch(const Sketch_STRIPS_Problem *problem);
    virtual ~DriverlogSketch() = default;
};

}
#endif
