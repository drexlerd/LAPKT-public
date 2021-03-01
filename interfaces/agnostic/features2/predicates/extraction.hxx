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

    virtual void compute_result(const State* state) override {
        m_result.clear();
        for (unsigned p : state->fluent_vec()) {
            if (m_problem->total_fluents()[p]->pddl_predicate_type() == m_predicate_type) {
                m_result.push_back(p);
            }
        }
    }

public:
    PredicateExtractionElement(const Sketch_STRIPS_Problem* problem, bool goal, std::string predicate_name)
    : PredicateElement(problem, goal), m_predicate_type(problem->predicate_type(predicate_name)) {
        if (goal) {
            for (const Fluent* fluent : m_problem->total_fluents()) {
                if (fluent->pddl_predicate_type() == m_predicate_type) {
                    m_result.push_back(fluent->index());
                }
            }
        }
    }
};

}

#endif
