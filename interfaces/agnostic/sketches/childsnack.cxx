#include "childsnack.hxx"
#include "elements/element_factory.hxx"

namespace aptk {

N_GlutenAllergicChildrenToBeServed::N_GlutenAllergicChildrenToBeServed(
    const BaseSketch* sketch,
    const std::string &name)
    : CountFeature(sketch, name,
        ElementFactory::make_concept_intersection(
            sketch->problem(),
            false,
            ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "allergic_gluten"), 0),
            ElementFactory::make_concept_setminus(
                sketch->problem(),
                false,
                ElementFactory::make_concept_extraction(sketch->problem(), true, ElementFactory::make_predicate_extraction(sketch->problem(), true, "served"), 0),
                ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "served"), 0)))) {
}


N_RegularChildrenToBeServed::N_RegularChildrenToBeServed(
    const BaseSketch* sketch, const std::string &name)
    : CountFeature(sketch, name,
        ElementFactory::make_concept_intersection(
            sketch->problem(),
            false,
            ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "not_allergic_gluten"), 0),
            ElementFactory::make_concept_setminus(
                sketch->problem(),
                false,
                ElementFactory::make_concept_extraction(sketch->problem(), true, ElementFactory::make_predicate_extraction(sketch->problem(), true, "served"), 0),
                ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "served"), 0)))) {
}

B_GlutenFreeSandwichAtKitchen::B_GlutenFreeSandwichAtKitchen(
    const BaseSketch* sketch, const std::string &name)
    : NonemptyFeature(sketch, name,
    ElementFactory::make_concept_intersection(
        sketch->problem(),
        false,
        ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "at_kitchen_sandwich"), 0),
        ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "no_gluten_sandwich"), 0))) {
}

B_RegularSandwichAtKitchen::B_RegularSandwichAtKitchen(
    const BaseSketch* sketch, const std::string &name)
    : NonemptyFeature(sketch, name,
        ElementFactory::make_concept_setminus(
        sketch->problem(),
        false,
        ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "at_kitchen_sandwich"), 0),
        ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "no_gluten_sandwich"), 0))) {
}

B_GlutenFreeSandwichOnTray::B_GlutenFreeSandwichOnTray(
    const BaseSketch* sketch, const std::string &name)
    : NonemptyFeature(sketch, name,
        ElementFactory::make_concept_intersection(
            sketch->problem(),
            false,
            ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "ontray"), 0),
            ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "no_gluten_sandwich"), 0))) {
}

B_RegularSandwichOnTray::B_RegularSandwichOnTray(
    const BaseSketch* sketch, const std::string &name)
    : NonemptyFeature(sketch, name,
        ElementFactory::make_concept_setminus(
            sketch->problem(),
            false,
            ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "ontray"), 0),
            ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "no_gluten_sandwich"), 0))) {
}

