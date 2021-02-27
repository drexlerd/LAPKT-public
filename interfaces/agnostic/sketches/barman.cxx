#include "barman.hxx"
#include "features/element_factory.hxx"
#include "features/concept.hxx"
#include "features/role.hxx"

namespace aptk {

N_UnachievedGoalAtoms::N_UnachievedGoalAtoms(const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name),
    m_unachieved_goal_atoms(
        ElementFactory::get_custom("p")) {
}

void N_UnachievedGoalAtoms::evaluate(const State* state) {
    new_eval = m_unachieved_goal_atoms->evaluate(state).size();
    //std::cout << m_name << ": ";
    //m_unachieved_goal_atoms->print_role();
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
    //std::cout << m_name << ": ";
    //m_ingredients_in_shaker->print_concept();
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
            ElementFactory::get_custom("p"),
            0),
        ElementFactory::make_concept(sketch->problem(), false, "clean", 0))) {

}

void N_DirtyShots::evaluate(const State* state) {
    new_eval = m_dirty_shots->evaluate(state).size();
    //std::cout << m_name << ": ";
    //m_dirty_shots->print_concept();
}

N_CocktailsConsistentWithPart1::N_CocktailsConsistentWithPart1(const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name) {
    m_cocktails_consistent_with_part_1 =
    ElementFactory::make_intersect(
        sketch->problem(),
        false,
        ElementFactory::get_custom("c1"),
        ElementFactory::make_extraction(
            sketch->problem(),
            false,
            ElementFactory::get_custom("p"),
            1));
}

void N_CocktailsConsistentWithPart1::evaluate(const State* state) {
    new_eval = m_cocktails_consistent_with_part_1->evaluate(state).size();
    //std::cout << m_name << ": ";
    //m_cocktails_consistent_with_part_1->print_concept();
}


N_CocktailsConsistentWithPart2::N_CocktailsConsistentWithPart2(const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name) {
    m_cocktails_consistent_with_part_2 =
    ElementFactory::make_intersect(
        sketch->problem(),
        false,
        ElementFactory::get_custom("c1"),
        ElementFactory::make_intersect(
            sketch->problem(),
            false,
            ElementFactory::get_custom("c2"),
            ElementFactory::make_extraction(
                sketch->problem(),
                false,
                ElementFactory::get_custom("p"),
                1)));
}

void N_CocktailsConsistentWithPart2::evaluate(const State* state) {
    new_eval = m_cocktails_consistent_with_part_2->evaluate(state).size();
    //std::cout << m_name << ": ";
    //m_cocktails_consistent_with_part_2->print_concept();
}


BarmanSketch::BarmanSketch(
    const Sketch_STRIPS_Problem *problem) : BaseSketch(problem) {
    ElementFactory::add_custom(
        "c1",
        ElementFactory::make_universal_abstraction(
            problem,
            false,
            ElementFactory::make_role(problem, false, "cocktail-part1"),
            ElementFactory::make_existential_abstraction(
                problem,
                false,
                ElementFactory::make_role(problem, false, "contains"),
                ElementFactory::make_concept(problem, false, "shaker-level", 0),
                1,
                0),
            0,
            1));
    ElementFactory::add_custom(
        "c2",
        ElementFactory::make_universal_abstraction(
            problem,
            false,
            ElementFactory::make_role(problem, false, "cocktail-part2"),
            ElementFactory::make_existential_abstraction(
                problem,
                false,
                ElementFactory::make_role(problem, false, "contains"),
                ElementFactory::make_concept(problem, false, "shaker-level", 0),
                1,
                0),
            0,
            1));
    ElementFactory::add_custom(
        "p",
        ElementFactory::make_setminus(
            problem,
            false,
            ElementFactory::make_role(problem, true, "contains"),
            ElementFactory::make_intersect(
                problem,
                false,
                ElementFactory::make_role(problem, true, "contains"),
                ElementFactory::make_role(problem, false, "contains"))));


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
        { new NonzeroNumerical(get_numerical_feature("unachieved_goal_atoms")) },
        {},
        { new DecrementNumerical(get_numerical_feature("unachieved_goal_atoms")) }
    ));
    // TODO: maybe we should clean shots that are used
}

}
