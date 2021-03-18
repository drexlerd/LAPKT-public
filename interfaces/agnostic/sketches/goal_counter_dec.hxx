
#ifndef __GOAL_COUNTER_DEC__
#define __GOAL_COUNTER_DEC__

#include "sketch.hxx"
#include "reachability.hxx"
#include "../features/boolean_feature.hxx"
#include "../features/numerical_feature.hxx"

namespace aptk {
class GoalCounterSketch;

/**
 * Counts the number of unsatisfied goal atoms.
 */
class GoalCounterDecFeature : public NumericalFeature {
private:
    aptk::agnostic::Reachability_Test* m_reachability;
    bool m_consistency_test;

    // Store achieved goal atoms
    Fluent_Vec m_goals_achieved;
    Fluent_Vec m_goal_candidates;

	// initial number of goal candidates
	unsigned m_initial_num_goal_candidates;

private:
    void exclude_actions( Bit_Set& excluded );

public:
    GoalCounterDecFeature(const BaseSketch* sketch, const std::string &name);
    virtual ~GoalCounterDecFeature() = default;

    virtual void evaluate(const State* state) override;

    virtual void print() const override;
};

/**
 * Sketch with single rule that decreases the number of achieved goal atoms.
 */
class GoalCounterDecSketch : public BaseSketch {
public:
    GoalCounterDecSketch(const Sketch_STRIPS_Problem *problem);
    virtual ~GoalCounterDecSketch() = default;
};

}

#endif
