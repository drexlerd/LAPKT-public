#include "tpp.hxx"
#include "../elements/element_factory.hxx"

namespace aptk {

N_Loaded::N_Loaded(const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name,
    ElementFactory::make_concept_setminus(
        sketch->problem(),
        false,
        ElementFactory::get_concept_custom("needed_goods"),
        ElementFactory::make_concept_existential_abstraction(
            sketch->problem(),
            false,
            ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "loaded"), 0, 2),
            ElementFactory::get_concept_custom("nonempty_levels")))) {
}

SD_Remaining::SD_Remaining(const BaseSketch* sketch, const std::string &name)
    : SumDistanceFeature(sketch, name,
        ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "stored"), 0, 1),
        ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "next"), 1, 0),
        ElementFactory::make_role_extraction(sketch->problem(), true, ElementFactory::make_predicate_extraction(sketch->problem(), true, "stored"), 0, 1)) {
}


/*N_Test::N_Test(const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name,
    ElementFactory::get_concept_custom("nonempty_levels")) {
}*/


TppSketch::TppSketch(
    const Sketch_STRIPS_Problem *problem) : BaseSketch(problem) {

    ElementFactory::add_concept_custom("empty_level",
        ElementFactory::make_concept_minimal(problem, false,
        ElementFactory::make_concept_extraction(problem , false, ElementFactory::make_role_extraction(problem, false, ElementFactory::make_predicate_extraction(problem, false, "next"), 1, 0)),
        ElementFactory::make_role_extraction(problem, false, ElementFactory::make_predicate_extraction(problem, false, "next"), 1, 0)));
    ElementFactory::add_concept_custom("nonempty_levels",
        ElementFactory::make_concept_setminus(problem, false,
            ElementFactory::make_concept_extraction(problem , false, ElementFactory::make_role_extraction(problem, false, ElementFactory::make_predicate_extraction(problem, false, "next"), 1, 0)),
            ElementFactory::get_concept_custom("empty_level")));
    ElementFactory::add_concept_custom("needed_goods",
        ElementFactory::make_concept_extraction(
            problem,
            false,
            ElementFactory::make_predicate_setminus(
                problem,
                false,
                ElementFactory::make_predicate_extraction(problem, true, "stored"),
                ElementFactory::make_predicate_extraction(problem, false, "stored")),
            0));

    add_numerical_feature(new N_Loaded(this, "loaded"));
    add_numerical_feature(new SD_Remaining(this, "remaining"));
    // add_numerical_feature(new N_Test(this, "test"));
    // store remaining
    add_rule(new Rule(this, "store_good",
        {},
        { new NonzeroNumerical(get_numerical_feature("remaining")) },
        {},
        { new DecrementNumerical(get_numerical_feature("remaining")) }
    ));
    add_rule(new Rule(this, "load_truck",
        { },
        { new NonzeroNumerical(get_numerical_feature("remaining")), },
        {},
        { new DecrementNumerical(get_numerical_feature("loaded")) }
    ));
}

}
