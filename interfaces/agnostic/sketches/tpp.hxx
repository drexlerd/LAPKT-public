#include <sketch.hxx>
#include "../features/numerical_features/count.hxx"
#include "../features/boolean_features/nonempty.hxx"
#include "../features/numerical_features/sum_role_distance.hxx"

namespace aptk {

class N_Loaded : public CountFeature {
public:
    N_Loaded(const BaseSketch* sketch, const std::string &name);
    virtual ~N_Loaded() = default;
};

class SD_Remaining : public SumRoleDistanceFeature {
public:
    SD_Remaining(const BaseSketch* sketch, const std::string &name);
    virtual ~SD_Remaining() = default;
};


/*class N_Test : public NumericalFeature {
public:
    N_Test(const BaseSketch* sketch, const std::string &name);
    virtual ~N_Test() = default;
};*/


class TppSketch : public BaseSketch {
public:
    TppSketch(const Sketch_STRIPS_Problem *problem);
    virtual ~TppSketch() = default;
};

}
