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

B_CurrentCar::B_CurrentCar(const BaseSketch* sketch, const std::string &name)
    : BooleanFeature(sketch, name,
    ElementFactory::make_concept_existential_abstraction(
        sketch->problem(),
        false,
        ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "at_car"), 0, 1),
        ElementFactory::make_concept_extraction(sketch->problem(), false, ElementFactory::get_role_custom("current_walked"), 1))) {
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

/*B_AtLeastOneAtInitial::B_AtLeastOneAtInitial(const BaseSketch* sketch, const std::string &name)
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

B_BothAtInitial::B_BothAtInitial(const BaseSketch* sketch, const std::string &name)
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
}*/

N_PreviousPerson::N_PreviousPerson(const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name,
    ElementFactory::make_concept_role_value_equality(
        sketch->problem(),
        false,
        ElementFactory::get_role_custom("couple_place"),
        ElementFactory::get_role_custom("previous_walked"))) {
}

N_CurrentPerson::N_CurrentPerson(const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name,
    ElementFactory::make_concept_role_value_equality(
        sketch->problem(),
        false,
        ElementFactory::get_role_custom("couple_place"),
        ElementFactory::get_role_custom("current_walked"))) {
}

N_NextPerson::N_NextPerson(const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name,
    ElementFactory::make_concept_role_value_equality(
        sketch->problem(),
        false,
        ElementFactory::get_role_custom("couple_place"),
        ElementFactory::get_role_custom("next_walked"))) {
}

N_PreviousCurrentPerson::N_PreviousCurrentPerson(const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name,
    ElementFactory::make_concept_role_value_equality(
        sketch->problem(),
        false,
        ElementFactory::get_role_custom("couple_place"),
        ElementFactory::make_role_union(
            sketch->problem(),
            false,
            ElementFactory::get_role_custom("previous_walked"),
            ElementFactory::get_role_custom("current_walked")))) {
}

N_CurrentNextPerson::N_CurrentNextPerson(const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name,
    ElementFactory::make_concept_role_value_equality(
        sketch->problem(),
        false,
        ElementFactory::get_role_custom("couple_place"),
        ElementFactory::make_role_union(
            sketch->problem(),
            false,
            ElementFactory::get_role_custom("current_walked"),
            ElementFactory::get_role_custom("next_walked")))) {
}

SD_RemainingHikes::SD_RemainingHikes(const BaseSketch* sketch, const std::string &name)
    : SumDistanceFeature(sketch, name,
        ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "walked"), 0, 1),
        ElementFactory::make_role_extraction(sketch->problem(), false, ElementFactory::make_predicate_extraction(sketch->problem(), false, "next"), 0, 1),
        ElementFactory::make_role_extraction(sketch->problem(), true, ElementFactory::make_predicate_extraction(sketch->problem(), true, "walked"), 0, 1)) {
}

N_Test::N_Test(const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name,
    ElementFactory::get_role_custom("min_current_walked")) {
}

