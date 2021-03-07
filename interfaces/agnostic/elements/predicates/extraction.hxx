#ifndef __PREDICATE_EXTRACTION__
#define __PREDICATE_EXTRACTION__

#include "../predicate.hxx"

namespace aptk {

/**
 * The underlying result represents all predicates of a certain type.
 */
class PredicateExtractionElement : public PredicateElement {
protected:
    const unsigned m_predicate_type;

    virtual void compute_result(const Fluent_Set& fluents) {
        m_result.clear();
        for (unsigned i = 0; i < m_problem->num_total_fluents(); ++i) {
            if (fluents.isset(i)) {
                const Fluent* fluent = m_problem->total_fluents()[i];
                if (fluent->pddl_predicate_type() == m_predicate_type) {
                    m_result.push_back(fluent->index());
                }
            }
        }
    }

    virtual void compute_result(const State* state) override {
        m_result.clear();
        compute_result(m_problem->state_fluents_set(state));
    }

public:
    PredicateExtractionElement(const Sketch_STRIPS_Problem* problem, bool goal, std::string predicate_name)
    : PredicateElement(problem, goal), m_predicate_type(problem->predicate_type(predicate_name)) {
        if (goal) {
            compute_result(m_problem->goal_fluents_set());
        }
    }
    virtual ~PredicateExtractionElement() = default;
};

}

#endif
