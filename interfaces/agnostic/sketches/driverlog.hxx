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

class SD_PackagesSumDistance : public SumDistanceFeature {
public:
    SD_PackagesSumDistance(const BaseSketch* sketch, const std::string &name);
    virtual ~SD_PackagesSumDistance() = default;
};

class SD_TrucksSumDistance : public SumDistanceFeature {
public:
    SD_TrucksSumDistance(const BaseSketch* sketch, const std::string &name);
    virtual ~SD_TrucksSumDistance() = default;
};

class SD_DriversSumDistance : public SumDistanceFeature {
public:
    SD_DriversSumDistance(const BaseSketch* sketch, const std::string &name);
    virtual ~SD_DriversSumDistance() = default;
};

class N_LoadedPackages : public NumericalFeature {
public:
    N_LoadedPackages(const BaseSketch* sketch, const std::string &name);
    virtual ~N_LoadedPackages() = default;
};

class N_Driving : public NumericalFeature {
public:
    N_Driving(const BaseSketch* sketch, const std::string &name);
    virtual ~N_Driving() = default;
};

class DriverlogSketch : public BaseSketch {
public:
    DriverlogSketch(const Sketch_STRIPS_Problem *problem);
    virtual ~DriverlogSketch() = default;
};


}
#endif
