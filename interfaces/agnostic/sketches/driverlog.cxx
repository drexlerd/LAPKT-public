#include "driverlog.hxx"
#include "features/element_factory.hxx"
#include "features/concept.hxx"
#include "features/role.hxx"

namespace aptk {

N_PackagesNotAtGoalLocation::N_PackagesNotAtGoalLocation(
    const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name,
        ElementFactory::make_concept_setminus(
            sketch->problem(),
            false,
            ElementFactory::make_concept_intersection(
                sketch->problem(),
                false,
                ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "obj"), 0),
                ElementFactory::make_concept_extraction(sketch->problem(), true, ElementFactory::make_predicate_extraction(sketch->problem(), true, "at"), 0)),
            ElementFactory::make_concept_extraction(
                sketch->problem(),
                false,
                ElementFactory::make_role_intersection(sketch->problem(), false,
                    ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "at"), 0, 1),
                    ElementFactory::make_role_extraction(sketch->problem(), true, ElementFactory::make_predicate_extraction(sketch->problem(), true, "at"), 0, 1)),
                0))) {
}

N_TrucksNotAtGoalLocation::N_TrucksNotAtGoalLocation(
    const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name,
        ElementFactory::make_concept_setminus(
            sketch->problem(),
            false,
            ElementFactory::make_concept_intersection(
                sketch->problem(),
                false,
                ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "truck"), 0),
                ElementFactory::make_concept_extraction(sketch->problem(), true, ElementFactory::make_predicate_extraction(sketch->problem(), true, "at"), 0)),
            ElementFactory::make_concept_extraction(
                sketch->problem(),
                false,
                ElementFactory::make_role_intersection(sketch->problem(), false,
                    ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "at"), 0, 1),
                    ElementFactory::make_role_extraction(sketch->problem(), true, ElementFactory::make_predicate_extraction(sketch->problem(), true, "at"), 0, 1)),
                0))) {
}

N_DriversNotAtGoalLocation::N_DriversNotAtGoalLocation(
    const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name,
        ElementFactory::make_concept_setminus(
            sketch->problem(),
            false,
            ElementFactory::make_concept_intersection(
                sketch->problem(),
                false,
                ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "driver"), 0),
                ElementFactory::make_concept_extraction(sketch->problem(), true, ElementFactory::make_predicate_extraction(sketch->problem(), true, "at"), 0)),
            ElementFactory::make_concept_extraction(
                sketch->problem(),
                false,
                ElementFactory::make_role_intersection(sketch->problem(), false,
                    ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "at"), 0, 1),
                    ElementFactory::make_role_extraction(sketch->problem(), true, ElementFactory::make_predicate_extraction(sketch->problem(), true, "at"), 0, 1)),
                0))) {
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
        { new UnchangedNumerical(get_numerical_feature("packages_not_at_goal")),
          new DecrementNumerical(get_numerical_feature("trucks_not_at_goal")) }
    ));
    add_rule(new Rule(this, "move_drivers",
        {},
        { new ZeroNumerical(get_numerical_feature("packages_not_at_goal")),
          new ZeroNumerical(get_numerical_feature("trucks_not_at_goal")),
          new NonzeroNumerical(get_numerical_feature("drivers_not_at_goal")) },
        {},
        { new UnchangedNumerical(get_numerical_feature("packages_not_at_goal")),
          new UnchangedNumerical(get_numerical_feature("trucks_not_at_goal")),
          new DecrementNumerical(get_numerical_feature("drivers_not_at_goal")) }
    ));
}

}
