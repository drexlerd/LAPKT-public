#ifndef __BARMAN_SKETCH__
#define __BARMAN_SKETCH__

#include <sketch.hxx>

namespace aptk {
class BaseElement;

class N_UnachievedGoalAtoms : public NumericalFeature {
public:
    N_UnachievedGoalAtoms(const BaseSketch* sketch, const std::string &name);
    virtual ~N_UnachievedGoalAtoms() = default;
};

class N_DirtyShots : public NumericalFeature {
public:
    N_DirtyShots(const BaseSketch* sketch, const std::string &name);
    virtual ~N_DirtyShots() = default;
};

class B_CocktailsConsistentWithPart1 : public BooleanFeature {
public:
    B_CocktailsConsistentWithPart1(const BaseSketch* sketch, const std::string &name);
    virtual ~B_CocktailsConsistentWithPart1() = default;
};

class B_CocktailsConsistentWithPart2 : public BooleanFeature {
public:
    B_CocktailsConsistentWithPart2(const BaseSketch* sketch, const std::string &name);
    virtual ~B_CocktailsConsistentWithPart2() = default;
};

class BarmanSketch : public BaseSketch {
public:
    BarmanSketch(const Sketch_STRIPS_Problem *problem);
    virtual ~BarmanSketch() = default;
};

}

#endif
