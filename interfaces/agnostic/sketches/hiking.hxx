#include <sketch.hxx>

namespace aptk {

class N_CouplesNotAtGoalLocation : public NumericalFeature {
public:
    N_CouplesNotAtGoalLocation(const BaseSketch* sketch, const std::string &name);
    virtual ~N_CouplesNotAtGoalLocation() = default;
};

class N_CouplesNotAtGoalWithTentUpAtPreviousSubgoal : public NumericalFeature {
public:
    N_CouplesNotAtGoalWithTentUpAtPreviousSubgoal(const BaseSketch* sketch, const std::string &name);
    virtual ~N_CouplesNotAtGoalWithTentUpAtPreviousSubgoal() = default;
};

class N_CouplesNotAtGoalWithTentUpAtNextSubgoal : public NumericalFeature {
public:
    N_CouplesNotAtGoalWithTentUpAtNextSubgoal(const BaseSketch* sketch, const std::string &name);
    virtual ~N_CouplesNotAtGoalWithTentUpAtNextSubgoal() = default;
};


class HikingSketch : public BaseSketch {
public:
    HikingSketch(const Sketch_STRIPS_Problem *problem);
    virtual ~HikingSketch() = default;
};

}
