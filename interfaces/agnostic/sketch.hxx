
#ifndef __SKETCH__
#define __SKETCH__

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <cassert>

namespace aptk
{
class BaseSketch;
class Sketch_STRIPS_Problem;
class State;
class BooleanFeature;
class NumericalFeature;

/**
 * Takes some feature and allow some more complex evaluation.
 * Useful to express preconditions or effects of rules.
 */
class BooleanFeatureEvalProxy {
protected:
    // the underlying boolean feature
    const BooleanFeature* m_feature;
public:
    BooleanFeatureEvalProxy(const BooleanFeature* feature);
    virtual bool evaluate() const = 0;
    const BooleanFeature* feature() const;
};
class PositiveBoolean : public BooleanFeatureEvalProxy {
public:
    PositiveBoolean(const BooleanFeature* feature);
    virtual bool evaluate() const override;
};
class NegativeBoolean : public BooleanFeatureEvalProxy {
public:
    NegativeBoolean(const BooleanFeature* feature);
    virtual bool evaluate() const override;
};
class ChangedPositiveBoolean : public BooleanFeatureEvalProxy {
public:
    ChangedPositiveBoolean(const BooleanFeature* feature);
    virtual bool evaluate() const override;
};
class ChangedNegativeBoolean : public BooleanFeatureEvalProxy {
public:
    ChangedNegativeBoolean(const BooleanFeature* feature);
    virtual bool evaluate() const override;
};
class UnchangedBoolean : public BooleanFeatureEvalProxy {
public:
    UnchangedBoolean(const BooleanFeature* feature);
    virtual bool evaluate() const override;
};

class NumericalFeatureEvalProxy {
protected:
    // the underlying numerical feature
    const NumericalFeature* m_feature;
public:
    NumericalFeatureEvalProxy(const NumericalFeature* feature);
    virtual bool evaluate() const = 0;
    const NumericalFeature* feature() const;
};
class ZeroNumerical : public NumericalFeatureEvalProxy {
public:
    ZeroNumerical(const NumericalFeature* feature);
    virtual bool evaluate() const override;
};
class NonzeroNumerical : public NumericalFeatureEvalProxy {
public:
    NonzeroNumerical(const NumericalFeature* feature);
    virtual bool evaluate() const override;
};
class DecrementNumerical : public NumericalFeatureEvalProxy {
public:
    DecrementNumerical(const NumericalFeature* feature);
    virtual bool evaluate() const override;
};
class IncrementNumerical : public NumericalFeatureEvalProxy {
public:
    IncrementNumerical(const NumericalFeature* feature);
    virtual bool evaluate() const override;
};
class UnchangedNumerical : public NumericalFeatureEvalProxy {
public:
    UnchangedNumerical(const NumericalFeature* feature);
    virtual bool evaluate() const override;
};

class Rule {
protected:
    const BaseSketch* m_sketch;
    const std::string m_name;
    // Preconditions
    std::vector<const BooleanFeatureEvalProxy*> m_boolean_preconditions;
    std::vector<const NumericalFeatureEvalProxy*> m_numerical_preconditions;
    // Effects
    std::vector<const BooleanFeatureEvalProxy*> m_boolean_effects;
    std::vector<const NumericalFeatureEvalProxy*> m_numerical_effects;

public:
    Rule(const BaseSketch* sketch,
        const std::string &name,
        std::vector<const BooleanFeatureEvalProxy*> &&boolean_preconditions,
        std::vector<const NumericalFeatureEvalProxy*> &&numerical_preconditions,
        std::vector<const BooleanFeatureEvalProxy*> &&boolean_effects,
        std::vector<const NumericalFeatureEvalProxy*> &&numerical_effects)
        : m_sketch(sketch),
          m_name(name),
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
    const std::string &name() const { return m_name; }
    const std::vector<const BooleanFeatureEvalProxy*> boolean_effects() const { return m_boolean_effects; }
    const std::vector<const NumericalFeatureEvalProxy*> numerical_effects() const { return m_numerical_effects; }
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

    // the rules applied until termination
    mutable std::vector<const Rule*> m_applied_rules;

    const bool m_verbose = false;
    int m_count = 0;
protected:
    /**
     * Add features with respective names.
     */
    void add_numerical_feature(NumericalFeature *feature);
    void add_boolean_feature(BooleanFeature *feature);
    const NumericalFeature* get_numerical_feature(const std::string &feature_name) const;
    const BooleanFeature* get_boolean_feature(const std::string &feature_name) const;

    /**
     * Add rule.
     */
    void add_rule(const Rule* rule);

    /**
     * Evaluate features for a given state.
     */
    void evaluate_features(const State* state);

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
    BaseSketch(const Sketch_STRIPS_Problem *problem);
    virtual ~BaseSketch();

    /**
     * Enter the first subproblem.
     */
    void initialize_first_subproblem(const State* state);


    /**
     * Try entering a new subproblem for a given state.
     * Returns true if the given state defines the initial state
     * of the next subproblem.
     */
    bool process_state(const State* state);

    /**
     * Getters
     */
    const Sketch_STRIPS_Problem* problem() const { return m_problem; }
    const std::vector<const Rule*> applied_rules() const { return m_applied_rules; }

    /**
     * Pretty printers.
     */
    void print_feature_evaluations() const;
    void print_applied_rules() const;
    void print_applicable_rules() const;
};


}

#endif
