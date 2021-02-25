#include "childsnack.hxx"
#include "features/element_factory.hxx"
#include "features/concept.hxx"
#include "features/role.hxx"

namespace aptk {

N_GlutenAllergicChildrenToBeServed::N_GlutenAllergicChildrenToBeServed(
    const BaseSketch* sketch,
    const std::string &name) : NumericalFeature(sketch, name),
    m_allergic_children_to_be_served(
        ElementFactory::make_intersect_concept(
        sketch->problem(),
        false,
        ElementFactory::make_leaf_concept(m_sketch->problem(), false, "allergic_gluten", 0),
        ElementFactory::make_setminus_concept(sketch->problem(), false,
            ElementFactory::make_leaf_concept(m_sketch->problem(), true, "served", 0),
            ElementFactory::make_leaf_concept(m_sketch->problem(), false, "served", 0)))) {
}

void N_GlutenAllergicChildrenToBeServed::evaluate(const State* state) {
    // TODO: remove duplicate code
    new_eval = m_allergic_children_to_be_served->evaluate(state).size();
}


N_RegularChildrenToBeServed::N_RegularChildrenToBeServed(
    const BaseSketch* sketch, const std::string &name) : NumericalFeature(sketch, name),
    m_regular_children_to_be_served(
        ElementFactory::make_intersect_concept(
        sketch->problem(),
        false,
        ElementFactory::make_leaf_concept(m_sketch->problem(), false, "not_allergic_gluten", 0),
        ElementFactory::make_setminus_concept(sketch->problem(), false,
            ElementFactory::make_leaf_concept(m_sketch->problem(), true, "served", 0),
            ElementFactory::make_leaf_concept(m_sketch->problem(), false, "served", 0)))) {
}

void N_RegularChildrenToBeServed::evaluate(const State* state) {
    // TODO: remove duplicate code
    new_eval = m_regular_children_to_be_served->evaluate(state).size();
}


B_GlutenFreeSandwichAtKitchen::B_GlutenFreeSandwichAtKitchen(
    const BaseSketch* sketch, const std::string &name) : BooleanFeature(sketch, name),
    m_gluten_free_sandwich_at_kitchen(
        ElementFactory::make_intersect_concept(
        m_sketch->problem(),
        false,
        ElementFactory::make_leaf_concept(m_sketch->problem(), false, "at_kitchen_sandwich", 0),
        ElementFactory::make_leaf_concept(m_sketch->problem(), false, "no_gluten_sandwich", 0))) {
}

void B_GlutenFreeSandwichAtKitchen::evaluate(const State* state) {
    const Bit_Set &result = m_gluten_free_sandwich_at_kitchen->evaluate(state);
    // TODO: remove duplicate code
    // size counts the number of set bits, i.e., the number of objects
    new_eval = (result.size() > 0) ? true : false;
}


B_RegularSandwichAtKitchen::B_RegularSandwichAtKitchen(
    const BaseSketch* sketch, const std::string &name) : BooleanFeature(sketch, name),
    m_regular_sandwich_at_kitchen(
        ElementFactory::make_setminus_concept(
        m_sketch->problem(),
        false,
        ElementFactory::make_leaf_concept(m_sketch->problem(), false, "at_kitchen_sandwich", 0),
        ElementFactory::make_leaf_concept(m_sketch->problem(), false, "no_gluten_sandwich", 0))) {
}

void B_RegularSandwichAtKitchen::evaluate(const State* state) {
    const Bit_Set &result = m_regular_sandwich_at_kitchen->evaluate(state);
    // TODO: remove duplicate code
    // size counts the number of set bits, i.e., the number of objects
    new_eval = (result.size() > 0) ? true : false;
}


B_GlutenFreeSandwichOnTray::B_GlutenFreeSandwichOnTray(
    const BaseSketch* sketch, const std::string &name) : BooleanFeature(sketch, name),
    m_gluten_free_sandwiches_on_tray(
        ElementFactory::make_intersect_concept(
        m_sketch->problem(),
        false,
        ElementFactory::make_leaf_concept(m_sketch->problem(), false, "ontray", 0),
        ElementFactory::make_leaf_concept(m_sketch->problem(), false, "no_gluten_sandwich", 0))) {
}

void B_GlutenFreeSandwichOnTray::evaluate(const State* state) {
    // TODO: remove duplicate code
    // size counts the number of set bits, i.e., the number of objects
    const Bit_Set &result = m_gluten_free_sandwiches_on_tray->evaluate(state);
    new_eval = (result.size() > 0) ? true : false;
}


B_RegularSandwichOnTray::B_RegularSandwichOnTray(
    const BaseSketch* sketch, const std::string &name) : BooleanFeature(sketch, name),
    m_regular_sandwiches_on_tray(
        ElementFactory::make_setminus_concept(
        m_sketch->problem(),
        false,
        ElementFactory::make_leaf_concept(m_sketch->problem(), false, "ontray", 0),
        ElementFactory::make_leaf_concept(m_sketch->problem(), false, "no_gluten_sandwich", 0))) {
}

void B_RegularSandwichOnTray::evaluate(const State* state) {
    // TODO: remove duplicate code
    // size counts the number of set bits, i.e., the number of objects
    const Bit_Set &result = m_regular_sandwiches_on_tray->evaluate(state);
    new_eval = (result.size() > 0) ? true : false;
}


ChildsnackSketch::ChildsnackSketch(
    const Sketch_STRIPS_Problem *problem) : BaseSketch(problem) {
    add_numerical_feature(new N_GlutenAllergicChildrenToBeServed(this, "gluten_allergic_children_to_be_served"));
    add_numerical_feature(new N_RegularChildrenToBeServed(this, "regular_children_to_be_served"));
    add_boolean_feature(new B_GlutenFreeSandwichAtKitchen(this, "gluten_free_sandwich_at_kitchen"));
    add_boolean_feature(new B_RegularSandwichAtKitchen(this, "regular_sandwich_at_kitchen"));
    add_boolean_feature(new B_GlutenFreeSandwichOnTray(this, "gluten_free_sandwiches_on_tray"));
    add_boolean_feature(new B_RegularSandwichOnTray(this, "regular_sandwiches_on_tray"));
    // r_1: produce gluten free sandwich
    add_rule(new Rule(this, "produce_gluten_free_sandwich",
        { new NegativeBoolean(get_boolean_feature("gluten_free_sandwich_at_kitchen")),
          new NegativeBoolean(get_boolean_feature("gluten_free_sandwiches_on_tray")), },
        { new NonzeroNumerical(get_numerical_feature("gluten_allergic_children_to_be_served")), },
        { new ChangedPositiveBoolean(get_boolean_feature("gluten_free_sandwich_at_kitchen")),},
        { }
    ));
    // r_2: produce regular sandwich
    add_rule(new Rule(this, "produce_regular_sandwich",
        { new NegativeBoolean(get_boolean_feature("regular_sandwich_at_kitchen")),
          new NegativeBoolean(get_boolean_feature("regular_sandwiches_on_tray")), },
        { new ZeroNumerical(get_numerical_feature("gluten_allergic_children_to_be_served")),
          new NonzeroNumerical(get_numerical_feature("regular_children_to_be_served")) },
        { new ChangedPositiveBoolean(get_boolean_feature("regular_sandwich_at_kitchen")), },
        { }
    ));
    // r_3: move gluten free sandwich on tray
    add_rule(new Rule(this, "move_gluten_free_sandwich_on_tray",
        { new PositiveBoolean(get_boolean_feature("gluten_free_sandwich_at_kitchen")),
          new NegativeBoolean(get_boolean_feature("gluten_free_sandwiches_on_tray")), },
        { new NonzeroNumerical(get_numerical_feature("gluten_allergic_children_to_be_served")), },
        { new ChangedPositiveBoolean(get_boolean_feature("gluten_free_sandwiches_on_tray")), },
        {}
    ));
    // r_4: move regular sandwich on tray
    add_rule(new Rule(this, "move_regular_sandwich_on_tray",
        { new PositiveBoolean(get_boolean_feature("regular_sandwich_at_kitchen")),
          new NegativeBoolean(get_boolean_feature("regular_sandwiches_on_tray")), },
        { new ZeroNumerical(get_numerical_feature("gluten_allergic_children_to_be_served")),
          new NonzeroNumerical(get_numerical_feature("regular_children_to_be_served")), },
        { new ChangedPositiveBoolean(get_boolean_feature("regular_sandwiches_on_tray")), },
        {}
    ));
    // r_5: serve gluten free sandwich
    add_rule(new Rule(this, "serve_gluten_free_sandwich",
        { new PositiveBoolean(get_boolean_feature("gluten_free_sandwiches_on_tray")), },
        { new NonzeroNumerical(get_numerical_feature("gluten_allergic_children_to_be_served")), },
        { new ChangedNegativeBoolean(get_boolean_feature("gluten_free_sandwiches_on_tray")), },
        { new DecrementNumerical(get_numerical_feature("gluten_allergic_children_to_be_served")), }
    ));
    // r_6: serve regular sandwich
    add_rule(new Rule(this, "serve_regular_sandwich",
        { new PositiveBoolean(get_boolean_feature("regular_sandwiches_on_tray")), },
        { new ZeroNumerical(get_numerical_feature("gluten_allergic_children_to_be_served")),
          new NonzeroNumerical(get_numerical_feature("regular_children_to_be_served")), },
        { new ChangedNegativeBoolean(get_boolean_feature("regular_sandwiches_on_tray")), },
        { new DecrementNumerical(get_numerical_feature("regular_children_to_be_served")), }
    ));
}

}
