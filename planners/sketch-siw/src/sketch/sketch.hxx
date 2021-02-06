#ifndef SKETCH_H
#define SKETCH_H

#include "elements.hxx"

#include <boost/python.hpp>

#include <string>
#include <vector>
#include <memory>

namespace sketch {

class Sketch {
private:
    std::vector<std::shared_ptr<DLBaseElement>> features;
    std::vector<int> rules;

    // Store the feature evaluation of the initial state
    // of the current subproblem
    std::vector<int> initial_state_evaluation;
    // Store the feature evaluation of a visited state.
    std::vector<int> visited_state_evaluation;

private:
    /**
     * Evaluate features for a given state.
     * This is executed for every visited state.
     * TODO: Information about predicates and objects
     * must be provided for each atom in the state.
     * We do not assume type information
     */
    void evaluate_features(const std::vector<int> &state, std::vector<int> &out_features) const;

    /**
     * When starting IW on a new subproblem
     * we determine applicable rules.
     * All other rules are irrelevant
     * for determining when next subproblem starts.
     */
    void compute_applicable_rules_for_initial_state_evaluation() const;

public:
    Sketch();

    /**
     * Add specific description logic element as state feature
     */
    void add_feature(int index);

    /**
     * Add a sketch rule
     */
    void add_rule();
    void add_precondition(int index, boost::python::list &lits);
    void add_effect(int index, boost::python::list &lits);

    /**
     * Initialize sketch for initial subproblem:
     * 1. Evaluate state features for initial state.
     * 2. Compute applicable rules for the initial state features
     */
    void on_enter_initial_subproblem(const std::vector<int> &state);

    /**
     * Evaluate sketch for visited state:
     * 1. Evaluate state features for visited state.
     * 2. If there exists a compatible rule:
     *    2.1. Set initial state features to current visited state features
     *         because they will remain constant during this subproblem.
     *    2.2. Compute applicable rules for the initial state features
     *         because they will remain constant during this subproblem.
     *    2.3. Return True to notify the caller about detected new subproblem.
     */
    bool on_visit_state(const std::vector<int> &state);
};

}

#endif
