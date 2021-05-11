#include <sketch.hxx>
#include "../features/numerical_features/count.hxx"
#include "../features/boolean_features/nonempty.hxx"

namespace aptk {

class N_LockedPlaces : public CountFeature {
public:
    N_LockedPlaces(const BaseSketch* sketch, const std::string &name);
    virtual ~N_LockedPlaces() = default;
};

class N_KeysNotAtTarget : public CountFeature {
public:
    N_KeysNotAtTarget(const BaseSketch* sketch, const std::string &name);
    virtual ~N_KeysNotAtTarget() = default;
};

/**
 * Whether the agent is holding a key that is not at its target location
 */
class B_HoldingTargetKey : public NonemptyFeature {
public:
    B_HoldingTargetKey(const BaseSketch* sketch, const std::string &name);
    virtual ~B_HoldingTargetKey() = default;
};

/**
 * Whether the agent is holding a key that can be used to open a reachable door
 */
class B_HoldingOpeningKey : public NonemptyFeature {
public:
    B_HoldingOpeningKey(const BaseSketch* sketch, const std::string &name);
    virtual ~B_HoldingOpeningKey() = default;
};


class GridSketch : public BaseSketch {
public:
    GridSketch(const Sketch_STRIPS_Problem *problem);
    virtual ~GridSketch() = default;
};

}
