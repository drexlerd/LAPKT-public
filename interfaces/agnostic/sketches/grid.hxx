#include <sketch.hxx>
#include "../features/numerical_feature.hxx"

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

class GridSketch : public BaseSketch {
public:
    GridSketch(const Sketch_STRIPS_Problem *problem);
    virtual ~GridSketch() = default;
};

}
