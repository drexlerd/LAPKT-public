#ifndef __ELEMENT__
#define __ELEMENT__

#include <aptk/bit_set.hxx>
#include <sketch_strips_prob.hxx>

namespace aptk {

enum class RESULT_TYPE {
    OBJECT,
    PREDICATE,
};

class BaseElement {
protected:
    // Reference to problem to obtain relevant information
    const Sketch_STRIPS_Problem* m_problem;
    // Goal is true iff the element is evaluated in goal only
    bool m_goal;
    // Reference to state used during evaluation.
    const State* m_state;

    /**
     * Compute the result for the given state
     */
    virtual void compute_result(const State* state) = 0;

public:
    BaseElement(const Sketch_STRIPS_Problem* problem, bool goal) : m_problem(problem), m_goal(goal), m_state(nullptr) { }
    virtual ~BaseElement() = default;

    /**
     * Getters
     */
    bool goal() const { return m_goal; }
    bool is_uninitialized(const State* state) const { return (!m_goal && m_state != state); }
    void set_initialized(const State* state) { m_state = state; }

    /**
     * Pretty printer.
     */
    virtual void print() const = 0;
};

}

#endif
