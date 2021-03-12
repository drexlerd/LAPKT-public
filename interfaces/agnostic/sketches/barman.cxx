#include "barman.hxx"
#include "elements/element_factory.hxx"


namespace aptk {

N_UnachievedGoalAtoms::N_UnachievedGoalAtoms(
    const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(
        sketch,
        name,
        ElementFactory::get_role_custom("p")) {
}

N_DirtyShots::N_DirtyShots(const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(
        sketch,
        name,
        ElementFactory::make_concept_setminus(
            sketch->problem(),
            false,
            ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "used"), 0),
            ElementFactory::make_concept_extraction(
                sketch->problem(),
                false,
                ElementFactory::make_predicate_intersection(sketch->problem(), false,
                    ElementFactory::make_predicate_extraction(sketch->problem(), true, "contains"),
                    ElementFactory::make_predicate_extraction(sketch->problem(), false, "contains")),
                0))) {
}

B_CocktailsConsistentWithPart1::B_CocktailsConsistentWithPart1(const BaseSketch* sketch, const std::string &name)
    : BooleanFeature(
        sketch,
        name,
        ElementFactory::make_concept_intersection(
            sketch->problem(),
            false,
            ElementFactory::get_concept_custom("c1"),
            ElementFactory::make_concept_extraction(
                sketch->problem(),
                false,
                ElementFactory::get_role_custom("p"),
                1))) {
}

B_CocktailsConsistentWithPart2::B_CocktailsConsistentWithPart2(const BaseSketch* sketch, const std::string &name)
    : BooleanFeature(
        sketch,
        name,
        ElementFactory::make_concept_intersection(
            sketch->problem(),
            false,
            ElementFactory::get_concept_custom("c1"),
            ElementFactory::make_concept_intersection(
                sketch->problem(),
                false,
                ElementFactory::get_concept_custom("c2"),
                ElementFactory::make_concept_extraction(
                    sketch->problem(),
                    false,
                    ElementFactory::get_role_custom("p"),
                    1)))) {
}

/*N_DirtyShakers::N_DirtyShakers(const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(
        sketch,
        name,
        ElementFactory::make_concept_union(
            sketch->problem(),
            false,
            ElementFactory::make_concept_intersection(
                sketch->problem(),
                false,
                ElementFactory::make_concept_extraction(
                    sketch->problem(),
                    false,
                    ElementFactory::make_role_restriction(
                        sketch->problem(),
                        false,
                        ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "shaker-level"), 0, 1),
                        ElementFactory::make_concept_extraction(sketch->problem(), "l1")),
                    0),
                ElementFactory::make_concept_extraction(
                    sketch->problem(),
                    false,
                    ElementFactory::make_role_restriction(
                        sketch->problem(),
                        false,
                        ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "contains"), 0, 1),
                        ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "cocktail-part1"), 0)),
                    0)),
            ElementFactory::make_concept_setminus(
                sketch->problem(),
                false,
                ElementFactory::make_concept_extraction(
                    sketch->problem(),
                    false,
                    ElementFactory::make_role_restriction(
                        sketch->problem(),
                        false,
                        ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "shaker-level"), 0, 1),
                        ElementFactory::make_concept_extraction(sketch->problem(), "l0")),
                    0),
                ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "clean"), 0)))) {
}*/

BarmanSketch::BarmanSketch(
    const Sketch_STRIPS_Problem *problem) : BaseSketch(problem) {
    ElementFactory::add_concept_custom(
        "c1",
        ElementFactory::make_concept_universal_abstraction(
            problem,
            false,
            ElementFactory::make_role_extraction(problem, false, ElementFactory::make_predicate_extraction(problem, false, "cocktail-part1"), 0, 1),
            ElementFactory::make_concept_existential_abstraction(
                problem,
                false,
                ElementFactory::make_role_extraction(problem, false, ElementFactory::make_predicate_extraction(problem, false, "contains"), 1, 0),
                ElementFactory::make_concept_extraction(problem, false, ElementFactory::make_predicate_extraction(problem, false, "shaker-level"), 0)
            )
        )
    );
    ElementFactory::add_concept_custom(
        "c2",
        ElementFactory::make_concept_universal_abstraction(
            problem,
            false,
            ElementFactory::make_role_extraction(problem, false, ElementFactory::make_predicate_extraction(problem, false, "cocktail-part2"), 0, 1),
            ElementFactory::make_concept_existential_abstraction(
                problem,
                false,
                ElementFactory::make_role_extraction(problem, false, ElementFactory::make_predicate_extraction(problem, false, "contains"), 1, 0),
                ElementFactory::make_concept_extraction(problem, false, ElementFactory::make_predicate_extraction(problem, false, "shaker-level"), 0))));
    ElementFactory::add_role_custom(
        "p",
        ElementFactory::make_role_extraction(
            problem,
            false,
            ElementFactory::make_predicate_setminus(
                problem,
                false,
                ElementFactory::make_predicate_extraction(problem, true, "contains"),
                ElementFactory::make_predicate_extraction(problem, false, "contains")),
            0,
            1));

    /**
     * Features & rules for width <= 2
     */
    add_numerical_feature(new N_UnachievedGoalAtoms(this, "unachieved_goal_atoms"));
    add_numerical_feature(new N_DirtyShots(this, "dirty_shots"));
    add_boolean_feature(new B_CocktailsConsistentWithPart1(this, "cocktails_consistent_with_part_1"));
    add_boolean_feature(new B_CocktailsConsistentWithPart2(this, "cocktails_consistent_with_part_2"));
    /**
     * Additional features to decrease width
     */
    // add_numerical_feature(new N_DirtyShakers(this, "dirty_shakers"));

    add_rule(new Rule(this, "fill_first_ingredient",
        { new NegativeBoolean(get_boolean_feature("cocktails_consistent_with_part_1")), },
        { },

        { new ChangedPositiveBoolean(get_boolean_feature("cocktails_consistent_with_part_1")),
          new UnchangedBoolean(get_boolean_feature("cocktails_consistent_with_part_2")), },
        { }
    ));
    add_rule(new Rule(this, "fill_second_ingredient",
        { new PositiveBoolean(get_boolean_feature("cocktails_consistent_with_part_1")),
          new NegativeBoolean(get_boolean_feature("cocktails_consistent_with_part_2")) },
        { },

        { new UnchangedBoolean(get_boolean_feature("cocktails_consistent_with_part_1")),
          new ChangedPositiveBoolean(get_boolean_feature("cocktails_consistent_with_part_2")), },
        { new UnchangedNumerical(get_numerical_feature("unachieved_goal_atoms")) }
    ));
    add_rule(new Rule(this, "clean_dirty_shots",
        { },
        { new NonzeroNumerical(get_numerical_feature("dirty_shots")), },

        { },
        { new DecrementNumerical(get_numerical_feature("dirty_shots")) }
    ));
    add_rule(new Rule(this, "serve_cocktail",
        {},
        { new NonzeroNumerical(get_numerical_feature("unachieved_goal_atoms")) },

        { },
        { new DecrementNumerical(get_numerical_feature("unachieved_goal_atoms")),
          new UnchangedNumerical(get_numerical_feature("dirty_shots")), }
    ));

    /**
     * Additional rule to decrease width
     */
    // cocktail in shaker and level = 1 => clean shaker
    /*add_rule(new Rule(this, "clean_shaker",
        {},
        { new NonzeroNumerical(get_numerical_feature("dirty_shakers")) },
        {},

        { new UnchangedNumerical(get_numerical_feature("dirty_shots")),
          new UnchangedNumerical(get_numerical_feature("unachieved_goal_atoms")),
          new DecrementNumerical(get_numerical_feature("dirty_shakers")) }
    ));*/
}

}
