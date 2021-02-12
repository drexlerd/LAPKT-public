
#include <types.hxx>
#include <vector>
#include <unordered_map>

namespace aptk
{


class BaseFeature {
protected:
    // access to predicate mappings
    const std::unordered_map<std::string, int> &m_predicate_name_to_idx;
    const std::unordered_map<std::string, int> &m_object_name_to_idx;
};


class BooleanFeature : public BaseFeature {
public:
    bool evaluate(const FOL_State &fol_state) const;
};


class NumericalFeature : public BaseFeature {
public:
    int evaluate(const FOL_State &fol_state) const;
};


struct EvaluationCache {
    std::vector<int> m_init_numerical_features;
    std::vector<bool> m_init_boolean_features;
};


class Rule {
protected:
    // access to feature evaluations
    const EvaluationCache &m_init_cache;
    const EvaluationCache &m_gen_cache;
    // access to feature mappings
    const std::unordered_map<std::string, int> &m_numerical_feature_name_to_idx;
    const std::unordered_map<std::string, int> &m_boolean_feature_name_to_idx;

public:
    bool is_applicable() const;

    bool is_compatible() const;
};



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
    // TODO: if the hashing gets a bottleneck, we can directly work with indices.
    // mapping of fluent names to indices used to define features
    const std::unordered_map<std::string, int> m_predicate_name_to_idx;
    const std::unordered_map<std::string, int> m_object_name_to_idx;
    // mapping of feature names to indices used to define rules
    std::unordered_map<std::string, int> m_numerical_feature_name_to_idx;
    std::unordered_map<std::string, int> m_boolean_feature_name_to_idx;

    // features to be evaluated
    std::vector<const NumericalFeature*> m_numerical_features;
    std::vector<const BooleanFeature*> m_boolean_features;
    // rules to be evaluated
    std::vector<const Rule*> m_rules;

    // memory for initial state
    std::vector<const Rule*> m_init_applicable_rules;
    EvaluationCache m_init_cache;
    // memory for generated state
    EvaluationCache m_gen_cache;
protected:
    /**
     * Add features with respective names.
     */
    void add_numerical_feature(const std::string &feature_name, const NumericalFeature *feature);
    void add_boolean_feature(const std::string &feature_name, const BooleanFeature *feature);

    /**
     * Evaluate features for a given state.
     */
    void evaluate_features();

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
    BaseSketch(
        std::unordered_map<std::string, int> &&predicate_name_to_idx,
        std::unordered_map<std::string, int> &&object_name_to_idx);
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
    bool process_state(const std::vector<std::vector<const Fluent*>> &first_order_state);
};

}