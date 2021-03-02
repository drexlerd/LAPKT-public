#include "hiking.hxx"
#include "features/element_factory.hxx"

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

    add_numerical_feature(new N_CouplesNotAtGoalLocation(this, "couples_not_at_goal_location"));
    add_boolean_feature(new B_CouplesNotAtGoalWithTentUpAtNextSubgoal(this, "tent_up_at_next_subgoal"));
    add_rule(new Rule(this, "move_couples_towards_target",
        {},
        { new NonzeroNumerical(get_numerical_feature("couples_not_at_goal_location")) },
        {},
        { new DecrementNumerical(get_numerical_feature("couples_not_at_goal_location")) }
    ));
    add_rule(new Rule(this, "set_up_tent_at_next_subgoal",
        { new NegativeBoolean(get_boolean_feature("tent_up_at_next_subgoal")) },
        {},
        { new ChangedPositiveBoolean(get_boolean_feature("tent_up_at_next_subgoal")) },
        {}
    ));
}

}
