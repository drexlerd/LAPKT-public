#include "driverlog.hxx"
#include "elements/element_factory.hxx"

namespace aptk {

SD_PackagesSumDistance::SD_PackagesSumDistance(
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
                    ElementFactory::make_role_composition(
                        sketch->problem(),
                        false,
                        ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "at"), 1, 0),
                        ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "in"), 1, 0))),
                ElementFactory::make_concept_intersection(
                    sketch->problem(),
                    false,
                    ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "obj"), 0),
                    ElementFactory::make_concept_extraction(sketch->problem(), true, ElementFactory::make_predicate_extraction(sketch->problem(), true, "at"), 0)))),
        ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "link"), 0, 1),
        ElementFactory::make_role_extraction(sketch->problem(), true, ElementFactory::make_predicate_extraction(sketch->problem(), true, "at"), 0, 1)) {
}

SD_TrucksSumDistance::SD_TrucksSumDistance(
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
                    ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "truck"), 0),
                    ElementFactory::make_concept_extraction(sketch->problem(), true, ElementFactory::make_predicate_extraction(sketch->problem(), true, "at"), 0)))),
        ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "link"), 0, 1),
        ElementFactory::make_role_inverse(
            sketch->problem(),
            false,
            ElementFactory::make_role_restriction(
                sketch->problem(),
                false,
                ElementFactory::make_role_extraction(sketch->problem(), true, ElementFactory::make_predicate_extraction(sketch->problem(), true, "at"), 1, 0),
                ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "truck"), 0)))) {
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
                    ElementFactory::make_concept_extraction(sketch->problem(), true, ElementFactory::make_predicate_extraction(sketch->problem(), true, "at"), 0)))),
        ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "path"), 0, 1),
        ElementFactory::make_role_inverse(
            sketch->problem(),
            false,
            ElementFactory::make_role_restriction(
                sketch->problem(),
                false,
                ElementFactory::make_role_extraction(sketch->problem(), true, ElementFactory::make_predicate_extraction(sketch->problem(), true, "at"), 1, 0),
                ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "driver"), 0)))) {
}

N_LoadedPackages::N_LoadedPackages(const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name,
        ElementFactory::make_concept_intersection(
            sketch->problem(),
            false,
            ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "in"), 0),
            ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "obj"), 0))) {
}

N_Driving::N_Driving(const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name,
        ElementFactory::make_predicate_extraction(sketch->problem(), false, "driving")) {
}



DriverlogSketch::DriverlogSketch(
    const Sketch_STRIPS_Problem *problem) : BaseSketch(problem) {
    add_numerical_feature(new SD_PackagesSumDistance(this, "packages_sum_distance"));
    add_numerical_feature(new SD_TrucksSumDistance(this, "trucks_sum_distance"));
    add_numerical_feature(new SD_DriversSumDistance(this, "drivers_sum_distance"));
    add_numerical_feature(new N_LoadedPackages(this, "loaded"));
    add_numerical_feature(new N_Driving(this, "driving"));
    // r_1 move packages closer
    add_rule(new Rule(this, "move_packages_step",
        { },
        { new NonzeroNumerical(get_numerical_feature("packages_sum_distance")) },
        {},
        { new DecrementNumerical(get_numerical_feature("packages_sum_distance")) }
    ));
    // r_2 unload packages
    add_rule(new Rule(this, "unload_packages",
        { },
        { new ZeroNumerical(get_numerical_feature("packages_sum_distance")),
          new NonzeroNumerical(get_numerical_feature("loaded")) },
        {},
        { new DecrementNumerical(get_numerical_feature("loaded")) }
    ));
    // r_3 move trucks closer
    add_rule(new Rule(this, "move_trucks_step",
        { },
        { new ZeroNumerical(get_numerical_feature("packages_sum_distance")),
          new ZeroNumerical(get_numerical_feature("loaded")),
          new NonzeroNumerical(get_numerical_feature("trucks_sum_distance")) },
        {},
        { new DecrementNumerical(get_numerical_feature("trucks_sum_distance")) }
    ));
    // r_4 unboard drivers
    add_rule(new Rule(this, "move_drivers_step",
        { },
        { new ZeroNumerical(get_numerical_feature("packages_sum_distance")),
          new ZeroNumerical(get_numerical_feature("loaded")),
          new ZeroNumerical(get_numerical_feature("trucks_sum_distance")),
          new NonzeroNumerical(get_numerical_feature("driving")) },
        {},
        { new DecrementNumerical(get_numerical_feature("driving")),
          new UnchangedNumerical(get_numerical_feature("loaded")), }
    ));
    // r_5 move drivers closer
    add_rule(new Rule(this, "move_drivers_step",
        { },
        { new ZeroNumerical(get_numerical_feature("packages_sum_distance")),
          new ZeroNumerical(get_numerical_feature("loaded")),
          new ZeroNumerical(get_numerical_feature("trucks_sum_distance")),
          new ZeroNumerical(get_numerical_feature("driving")),
          new NonzeroNumerical(get_numerical_feature("drivers_sum_distance")) },
        {},
        { new DecrementNumerical(get_numerical_feature("drivers_sum_distance")),
          new UnchangedNumerical(get_numerical_feature("loaded")),
          new UnchangedNumerical(get_numerical_feature("driving")), }
    ));
}

}
