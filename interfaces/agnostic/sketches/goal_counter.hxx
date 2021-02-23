
#ifndef __GOAL_COUNTER__
#define __GOAL_COUNTER__

#include "sketch.hxx"
#include "reachability.hxx"

namespace aptk {
class GoalCounterSketch;

/**
 * Counts the number of unsatisfied goal atoms.
 */
class GoalCounterFeature : public NumericalFeature {
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
    GoalCounterFeature(const BaseSketch* sketch, const std::string &name);
    virtual ~GoalCounterFeature() = default;

    virtual void evaluate(const State* state) override;
};

/**
 * Sketch with single rule that decreases the number of achieved goal atoms.
 */
class GoalCounterSketch : public BaseSketch {
public:
    GoalCounterSketch(const Sketch_STRIPS_Problem *problem);
    virtual ~GoalCounterSketch() = default;
};

}

#endif
