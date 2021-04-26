#include "floortile.hxx"
#include "../elements/element_factory.hxx"

namespace aptk {


N_Unpainted::N_Unpainted(const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name,
    ElementFactory::get_concept_custom("unpainted_tiles")) {
}

B_InvariantPaintable::B_InvariantPaintable(const BaseSketch* sketch, const std::string &name)
    : BooleanFeature(sketch, name,
        ElementFactory::make_concept_setminus(
            sketch->problem(),
            false,
            ElementFactory::get_concept_custom("unpainted_tiles"),
            ElementFactory::make_concept_extraction(
                sketch->problem(),
                false,
                ElementFactory::make_role_composition(
                    sketch->problem(),
                    false,
                    ElementFactory::make_role_reflexive_transitive_closure(sketch->problem(), false, ElementFactory::get_role_custom("up_or_down_unpainted_to_unpainted")),
                    ElementFactory::get_role_custom("up_or_down_unpainted_to_normal")),
                0))) {
}


FloortileSketch::FloortileSketch(
    const Sketch_STRIPS_Problem *problem) : BaseSketch(problem) {
    ElementFactory::add_concept_custom("unpainted_tiles",
        ElementFactory::make_concept_extraction(
            problem,
            false,
            ElementFactory::make_role_setminus(
                problem,
                false,
                ElementFactory::make_role_extraction(problem, true, ElementFactory::make_predicate_extraction(problem, true, "painted"), 0, 1),
                ElementFactory::make_role_extraction(problem, false, ElementFactory::make_predicate_extraction(problem, false, "painted"), 0, 1)),
            0));
    ElementFactory::add_concept_custom("normal_tiles",
        ElementFactory::make_concept_setminus(
            problem,
            false,
            ElementFactory::make_concept_extraction(problem, false, ElementFactory::make_role_extraction(problem, false, ElementFactory::make_predicate_extraction(problem, false, "left"), 0, 1)),
            ElementFactory::make_concept_extraction(problem, true, ElementFactory::make_predicate_extraction(problem, true, "painted"), 0)));
    ElementFactory::add_role_custom("up_or_down",
        ElementFactory::make_role_union(
            problem,
            false,
            ElementFactory::make_role_extraction(problem, false, ElementFactory::make_predicate_extraction(problem, false, "up"), 0, 1),
            ElementFactory::make_role_extraction(problem, false, ElementFactory::make_predicate_extraction(problem, false, "down"), 0, 1)));
    ElementFactory::add_role_custom("up_or_down_or_same",
        ElementFactory::make_role_union(
            problem,
            false,
            ElementFactory::get_role_custom("up_or_down"),
            ElementFactory::make_role_identity(problem, false, ElementFactory::make_concept_extraction(problem, false, ElementFactory::make_role_extraction(problem, false, ElementFactory::make_predicate_extraction(problem, false, "down"), 0, 1)))));
    ElementFactory::add_role_custom("up_or_down_unpainted_to_unpainted",
        ElementFactory::make_role_inverse(
            problem,
            false,
            ElementFactory::make_role_restriction(
                problem,
                false,
                ElementFactory::make_role_inverse(
                    problem,
                    false,
                    ElementFactory::make_role_restriction(
                        problem,
                        false,
                        ElementFactory::get_role_custom("up_or_down_or_same"),
                        ElementFactory::get_concept_custom("unpainted_tiles"))),
                ElementFactory::get_concept_custom("unpainted_tiles"))));
    ElementFactory::add_role_custom("up_or_down_unpainted_to_normal",
        ElementFactory::make_role_inverse(
            problem,
            false,
            ElementFactory::make_role_restriction(
                problem,
                false,
                ElementFactory::make_role_inverse(
                    problem,
                    false,
                    ElementFactory::make_role_restriction(
                        problem,
                        false,
                        ElementFactory::get_role_custom("up_or_down"),
                        ElementFactory::get_concept_custom("normal_tiles"))),
                ElementFactory::get_concept_custom("unpainted_tiles"))));

    add_numerical_feature(new N_Unpainted(this, "unpainted"));
    add_boolean_feature(new B_InvariantPaintable(this, "invariant_paintable"));

    add_rule(new Rule(this, "goal_counter_with_invariant",
        { new NegativeBoolean(get_boolean_feature("invariant_paintable"))},
        { new NonzeroNumerical(get_numerical_feature("unpainted")) },
        { new UnchangedBoolean(get_boolean_feature("invariant_paintable")) },
        { new DecrementNumerical(get_numerical_feature("unpainted")) }
    ));
}

}
