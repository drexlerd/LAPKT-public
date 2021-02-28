#include "hiking.hxx"
#include "features/element_factory.hxx"

namespace aptk {

N_CouplesNotAtGoalLocation::N_CouplesNotAtGoalLocation(
    const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name,
        ElementFactory::get_custom("p")) {

}

N_CouplesNotAtGoalWithTentUpAtPreviousSubgoal::N_CouplesNotAtGoalWithTentUpAtPreviousSubgoal(
    const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name,
        ElementFactory::make_universal_abstraction(
            sketch->problem(),
            false,
            ElementFactory::make_role(sketch->problem(), false, "at_tent"),
            ElementFactory::make_concept(sketch->problem(), false, "up", 0),
            1,
            0)) {
}

N_CouplesNotAtGoalWithTentUpAtNextSubgoal::N_CouplesNotAtGoalWithTentUpAtNextSubgoal(
    const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name,
        ElementFactory::get_custom("p")) {
}

HikingSketch::HikingSketch(
    const Sketch_STRIPS_Problem *problem) : BaseSketch(problem) {
    ElementFactory::add_custom("p",
        ElementFactory::make_extraction(
            problem,
            false,
            ElementFactory::make_setminus(
                problem,
                false,
                ElementFactory::make_role(problem, true, "walked"),
                ElementFactory::make_role(problem, false, "walked")),
        0));

    add_numerical_feature(new N_CouplesNotAtGoalLocation(this, "couples_not_at_goal_location"));
    add_numerical_feature(new N_CouplesNotAtGoalWithTentUpAtPreviousSubgoal(this, "couples_with_tent_up_at_previous_subgoal"));
    add_rule(new Rule(this, "move_couples_towards_target",
        {},
        { new NonzeroNumerical(get_numerical_feature("couples_not_at_goal_location")) },
        {},
        { new DecrementNumerical(get_numerical_feature("couples_not_at_goal_location")) }
    ));
}

}
