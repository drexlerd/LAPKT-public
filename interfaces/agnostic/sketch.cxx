#include <sketch.hxx>

// #include <strips_state.hxx>
#include "features/boolean_feature.hxx"
#include "features/numerical_feature.hxx"

namespace aptk
{

BooleanFeatureEvalProxy::BooleanFeatureEvalProxy(const BooleanFeature* feature) : m_feature(feature) { }

const BooleanFeature* BooleanFeatureEvalProxy::feature() const { return m_feature; }

PositiveBoolean::PositiveBoolean(const BooleanFeature* feature) : BooleanFeatureEvalProxy(feature) { }

bool PositiveBoolean::evaluate() const {
    return m_feature->get_old_eval();
}


NegativeBoolean::NegativeBoolean(const BooleanFeature* feature) : BooleanFeatureEvalProxy(feature) { }

bool NegativeBoolean::evaluate() const {
    return !m_feature->get_old_eval();
}

ChangedPositiveBoolean::ChangedPositiveBoolean(const BooleanFeature* feature) : BooleanFeatureEvalProxy(feature) { }
bool ChangedPositiveBoolean::evaluate() const {
    return !m_feature->get_old_eval() && m_feature->get_new_eval();
}

ChangedNegativeBoolean::ChangedNegativeBoolean(const BooleanFeature* feature) : BooleanFeatureEvalProxy(feature) { }
bool ChangedNegativeBoolean::evaluate() const {
    return m_feature->get_old_eval() && !m_feature->get_new_eval();
}


UnchangedBoolean::UnchangedBoolean(const BooleanFeature* feature) : BooleanFeatureEvalProxy(feature) { }
bool UnchangedBoolean::evaluate() const {
    return m_feature->get_old_eval() == m_feature->get_new_eval();
}

NumericalFeatureEvalProxy::NumericalFeatureEvalProxy(const NumericalFeature* feature) : m_feature(feature) { }

const NumericalFeature* NumericalFeatureEvalProxy::feature() const { return m_feature; }

ZeroNumerical::ZeroNumerical(const NumericalFeature* feature) : NumericalFeatureEvalProxy(feature) { }

bool ZeroNumerical::evaluate() const {
    return m_feature->get_old_eval() == 0;
}

NonzeroNumerical::NonzeroNumerical(const NumericalFeature* feature) : NumericalFeatureEvalProxy(feature) { }

bool NonzeroNumerical::evaluate() const {
    return m_feature->get_old_eval() > 0;
}

DecrementNumerical::DecrementNumerical(const NumericalFeature* feature) : NumericalFeatureEvalProxy(feature) { }

bool DecrementNumerical::evaluate() const {
    return m_feature->get_old_eval() > m_feature->get_new_eval();
}

IncrementNumerical::IncrementNumerical(const NumericalFeature* feature) : NumericalFeatureEvalProxy(feature) { }

bool IncrementNumerical::evaluate() const {
    return m_feature->get_old_eval() < m_feature->get_new_eval();
}

UnchangedNumerical::UnchangedNumerical(const NumericalFeature* feature) : NumericalFeatureEvalProxy(feature) { }

bool UnchangedNumerical::evaluate() const {
    return m_feature->get_old_eval() == m_feature->get_new_eval();
}


void BaseSketch::add_numerical_feature(NumericalFeature *feature) {
    m_numerical_feature_name_to_idx.insert(make_pair(feature->name(), m_numerical_features.size()));
    m_numerical_features.push_back(feature);
}

void BaseSketch::add_boolean_feature(BooleanFeature *feature) {
    m_boolean_feature_name_to_idx.insert(make_pair(feature->name(), m_boolean_features.size()));
    m_boolean_features.push_back(feature);
}

const NumericalFeature* BaseSketch::get_numerical_feature(const std::string &feature_name) const {
    return m_numerical_features[m_numerical_feature_name_to_idx.at(feature_name)];
}

const BooleanFeature* BaseSketch::get_boolean_feature(const std::string &feature_name) const {
    return m_boolean_features[m_boolean_feature_name_to_idx.at(feature_name)];
}

void BaseSketch::add_rule(const Rule* rule) {
    m_rules.push_back(rule);
}

void BaseSketch::evaluate_features(const State* state) {
    for (NumericalFeature* nf : m_numerical_features) {
        nf->evaluate(state);
    }
    for (BooleanFeature* bf : m_boolean_features) {
        bf->evaluate(state);
    }
}

bool BaseSketch::exists_compatible_rule() const {
    for (const Rule* rule : m_init_applicable_rules) {
        if (rule->is_compatible()) {
            m_applied_rules.push_back(rule);
            return true;
        }
    }
    return false;
}

void BaseSketch::compute_applicable_rules_for_init() {
    m_init_applicable_rules.clear();
    for (const Rule* rule : m_rules) {
        if (rule->is_applicable()) {
            m_init_applicable_rules.push_back(rule);
        }
    }
    /*if (m_init_applicable_rules.size() == 0) {
        std::cout << "Error - compute_applicable_rules_for_init: no applicable rule was found!" << std::endl;
        exit(1);
    }*/
}

void BaseSketch::set_generated_state_information_as_init() {
    for (const NumericalFeature* nf : m_numerical_features) {
        nf->backup_evaluation();
    }
    for (const BooleanFeature* bf : m_boolean_features) {
        bf->backup_evaluation();
    }
}

BaseSketch::BaseSketch(const Sketch_STRIPS_Problem *problem)
    : m_problem(problem) { }

BaseSketch::~BaseSketch() { }

void BaseSketch::initialize_first_subproblem(const State* state) {
    // 1. Evaluate features f(s').
    // TODO: we assume that state are never checked twice
    // because it would not be novel anyways.
    evaluate_features(state);
    // 2. set the generate state information as the new initial state,
    set_generated_state_information_as_init();
    // 3. recompute applicable rules for the generated state, and
    compute_applicable_rules_for_init();
    // debug print
    print_applied_rules();
    print_feature_evaluations();
    print_applicable_rules();
    state->print(std::cout);
}

bool BaseSketch::process_state(const State* state) {
    // 1. Evaluate features f(s').
    // TODO: we assume that state are never checked twice
    // because it would not be novel anyways.
    evaluate_features(state);
    //print_feature_evaluations();
    //get_boolean_feature("one_at_initial")->print();
    //state->print(std::cout);
    // 2.1. If there exists a rules r that is compatible with (f(s),f(s'))
    if (exists_compatible_rule()) {
        // (i) set the generate state information as the new initial state,
        set_generated_state_information_as_init();
        // (ii) recompute applicable rules for the generated state, and
        compute_applicable_rules_for_init();
        // print debug information
        print_applied_rules();
        print_feature_evaluations();
        print_applicable_rules();
        state->print(std::cout);
        ++m_count;
        // if (m_count == 7) exit(1);
        // (iii) return true to indicate SIW that a new subproblem was found
        return true;
    }
    // 2.2. Otherwise, return false to indicate SIW that we remain in the same subproblem.
    return false;
}

void BaseSketch::print_feature_evaluations() const {
    std::cout << "Numerical features: " << m_numerical_features.size() << "\n";
    for (NumericalFeature* nf : m_numerical_features) {
        nf->print();
    }
    std::cout << "Boolean features: " << m_boolean_features.size() << "\n";
    for (BooleanFeature* bf : m_boolean_features) {
        bf->print();
    }
    std::cout << std::endl;
}

void BaseSketch::print_applied_rules() const {
    std::cout << "Rules applied:\n";
    for (const Rule* rule : m_applied_rules) {
        std::cout << "\t" << rule->name() << "\n";
    }
    std::cout << std::endl;
}

void BaseSketch::print_applicable_rules() const {
    std::cout << "Applicable rules:\n";
    for (auto rule : m_init_applicable_rules) {
        std::cout << "\t" << rule->name() << "\n";
    }
    std::cout << std::endl;
}
}