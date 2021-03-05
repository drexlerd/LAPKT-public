#include <sketch.hxx>
#include "../features/boolean_feature.hxx"
#include "../features/numerical_feature.hxx"
#include "../features/numerical/sum_distance.hxx"

namespace aptk {

/**
 * Tent features
 */
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

/**
 * Car features
 */
class B_NextCar : public BooleanFeature {
public:
    B_NextCar(const BaseSketch* sketch, const std::string &name);
    virtual ~B_NextCar() = default;
};

class B_CurrentCar : public BooleanFeature {
public:
    B_CurrentCar(const BaseSketch* sketch, const std::string &name);
    virtual ~B_CurrentCar() = default;
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

/**
 * Person features
 */
/*class B_AtLeastOneAtInitial : public BooleanFeature {
public:
    B_AtLeastOneAtInitial(const BaseSketch* sketch, const std::string &name);
    virtual ~B_AtLeastOneAtInitial() = default;
};

class B_BothAtInitial : public BooleanFeature {
public:
    B_BothAtInitial(const BaseSketch* sketch, const std::string &name);
    virtual ~B_BothAtInitial() = default;
};
*/
// whether there is a couple with 1 person at previous and 1 person at current location.
class N_PreviousCurrentPerson : public NumericalFeature {
public:
    N_PreviousCurrentPerson(const BaseSketch* sketch, const std::string &name);
    virtual ~N_PreviousCurrentPerson() = default;
};

class N_CurrentNextPerson : public NumericalFeature {
public:
    N_CurrentNextPerson(const BaseSketch* sketch, const std::string &name);
    virtual ~N_CurrentNextPerson() = default;
};

// whether there is a couple with both persons at previous location
class N_PreviousPerson : public NumericalFeature {
public:
    N_PreviousPerson(const BaseSketch* sketch, const std::string &name);
    virtual ~N_PreviousPerson() = default;
};

// whether there is a couple with both persons at current location
class N_CurrentPerson : public NumericalFeature {
public:
    N_CurrentPerson(const BaseSketch* sketch, const std::string &name);
    virtual ~N_CurrentPerson() = default;
};

// whether there is a couple with both persons at next location
class N_NextPerson : public NumericalFeature {
public:
    N_NextPerson(const BaseSketch* sketch, const std::string &name);
    virtual ~N_NextPerson() = default;
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
