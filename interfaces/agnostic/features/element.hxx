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

protected:
    void allocate_or_reset(unsigned size) {
        if (m_result.max_index() == 0) {
            m_result = Bit_Set(size);
        } else {
            m_result.reset();
        }
    }

    void assert_concept(const Bit_Set &concept, const std::string &error) const {
        // sanity check
        if (concept.max_index() != m_problem->num_objects() + 1) {
            std::cout << error << " failed concept assertion! max_index=" << concept.max_index() << " != objects=" << m_problem->num_objects() << " + 1\n";
            exit(1);
        }
    }

    void assert_role(const Bit_Set &role, const std::string &error) const {
        // sanity check
        if (role.max_index() != m_problem->num_total_fluents() + 1) {
            std::cout << error << " failed role assertion! max_index=" << role.max_index() << " != predicates=" << m_problem->num_total_fluents() << " + 1\n";
            exit(1);
        }
    }

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
    void set_initialized(const State* state) { m_state = state; }

    /**
     * Pretty printer.
     */
    virtual void print_concept() const {
        std::cout << "{ ";
        for (unsigned i = 0; i < m_problem->num_objects(); ++i) {
            if (m_result.isset(i)) {
                std::cout << m_problem->object_index_to_object_name().at(i) << ", ";
            }
        }
        std::cout << "}" << std::endl;
    }

    virtual void print_role() const {
        std::cout << "{ ";
        for (unsigned i = 0; i < m_problem->num_total_fluents(); ++i) {
            if (m_result.isset(i)) {
                std::cout << i << " " << m_problem->predicate_index_to_predicate_signature().size() << std::endl;
                std::cout << m_problem->predicate_index_to_predicate_signature().at(i) << ", ";
            }
        }
        std::cout << "}" << std::endl;
    }
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
