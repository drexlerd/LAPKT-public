#include "goal_counter_dec.hxx"
#include "action.hxx"
#include "sketch_strips_prob.hxx"

namespace aptk {


// TODO(dominik): set BaseElement to nullptr?
GoalCounterDecFeature::GoalCounterDecFeature(const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name, nullptr),
    m_consistency_test(true),
    m_reachability(new aptk::agnostic::Reachability_Test( *(this->sketch()->problem()) )) {
    // initialize goal candidates
    this->m_goal_candidates = this->sketch()->problem()->goal();
    this->m_initial_num_goal_candidates = this->m_goal_candidates.size();
}


void GoalCounterDecFeature::evaluate(const State* state) {
    // 2. check if there is an additional goal atom that becomes true in s.
    m_goals_achieved.clear();
    for(Fluent_Vec::iterator it = m_goal_candidates.begin(); it != m_goal_candidates.end(); it++){
        if(  state->entails( *it ) )  // goal atom satisfied in s
        {
            m_goals_achieved.push_back( *it );
        }
    }
    // set the result value
    new_eval = m_initial_num_goal_candidates - m_goals_achieved.size();
}

void GoalCounterDecFeature::print() const {
    std::cout << m_name << ": " << new_eval << "\n";
}


GoalCounterDecSketch::GoalCounterDecSketch(const Sketch_STRIPS_Problem *problem) : BaseSketch(problem) {
    add_numerical_feature(new GoalCounterDecFeature(this, "goal_counter"));
    add_rule(new Rule(this, "decrement_unsatisified_goals",
        {},
        { new NonzeroNumerical(get_numerical_feature("goal_counter")), },
        {},
        { new DecrementNumerical(get_numerical_feature("goal_counter")), }
    ));
}


}