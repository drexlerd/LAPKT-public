
#ifndef __SKETCH__
#define __SKETCH__

#include <vector>
#include <unordered_map>
#include <iostream>
#include <sketch_state.hxx>

namespace aptk
{
class Sketch_STRIPS_Problem;
class BaseSketch;


class BaseFeature {
protected:
    const BaseSketch* m_sketch;
public:
    BaseFeature(const BaseSketch* sketch) : m_sketch(sketch) { }
    virtual ~BaseFeature() = default;

    virtual void backup_evaluation() const = 0;

    const BaseSketch* sketch() const { return m_sketch; }
};


class BooleanFeature : public BaseFeature {
protected:
    mutable bool old_eval;
    bool new_eval;
public:
    BooleanFeature(const BaseSketch* sketch) : BaseFeature(sketch) { }
    virtual ~BooleanFeature() = default;

    /**
     * Evaluate the feature for a given state.
     * The problem provides additional information.
     */
    virtual void evaluate(const SketchState &sketch_state) = 0;

    virtual void backup_evaluation() const override {
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
    mutable int old_eval;
    int new_eval;
public:
    NumericalFeature(const BaseSketch* sketch) : BaseFeature(sketch) { }
    virtual ~NumericalFeature() = default;

    virtual void evaluate(const SketchState &sketch_state) = 0;

    virtual void backup_evaluation() const override {
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
    // the underlying boolean feature
    const BooleanFeature* m_feature;
public:
    BooleanFeatureEvalProxy(const BooleanFeature* feature) : m_feature(feature) { }
    virtual bool evaluate() const = 0;
};
class PositiveBoolean : public BooleanFeatureEvalProxy {
public:
    PositiveBoolean(const BooleanFeature* feature) : BooleanFeatureEvalProxy(feature) { }
    virtual bool evaluate() const override {
        return m_feature->get_old_eval();
    }
};
class NegativeBoolean : public BooleanFeatureEvalProxy {
public:
    NegativeBoolean(const BooleanFeature* feature) : BooleanFeatureEvalProxy(feature) { }
    virtual bool evaluate() const override {
        return !m_feature->get_old_eval();
    }
};
class ChangedPositiveBoolean : public BooleanFeatureEvalProxy {
public:
    ChangedPositiveBoolean(const BooleanFeature* feature) : BooleanFeatureEvalProxy(feature) { }
    virtual bool evaluate() const override {
        return !m_feature->get_old_eval() && m_feature->get_new_eval();
    }
};
class ChangedNegativeBoolean : public BooleanFeatureEvalProxy {
public:
    ChangedNegativeBoolean(const BooleanFeature* feature) : BooleanFeatureEvalProxy(feature) { }
    virtual bool evaluate() const override {
        return m_feature->get_old_eval() && !m_feature->get_new_eval();
    }
};
class UnchangedBoolean : public BooleanFeatureEvalProxy {
public:
    UnchangedBoolean(const BooleanFeature* feature) : BooleanFeatureEvalProxy(feature) { }
    virtual bool evaluate() const override {
        return m_feature->get_old_eval() == m_feature->get_new_eval();
    }
};

class NumericalFeatureEvalProxy {
protected:
    // the underlying numerical feature
    const NumericalFeature* m_feature;
public:
    NumericalFeatureEvalProxy(const NumericalFeature* feature) : m_feature(feature) { }
    virtual bool evaluate() const = 0;
};
class ZeroNumerical : public NumericalFeatureEvalProxy {
public:
    ZeroNumerical(const NumericalFeature* feature) : NumericalFeatureEvalProxy(feature) { }
    virtual bool evaluate() const override {
        return m_feature->get_old_eval() == 0;
    }
};
class NonzeroNumerical : public NumericalFeatureEvalProxy {
public:
    NonzeroNumerical(const NumericalFeature* feature) : NumericalFeatureEvalProxy(feature) { }
    virtual bool evaluate() const override {
        return m_feature->get_old_eval() > 0;
    }
};
class DecrementNumerical : public NumericalFeatureEvalProxy {
public:
    DecrementNumerical(const NumericalFeature* feature) : NumericalFeatureEvalProxy(feature) { }
    virtual bool evaluate() const override {
        return m_feature->get_old_eval() > m_feature->get_new_eval();
    }
};
class IncrementNumerical : public NumericalFeatureEvalProxy {
public:
    IncrementNumerical(const NumericalFeature* feature) : NumericalFeatureEvalProxy(feature) { }
    virtual bool evaluate() const override {
        return m_feature->get_old_eval() < m_feature->get_new_eval();
    }
};
class UnchangedNumerical : public NumericalFeatureEvalProxy {
public:
    UnchangedNumerical(const NumericalFeature* feature) : NumericalFeatureEvalProxy(feature) { }
    virtual bool evaluate() const override {
        return m_feature->get_old_eval() == m_feature->get_new_eval();
    }
};

class Rule {
protected:
    const BaseSketch* m_sketch;
    // Preconditions
    std::vector<const BooleanFeatureEvalProxy*> m_boolean_preconditions;
    std::vector<const NumericalFeatureEvalProxy*> m_numerical_preconditions;
    // Effects
    std::vector<const BooleanFeatureEvalProxy*> m_boolean_effects;
    std::vector<const NumericalFeatureEvalProxy*> m_numerical_effects;

public:
    Rule(const BaseSketch* sketch,
        std::vector<const BooleanFeatureEvalProxy*> &&boolean_preconditions,
        std::vector<const NumericalFeatureEvalProxy*> &&numerical_preconditions,
        std::vector<const BooleanFeatureEvalProxy*> &&boolean_effects,
        std::vector<const NumericalFeatureEvalProxy*> &&numerical_effects)
        : m_sketch(sketch),
          m_boolean_preconditions(std::move(boolean_preconditions)),
          m_numerical_preconditions(std::move(numerical_preconditions)),
          m_boolean_effects(std::move(boolean_effects)),
          m_numerical_effects(std::move(numerical_effects)) { }

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

    /**
     * Getters.
     */
    const BaseSketch* sketch() const { return m_sketch; }
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
    // the problem to which this sketch belongs
    const Sketch_STRIPS_Problem *m_problem;
    // the state information with different views
    SketchState m_sketch_state;

    // mapping of feature names to indices used to define rules
    std::unordered_map<std::string, unsigned> m_numerical_feature_name_to_idx;
    std::unordered_map<std::string, unsigned> m_boolean_feature_name_to_idx;

    // features to be evaluated
    std::vector<NumericalFeature*> m_numerical_features;
    std::vector<BooleanFeature*> m_boolean_features;
    // rules to be evaluated
    std::vector<const Rule*> m_rules;
    // rules applicable in the subproblem's initial state.
    std::vector<const Rule*> m_init_applicable_rules;
protected:
    /**
     * Add features with respective names.
     */
    void add_numerical_feature(const std::string &feature_name, NumericalFeature *feature) {
        m_numerical_feature_name_to_idx.insert(make_pair(feature_name, m_numerical_features.size()));
        m_numerical_features.push_back(feature);
    }
    void add_boolean_feature(const std::string &feature_name, BooleanFeature *feature) {
        m_boolean_feature_name_to_idx.insert(make_pair(feature_name, m_numerical_features.size()));
        m_boolean_features.push_back(feature);
    }
    const NumericalFeature* get_numerical_feature(const std::string &feature_name) const {
        return m_numerical_features[m_numerical_feature_name_to_idx.at(feature_name)];
    }
    const BooleanFeature* get_boolean_feature(const std::string &feature_name) const {
        return m_boolean_features[m_boolean_feature_name_to_idx.at(feature_name)];
    }


    /**
     * Add rule.
     */
    void add_rule(const Rule* rule) {
        m_rules.push_back(rule);
    }

    /**
     * Evaluate features for a given state.
     */
    void evaluate_features() {
        for (NumericalFeature* nf : m_numerical_features) {
            nf->evaluate(m_sketch_state);
        }
        for (BooleanFeature* bf : m_boolean_features) {
            bf->evaluate(m_sketch_state);
        }
    }

    /**
     * Return true iff there exists an applicable rule
     * between the initial and generated state information.
     */
    bool exists_compatible_rule() const {
        for (const Rule* rule : m_init_applicable_rules) {
            if (rule->is_compatible()) return true;
        }
        return false;
    }

    /**
     * Compute applicable rules for the initial state.
     */
    void compute_applicable_rules_for_init() {
        m_init_applicable_rules.clear();
        for (const Rule* rule : m_rules) {
            if (rule->is_applicable()) {
                m_init_applicable_rules.push_back(rule);
            }
        }
    }

    /**
     * Set generated state information as the new initial state information
     */
    void set_generated_state_information_as_init() {
        for (const NumericalFeature* nf : m_numerical_features) {
            nf->backup_evaluation();
        }
        for (const BooleanFeature* bf : m_boolean_features) {
            bf->backup_evaluation();
        }
    }

public:
    BaseSketch(const Sketch_STRIPS_Problem *problem)
    : m_problem(problem),
      m_sketch_state(problem) { }
    virtual ~BaseSketch() = default;

    /**
     * Try entering a new subproblem for a given state.
     * Returns true if the given state defines the initial state
     * of the next subproblem.
     */
    bool process_state(const State* state) {
        // update view
        m_sketch_state.set_state(state);
        // 1. Evaluate features f(s').
        // TODO: we assume that state are never checked twice
        // because it would not be novel anyways.
        evaluate_features();
        // 2.1. If there exists a rules r that is compatible with (f(s),f(s'))
        if (exists_compatible_rule()) {
            // (i) set the generate state information as the new initial state,
            set_generated_state_information_as_init();
            // (ii) recompute applicable rules for the generated state, and
            compute_applicable_rules_for_init();
            // (iii) return true to indicate SIW that a new subproblem was found
            return true;
        }
        // 2.2. Otherwise, return false to indicate SIW that we remain in the same subproblem.
        return false;
    }

    /**
     * Getters
     */
    const Sketch_STRIPS_Problem* problem() const { return m_problem; }
};


}

#endif
