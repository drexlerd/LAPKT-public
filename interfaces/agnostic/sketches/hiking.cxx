#include "hiking.hxx"
#include "elements/element_factory.hxx"

namespace aptk {

B_TentUpAtNextSubgoal::B_TentUpAtNextSubgoal(
    const BaseSketch* sketch, const std::string &name)
    : BooleanFeature(sketch, name,
        ElementFactory::make_concept_intersection(
            sketch->problem(),
            false,
            ElementFactory::get_concept_custom("p"),
            ElementFactory::make_concept_existential_abstraction(
                sketch->problem(),
                false,
                ElementFactory::make_role_composition(
                    sketch->problem(),
                    false,
                    ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "walked"), 0, 1),
                    ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "next"), 0, 1)),
                ElementFactory::make_concept_existential_abstraction(
                    sketch->problem(),
                    false,
                    ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "at_tent"), 1, 0),
                    ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "up"), 0))))) {
}

B_TentAvailableAtNextSubgoal::B_TentAvailableAtNextSubgoal(
    const BaseSketch* sketch, const std::string &name)
    : BooleanFeature(sketch, name,
        ElementFactory::make_concept_intersection(
            sketch->problem(),
            false,
            ElementFactory::get_concept_custom("p"),
            ElementFactory::make_concept_existential_abstraction(
                sketch->problem(),
                false,
                ElementFactory::make_role_composition(
                    sketch->problem(),
                    false,
                    ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "walked"), 0, 1),
                    ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "next"), 0, 1)),
                ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "at_tent"), 1)))) {
}

N_TentsUpAtCurrentLocation::N_TentsUpAtCurrentLocation(
    const BaseSketch* sketch, const std::string &name) :
    NumericalFeature(sketch, name,
    ElementFactory::make_concept_existential_abstraction(
        sketch->problem(),
        false,
        ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "walked"), 0, 1),
        ElementFactory::make_concept_existential_abstraction(
            sketch->problem(),
            false,
            ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "at_tent"), 1, 0),
            ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "up"), 0)))) {
}

B_CarAtPlaceWithTentUp::B_CarAtPlaceWithTentUp(
    const BaseSketch* sketch, const std::string &name)
    : BooleanFeature(sketch, name,
        ElementFactory::make_concept_intersection(
            sketch->problem(),
            false,
            ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "at_car"), 1),
            ElementFactory::make_concept_existential_abstraction(
                sketch->problem(),
                false,
                ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "at_tent"), 1, 0),
                ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "up"), 0)))) {
}

B_AtLeastOneAtInitial::B_AtLeastOneAtInitial(
    const BaseSketch* sketch, const std::string &name)
    : BooleanFeature(sketch, name,
        ElementFactory::make_concept_intersection(
            sketch->problem(),
            false,
            ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "walked"), 0),
            ElementFactory::make_concept_role_value_subset(
                sketch->problem(),
                false,
                ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "walked"), 0, 1),
                ElementFactory::make_role_composition(
                    sketch->problem(),
                    false,
                    ElementFactory::make_role_union(
                        sketch->problem(),
                        false,
                        ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "partners"), 0, 1),
                        ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "partners"), 0, 2)),
                    ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "at_person"), 0, 1))))) {
}

B_BothAtInitial::B_BothAtInitial(
    const BaseSketch* sketch, const std::string &name)
    : BooleanFeature(sketch, name,
        ElementFactory::make_concept_intersection(
            sketch->problem(),
            false,
            ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "walked"), 0),
            ElementFactory::make_concept_role_value_equality(
                sketch->problem(),
                false,
                ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "walked"), 0, 1),
                ElementFactory::make_role_composition(
                    sketch->problem(),
                    false,
                    ElementFactory::make_role_union(
                        sketch->problem(),
                        false,
                        ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "partners"), 0, 1),
                        ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "partners"), 0, 2)),
                    ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "at_person"), 0, 1))))) {
}

SD_RemainingHikes::SD_RemainingHikes(const BaseSketch* sketch, const std::string &name)
    : SumDistanceFeature(sketch, name,
        ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "walked"), 0, 1),
        ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "next"), 0, 1),
        ElementFactory::make_role_extraction(sketch->problem(), true, ElementFactory::make_predicate_extraction(sketch->problem(), true, "walked"), 0, 1)) {
}

