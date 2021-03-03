#include "schedule.hxx"
#include "elements/element_factory.hxx"

namespace aptk {

N_Shape::N_Shape(const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name,
    ElementFactory::make_predicate_setminus(
        sketch->problem(),
        false,
        ElementFactory::make_predicate_extraction(sketch->problem(), true, "shape"),
        ElementFactory::make_predicate_intersection(
            sketch->problem(),
            false,
            ElementFactory::make_predicate_extraction(sketch->problem(), true, "shape"),
            ElementFactory::make_predicate_extraction(sketch->problem(), false, "shape")))) {
}

N_Surface::N_Surface(const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name,
    ElementFactory::make_predicate_setminus(
        sketch->problem(),
        false,
        ElementFactory::make_predicate_extraction(sketch->problem(), true, "surface-condition"),
        ElementFactory::make_predicate_intersection(
            sketch->problem(),
            false,
            ElementFactory::make_predicate_extraction(sketch->problem(), true, "surface-condition"),
            ElementFactory::make_predicate_extraction(sketch->problem(), false, "surface-condition")))) {
}

N_Color::N_Color(const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name,
    ElementFactory::make_predicate_setminus(
        sketch->problem(),
        false,
        ElementFactory::make_predicate_extraction(sketch->problem(), true, "painted"),
        ElementFactory::make_predicate_intersection(
            sketch->problem(),
            false,
            ElementFactory::make_predicate_extraction(sketch->problem(), true, "painted"),
            ElementFactory::make_predicate_extraction(sketch->problem(), false, "painted")))) {
}

ScheduleSketch::ScheduleSketch(
    const Sketch_STRIPS_Problem *problem) : BaseSketch(problem) {

    add_numerical_feature(new N_Shape(this, "shape"));
    add_numerical_feature(new N_Surface(this, "surface"));
    add_numerical_feature(new N_Color(this, "paint"));
    add_rule(new Rule(this, "shape_obj",
        {},
        { new NonzeroNumerical(get_numerical_feature("shape")) },
        {},
        { new DecrementNumerical(get_numerical_feature("shape"))}
    ));
    add_rule(new Rule(this, "surface_obj",
        {},
        { new NonzeroNumerical(get_numerical_feature("surface")) },
        {},
        { new DecrementNumerical(get_numerical_feature("surface"))}
    ));
    add_rule(new Rule(this, "paint_obj",
        {},
        { new NonzeroNumerical(get_numerical_feature("paint")) },
        {},
        { new DecrementNumerical(get_numerical_feature("paint"))}
    ));
}

}
