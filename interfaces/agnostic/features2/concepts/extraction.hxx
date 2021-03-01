#ifndef __CONCEPT_EXTRACTION__
#define __CONCEPT_EXTRACTION__

#include "../concept.hxx"

namespace aptk {


class ConceptExtractionElement : public ConceptElement {
protected:
    const unsigned m_predicate_type;
    const unsigned m_position;

    void compute_result(const State* state) {
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

public:
    ConceptElement(const Sketch_STRIPS_Problem* problem, bool goal, std::string predicate_name, unsigned position)
    : BaseElement(problem, goal, RESULT_TYPE::OBJECT), m_predicate_type(problem->predicate_type(predicate_name)), m_position(position) {
        m_result = Bit_Set(problem->num_objects());
        if (goal) {
            compute_result(problem->goal_fluents_set());
        }
    }
    virtual ~ConceptIntersectionElement() = default;

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
