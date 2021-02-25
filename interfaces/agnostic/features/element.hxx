#ifndef __ELEMENT__
#define __ELEMENT__

#include <aptk/bit_set.hxx>
#include <sketch_strips_prob.hxx>

namespace aptk {

/**
 * Abstract concepts and roles.
 */

class BaseElement {
protected:
    // Reference to problem to obtain relevant information
    const Sketch_STRIPS_Problem* m_problem;
    // Goal is true iff the element is evaluated in goal only
    bool m_goal;
    // Reference to state used during evaluation.
    const State* m_state;
    // Preallocated memory to store evaluation result
    Bit_Set m_result;

public:
    BaseElement(const Sketch_STRIPS_Problem* problem, bool goal) : m_problem(problem), m_goal(goal), m_state(nullptr) {}
    virtual ~BaseElement() = default;
    /**
     * Evaluate the Element for a given state and return a reference to the result.
     * The resulting Bit_Set can either represent objects or predicates.
     */
    virtual const Bit_Set& evaluate(const State* state=nullptr) = 0;

    /**
     * Getters
     */
    bool goal() const { return m_goal; }
    bool is_uninitialized(const State* state) const { return (!m_goal && m_state != state); }

    /**
     * Pretty printer.
     */
    virtual void print_result() const = 0;
};


/**
 * Extract objects occuring at specific location in a role.
 */
/*
class ExtractConceptElement : public BaseConceptElement {
protected:
    BaseRoleElement* m_role;

    void set_result(const Bit_Set &fluent_set) {
        m_result = m_left->evaluate(state);
        const ObjectsResult &right_result = m_right->evaluate(state);
        for (unsigned i = 0; i < m_problem->num_objects(); ++i) {
            if (right_result.isset(i)) {
                m_result.unset(i);
            }
        }
    }

public:
    ExtractConceptElement(
        const Sketch_STRIPS_Problem* problem, bool goal, BaseRoleElement* role)
        : BaseConceptElement(problem, goal), m_role(role) {
        m_result = Bit_Set(m_problem->num_objects());
        if (this->goal()) {
            // children must be goal initialized as well
            assert(m_role->goal());
            set_result(m_problem->goal_fluents_set());
        }
    }
    virtual ~ExtractConceptElement() = default;

    virtual const ObjectsResult& evaluate(const State* state) override {
        if (is_uninitialized(state)) {
            m_state = state;
            set_result(m_role->evaluate(state));
        }
        return m_result;
    }
};
*/

}

#endif
