#include "schedule.hxx"
#include "elements/element_factory.hxx"

namespace aptk {

N_Shape::N_Shape(const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name,
    ElementFactory::make_role_setminus(
        sketch->problem(),
        false,
        ElementFactory::make_role_extraction(sketch->problem(), true, ElementFactory::make_predicate_extraction(sketch->problem(), true, "shape"), 0, 1),
        ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "shape"), 0, 1))) {
}

N_Surface::N_Surface(const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name,
    ElementFactory::make_role_setminus(
        sketch->problem(),
        false,
        ElementFactory::make_role_extraction(sketch->problem(), true, ElementFactory::make_predicate_extraction(sketch->problem(), true, "surface-condition"), 0, 1),
        ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "surface-condition"), 0, 1))) {
}

N_Color::N_Color(const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name,
    ElementFactory::make_role_setminus(
        sketch->problem(),
        false,
        ElementFactory::make_role_extraction(sketch->problem(), true, ElementFactory::make_predicate_extraction(sketch->problem(), true, "painted"), 0, 1),
        ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "painted"), 0, 1))) {
}

B_ScheduledOrBusy::B_ScheduledOrBusy(const BaseSketch* sketch, const std::string &name)
    : BooleanFeature(sketch, name,
    ElementFactory::make_concept_union(
      sketch->problem(),
      false,
      ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "scheduled"), 0),
      ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "busy"), 0))) {
}

N_Hot::N_Hot(const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name,
    ElementFactory::make_role_restriction(
        sketch->problem(),
        false,
        ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "temperature"), 0, 1),
        ElementFactory::make_concept_extraction(sketch->problem(), "hot"))) {
}

ScheduleSketch::ScheduleSketch(
    const Sketch_STRIPS_Problem *problem) : BaseSketch(problem) {

    add_numerical_feature(new N_Shape(this, "shape"));
    add_numerical_feature(new N_Surface(this, "surface"));
    add_numerical_feature(new N_Color(this, "paint"));
    add_numerical_feature(new N_Hot(this, "hot"));
    add_boolean_feature(new B_ScheduledOrBusy(this, "scheduled_or_busy"));

    add_rule(new Rule(this, "shape_obj",
        {},
        { new NonzeroNumerical(get_numerical_feature("shape")) },
        {},
        { new DecrementNumerical(get_numerical_feature("shape")),
          new UnchangedNumerical(get_numerical_feature("hot")) }
    ));
    add_rule(new Rule(this, "surface_obj",
        {},
        { new NonzeroNumerical(get_numerical_feature("surface")),
          new ZeroNumerical(get_numerical_feature("shape")) },
        {},
        { new UnchangedNumerical(get_numerical_feature("shape")),
          new DecrementNumerical(get_numerical_feature("surface")),
          new UnchangedNumerical(get_numerical_feature("hot")) }
    ));
    add_rule(new Rule(this, "paint_obj",
        {},
        { new NonzeroNumerical(get_numerical_feature("paint")),
          new ZeroNumerical(get_numerical_feature("surface")),
          new ZeroNumerical(get_numerical_feature("shape")) },
        {},
        { new UnchangedNumerical(get_numerical_feature("shape")),
          new UnchangedNumerical(get_numerical_feature("surface")),
          new DecrementNumerical(get_numerical_feature("paint")),
          new UnchangedNumerical(get_numerical_feature("hot"))}
    ));
    add_rule(new Rule(this, "do-time-step",
      { new PositiveBoolean(get_boolean_feature("scheduled_or_busy"))},
      {},
      { new ChangedNegativeBoolean(get_boolean_feature("scheduled_or_busy")) },
      { new UnchangedNumerical(get_numerical_feature("shape")),
        new UnchangedNumerical(get_numerical_feature("surface")),
        new UnchangedNumerical(get_numerical_feature("paint")),
        new UnchangedNumerical(get_numerical_feature("hot")) }
    ));
}

}