HikingSketch::HikingSketch(
    const Sketch_STRIPS_Problem *problem) : BaseSketch(problem) {
    // walked minimum
    ElementFactory::add_role_custom("min_current_walked",
        ElementFactory::make_role_minimal(
            problem,
            false,
            ElementFactory::make_role_extraction(problem, false, ElementFactory::make_predicate_extraction(problem, false, "walked"), 1, 0),
            ElementFactory::make_role_extraction(problem, false, ElementFactory::make_predicate_extraction(problem, false, "next"), 0, 1)));
    // walked current
    ElementFactory::add_role_custom("current_walked",
        ElementFactory::make_role_extraction(
            problem,
            false,
            ElementFactory::make_predicate_extraction(problem, false, "walked"),
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
                ElementFactory::make_predicate_extraction(problem, false, "walked"),
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
                ElementFactory::make_predicate_extraction(problem, false, "walked"),
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
    add_boolean_feature(new B_CurrentCar(this, "current_car"));
    add_numerical_feature(new N_CurrentCar(this, "current_car"));
    add_numerical_feature(new N_PreviousCar(this, "previous_car"));

    add_numerical_feature(new N_PreviousPerson(this, "previous_person"));
    add_numerical_feature(new N_CurrentPerson(this, "current_person"));
    add_numerical_feature(new N_NextPerson(this, "next_person"));

    add_numerical_feature(new N_PreviousCurrentPerson(this, "previous_current_person"));
    add_numerical_feature(new N_CurrentNextPerson(this, "current_next_person"));

    add_numerical_feature(new SD_RemainingHikes(this, "remaining_hikes"));
    add_numerical_feature(new N_Test(this, "test"));

    // r1
    add_rule(new Rule(this, "down_tent",
        {},
        { new NonzeroNumerical(get_numerical_feature("current_tent_up")) },
        {},
        { new DecrementNumerical(get_numerical_feature("current_tent_up")) }
    ));
    // r2 move first person with tent (assuming both in the couple are at current location)
    // Ensures that we bring car with us while leaving one at the target
    add_rule(new Rule(this, "move_tent",
        { new NegativeBoolean(get_boolean_feature("next_tent_up")),
          new NegativeBoolean(get_boolean_feature("next_tent_available")),
          new PositiveBoolean(get_boolean_feature("current_car") )},
        { new NonzeroNumerical(get_numerical_feature("current_person")), },

        { new ChangedPositiveBoolean(get_boolean_feature("next_tent_available")),
          new UnchangedBoolean(get_boolean_feature("current_car")) },
        { new DecrementNumerical(get_numerical_feature("current_person")),
          new IncrementNumerical(get_numerical_feature("current_next_person"))
        }
    ));
    // r3 build up the tent
    add_rule(new Rule(this, "up_tent",
        { new PositiveBoolean(get_boolean_feature("next_tent_available")),
          new NegativeBoolean(get_boolean_feature("next_tent_up")) },
        { new NonzeroNumerical(get_numerical_feature("current_next_person")) },

        { new UnchangedBoolean(get_boolean_feature("next_tent_available")),
          new ChangedPositiveBoolean(get_boolean_feature("next_tent_up")), },
        { new UnchangedNumerical(get_numerical_feature("current_next_person")) }
    ));
    //r4 move second person
    add_rule(new Rule(this, "move_second",
        { new PositiveBoolean(get_boolean_feature("next_tent_up")),
          new PositiveBoolean(get_boolean_feature("next_car")), },
        { new NonzeroNumerical(get_numerical_feature("current_next_person")) },

        { new UnchangedBoolean(get_boolean_feature("next_tent_up")),
          new UnchangedBoolean(get_boolean_feature("next_car"))},
        { new DecrementNumerical(get_numerical_feature("current_next_person")),
          new IncrementNumerical(get_numerical_feature("next_person")) }
    ));

    // r5 regroup at initial location
    add_rule(new Rule(this, "regroup",
        { new PositiveBoolean(get_boolean_feature("next_tent_up")),
          new PositiveBoolean(get_boolean_feature("next_car")), },
        { new NonzeroNumerical(get_numerical_feature("next_person"))},

        { new UnchangedBoolean(get_boolean_feature("next_tent_up")),
          new UnchangedBoolean(get_boolean_feature("next_car")),},
        { new DecrementNumerical(get_numerical_feature("next_person")),
          new IncrementNumerical(get_numerical_feature("current_person"))}
    ));
    // r6 walk to next location
    add_rule(new Rule(this, "walk",
        { new PositiveBoolean(get_boolean_feature("next_tent_up")),
          new PositiveBoolean(get_boolean_feature("next_car")), },
        { new NonzeroNumerical(get_numerical_feature("remaining_hikes")),
          new NonzeroNumerical(get_numerical_feature("current_person")) },

        { },
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
