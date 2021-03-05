#include <sketch.hxx>
#include "../features/boolean_feature.hxx"
#include "../features/numerical_feature.hxx"
#include "../features/numerical/sum_distance.hxx"

namespace aptk {

class B_NextTentUp : public BooleanFeature {
public:
    B_NextTentUp(const BaseSketch* sketch, const std::string &name);
    virtual ~B_NextTentUp() = default;
};

class B_NextTentAvailable : public BooleanFeature {
public:
    B_NextTentAvailable(const BaseSketch* sketch, const std::string &name);
    virtual ~B_NextTentAvailable() = default;
};

class N_CurrentTentUp : public NumericalFeature {
public:
    N_CurrentTentUp(const BaseSketch* sketch, const std::string &name);
    virtual ~N_CurrentTentUp() = default;
};


class B_NextCar : public BooleanFeature {
public:
    B_NextCar(const BaseSketch* sketch, const std::string &name);
    virtual ~B_NextCar() = default;
};

class N_CurrentCar : public NumericalFeature {
public:
    N_CurrentCar(const BaseSketch* sketch, const std::string &name);
    virtual ~N_CurrentCar() = default;
};

class N_PreviousCar : public NumericalFeature {
public:
    N_PreviousCar(const BaseSketch* sketch, const std::string &name);
    virtual ~N_PreviousCar() = default;
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

// TODO(dominik): Select walked role in a clever way to move couples towards global goal.
class HikingSketch : public BaseSketch {
public:
    HikingSketch(const Sketch_STRIPS_Problem *problem);
    virtual ~HikingSketch() = default;
};

}
