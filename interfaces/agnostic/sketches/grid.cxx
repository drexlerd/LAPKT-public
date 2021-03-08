#include "grid.hxx"
#include "elements/element_factory.hxx"

namespace aptk {

N_LockedPlaces::N_LockedPlaces(const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name,
    ElementFactory::make_predicate_extraction(sketch->problem(), false, "locked")) {
}

N_KeysNotAtTarget::N_KeysNotAtTarget(const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name,
    ElementFactory::make_predicate_setminus(
        sketch->problem(),
        false,
        ElementFactory::make_predicate_extraction(sketch->problem(), true, "at"),
        ElementFactory::make_predicate_extraction(sketch->problem(), false, "at"))) {
}

GridSketch::GridSketch(
    const Sketch_STRIPS_Problem *problem) : BaseSketch(problem) {
    add_numerical_feature(new N_LockedPlaces(this, "locked_places"));
    add_numerical_feature(new N_KeysNotAtTarget(this, "keys_not_at_target"));

    add_rule(new Rule(this, "unlock_place",
        {},
        { new NonzeroNumerical(get_numerical_feature("locked_places")) },
        {},
        { new DecrementNumerical(get_numerical_feature("locked_places")) }
    ));
    add_rule(new Rule(this, "move_key_to_target",
        {},
        { new ZeroNumerical(get_numerical_feature("locked_places")),
          new NonzeroNumerical(get_numerical_feature("keys_not_at_target"))},
        {},
        { new DecrementNumerical(get_numerical_feature("keys_not_at_target")) }
    ));
}

}
