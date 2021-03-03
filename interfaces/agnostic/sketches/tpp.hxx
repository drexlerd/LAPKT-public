#include <sketch.hxx>
#include "../features/numerical_feature.hxx"
#include "../features/boolean_feature.hxx"
#include "../features/numerical/sum_distance.hxx"

namespace aptk {

class B_Loadable : public BooleanFeature {
public:
    B_Loadable(const BaseSketch* sketch, const std::string &name);
    virtual ~B_Loadable() = default;
};

class N_Loaded : public NumericalFeature {
public:
    N_Loaded(const BaseSketch* sketch, const std::string &name);
    virtual ~N_Loaded() = default;
};

class SD_Remaining : public SumDistanceFeature {
public:
    SD_Remaining(const BaseSketch* sketch, const std::string &name);
    virtual ~SD_Remaining() = default;
};

class TppSketch : public BaseSketch {
public:
    TppSketch(const Sketch_STRIPS_Problem *problem);
    virtual ~TppSketch() = default;
};

}
