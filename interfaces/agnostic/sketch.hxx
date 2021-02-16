
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

public:
    BaseFeature(
        const std::unordered_map<std::string, int> &predicate_name_to_idx,
        const std::unordered_map<std::string, int> &object_name_to_idx)
        : m_predicate_name_to_idx(predicate_name_to_idx),
          m_object_name_to_idx(m_object_name_to_idx) { }
    virtual ~BaseFeature() = default;

    virtual void backup_evaluation() = 0;
};


class BooleanFeature : public BaseFeature {
protected:
    bool old_eval;
    bool new_eval;
public:
    BooleanFeature(
        const std::unordered_map<std::string, int> &predicate_name_to_idx,
        const std::unordered_map<std::string, int> &object_name_to_idx)
        : BaseFeature(predicate_name_to_idx, object_name_to_idx) { }
    virtual ~BooleanFeature() = default;

    /**
     * Evaluate the feature for a given state.
     */
    virtual bool evaluate(
        const std::vector<std::vector<const Fluent*>> &state_description,
        const std::vector<std::vector<const Fluent*>> &goal_description,
        const Fluent_Vec &state,
        const Fluent_Vec &goal) const = 0;

    virtual void backup_evaluation() override {
        old_eval = new_eval;
    }

    bool get_old_eval() const {
        return old_eval;
    }

    bool get_new_eval() const {
        return new_eval;
    }
};


class NumericalFeature : public BaseFeature {
protected:
    int old_eval;
    int new_eval;
public:
    NumericalFeature(
        const std::unordered_map<std::string, int> &predicate_name_to_idx,
        const std::unordered_map<std::string, int> &object_name_to_idx)
        : BaseFeature(predicate_name_to_idx, object_name_to_idx) { }
    virtual ~NumericalFeature() = default;

    virtual int evaluate(const std::vector<std::vector<const Fluent*>> &state_description, const std::vector<std::vector<const Fluent*>> &goal_description) const = 0;

    virtual void backup_evaluation() override {
        old_eval = new_eval;
    }

    int get_old_eval() const {
        return old_eval;
    }

    int get_new_eval() const {
        return new_eval;
    }
};

/**
 * Takes some feature and allow some more complex evaluation.
 * Useful to express preconditions or effects of rules.
 */
class BooleanFeatureEvalProxy {
protected:
    const BooleanFeature* m_feature;
public:
    BooleanFeatureEvalProxy(const BooleanFeature* feature) : m_feature(feature) { }
    virtual bool evaluate() const = 0;
};
class PositiveBoolean : BooleanFeatureEvalProxy {
public:
    PositiveBoolean(const BooleanFeature* feature) : BooleanFeatureEvalProxy(feature) { }
    virtual bool evaluate() const override {
        return m_feature->get_old_eval();
    }
};
class NegativeBoolean : BooleanFeatureEvalProxy {
public:
    NegativeBoolean(const BooleanFeature* feature) : BooleanFeatureEvalProxy(feature) { }
    virtual bool evaluate() const override {
        return !m_feature->get_old_eval();
    }
};
class ChangedPositiveBoolean : BooleanFeatureEvalProxy {
public:
    ChangedPositiveBoolean(const BooleanFeature* feature) : BooleanFeatureEvalProxy(feature) { }
    virtual bool evaluate() const override {
        return !m_feature->get_old_eval() && m_feature->get_new_eval();
    }
};
class ChangedNegativeBoolean : BooleanFeatureEvalProxy {
public:
    ChangedNegativeBoolean(const BooleanFeature* feature) : BooleanFeatureEvalProxy(feature) { }
    virtual bool evaluate() const override {
        return m_feature->get_old_eval() && !m_feature->get_new_eval();
    }
};
class UnchangedBoolean : BooleanFeatureEvalProxy {
public:
    UnchangedBoolean(const BooleanFeature* feature) : BooleanFeatureEvalProxy(feature) { }
    virtual bool evaluate() const override {
        return m_feature->get_old_eval() == m_feature->get_new_eval();
    }
};

