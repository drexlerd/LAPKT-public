#include "schedule.hxx"
#include "elements/element_factory.hxx"

namespace aptk {

N_Shape::N_Shape(const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name,
    ElementFactory::make_predicate_setminus(
        sketch->problem(),
        false,
        ElementFactory::make_predicate_extraction(sketch->problem(), true, "shape"),
        ElementFactory::make_predicate_extraction(sketch->problem(), false, "shape"))) {
}

N_Surface::N_Surface(const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name,
    ElementFactory::make_predicate_setminus(
        sketch->problem(),
        false,
        ElementFactory::make_predicate_extraction(sketch->problem(), true, "surface-condition"),
        ElementFactory::make_predicate_extraction(sketch->problem(), false, "surface-condition"))) {
}

N_Color::N_Color(const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name,
    ElementFactory::make_predicate_setminus(
        sketch->problem(),
        false,
        ElementFactory::make_predicate_extraction(sketch->problem(), true, "painted"),
        ElementFactory::make_predicate_extraction(sketch->problem(), false, "painted"))) {
}

B_Scheduled::B_Scheduled(const BaseSketch* sketch, const std::string &name)
    : BooleanFeature(sketch, name,
    ElementFactory::make_predicate_extraction(sketch->problem(), false, "scheduled")) {
}

B_Busy::B_Busy(const BaseSketch* sketch, const std::string &name)
    : BooleanFeature(sketch, name,
    ElementFactory::make_predicate_extraction(sketch->problem(), false, "busy")) {
}



ScheduleSketch::ScheduleSketch(
    const Sketch_STRIPS_Problem *problem) : BaseSketch(problem) {

    add_numerical_feature(new N_Shape(this, "shape"));
    add_numerical_feature(new N_Surface(this, "surface"));
    add_numerical_feature(new N_Color(this, "paint"));
    add_boolean_feature(new B_Scheduled(this, "scheduled"));
    add_boolean_feature(new B_Busy(this, "busy"));
    add_rule(new Rule(this, "do-time-step1",
      { new PositiveBoolean(get_boolean_feature("scheduled"))},
      {},
      { new ChangedNegativeBoolean(get_boolean_feature("scheduled")) },
      {}
    ));
    add_rule(new Rule(this, "do-time-step2",
      { new PositiveBoolean(get_boolean_feature("busy"))},
      {},
      { new ChangedNegativeBoolean(get_boolean_feature("busy")) },
      {}
    ));
    add_rule(new Rule(this, "shape_obj",
        {},
        { new NonzeroNumerical(get_numerical_feature("shape")) },
        {},
        { new DecrementNumerical(get_numerical_feature("shape"))}
    ));
    add_rule(new Rule(this, "surface_obj",
        {},
        { new NonzeroNumerical(get_numerical_feature("surface")),
          new ZeroNumerical(get_numerical_feature("shape")) },
        {},
        { new DecrementNumerical(get_numerical_feature("surface"))}
    ));
    add_rule(new Rule(this, "paint_obj",
        {},
        { new NonzeroNumerical(get_numerical_feature("paint")),
          new ZeroNumerical(get_numerical_feature("surface")),
          new ZeroNumerical(get_numerical_feature("shape")) },
        {},
        { new DecrementNumerical(get_numerical_feature("paint"))}
    ));
}

}
