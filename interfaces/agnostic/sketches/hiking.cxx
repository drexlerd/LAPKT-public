#include "hiking.hxx"
#include "elements/element_factory.hxx"

namespace aptk {

N_CouplesNotAtGoalLocation::N_CouplesNotAtGoalLocation(
    const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name,
        ElementFactory::get_concept_custom("p")) {
}

B_CouplesNotAtGoalWithTentUpAtNextSubgoal::B_CouplesNotAtGoalWithTentUpAtNextSubgoal(
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

B_Test::B_Test(
    const BaseSketch* sketch, const std::string &name)
    : BooleanFeature(sketch, name,
        ElementFactory::make_role_composition(
                    sketch->problem(),
                    false,
                    ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "walked"), 0, 1),
                    ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "next"), 0, 1))) {
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
    add_boolean_feature(new B_CouplesNotAtGoalWithTentUpAtNextSubgoal(this, "tent_up_at_next_subgoal"));
    add_boolean_feature(new B_CarAtPlaceWithTentUp(this, "car_at_place_with_tent_up"));
    add_numerical_feature(new SD_RemainingHikes(this, "remaining_hikes"));
    add_rule(new Rule(this, "move_couples",
        {},
        { new NonzeroNumerical(get_numerical_feature("remaining_hikes")) },
        { new UnchangedBoolean(get_boolean_feature("car_at_place_with_tent_up")) },
        { new DecrementNumerical(get_numerical_feature("remaining_hikes")) }
    ));
    add_rule(new Rule(this, "move_tent",
        { new NegativeBoolean(get_boolean_feature("tent_up_at_next_subgoal")) },
        {},
        { new ChangedPositiveBoolean(get_boolean_feature("tent_up_at_next_subgoal")) },
        {}
    ));
    add_rule(new Rule(this, "move_car",
        { new PositiveBoolean(get_boolean_feature("tent_up_at_next_subgoal")),
          new NegativeBoolean(get_boolean_feature("car_at_place_with_tent_up")) },
        {},
        { new UnchangedBoolean(get_boolean_feature("tent_up_at_next_subgoal")),
          new ChangedPositiveBoolean(get_boolean_feature("car_at_place_with_tent_up")),},
        {}
    ));
}

}
