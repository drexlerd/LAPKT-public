#ifndef __ELEMENT__
#define __ELEMENT__

#include <aptk/bit_set.hxx>
#include <sketch_strips_prob.hxx>

namespace aptk {

enum class RESULT_TYPE {
    CONCEPT,
    ROLE,
};

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
    RESULT_TYPE m_result_type;

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
    RESULT_TYPE result_type() const { return m_result_type; }
    bool is_uninitialized(const State* state) const { return (!m_goal && m_state != state); }
    void set_initialized(const State* state) { m_state = state; }

    /**
     * Pretty printer.
     */
    virtual void print() const {
        std::cout << "{ ";
        if (m_result_type == RESULT_TYPE::CONCEPT) {
            for (unsigned i = 0; i < m_problem->num_objects(); ++i) {
                if (m_result.isset(i)) {
                    std::cout << m_problem->object_index_to_object_name().at(i) << ", ";
                }
            }
        } else if (m_result_type == RESULT_TYPE::ROLE) {
            for (unsigned i = 0; i < m_problem->num_total_fluents(); ++i) {
                if (m_result.isset(i)) {
                    std::cout << m_problem->predicate_index_to_predicate_signature().at(i) << ", ";
                }
            }
        }
        std::cout << "}" << std::endl;
    }
};

}

#endif
