#include "philosophers.hxx"
#include "../elements/element_factory.hxx"

namespace aptk {

N_NotInState3::N_NotInState3(const BaseSketch* sketch, const std::string &name)
    : CountFeature(sketch, name,
    ElementFactory::make_role_setminus(
        sketch->problem(),
        false,
        ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "at-process"), 0, 1),
        ElementFactory::make_role_restriction(
            sketch->problem(),
            false,
            ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "at-process"), 0, 1),
            ElementFactory::make_concept_extraction(sketch->problem(), "state-3")))) {
}

PhilosophersSketch::PhilosophersSketch(
    const Sketch_STRIPS_Problem *problem) : BaseSketch(problem) {
    add_numerical_feature(new N_NotInState3(this, "N_not_in_state_3"));
    add_rule(new Rule(this, "move_to_state_3",
      {},
      { new NonzeroNumerical(get_numerical_feature("N_not_in_state_3")) },
      {},
      { new DecrementNumerical(get_numerical_feature("N_not_in_state_3")) }
    ));
}

}
