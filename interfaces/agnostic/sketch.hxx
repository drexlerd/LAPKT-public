
#include <types.hxx>
#include <vector>


/**
 * A BaseSketch provides an interface for sketches.
 * Concrete implementation depends on a specific domain
 * and provides functionality for evaluating features and rules.
 *
 * For efficient evaluation, this class allocates memory once
 * where evaluation results are stored and simply returns a references.
 */
class BaseSketch {
protected:
    // memory for initial state
    std::vector<int> init_applicable_rules;
    std::vector<int> init_numerical_features;
    std::vector<bool> init_boolean_features;

    // memory for generated state
    std::vector<int> gen_numerical_features;
    std::vector<bool> gen_boolean_features;

protected:
    /**
     * Evaluate features for a given state.
     */
    void evaluate_numerical_features();
    void evaluate_boolean_features();

    /**
     * Return true iff there exists an applicable rule
     * between the initial and generated state information.
     */
    bool exists_compatible_rule() const;

    /**
     * Compute applicable rules for the initial state.
     */
    void compute_applicable_rules_for_init();

    /**
     * Set generated state information as the new initial state information
     */
    void set_generated_state_information_as_init();

public:
    virtual ~BaseSketch() = default;

    /**
     * Given a generated state s' perform the following actions:
     * 1. Evaluate features f(s').
     * 2.1. If there exists a rules r that is compatible with (f(s),f(s'))
     *      where f(s) is the feature evaluation of
     *      the initial state of the current subproblem then
     *      (i) set the generate state information as the new initial state,
     *      (ii) recompute applicable rules for the generated state, and
     *      (iii) return true to indicate SIW that a new subproblem was found
     * 2.2. Otherwise, return false to indicate SIW that we remain in the same subproblem.
     */
    bool process_state();
};