HikingSketch::HikingSketch(
    const Sketch_STRIPS_Problem *problem) : BaseSketch(problem) {
    ElementFactory::add_concept_custom("p",
        ElementFactory::make_concept_extraction(
            problem,
            false,
            ElementFactory::make_predicate_setminus(
                problem,
                false,
                ElementFactory::make_predicate_extraction(problem, true, "walked"),
                ElementFactory::make_predicate_extraction(problem, false, "walked")),
        0));

    // add_numerical_feature(new N_CouplesNotAtGoalLocation(this, "couples_not_at_goal_location"));
    add_boolean_feature(new B_TentUpAtNextSubgoal(this, "tent_up_at_next_subgoal"));
    add_boolean_feature(new B_TentAvailableAtNextSubgoal(this, "tent_available_at_next_subgoal"));
    add_boolean_feature(new B_CarAtPlaceWithTentUp(this, "car_at_next_subgoal"));
    add_boolean_feature(new B_BothAtInitial(this, "both_at_initial"));
    add_boolean_feature(new B_AtLeastOneAtInitial(this, "one_at_initial"));
    add_numerical_feature(new SD_RemainingHikes(this, "remaining_hikes"));
    add_numerical_feature(new N_TentsUpAtCurrentLocation(this, "downable_tents"));
    // r1
    add_rule(new Rule(this, "down_tent",
        {},
        { new NonzeroNumerical(get_numerical_feature("downable_tents")) },
        {},
        { new DecrementNumerical(get_numerical_feature("downable_tents")) }
    ));
    // r2 move first person with tent
    add_rule(new Rule(this, "move_tent",
        { new NegativeBoolean(get_boolean_feature("tent_up_at_next_subgoal")),
          new NegativeBoolean(get_boolean_feature("tent_available_at_next_subgoal")),
          new PositiveBoolean(get_boolean_feature("both_at_initial")),
          new PositiveBoolean(get_boolean_feature("one_at_initial")) },
        {},
        { new ChangedPositiveBoolean(get_boolean_feature("tent_available_at_next_subgoal")),
          new UnchangedBoolean(get_boolean_feature("one_at_initial")),
          },
        {}
    ));
    // r3 build up the tent
    add_rule(new Rule(this, "up_tent",
        { new PositiveBoolean(get_boolean_feature("one_at_initial")),
          new PositiveBoolean(get_boolean_feature("tent_available_at_next_subgoal")),
          new NegativeBoolean(get_boolean_feature("tent_up_at_next_subgoal")) },
        {},
        { new UnchangedBoolean(get_boolean_feature("one_at_initial")),
          new UnchangedBoolean(get_boolean_feature("tent_available_at_next_subgoal")),
          new ChangedPositiveBoolean(get_boolean_feature("tent_up_at_next_subgoal")), },
        {}
    ));
    // r4 move second person
    add_rule(new Rule(this, "move_second",
        { new PositiveBoolean(get_boolean_feature("tent_up_at_next_subgoal")),
          new PositiveBoolean(get_boolean_feature("car_at_next_subgoal")),
          new PositiveBoolean(get_boolean_feature("one_at_initial")),
          new NegativeBoolean(get_boolean_feature("both_at_initial")) },
        {},
        { new UnchangedBoolean(get_boolean_feature("tent_up_at_next_subgoal")),
          new UnchangedBoolean(get_boolean_feature("car_at_next_subgoal")),
          new ChangedNegativeBoolean(get_boolean_feature("one_at_initial")),
          new UnchangedBoolean(get_boolean_feature("both_at_initial")) },
        {}
    ));
    // r5 regroup at initial location
    add_rule(new Rule(this, "regroup",
        { new PositiveBoolean(get_boolean_feature("tent_up_at_next_subgoal")),
          new PositiveBoolean(get_boolean_feature("car_at_next_subgoal")),
          new NegativeBoolean(get_boolean_feature("both_at_initial")) },
        {},
        { new UnchangedBoolean(get_boolean_feature("tent_up_at_next_subgoal")),
          new UnchangedBoolean(get_boolean_feature("car_at_next_subgoal")),
          new ChangedPositiveBoolean(get_boolean_feature("both_at_initial")) },
        {}
    ));
    // r6 walk to next location
    add_rule(new Rule(this, "walk",
        { new PositiveBoolean(get_boolean_feature("both_at_initial")) },
        { new NonzeroNumerical(get_numerical_feature("remaining_hikes")) },
        { new ChangedNegativeBoolean(get_boolean_feature("both_at_initial")) },
        { new DecrementNumerical(get_numerical_feature("remaining_hikes")) }
    ));
    // after walking bring car from previous location to current location?
    /*add_rule(new Rule(this, "move_car",
        { new PositiveBoolean(get_boolean_feature("tent_up_at_next_subgoal")),
          new NegativeBoolean(get_boolean_feature("car_at_next_subgoal")) },
        {},
        { new UnchangedBoolean(get_boolean_feature("tent_up_at_next_subgoal")),
          new ChangedPositiveBoolean(get_boolean_feature("car_at_next_subgoal")),},
        {}
    ));*/
}

}
