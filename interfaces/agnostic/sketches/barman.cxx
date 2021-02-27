#include "barman.hxx"
#include "features/element_factory.hxx"
#include "features/concept.hxx"
#include "features/role.hxx"

namespace aptk {

N_UnachievedGoalAtoms::N_UnachievedGoalAtoms(
    const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(
        sketch,
        name,
        ElementFactory::get_custom("p")) {
}

N_DirtyShots::N_DirtyShots(const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(
        sketch,
        name,
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

N_CocktailsConsistentWithPart1::N_CocktailsConsistentWithPart1(const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(
        sketch,
        name,
        ElementFactory::make_intersect(
            sketch->problem(),
            false,
            ElementFactory::get_custom("c1"),
            ElementFactory::make_extraction(
                sketch->problem(),
                false,
                ElementFactory::get_custom("p"),
                1))) {
}

N_CocktailsConsistentWithPart2::N_CocktailsConsistentWithPart2(const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(
        sketch,
        name,
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
                    1)))) {
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
}

}
