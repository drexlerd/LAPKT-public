#include "tpp.hxx"
#include "../elements/element_factory.hxx"

namespace aptk {

B_Loadable::B_Loadable(const BaseSketch* sketch, const std::string &name)
    : BooleanFeature(sketch, name,
    ElementFactory::make_concept_intersection(
        sketch->problem(),
        false,
        ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "ready-to-load"), 0),
        ElementFactory::make_concept_extraction(
            sketch->problem(),
            false,
            ElementFactory::make_predicate_setminus(
                sketch->problem(),
                false,
                ElementFactory::make_predicate_extraction(sketch->problem(), true, "stored"),
                ElementFactory::make_predicate_extraction(sketch->problem(), false, "stored")),
            0))) {
}

N_Loaded::N_Loaded(const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name,
    ElementFactory::make_concept_intersection(
        sketch->problem(),
        false,
        ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "loaded"), 0),
        ElementFactory::make_concept_extraction(
            sketch->problem(),
            false,
            ElementFactory::make_predicate_setminus(
                sketch->problem(),
                false,
                ElementFactory::make_predicate_extraction(sketch->problem(), true, "stored"),
                ElementFactory::make_predicate_extraction(sketch->problem(), false, "stored")),
            0))) {
}

SD_Remaining::SD_Remaining(const BaseSketch* sketch, const std::string &name)
    : SumDistanceFeature(sketch, name,
        ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "stored"), 0, 1),
        ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "next"), 1, 0),
        ElementFactory::make_role_extraction(sketch->problem(), true, ElementFactory::make_predicate_extraction(sketch->problem(), true, "stored"), 0, 1)) {
}


TppSketch::TppSketch(
    const Sketch_STRIPS_Problem *problem) : BaseSketch(problem) {
    add_boolean_feature(new B_Loadable(this, "loadable"));
    add_numerical_feature(new N_Loaded(this, "loaded"));
    add_numerical_feature(new SD_Remaining(this, "remaining"));
    // store remaining
    add_rule(new Rule(this, "store_good",
        {},
        { new NonzeroNumerical(get_numerical_feature("loaded")),
          new NonzeroNumerical(get_numerical_feature("remaining")) },
        {},
        { new DecrementNumerical(get_numerical_feature("remaining")) }
    ));
    add_rule(new Rule(this, "load_truck",
        { new PositiveBoolean(get_boolean_feature("loadable")) },
        { new NonzeroNumerical(get_numerical_feature("remaining")), },
        {},
        { new IncrementNumerical(get_numerical_feature("loaded")) }
    ));
    add_rule(new Rule(this, "buy_good",
        { new NegativeBoolean(get_boolean_feature("loadable")) },
        { },
        { new ChangedPositiveBoolean(get_boolean_feature("loadable")) },
        { }
    ));
}

}
