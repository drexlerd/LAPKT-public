#include <sketch.hxx>

namespace aptk {
class BaseElement;

class N_UnachievedGoalAtoms : public NumericalFeature {
public:
    N_UnachievedGoalAtoms(const BaseSketch* sketch, const std::string &name);
    virtual ~N_UnachievedGoalAtoms() = default;
};

class N_IngredientsInShaker : public NumericalFeature {
public:
    N_IngredientsInShaker(const BaseSketch* sketch, const std::string &name);
    virtual ~N_IngredientsInShaker() = default;
};

/**
 * Count the number of shots that are needed in the goal
 * and are not clean
 */
class N_DirtyShots : public NumericalFeature {
public:
    N_DirtyShots(const BaseSketch* sketch, const std::string &name);
    virtual ~N_DirtyShots() = default;
};

class N_CocktailsConsistentWithPart1 : public NumericalFeature {
public:
    N_CocktailsConsistentWithPart1(const BaseSketch* sketch, const std::string &name);
    virtual ~N_CocktailsConsistentWithPart1() = default;
};

class N_CocktailsConsistentWithPart2 : public NumericalFeature {
public:
    N_CocktailsConsistentWithPart2(const BaseSketch* sketch, const std::string &name);
    virtual ~N_CocktailsConsistentWithPart2() = default;
};

class BarmanSketch : public BaseSketch {
public:
    BarmanSketch(const Sketch_STRIPS_Problem *problem);
    virtual ~BarmanSketch() = default;
};

}
