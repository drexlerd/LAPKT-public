#include <sketch.hxx>
#include "../features/boolean_feature.hxx"
#include "../features/numerical_feature.hxx"
#include "../features/numerical/sum_distance.hxx"

namespace aptk {

class B_TentUpAtNextSubgoal : public BooleanFeature {
public:
    B_TentUpAtNextSubgoal(const BaseSketch* sketch, const std::string &name);
    virtual ~B_TentUpAtNextSubgoal() = default;
};

class B_TentAvailableAtNextSubgoal : public BooleanFeature {
public:
    B_TentAvailableAtNextSubgoal(const BaseSketch* sketch, const std::string &name);
    virtual ~B_TentAvailableAtNextSubgoal() = default;
};

class N_TentsUpAtCurrentLocation : public NumericalFeature {
public:
    N_TentsUpAtCurrentLocation(const BaseSketch* sketch, const std::string &name);
    virtual ~N_TentsUpAtCurrentLocation() = default;
};


class B_CarAtPlaceWithTentUp : public BooleanFeature {
public:
    B_CarAtPlaceWithTentUp(const BaseSketch* sketch, const std::string &name);
    virtual ~B_CarAtPlaceWithTentUp() = default;
};

class B_AtLeastOneAtInitial : public BooleanFeature {
public:
    B_AtLeastOneAtInitial(const BaseSketch* sketch, const std::string &name);
    virtual ~B_AtLeastOneAtInitial() = default;
};

class B_BothAtInitial : public BooleanFeature {
public:
    B_BothAtInitial(const BaseSketch* sketch, const std::string &name);
    virtual ~B_BothAtInitial() = default;
};

class SD_RemainingHikes : public SumDistanceFeature {
public:
    SD_RemainingHikes(const BaseSketch* sketch, const std::string &name);
    virtual ~SD_RemainingHikes() = default;
};

class HikingSketch : public BaseSketch {
public:
    HikingSketch(const Sketch_STRIPS_Problem *problem);
    virtual ~HikingSketch() = default;
};

}
