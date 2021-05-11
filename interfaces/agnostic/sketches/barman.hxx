#ifndef __BARMAN_SKETCH__
#define __BARMAN_SKETCH__

#include <sketch.hxx>
#include "../features/boolean_features/nonempty.hxx"
#include "../features/numerical_features/count.hxx"

namespace aptk {
class BaseElement;

class N_UnachievedGoalAtoms : public CountFeature {
public:
    N_UnachievedGoalAtoms(const BaseSketch* sketch, const std::string &name);
    virtual ~N_UnachievedGoalAtoms() = default;
};

class N_DirtyShots : public CountFeature {
public:
    N_DirtyShots(const BaseSketch* sketch, const std::string &name);
    virtual ~N_DirtyShots() = default;
};

class B_CocktailsConsistentWithPart1 : public NonemptyFeature {
public:
    B_CocktailsConsistentWithPart1(const BaseSketch* sketch, const std::string &name);
    virtual ~B_CocktailsConsistentWithPart1() = default;
};

class B_CocktailsConsistentWithPart2 : public NonemptyFeature {
public:
    B_CocktailsConsistentWithPart2(const BaseSketch* sketch, const std::string &name);
    virtual ~B_CocktailsConsistentWithPart2() = default;
};

/*class N_DirtyShakers : public NumericalFeature {
public:
    N_DirtyShakers(const BaseSketch* sketch, const std::string &name);
    virtual ~N_DirtyShakers() = default;
};*/

class BarmanSketch : public BaseSketch {
public:
    BarmanSketch(const Sketch_STRIPS_Problem *problem);
    virtual ~BarmanSketch() = default;
};

}

#endif
