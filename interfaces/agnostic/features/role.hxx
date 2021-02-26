#ifndef __ROLE__
#define __ROLE__

#include "element.hxx"

namespace aptk {

/**
 * The underlying result represents a set of predicates.
 */
class RoleElement : public BaseElement {
protected:
    const unsigned m_predicate_type;

    void compute_result(const Bit_Set &fluent_set) {
        m_result.reset();
        const std::vector<const Fluent*> fluents = m_problem->total_fluents();
        for (unsigned i = 0; i < m_problem->num_total_fluents(); ++i) {
            if (!fluent_set.isset(i)) continue;
            const Fluent* fluent = fluents[i];
            if (fluent->pddl_predicate_type() == m_predicate_type) {
                m_result.set(i);
            }
        }
    }
public:
    RoleElement(const Sketch_STRIPS_Problem* problem, bool goal, std::string predicate_name)
    : BaseElement(problem, goal), m_predicate_type(problem->predicate_index(predicate_name)) {
        m_result = Bit_Set(m_problem->num_total_fluents());
        if (this->goal()) {
            compute_result(m_problem->goal_fluents_set());
        }
    }

    const Bit_Set& evaluate(const State* state) override {
        if (is_uninitialized(state)) {
            m_state = state;
            compute_result(m_problem->state_fluents_set(state));
        }
        return m_result;
    }
};

}

#endif
