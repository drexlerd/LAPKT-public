#include "hiking.hxx"
#include "elements/element_factory.hxx"

namespace aptk {

B_NextTentUp::B_NextTentUp(
    const BaseSketch* sketch, const std::string &name)
    : BooleanFeature(sketch, name,
        ElementFactory::make_concept_intersection(
        sketch->problem(),
        false,
        ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "up"), 0),
        ElementFactory::make_concept_existential_abstraction(
            sketch->problem(),
            false,
            ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "at_tent"), 0, 1),
            ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::get_role_custom("next_walked"), 1)))) {
}

B_NextTentAvailable::B_NextTentAvailable(
    const BaseSketch* sketch, const std::string &name)
    : BooleanFeature(sketch, name,
        ElementFactory::make_concept_existential_abstraction(
            sketch->problem(),
            false,
            ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "at_tent"), 0, 1),
            ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::get_role_custom("next_walked"), 1))) {
}

N_CurrentTentUp::N_CurrentTentUp(
    const BaseSketch* sketch, const std::string &name) :
    NumericalFeature(sketch, name,
    ElementFactory::make_concept_intersection(
        sketch->problem(),
        false,
        ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "up"), 0),
        ElementFactory::make_concept_existential_abstraction(
            sketch->problem(),
            false,
            ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "at_tent"), 0, 1),
            ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::get_role_custom("current_walked"), 1)))) {
}

B_NextCar::B_NextCar(const BaseSketch* sketch, const std::string &name)
    : BooleanFeature(sketch, name,
    ElementFactory::make_concept_existential_abstraction(
        sketch->problem(),
        false,
        ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "at_car"), 0, 1),
        ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::get_role_custom("next_walked"), 1))) {
}

N_CurrentCar::N_CurrentCar(const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name,
    ElementFactory::make_concept_existential_abstraction(
        sketch->problem(),
        false,
        ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "at_car"), 0, 1),
        ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::get_role_custom("current_walked"), 1))) {
}

N_PreviousCar::N_PreviousCar(const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name,
    ElementFactory::make_concept_existential_abstraction(
        sketch->problem(),
        false,
        ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "at_car"), 0, 1),
        ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::get_role_custom("previous_walked"), 1))) {
}

B_AtLeastOneAtInitial::B_AtLeastOneAtInitial(
    const BaseSketch* sketch, const std::string &name)
    : BooleanFeature(sketch, name,
        ElementFactory::make_concept_intersection(
            sketch->problem(),
            false,
            ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "walked"), 0),
            ElementFactory::make_concept_role_value_subset(
                sketch->problem(),
                false,
                ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "walked"), 0, 1),
                ElementFactory::get_role_custom("couple_place")))) {
}

B_BothAtInitial::B_BothAtInitial(
    const BaseSketch* sketch, const std::string &name)
    : BooleanFeature(sketch, name,
        ElementFactory::make_concept_intersection(
            sketch->problem(),
            false,
            ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "walked"), 0),
            ElementFactory::make_concept_role_value_equality(
                sketch->problem(),
                false,
                ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "walked"), 0, 1),
                ElementFactory::get_role_custom("couple_place")))) {
}

SD_RemainingHikes::SD_RemainingHikes(const BaseSketch* sketch, const std::string &name)
    : SumDistanceFeature(sketch, name,
        ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "walked"), 0, 1),
        ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "next"), 0, 1),
        ElementFactory::make_role_extraction(sketch->problem(), true, ElementFactory::make_predicate_extraction(sketch->problem(), true, "walked"), 0, 1)) {
}

