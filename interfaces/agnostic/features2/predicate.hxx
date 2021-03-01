#ifndef __PREDICATE__
#define __PREDICATE__

#include "element.hxx"

namespace aptk {

using Predicate = unsigned;
using Predicates = std::vector<Predicate>;

/**
 * The underlying result represents all predicates of a certain type.
 */
class PredicateElement : public BaseElement {
protected:
    Predicates m_result;

public:
    PredicateElement(const Sketch_STRIPS_Problem* problem, bool goal) : BaseElement(problem, goal) { }

    /**
     * A RoleElement returns a reference to Predicates.
     */
    const Predicates& evaluate(const State* state) {
        if (is_uninitialized(state)) {
            set_initialized(state);
            compute_result(state);
        }
        return m_result;
    }

    /**
     * Getters
     */
    const Predicates& result() const { return m_result; }

    virtual void print() const override {
    }
};

}

#endif
