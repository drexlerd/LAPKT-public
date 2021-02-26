#include "barman.hxx"
#include "features/element_factory.hxx"
#include "features/concept.hxx"
#include "features/role.hxx"

namespace aptk {

N_UnachievedGoalAtoms::N_UnachievedGoalAtoms(const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name),
    m_unachieved_goal_atoms(
        ElementFactory::make_setminus(
            sketch->problem(),
            false,
            ElementFactory::make_role(sketch->problem(), true, "contains"),
            ElementFactory::make_intersect(
                sketch->problem(),
                false,
                ElementFactory::make_role(sketch->problem(), true, "contains"),
                ElementFactory::make_role(sketch->problem(), false, "contains"))
            )) {
}

void N_UnachievedGoalAtoms::evaluate(const State* state) {
    new_eval = m_unachieved_goal_atoms->evaluate(state).size();
    // m_unachieved_goal_atoms->print_role();
    std::cout << "unachieved goals: " << new_eval << std::endl;
}


N_IngredientsInShaker::N_IngredientsInShaker(const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name),
    m_ingredients_in_shaker(
        ElementFactory::make_existential_abstraction(
            sketch->problem(),
            false,
            ElementFactory::make_role(sketch->problem(), false, "contains"),
            ElementFactory::make_concept(sketch->problem(), false, "shaker-level", 0),
            1,
            0)) {
}

void N_IngredientsInShaker::evaluate(const State* state) {
    new_eval = m_ingredients_in_shaker->evaluate(state).size();
    // m_ingredients_in_shaker->print_concept();
}


N_DirtyShots::N_DirtyShots(const BaseSketch* sketch, const std::string &name)
  : NumericalFeature(sketch, name),
    m_dirty_shots(
        ElementFactory::make_setminus(
        sketch->problem(),
        false,
        ElementFactory::make_extraction(
            sketch->problem(),
            false,
            ElementFactory::make_setminus(
                sketch->problem(),
                false,
                ElementFactory::make_role(sketch->problem(), true, "contains"),
                ElementFactory::make_intersect(
                    sketch->problem(),
                    false,
                    ElementFactory::make_role(sketch->problem(), true, "contains"),
                    ElementFactory::make_role(sketch->problem(), false, "contains"))
                ),
            1),
        ElementFactory::make_concept(sketch->problem(), false, "clean", 0))) {

}

void N_DirtyShots::evaluate(const State* state) {
    new_eval = m_dirty_shots->evaluate(state).size();
    std::cout << "dirty_shots: " << new_eval << std::endl;
}

N_CocktailsConsistentWithPart1::N_CocktailsConsistentWithPart1(const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name) {
    BaseElement* c1 =
    ElementFactory::make_universal_abstraction(
        sketch->problem(),
        false,
        ElementFactory::make_role(sketch->problem(), false, "cocktail-part1"),
        ElementFactory::make_existential_abstraction(
            sketch->problem(),
            false,
            ElementFactory::make_role(sketch->problem(), false, "contains"),
            ElementFactory::make_concept(sketch->problem(), false, "shaker-level", 0),
            1,
            0),
        0,
        1);
    BaseElement* p =
    ElementFactory::make_extraction(
        sketch->problem(),
        false,
        ElementFactory::make_setminus(
            sketch->problem(),
            false,
            ElementFactory::make_role(sketch->problem(), true, "contains"),
            ElementFactory::make_intersect(
                sketch->problem(),
                false,
                ElementFactory::make_role(sketch->problem(), true, "contains"),
                ElementFactory::make_role(sketch->problem(), false, "contains"))
            ),
        1);
    m_cocktails_consistent_with_part_1 =
    ElementFactory::make_intersect(
        sketch->problem(),
        false,
        c1,
        p);
}

void N_CocktailsConsistentWithPart1::evaluate(const State* state) {
    new_eval = m_cocktails_consistent_with_part_1->evaluate(state).size();
    // m_cocktails_consistent_with_part_1->print_concept();
    //std::cout << new_eval << std::endl;
}


N_CocktailsConsistentWithPart2::N_CocktailsConsistentWithPart2(const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name) {
    BaseElement* c1 =
    ElementFactory::make_universal_abstraction(
        sketch->problem(),
        false,
        ElementFactory::make_role(sketch->problem(), false, "cocktail-part1"),
        ElementFactory::make_existential_abstraction(
            sketch->problem(),
            false,
            ElementFactory::make_role(sketch->problem(), false, "contains"),
            ElementFactory::make_concept(sketch->problem(), false, "shaker-level", 0),
            1,
            0),
        0,
        1);
    BaseElement* c2 =
    ElementFactory::make_universal_abstraction(
        sketch->problem(),
        false,
        ElementFactory::make_role(sketch->problem(), false, "cocktail-part2"),
        ElementFactory::make_existential_abstraction(
            sketch->problem(),
            false,
            ElementFactory::make_role(sketch->problem(), false, "contains"),
            ElementFactory::make_concept(sketch->problem(), false, "shaker-level", 0),
            1,
            0),
        0,
        1);
    BaseElement* p =
    ElementFactory::make_extraction(
        sketch->problem(),
        false,
        ElementFactory::make_setminus(
            sketch->problem(),
            false,
            ElementFactory::make_role(sketch->problem(), true, "contains"),
            ElementFactory::make_intersect(
                sketch->problem(),
                false,
                ElementFactory::make_role(sketch->problem(), true, "contains"),
                ElementFactory::make_role(sketch->problem(), false, "contains"))
            ),
        1);
    m_cocktails_consistent_with_part_2 =
    ElementFactory::make_intersect(
        sketch->problem(),
        false,
        c1,
        ElementFactory::make_intersect(
            sketch->problem(),
            false,
            c2,
            p));
}

void N_CocktailsConsistentWithPart2::evaluate(const State* state) {
    new_eval = m_cocktails_consistent_with_part_2->evaluate(state).size();
    // m_cocktails_consistent_with_part_2->print_concept();
    // std::cout << new_eval << std::endl;
}


BarmanSketch::BarmanSketch(
    const Sketch_STRIPS_Problem *problem) : BaseSketch(problem) {
    add_numerical_feature(new N_UnachievedGoalAtoms(this, "unachieved_goal_atoms"));
    add_numerical_feature(new N_IngredientsInShaker(this, "ingredients_in_shaker"));
    add_numerical_feature(new N_CocktailsConsistentWithPart1(this, "cocktails_consistent_with_part_1"));
    add_numerical_feature(new N_CocktailsConsistentWithPart2(this, "cocktails_consistent_with_part_2"));
    add_numerical_feature(new N_DirtyShots(this, "dirty_shots"));
    add_rule(new Rule(this, "fill_first_ingredient",
        {},
        { new NonzeroNumerical(get_numerical_feature("unachieved_goal_atoms")),
          new ZeroNumerical(get_numerical_feature("cocktails_consistent_with_part_1")) },
        { },
        { new IncrementNumerical(get_numerical_feature("cocktails_consistent_with_part_1")) }
    ));
    add_rule(new Rule(this, "fill_second_ingredient",
        {},
        { new NonzeroNumerical(get_numerical_feature("unachieved_goal_atoms")),
          new NonzeroNumerical(get_numerical_feature("cocktails_consistent_with_part_1")),
          new ZeroNumerical(get_numerical_feature("cocktails_consistent_with_part_2")) },
        {},
        { new IncrementNumerical(get_numerical_feature("cocktails_consistent_with_part_2")) }
    ));
    add_rule(new Rule(this, "clean_dirty_shots",
        {},
        { new NonzeroNumerical(get_numerical_feature("cocktails_consistent_with_part_1")),
          new NonzeroNumerical(get_numerical_feature("cocktails_consistent_with_part_2")),
          new NonzeroNumerical(get_numerical_feature("dirty_shots")) },
        {},
        { new DecrementNumerical(get_numerical_feature("dirty_shots")) }
    ));
    add_rule(new Rule(this, "serve_cocktail",
        {},
        { new NonzeroNumerical(get_numerical_feature("cocktails_consistent_with_part_1")),
          new NonzeroNumerical(get_numerical_feature("cocktails_consistent_with_part_2")),
          new NonzeroNumerical(get_numerical_feature("unachieved_goal_atoms")) },
        {},
        { new DecrementNumerical(get_numerical_feature("unachieved_goal_atoms")) }
    ));
    // TODO: maybe we should clean shots that are used
}

}
