#include "driverlog.hxx"
#include "elements/element_factory.hxx"

namespace aptk {

N_PackagesNotAtGoalLocation::N_PackagesNotAtGoalLocation(
    const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name,
        ElementFactory::make_concept_intersection(
            sketch->problem(),
            false,
            ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "obj"), 0),
            ElementFactory::get_concept_custom("not_at_goal"))) {
}

N_TrucksNotAtGoalLocation::N_TrucksNotAtGoalLocation(
    const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name,
        ElementFactory::make_concept_intersection(
            sketch->problem(),
            false,
            ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "truck"), 0),
            ElementFactory::get_concept_custom("not_at_goal"))) {
}

N_DriversNotAtGoalLocation::N_DriversNotAtGoalLocation(
    const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name,
        ElementFactory::make_concept_intersection(
            sketch->problem(),
            false,
            ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "driver"), 0),
            ElementFactory::get_concept_custom("not_at_goal"))) {
}

N_Driving::N_Driving(const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name,
        ElementFactory::make_predicate_extraction(sketch->problem(), false, "driving")) {
}

SD_DriversSumDistance::SD_DriversSumDistance(
    const BaseSketch* sketch, const std::string &name)
    : SumDistanceFeature(sketch, name,
        ElementFactory::make_role_inverse(
            sketch->problem(),
            false,
            ElementFactory::make_role_restriction(
                sketch->problem(),
                false,
                ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "at"), 1, 0),
                ElementFactory::make_concept_intersection(
                    sketch->problem(),
                    false,
                    ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "driver"), 0),
                    ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), true, "at"), 0)))),
        ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "path"), 0, 1),
        ElementFactory::make_role_inverse(
            sketch->problem(),
            false,
            ElementFactory::make_role_restriction(
                sketch->problem(),
                false,
                ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), true, "at"), 1, 0),
                ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "driver"), 0)))) {
}


DriverlogSketch::DriverlogSketch(
    const Sketch_STRIPS_Problem *problem) : BaseSketch(problem) {
    ElementFactory::add_concept_custom("not_at_goal",
        ElementFactory::make_concept_extraction(problem, false,
            ElementFactory::make_predicate_setminus(
                problem,
                false,
                ElementFactory::make_predicate_extraction(problem, true, "at"),
                ElementFactory::make_predicate_extraction(problem, false, "at")),
            0));
    add_numerical_feature(new N_PackagesNotAtGoalLocation(this, "packages_not_at_goal"));
    add_numerical_feature(new N_TrucksNotAtGoalLocation(this, "trucks_not_at_goal"));
    add_numerical_feature(new N_DriversNotAtGoalLocation(this, "drivers_not_at_goal"));
    add_numerical_feature(new N_Driving(this, "driving"));
    add_numerical_feature(new SD_DriversSumDistance(this, "drivers_sum_distance"));
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
        { new UnchangedNumerical(get_numerical_feature("packages_not_at_goal")),
          new DecrementNumerical(get_numerical_feature("trucks_not_at_goal")) }
    ));
    /*add_rule(new Rule(this, "move_drivers",
        {},
        { new ZeroNumerical(get_numerical_feature("packages_not_at_goal")),
          new ZeroNumerical(get_numerical_feature("trucks_not_at_goal")),
          new NonzeroNumerical(get_numerical_feature("drivers_not_at_goal")) },
        {},
        { new UnchangedNumerical(get_numerical_feature("packages_not_at_goal")),
          new UnchangedNumerical(get_numerical_feature("trucks_not_at_goal")),
          new DecrementNumerical(get_numerical_feature("drivers_not_at_goal")) }
    ));*/
    add_rule(new Rule(this, "unboard_drivers",
        {},
        { new ZeroNumerical(get_numerical_feature("packages_not_at_goal")),
          new ZeroNumerical(get_numerical_feature("trucks_not_at_goal")),
          new NonzeroNumerical(get_numerical_feature("driving")) },
        {},
        { new DecrementNumerical(get_numerical_feature("driving")) }
    ));
    add_rule(new Rule(this, "move_drivers_step",
        { },
        { new ZeroNumerical(get_numerical_feature("packages_not_at_goal")),
          new ZeroNumerical(get_numerical_feature("trucks_not_at_goal")),
          new ZeroNumerical(get_numerical_feature("driving")),
          new NonzeroNumerical(get_numerical_feature("drivers_sum_distance")) },
        {},
        { new DecrementNumerical(get_numerical_feature("drivers_sum_distance")),
          new UnchangedNumerical(get_numerical_feature("driving")) }
    ));
}

}
