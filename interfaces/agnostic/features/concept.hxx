#ifndef __CONCEPT__
#define __CONCEPT__

#include "element.hxx"

namespace aptk {

using Concept = unsigned;
using Concepts = std::vector<Concept>;
using Concepts_Set = std::unordered_set<Concept>;

/**
 * The underlying result represents a set of objects.
 */
class ConceptElement : public BaseElement {
protected:
    Concepts m_result;

public:
    ConceptElement(const Sketch_STRIPS_Problem* problem, bool goal) : BaseElement(problem, goal) { }
    virtual ~ConceptElement() = default;

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

    virtual unsigned get_result_size(const State* state) override {
        return evaluate(state).size();
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