B_SandwichOnTray::B_SandwichOnTray(
    const BaseSketch* sketch, const std::string &name)
    : NonemptyFeature(sketch, name,
        ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "ontray"), 0)) {
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

        { new ChangedPositiveBoolean(get_boolean_feature("gluten_free_sandwich_at_kitchen")),
          new UnchangedBoolean(get_boolean_feature("regular_sandwich_at_kitchen")),
          new UnchangedBoolean(get_boolean_feature("gluten_free_sandwiches_on_tray")),
          new UnchangedBoolean(get_boolean_feature("regular_sandwiches_on_tray")), },
        { new UnchangedNumerical(get_numerical_feature("gluten_allergic_children_to_be_served")),
          new UnchangedNumerical(get_numerical_feature("regular_children_to_be_served")) }
    ));
    // r_2: produce regular sandwich
    add_rule(new Rule(this, "produce_regular_sandwich",
        { new NegativeBoolean(get_boolean_feature("regular_sandwich_at_kitchen")),
          new NegativeBoolean(get_boolean_feature("regular_sandwiches_on_tray")), },
        { new ZeroNumerical(get_numerical_feature("gluten_allergic_children_to_be_served")),
          new NonzeroNumerical(get_numerical_feature("regular_children_to_be_served")) },

        { new UnchangedBoolean(get_boolean_feature("gluten_free_sandwich_at_kitchen")),
          new ChangedPositiveBoolean(get_boolean_feature("regular_sandwich_at_kitchen")),
          new UnchangedBoolean(get_boolean_feature("gluten_free_sandwiches_on_tray")),
          new UnchangedBoolean(get_boolean_feature("regular_sandwiches_on_tray")), },
        { new UnchangedNumerical(get_numerical_feature("gluten_allergic_children_to_be_served")),
          new UnchangedNumerical(get_numerical_feature("regular_children_to_be_served")) }
    ));
    // r_3: move gluten free sandwich on tray
    add_rule(new Rule(this, "move_gluten_free_sandwich_on_tray",
        { new PositiveBoolean(get_boolean_feature("gluten_free_sandwich_at_kitchen")),
          new NegativeBoolean(get_boolean_feature("gluten_free_sandwiches_on_tray")), },
        { },

        { new ChangedNegativeBoolean(get_boolean_feature("gluten_free_sandwich_at_kitchen")),
          new UnchangedBoolean(get_boolean_feature("regular_sandwich_at_kitchen")),
          new ChangedPositiveBoolean(get_boolean_feature("gluten_free_sandwiches_on_tray")),
          new UnchangedBoolean(get_boolean_feature("regular_sandwiches_on_tray")), },
        { new UnchangedNumerical(get_numerical_feature("gluten_allergic_children_to_be_served")),
          new UnchangedNumerical(get_numerical_feature("regular_children_to_be_served"))}
    ));
    // r_4: move regular sandwich on tray
    add_rule(new Rule(this, "move_regular_sandwich_on_tray",
        { new PositiveBoolean(get_boolean_feature("regular_sandwich_at_kitchen")),
          new NegativeBoolean(get_boolean_feature("regular_sandwiches_on_tray")), },
        { },

        { new UnchangedBoolean(get_boolean_feature("gluten_free_sandwich_at_kitchen")),
          new ChangedNegativeBoolean(get_boolean_feature("regular_sandwich_at_kitchen")),
          new UnchangedBoolean(get_boolean_feature("gluten_free_sandwiches_on_tray")),
          new ChangedPositiveBoolean(get_boolean_feature("regular_sandwiches_on_tray")), },
        { new UnchangedNumerical(get_numerical_feature("gluten_allergic_children_to_be_served")),
          new UnchangedNumerical(get_numerical_feature("regular_children_to_be_served")) }
    ));
    // r_5: serve gluten free sandwich
    add_rule(new Rule(this, "serve_gluten_free_sandwich",
        { new PositiveBoolean(get_boolean_feature("gluten_free_sandwiches_on_tray")), },
        { new NonzeroNumerical(get_numerical_feature("gluten_allergic_children_to_be_served")), },

        { new UnchangedBoolean(get_boolean_feature("gluten_free_sandwich_at_kitchen")),
          new UnchangedBoolean(get_boolean_feature("regular_sandwich_at_kitchen")),
          new ChangedNegativeBoolean(get_boolean_feature("gluten_free_sandwiches_on_tray")),
          new UnchangedBoolean(get_boolean_feature("regular_sandwiches_on_tray")), },
        { new DecrementNumerical(get_numerical_feature("gluten_allergic_children_to_be_served")),
          new UnchangedNumerical(get_numerical_feature("regular_children_to_be_served")) }
    ));
    // r_6: serve regular sandwich
    add_rule(new Rule(this, "serve_regular_sandwich",
        { new PositiveBoolean(get_boolean_feature("regular_sandwiches_on_tray")), },
        { new ZeroNumerical(get_numerical_feature("gluten_allergic_children_to_be_served")),
          new NonzeroNumerical(get_numerical_feature("regular_children_to_be_served")), },

        { new UnchangedBoolean(get_boolean_feature("gluten_free_sandwich_at_kitchen")),
          new UnchangedBoolean(get_boolean_feature("regular_sandwich_at_kitchen")),
          new UnchangedBoolean(get_boolean_feature("gluten_free_sandwiches_on_tray")),
          new ChangedNegativeBoolean(get_boolean_feature("regular_sandwiches_on_tray")), },
        { new UnchangedNumerical(get_numerical_feature("gluten_allergic_children_to_be_served")),
          new DecrementNumerical(get_numerical_feature("regular_children_to_be_served")), }
    ));
}


}
