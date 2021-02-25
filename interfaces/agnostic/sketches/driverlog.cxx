#include "driverlog.hxx"
#include "features/element_factory.hxx"
#include "features/concept.hxx"
#include "features/role.hxx"

namespace aptk {

N_PackagesNotAtGoalLocation::N_PackagesNotAtGoalLocation(
    const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name),
    m_packages_not_at_goal_location(
        ElementFactory::make_intersect_concept(
            sketch->problem(),
            false,
            ElementFactory::make_leaf_concept(sketch->problem(), false, "obj", 0),
            ElementFactory::make_setminus_concept(sketch->problem(), false,
                ElementFactory::make_leaf_concept(sketch->problem(), true, "at", 0),
                ElementFactory::make_leaf_concept(sketch->problem(), false, "at", 0)))) {

}

void N_PackagesNotAtGoalLocation::evaluate(const State* state) {
    new_eval = m_packages_not_at_goal_location->evaluate(state).size();
    std::cout << "packages: " << new_eval << std::endl;
}

N_TrucksNotAtGoalLocation::N_TrucksNotAtGoalLocation(
    const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name),
    m_trucks_not_at_goal_location(
        ElementFactory::make_intersect_concept(
            sketch->problem(),
            false,
            ElementFactory::make_leaf_concept(sketch->problem(), false, "truck", 0),
            ElementFactory::make_setminus_concept(sketch->problem(), false,
                ElementFactory::make_leaf_concept(sketch->problem(), true, "at", 0),
                ElementFactory::make_leaf_concept(sketch->problem(), false, "at", 0)))) {

}

void N_TrucksNotAtGoalLocation::evaluate(const State* state) {
    new_eval = m_trucks_not_at_goal_location->evaluate(state).size();
    std::cout << "trucks: " << new_eval << std::endl;
}

N_DriversNotAtGoalLocation::N_DriversNotAtGoalLocation(
    const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name),
    m_drivers_not_at_goal_location(
        ElementFactory::make_intersect_concept(
            sketch->problem(),
            false,
            ElementFactory::make_leaf_concept(sketch->problem(), false, "driver", 0),
            ElementFactory::make_setminus_concept(sketch->problem(), false,
                ElementFactory::make_leaf_concept(sketch->problem(), true, "at", 0),
                ElementFactory::make_leaf_concept(sketch->problem(), false, "at", 0)))) {

}

void N_DriversNotAtGoalLocation::evaluate(const State* state) {
    new_eval = m_drivers_not_at_goal_location->evaluate(state).size();
    std::cout << "drivers: " << new_eval << std::endl;
}

DriverlogSketch::DriverlogSketch(
    const Sketch_STRIPS_Problem *problem) : BaseSketch(problem) {
    add_numerical_feature(new N_PackagesNotAtGoalLocation(this, "packages_not_at_goal"));
    add_numerical_feature(new N_TrucksNotAtGoalLocation(this, "trucks_not_at_goal"));
    add_numerical_feature(new N_DriversNotAtGoalLocation(this, "drivers_not_at_goal"));
    add_rule(new Rule(this, "move_packages",
        {},
        { new NonzeroNumerical(get_numerical_feature("packages_not_at_goal")) },
        {},
        { new DecrementNumerical(get_numerical_feature("packages_not_at_goal")) }
    ));
    add_rule(new Rule(this, "move_trucks",
        {},
        { new ZeroNumerical(get_numerical_feature("packages_not_at_goal")),
          new NonzeroNumerical(get_numerical_feature("trucks_not_at_goal")) },
        {},
        { new DecrementNumerical(get_numerical_feature("trucks_not_at_goal")) }
    ));
    add_rule(new Rule(this, "move_drivers",
        {},
        { new ZeroNumerical(get_numerical_feature("packages_not_at_goal")),
          new ZeroNumerical(get_numerical_feature("trucks_not_at_goal")),
          new NonzeroNumerical(get_numerical_feature("drivers_not_at_goal")) },
        {},
        { new DecrementNumerical(get_numerical_feature("drivers_not_at_goal")) }
    ));
}

}
