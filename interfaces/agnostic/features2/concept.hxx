#ifndef __CONCEPT__
#define __CONCEPT__

#include "element.hxx"

namespace aptk {

using Concept = unsigned;
using Concepts = std::vector<Concept>;

/**
 * The underlying result represents a set of objects.
 */
class ConceptElement : public BaseElement {
protected:
    Concepts m_result;

public:
    ConceptElement(const Sketch_STRIPS_Problem* problem, bool goal)
    : BaseElement(problem, goal) {
    }

    /**
     * A ConceptElement returns a reference to Concepts.
     */
    virtual const Concepts& evaluate(const State* state) {
        if (is_uninitialized(state)) {
            set_initialized(state);
            compute_result(state);
        }
        return m_result;
    }

    /**
     * Getters
     */
    const Concepts& result() const { return m_result; }

    virtual void print() const override {
    }
};

}

#endif
