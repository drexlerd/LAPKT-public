#ifndef __PREDICATE__
#define __PREDICATE__

#include "element.hxx"

namespace aptk {

/**
 * The underlying result represents all predicates of a certain type.
 */
class PredicateElement : public BaseElement {
protected:
    virtual bool assert_parameters() const override {
        // nothing to be checked
    }

public:
    PredicateElement(const Sketch_STRIPS_Problem* problem, std::string predicate_name)
    : BaseElement(problem, true, RESULT_TYPE::PREDICATE) {
        m_result = Bit_Set(m_problem->num_total_fluents());
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
