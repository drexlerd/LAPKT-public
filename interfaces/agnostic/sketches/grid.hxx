#include <sketch.hxx>
#include "../features/numerical_feature.hxx"
#include "../features/boolean_feature.hxx"

namespace aptk {

class N_LockedPlaces : public NumericalFeature {
public:
    N_LockedPlaces(const BaseSketch* sketch, const std::string &name);
    virtual ~N_LockedPlaces() = default;
};

class N_KeysNotAtTarget : public NumericalFeature {
public:
    N_KeysNotAtTarget(const BaseSketch* sketch, const std::string &name);
    virtual ~N_KeysNotAtTarget() = default;
};

/**
 * Whether the agent is holding a key that is not at its target location
 */
class B_Holding : public BooleanFeature {
public:
    B_Holding(const BaseSketch* sketch, const std::string &name);
    virtual ~B_Holding() = default;
};

class GridSketch : public BaseSketch {
public:
    GridSketch(const Sketch_STRIPS_Problem *problem);
    virtual ~GridSketch() = default;
};

}
