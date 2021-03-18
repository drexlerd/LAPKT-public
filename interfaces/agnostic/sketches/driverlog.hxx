
#ifndef __SKETCH_DRIVERLOG__
#define __SKETCH_DRIVERLOG__


#include <sketch.hxx>
#include "../features/boolean_feature.hxx"
#include "../features/numerical_feature.hxx"
#include "../features/numerical/sum_distance.hxx"
#include "../features/numerical/min_concept_distance.hxx"
#include "../features/numerical/min_role_distance.hxx"


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

class B_Loaded : public BooleanFeature {
public:
    B_Loaded(const BaseSketch* sketch, const std::string &name);
    virtual ~B_Loaded() = default;
};

class N_Driving : public NumericalFeature {
public:
    N_Driving(const BaseSketch* sketch, const std::string &name);
    virtual ~N_Driving() = default;
};

class N_DrivingGoalTruck : public NumericalFeature {
public:
    N_DrivingGoalTruck(const BaseSketch* sketch, const std::string &name);
    virtual ~N_DrivingGoalTruck() = default;
};

class MRD_DriverTruckDistance : public MinRoleDistanceFeature {
public:
    MRD_DriverTruckDistance(const BaseSketch* sketch, const std::string &name);
    virtual ~MRD_DriverTruckDistance() = default;
};

class SD_DriversSumDistance : public SumDistanceFeature {
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
