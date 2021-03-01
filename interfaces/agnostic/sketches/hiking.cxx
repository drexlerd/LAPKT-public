#include "hiking.hxx"
#include "features/element_factory.hxx"

namespace aptk {

N_CouplesNotAtGoalLocation::N_CouplesNotAtGoalLocation(
    const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name,
        ElementFactory::get_custom("p")) {

}

B_CouplesNotAtGoalWithTentUpAtNextSubgoal::B_CouplesNotAtGoalWithTentUpAtNextSubgoal(
    const BaseSketch* sketch, const std::string &name)
    : BooleanFeature(sketch, name,
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
    add_boolean_feature(new B_CouplesNotAtGoalWithTentUpAtNextSubgoal(this, "couple_with_tent_up_at_next_subgoal"));
    add_rule(new Rule(this, "move_couples_towards_target",
        {},
        { new NonzeroNumerical(get_numerical_feature("couples_not_at_goal_location")) },
        {},
        { new DecrementNumerical(get_numerical_feature("couples_not_at_goal_location")) }
    ));
}

}
