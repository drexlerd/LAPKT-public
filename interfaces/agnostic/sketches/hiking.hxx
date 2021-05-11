#include <sketch.hxx>
#include "../features/boolean_features/nonempty.hxx"
#include "../features/numerical_features/count.hxx"
#include "../features/numerical_features/sum_role_distance.hxx"

namespace aptk {

/**
 * Tent features
 */
class B_NextTentUp : public NonemptyFeature {
public:
    B_NextTentUp(const BaseSketch* sketch, const std::string &name);
    virtual ~B_NextTentUp() = default;
};

class B_NextTentAvailable : public NonemptyFeature {
public:
    B_NextTentAvailable(const BaseSketch* sketch, const std::string &name);
    virtual ~B_NextTentAvailable() = default;
};

class N_CurrentTentUp : public CountFeature {
public:
    N_CurrentTentUp(const BaseSketch* sketch, const std::string &name);
    virtual ~N_CurrentTentUp() = default;
};

/**
 * Car features
 */
class B_PreviousCar : public NonemptyFeature {
public:
    B_PreviousCar(const BaseSketch* sketch, const std::string &name);
    virtual ~B_PreviousCar() = default;
};

class B_CurrentCar : public NonemptyFeature {
public:
    B_CurrentCar(const BaseSketch* sketch, const std::string &name);
    virtual ~B_CurrentCar() = default;
};

class B_NextCar : public NonemptyFeature {
public:
    B_NextCar(const BaseSketch* sketch, const std::string &name);
    virtual ~B_NextCar() = default;
};

class N_PreviousCar : public CountFeature {
public:
    N_PreviousCar(const BaseSketch* sketch, const std::string &name);
    virtual ~N_PreviousCar() = default;
};

class N_CurrentCar : public CountFeature {
public:
    N_CurrentCar(const BaseSketch* sketch, const std::string &name);
    virtual ~N_CurrentCar() = default;
};

class N_NextCar : public CountFeature {
public:
    N_NextCar(const BaseSketch* sketch, const std::string &name);
    virtual ~N_NextCar() = default;
};

/**
 * Person features
 */
// whether there is a couple with 1 person at previous and 1 person at current location.
class N_PreviousCurrentPerson : public CountFeature {
public:
    N_PreviousCurrentPerson(const BaseSketch* sketch, const std::string &name);
    virtual ~N_PreviousCurrentPerson() = default;
};

class N_CurrentNextPerson : public CountFeature {
public:
    N_CurrentNextPerson(const BaseSketch* sketch, const std::string &name);
    virtual ~N_CurrentNextPerson() = default;
};

// whether there is a couple with both persons at previous location
class N_PreviousPerson : public CountFeature {
public:
    N_PreviousPerson(const BaseSketch* sketch, const std::string &name);
    virtual ~N_PreviousPerson() = default;
};

// whether there is a couple with both persons at current location
class N_CurrentPerson : public CountFeature {
public:
    N_CurrentPerson(const BaseSketch* sketch, const std::string &name);
    virtual ~N_CurrentPerson() = default;
};

// whether there is a couple with both persons at next location
class N_NextPerson : public CountFeature {
public:
    N_NextPerson(const BaseSketch* sketch, const std::string &name);
    virtual ~N_NextPerson() = default;
};

/**
 * High level feature
 */
// High level features that counts number of remaining hikes.
class SD_RemainingHikes : public SumRoleDistanceFeature {
public:
    SD_RemainingHikes(const BaseSketch* sketch, const std::string &name);
    virtual ~SD_RemainingHikes() = default;
};

class N_Test : public CountFeature {
public:
    N_Test(const BaseSketch* sketch, const std::string &name);
    virtual ~N_Test() = default;
};


class HikingSketch : public BaseSketch {
public:
    HikingSketch(const Sketch_STRIPS_Problem *problem);
    virtual ~HikingSketch() = default;
};

}
