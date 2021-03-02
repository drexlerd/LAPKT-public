#include <sketch.hxx>
#include "../features/boolean_feature.hxx"
#include "../features/numerical_feature.hxx"
#include "../features/numerical/sum_distance.hxx"

namespace aptk {

class N_CouplesNotAtGoalLocation : public NumericalFeature {
public:
    N_CouplesNotAtGoalLocation(const BaseSketch* sketch, const std::string &name);
    virtual ~N_CouplesNotAtGoalLocation() = default;
};


class B_CouplesNotAtGoalWithTentUpAtNextSubgoal : public BooleanFeature {
public:
    B_CouplesNotAtGoalWithTentUpAtNextSubgoal(const BaseSketch* sketch, const std::string &name);
    virtual ~B_CouplesNotAtGoalWithTentUpAtNextSubgoal() = default;
};

class B_CarAtPlaceWithTentUp : public BooleanFeature {
public:
    B_CarAtPlaceWithTentUp(const BaseSketch* sketch, const std::string &name);
    virtual ~B_CarAtPlaceWithTentUp() = default;
};

class B_Test : public BooleanFeature {
public:
    B_Test(const BaseSketch* sketch, const std::string &name);
    virtual ~B_Test() = default;
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
