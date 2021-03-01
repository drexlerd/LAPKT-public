#ifndef __ROLE__
#define __ROLE__

#include "element.hxx"

namespace aptk {

/**
 * The underlying result represents all predicates of a certain type.
 */
class PredicateElement : public BaseElement {
public:
    PredicateElement(const Sketch_STRIPS_Problem* problem, std::string predicate_name)
    : BaseElement(problem, true) {
        m_result = Bit_Set(m_problem->num_total_fluents());
        m_result_type = RESULT_TYPE::PREDICATE;
        for (const Fluent* fluent : m_problem->total_fluents()) {
            if (fluent->pddl_predicate_type() == problem->predicate_type(predicate_name)) {
                m_result.set(fluent->index());
            }
        }
    }

    const Bit_Set& evaluate(const State* state) override {
        return m_result;
    }
};

}

#endif
