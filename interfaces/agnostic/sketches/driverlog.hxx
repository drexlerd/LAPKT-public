
#ifndef __SKETCH_DRIVERLOG__
#define __SKETCH_DRIVERLOG__


#include <sketch.hxx>
#include "../features/boolean_features/nonempty.hxx"
#include "../features/numerical_features/count.hxx"
#include "../features/numerical_features/sum_role_distance.hxx"
#include "../features/numerical_features/concept_distance.hxx"

namespace aptk {
class BaseElement;

class N_PackagesNotAtGoalLocation : public CountFeature {
public:
    N_PackagesNotAtGoalLocation(const BaseSketch* sketch, const std::string &name);
    virtual ~N_PackagesNotAtGoalLocation() = default;
};

class N_TrucksNotAtGoalLocation : public CountFeature {
public:
    N_TrucksNotAtGoalLocation(const BaseSketch* sketch, const std::string &name);
    virtual ~N_TrucksNotAtGoalLocation() = default;
};

class B_Loaded : public NonemptyFeature {
public:
    B_Loaded(const BaseSketch* sketch, const std::string &name);
    virtual ~B_Loaded() = default;
};

class B_Driving : public NonemptyFeature {
public:
    B_Driving(const BaseSketch* sketch, const std::string &name);
    virtual ~B_Driving() = default;
};

class MRD_DriverTruckDistance : public ConceptDistanceFeature {
public:
    MRD_DriverTruckDistance(const BaseSketch* sketch, const std::string &name);
    virtual ~MRD_DriverTruckDistance() = default;
};


class SD_DriversSumDistance : public SumRoleDistanceFeature {
public:
    SD_DriversSumDistance(const BaseSketch* sketch, const std::string &name);
    virtual ~SD_DriversSumDistance() = default;
};

class DriverlogSketch : public BaseSketch {
public:
    DriverlogSketch(const Sketch_STRIPS_Problem *problem);
    virtual ~DriverlogSketch() = default;
};


}
#endif