class NumericalFeatureEvalProxy {
protected:
    const NumericalFeature* m_feature;
public:
    NumericalFeatureEvalProxy(const NumericalFeature* feature) : m_feature(feature) { }
    virtual bool evaluate() const = 0;
};
class ZeroNumerical : NumericalFeatureEvalProxy {
public:
    ZeroNumerical(const NumericalFeature* feature) : NumericalFeatureEvalProxy(feature) { }
    virtual bool evaluate() const override {
        return m_feature->get_old_eval() == 0;
    }
};
class NonzeroNumerical : NumericalFeatureEvalProxy {
public:
    NonzeroNumerical(const NumericalFeature* feature) : NumericalFeatureEvalProxy(feature) { }
    virtual bool evaluate() const override {
        return !m_feature->get_old_eval() > 0;
    }
};
class DecrementNumerical : NumericalFeatureEvalProxy {
public:
    DecrementNumerical(const NumericalFeature* feature) : NumericalFeatureEvalProxy(feature) { }
    virtual bool evaluate() const override {
        return m_feature->get_old_eval() > m_feature->get_new_eval();
    }
};
class IncrementNumerical : NumericalFeatureEvalProxy {
public:
    IncrementNumerical(const NumericalFeature* feature) : NumericalFeatureEvalProxy(feature) { }
    virtual bool evaluate() const override {
        return m_feature->get_old_eval() < m_feature->get_new_eval();
    }
};
class UnchangedNumerical : NumericalFeatureEvalProxy {
public:
    UnchangedNumerical(const NumericalFeature* feature) : NumericalFeatureEvalProxy(feature) { }
    virtual bool evaluate() const override {
        return m_feature->get_old_eval() == m_feature->get_new_eval();
    }
};

class Rule {
protected:
    // Access to feature mappings
    // Note that the hash value should be computed in the constructor
    // to make this as efficient as possible.
    const std::unordered_map<std::string, int> &m_numerical_feature_name_to_idx;
    const std::unordered_map<std::string, int> &m_boolean_feature_name_to_idx;

    // Preconditions
    std::vector<const BooleanFeatureEvalProxy*> m_boolean_preconditions;
    std::vector<const NumericalFeatureEvalProxy*> m_numerical_preconditions;
    // Effects
    std::vector<const BooleanFeatureEvalProxy*> m_boolean_effects;
    std::vector<const NumericalFeatureEvalProxy*> m_numerical_effects;

protected:
    void add_boolean_precondition(const BooleanFeatureEvalProxy *boolean_precondition) {
        m_boolean_preconditions.push_back(boolean_precondition);
    }

    void add_numerial_precondition(const NumericalFeatureEvalProxy *numerical_precondition) {
        m_numerical_preconditions.push_back(numerical_precondition);
    }

    void add_boolean_effect(const BooleanFeatureEvalProxy *boolean_effect) {
        m_boolean_effects.push_back(boolean_effect);
    }

    void add_numerial_effect(const NumericalFeatureEvalProxy *numerical_effect) {
        m_numerical_effects.push_back(numerical_effect);
    }

public:
    /**
     * Returns true iff rule is applicable in the problems initial state's feature evaluation
     */
    bool is_applicable() const {
        for (const auto &f : m_boolean_preconditions) {
            if (!f->evaluate()) return false;
        }
        for (const auto &f : m_numerical_preconditions) {
            if (!f->evaluate()) return false;
        }
        return true;
    }

    /**
     * Returns true iff rule is compatible with generated state's feature evaluation.
     * Attention: this rules does not check for applicability!
     */
    bool is_compatible() const {
        for (const auto &f : m_boolean_effects) {
            if (!f->evaluate()) return false;
        }
        for (const auto &f : m_numerical_effects) {
            if (!f->evaluate()) return false;
        }
        return true;
    }
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
    // rules applicable in the subproblem's initial state.
    std::vector<const Rule*> m_init_applicable_rules;
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
        std::unordered_map<std::string, int> &&object_name_to_idx) :
        m_predicate_name_to_idx(std::move(predicate_name_to_idx)),
        m_object_name_to_idx(std::move(object_name_to_idx)) { }
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

/**
 * Sketch with single rule that decreases the number of achieved goal atoms.
 */
class GoalSketch : public BaseSketch {
public:
    GoalSketch(
        std::unordered_map<std::string, int> &&predicate_name_to_idx,
        std::unordered_map<std::string, int> &&object_name_to_idx)
        : BaseSketch(std::move(predicate_name_to_idx), std::move(object_name_to_idx)) { }
};

}