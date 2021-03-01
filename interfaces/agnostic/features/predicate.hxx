#ifndef __PREDICATE__
#define __PREDICATE__

#include "element.hxx"

namespace aptk {

using Predicate = unsigned;
using Predicates = std::vector<Predicate>;
using Predicates_Set = std::unordered_set<Predicate>;

/**
 * The underlying result represents all predicates of a certain type.
 */
class PredicateElement : public BaseElement {
protected:
    Predicates m_result;

public:
    PredicateElement(const Sketch_STRIPS_Problem* problem, bool goal) : BaseElement(problem, goal) { }
    virtual ~PredicateElement() = default;

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

    virtual unsigned get_result_size(const State* state) override {
        return evaluate(state).size();
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