HikingSketch::HikingSketch(
    const Sketch_STRIPS_Problem *problem) : BaseSketch(problem) {
    // walked current
    ElementFactory::add_role_custom("current_walked",
        ElementFactory::make_role_extraction(
            problem,
            false,
            ElementFactory::make_predicate_setminus(
                problem, false,
                ElementFactory::make_predicate_extraction(problem, false, "walked"),
                ElementFactory::make_predicate_extraction(problem, true, "walked")),
            0,
            1));
    // walked previous
    ElementFactory::add_role_custom("previous_walked",
        ElementFactory::make_role_composition(
            problem,
            false,
            ElementFactory::make_role_extraction(
                problem,
                false,
                ElementFactory::make_predicate_setminus(
                    problem, false,
                    ElementFactory::make_predicate_extraction(problem, false, "walked"),
                    ElementFactory::make_predicate_extraction(problem, true, "walked")),
                0,
                1),
            ElementFactory::make_role_extraction(problem, false, ElementFactory::make_predicate_extraction(problem, false, "next"), 1, 0)));
    // walked next
    ElementFactory::add_role_custom("next_walked",
        ElementFactory::make_role_composition(
            problem,
            false,
            ElementFactory::make_role_extraction(
                problem,
                false,
                ElementFactory::make_predicate_setminus(
                    problem, false,
                    ElementFactory::make_predicate_extraction(problem, false, "walked"),
                    ElementFactory::make_predicate_extraction(problem, true, "walked")),
                0,
                1),
            ElementFactory::make_role_extraction(problem, false, ElementFactory::make_predicate_extraction(problem, false, "next"), 0, 1)));
    // a role of the form (couple, place) for each person
    ElementFactory::add_role_custom("couple_place",
        ElementFactory::make_role_composition(
            problem,
            false,
            ElementFactory::make_role_union(
                problem,
                false,
                ElementFactory::make_role_extraction(problem, false, ElementFactory::make_predicate_extraction(problem, false, "partners"), 0, 1),
                ElementFactory::make_role_extraction(problem, false, ElementFactory::make_predicate_extraction(problem, false, "partners"), 0, 2)),
            ElementFactory::make_role_extraction(problem, false, ElementFactory::make_predicate_extraction(problem, false, "at_person"), 0, 1)));

    add_boolean_feature(new B_NextTentUp(this, "next_tent_up"));
    add_boolean_feature(new B_NextTentAvailable(this, "next_tent_available"));
    add_numerical_feature(new N_CurrentTentUp(this, "current_tent_up"));

    add_boolean_feature(new B_NextCar(this, "next_car"));
    add_numerical_feature(new N_CurrentCar(this, "current_car"));
    add_numerical_feature(new N_PreviousCar(this, "previous_car"));

    add_boolean_feature(new B_BothAtInitial(this, "both_at_initial"));
    add_boolean_feature(new B_AtLeastOneAtInitial(this, "one_at_initial"));
    add_numerical_feature(new SD_RemainingHikes(this, "remaining_hikes"));
    // r1
    add_rule(new Rule(this, "down_tent",
        {},
        { new NonzeroNumerical(get_numerical_feature("current_tent_up")) },
        {},
        { new DecrementNumerical(get_numerical_feature("current_tent_up")) }
    ));
    // r2 move first person with tent
    add_rule(new Rule(this, "move_tent",
        { new NegativeBoolean(get_boolean_feature("next_tent_up")),
          new NegativeBoolean(get_boolean_feature("next_tent_available")),
          new PositiveBoolean(get_boolean_feature("both_at_initial")),
          new PositiveBoolean(get_boolean_feature("one_at_initial")) },
        {},
        { new ChangedPositiveBoolean(get_boolean_feature("next_tent_available")),
          new UnchangedBoolean(get_boolean_feature("one_at_initial")),
          },
        {}
    ));
    // r3 build up the tent
    add_rule(new Rule(this, "up_tent",
        { new PositiveBoolean(get_boolean_feature("one_at_initial")),
          new PositiveBoolean(get_boolean_feature("next_tent_available")),
          new NegativeBoolean(get_boolean_feature("next_tent_up")) },
        {},
        { new UnchangedBoolean(get_boolean_feature("one_at_initial")),
          new UnchangedBoolean(get_boolean_feature("next_tent_available")),
          new ChangedPositiveBoolean(get_boolean_feature("next_tent_up")), },
        {}
    ));
    // r4 move second person
    add_rule(new Rule(this, "move_second",
        { new PositiveBoolean(get_boolean_feature("next_tent_up")),
          new PositiveBoolean(get_boolean_feature("next_car")),
          new PositiveBoolean(get_boolean_feature("one_at_initial")),
          new NegativeBoolean(get_boolean_feature("both_at_initial")) },
        {},
        { new UnchangedBoolean(get_boolean_feature("next_tent_up")),
          new UnchangedBoolean(get_boolean_feature("next_car")),
          new ChangedNegativeBoolean(get_boolean_feature("one_at_initial")),
          new UnchangedBoolean(get_boolean_feature("both_at_initial")) },
        {}
    ));
    // r5 regroup at initial location
    add_rule(new Rule(this, "regroup",
        { new PositiveBoolean(get_boolean_feature("next_tent_up")),
          new PositiveBoolean(get_boolean_feature("next_car")),
          new NegativeBoolean(get_boolean_feature("both_at_initial")) },
        {},
        { new UnchangedBoolean(get_boolean_feature("next_tent_up")),
          new UnchangedBoolean(get_boolean_feature("next_car")),
          new ChangedPositiveBoolean(get_boolean_feature("both_at_initial")) },
        {}
    ));
    // r6 walk to next location
    add_rule(new Rule(this, "walk",
        { new PositiveBoolean(get_boolean_feature("both_at_initial")) },
        { new NonzeroNumerical(get_numerical_feature("remaining_hikes")) },
        { new ChangedNegativeBoolean(get_boolean_feature("both_at_initial")) },
        { new DecrementNumerical(get_numerical_feature("remaining_hikes")) }
    ));
    // after walking bring car from previous location to current location?
    /*add_rule(new Rule(this, "move_car",
        { new PositiveBoolean(get_boolean_feature("next_tent_up")),
          new NegativeBoolean(get_boolean_feature("next_car")) },
        {},
        { new UnchangedBoolean(get_boolean_feature("next_tent_up")),
          new ChangedPositiveBoolean(get_boolean_feature("next_car")),},
        {}
    ));*/
}

}
