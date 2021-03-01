#ifndef __CONCEPT__
#define __CONCEPT__

#include "element.hxx"

namespace aptk {

/**
 * The underlying result represents a set of objects.
 */
class ConceptElement : public BaseElement {
protected:
    const unsigned m_predicate_type;
    const unsigned m_position;

    void compute_result(const Bit_Set &fluent_set) {
        m_result.reset();
        const std::vector<const Fluent*> fluents = m_problem->total_fluents();
        for (unsigned i = 0; i < m_problem->num_total_fluents(); ++i) {
            if (!fluent_set.isset(i)) continue;
            const Fluent* fluent = fluents[i];
            if (fluent->pddl_predicate_type() == m_predicate_type) {
                m_result.set(fluent->pddl_objs_idx()[m_position]);
            }
        }
    }

    virtual bool assert_parameters() const override {
        // nothing to be checked
    }

public:
    ConceptElement(const Sketch_STRIPS_Problem* problem, bool goal, std::string predicate_name, unsigned position)
    : BaseElement(problem, goal, RESULT_TYPE::OBJECT), m_predicate_type(problem->predicate_type(predicate_name)), m_position(position) {
        m_result = Bit_Set(problem->num_objects());
        if (goal) {
            compute_result(problem->goal_fluents_set());
        }
    }

    virtual const Bit_Set& evaluate(const State* state) override {
        if (is_uninitialized(state)) {
            m_state = state;
            compute_result(m_problem->state_fluents_set(state));
        }
        return m_result;
    }
};

}

#endif
