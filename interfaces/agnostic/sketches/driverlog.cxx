
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

B_Loaded::B_Loaded(const BaseSketch* sketch, const std::string &name)
    : BooleanFeature(sketch, name,
        ElementFactory::make_concept_intersection(
            sketch->problem(),
            false,
            ElementFactory::make_concept_intersection(
                sketch->problem(),
                false,
                ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "in"), 0),
                ElementFactory::make_concept_extraction(sketch->problem(), true, ElementFactory::make_predicate_extraction(sketch->problem(), true, "at"), 0)),
            ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "obj"), 0))) {
}

N_Driving::N_Driving(const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name,
        ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "driving"), 0)) {
}

N_DrivingGoalTruck::N_DrivingGoalTruck(const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name,
        ElementFactory::make_concept_existential_abstraction(
            sketch->problem(),
            false,
            ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "driving"), 0, 1),
            ElementFactory::make_concept_extraction(
                sketch->problem(),
                false,
                ElementFactory::make_role_restriction(
                    sketch->problem(),
                    false,
                    ElementFactory::make_role_extraction(
                        sketch->problem(),
                        false,
                        ElementFactory::make_predicate_setminus(
                            sketch->problem(),
                            false,
                            ElementFactory::make_predicate_extraction(sketch->problem(), true, "at"),
                            ElementFactory::make_predicate_extraction(sketch->problem(), false, "at")),
                        1,
                        0),
                    ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "truck"), 0)),
                1))) {
}

/*
MRD_DriverTruckDistance::MRD_DriverTruckDistance(const BaseSketch* sketch, const std::string &name)
    : MinConceptDistanceFeature(sketch, name,
    ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "driver"), 0),

    ) {
}
*/

SD_DriversSumDistance::SD_DriversSumDistance(
    const BaseSketch* sketch, const std::string &name)
    : SumDistanceFeature(sketch, name,
        ElementFactory::make_role_inverse(
            sketch->problem(),
            false,
            ElementFactory::make_role_restriction(
                sketch->problem(),
                false,
                ElementFactory::make_role_union(
                    sketch->problem(),
                    false,
                    ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "at"), 1, 0),
                    ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "driving"), 1, 0)),
                ElementFactory::make_concept_intersection(
                    sketch->problem(),
                    false,
                    ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "driver"), 0),
                    ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), true, "at"), 0)))),
        ElementFactory::get_role_custom("driver_graph"),
        ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), true, "at"), 0, 1)) {
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
    ElementFactory::add_role_custom("driver_graph",
        ElementFactory::make_role_union(
            problem,
            false,
            ElementFactory::make_role_extraction(problem, false, ElementFactory::make_predicate_extraction(problem, false, "at"), 0, 1),
            ElementFactory::make_role_union(
                problem,
                false,
                ElementFactory::make_role_extraction(problem, false, ElementFactory::make_predicate_extraction(problem, false, "at"), 1, 0),
                ElementFactory::make_role_extraction(problem, false, ElementFactory::make_predicate_extraction(problem, false, "path"), 0, 1))));
    add_numerical_feature(new N_PackagesNotAtGoalLocation(this, "packages_not_at_goal"));
    add_numerical_feature(new N_TrucksNotAtGoalLocation(this, "trucks_not_at_goal"));
    add_numerical_feature(new N_Driving(this, "driving"));
    add_numerical_feature(new N_DrivingGoalTruck(this, "driving_goal_truck"));
    add_boolean_feature(new B_Loaded(this, "loaded"));
    add_numerical_feature(new SD_DriversSumDistance(this, "drivers_sum_distance"));

    // r_1: board someone
    add_rule(new Rule(this, "board_someone",
        { },
        { new ZeroNumerical(get_numerical_feature("driving")),
          new NonzeroNumerical(get_numerical_feature("packages_not_at_goal")) },

        { new UnchangedBoolean(get_boolean_feature("loaded")) },
        { new UnchangedNumerical(get_numerical_feature("packages_not_at_goal")),
          new UnchangedNumerical(get_numerical_feature("trucks_not_at_goal")),
          new IncrementNumerical(get_numerical_feature("driving")), }
    ));
    // r_2: load package
    add_rule(new Rule(this, "load_package",
        { new NegativeBoolean(get_boolean_feature("loaded")) },
        { new NonzeroNumerical(get_numerical_feature("packages_not_at_goal")) },

        { new ChangedPositiveBoolean(get_boolean_feature("loaded")) },
        { new UnchangedNumerical(get_numerical_feature("packages_not_at_goal")),
        }
    ));
    // r_3: move package to goal
    add_rule(new Rule(this, "move_packages",
        { },
        { new NonzeroNumerical(get_numerical_feature("packages_not_at_goal")) },

        {},
        { new DecrementNumerical(get_numerical_feature("packages_not_at_goal")), }
    ));
    // r_4: board driver into truck not at goal location
    // TODO (dominik): this should decrease minimal distance of some driver
    add_rule(new Rule(this, "board_truck",
        { },
        { new ZeroNumerical(get_numerical_feature("packages_not_at_goal")),
          new ZeroNumerical(get_numerical_feature("driving_goal_truck")),
          new NonzeroNumerical(get_numerical_feature("trucks_not_at_goal")),},

        { new UnchangedBoolean(get_boolean_feature("loaded"))},
        { new IncrementNumerical(get_numerical_feature("driving_goal_truck")),
          new UnchangedNumerical(get_numerical_feature("packages_not_at_goal")),
          new UnchangedNumerical(get_numerical_feature("trucks_not_at_goal")),
        }
    ));
    // r_5: move truck to target location
    add_rule(new Rule(this, "move_trucks",
        { },
        { new ZeroNumerical(get_numerical_feature("packages_not_at_goal")),
          new NonzeroNumerical(get_numerical_feature("trucks_not_at_goal")) },

        { new UnchangedBoolean(get_boolean_feature("loaded"))},
        { new UnchangedNumerical(get_numerical_feature("packages_not_at_goal")),
          new DecrementNumerical(get_numerical_feature("trucks_not_at_goal")) }
    ));
    // r_7: move drivers stepwise
    add_rule(new Rule(this, "move_drivers_step",
        { },
        { new ZeroNumerical(get_numerical_feature("packages_not_at_goal")),
          new ZeroNumerical(get_numerical_feature("trucks_not_at_goal")),
          new NonzeroNumerical(get_numerical_feature("drivers_sum_distance")) },

        { new UnchangedBoolean(get_boolean_feature("loaded")) },
        { new UnchangedNumerical(get_numerical_feature("packages_not_at_goal")),
          new UnchangedNumerical(get_numerical_feature("trucks_not_at_goal")),
          new DecrementNumerical(get_numerical_feature("drivers_sum_distance")), }
    ));
}

}
