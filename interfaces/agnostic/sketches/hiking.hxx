#include <sketch.hxx>

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


class HikingSketch : public BaseSketch {
public:
    HikingSketch(const Sketch_STRIPS_Problem *problem);
    virtual ~HikingSketch() = default;
};

}
