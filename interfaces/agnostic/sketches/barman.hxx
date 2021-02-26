#include <sketch.hxx>

namespace aptk {
class BaseElement;

class N_UnachievedGoalAtoms : public NumericalFeature {
private:
    BaseElement* m_unachieved_goal_atoms;
public:
    N_UnachievedGoalAtoms(const BaseSketch* sketch, const std::string &name);
    virtual ~N_UnachievedGoalAtoms() = default;

    virtual void evaluate(const State* state) override;
};

class N_IngredientsInShaker : public NumericalFeature {
private:
    BaseElement* m_ingredients_in_shaker;
public:
    N_IngredientsInShaker(const BaseSketch* sketch, const std::string &name);
    virtual ~N_IngredientsInShaker() = default;

    virtual void evaluate(const State* state) override;
};

class N_CocktailsConsistentWithPart1 : public NumericalFeature {
private:
    BaseElement* m_cocktails_consistent_with_part_1;
public:
    N_CocktailsConsistentWithPart1(const BaseSketch* sketch, const std::string &name);
    virtual ~N_CocktailsConsistentWithPart1() = default;

    virtual void evaluate(const State* state) override;
};

class N_CocktailsConsistentWithPart2 : public NumericalFeature {
private:
    BaseElement* m_cocktails_consistent_with_part_2;
public:
    N_CocktailsConsistentWithPart2(const BaseSketch* sketch, const std::string &name);
    virtual ~N_CocktailsConsistentWithPart2() = default;

    virtual void evaluate(const State* state) override;
};

class BarmanSketch : public BaseSketch {
public:
    BarmanSketch(const Sketch_STRIPS_Problem *problem);
    virtual ~BarmanSketch() = default;
};

}
