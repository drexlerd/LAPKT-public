#ifndef __ELEMENTS__
#define __ELEMENTS__

#include <aptk/bit_set.hxx>
#include <sketch_strips_prob.hxx>

namespace aptk {

// Note: standard set operations over Bit_Set is linear in number of atoms
using ObjectsResult = Bit_Set;
// TODO: find efficient storage / manipulation for roles.
// Probably store objects occuring in relation
// to compress the size of an adjacency matrix to only relevant objects.
using RelationsResult = std::vector<Bit_Set>;

/**
 * Abstract concepts and roles.
 */

template<typename T>
class BaseElement {
protected:
    // Reference to problem to obtain relevant information
    const Sketch_STRIPS_Problem* m_problem;
    // Goal is true iff the element is evaluated in goal only
    bool m_goal;
    // Reference to state used during evaluation.
    const State* m_state;
    // Preallocated memory to store evaluation result
    T m_result;

public:
    BaseElement(const Sketch_STRIPS_Problem* problem, bool goal) : m_problem(problem), m_goal(goal) {}
    virtual ~BaseElement() = default;
    /**
     * Evaluate the Element for a given state
     * and return a reference to the result.
     */
    virtual const T& evaluate(const State* state=nullptr) = 0;

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

class BaseConceptElement : public BaseElement<ObjectsResult> {
protected:
public:
    // Empty constructor used in more complex concept elements.
    BaseConceptElement(const Sketch_STRIPS_Problem* problem, bool goal) : BaseElement(problem, goal) {}
    virtual ~BaseConceptElement() = default;

    virtual const ObjectsResult& evaluate(const State* state) = 0;

    void print_result() const {
        std::cout << "{ ";
        for (unsigned i = 0; i < m_problem->num_objects(); ++i) {
            if (m_result.isset(i)) {
                std::cout << m_problem->index_to_object_name().at(i) << ", ";
            }
        }
        std::cout << "}" << std::endl;
    }
};

class LeafConceptElement : public BaseConceptElement {
protected:
    const unsigned m_predicate_type;
    const unsigned m_position;

    void set_result(const Bit_Set &fluent_set) {
        m_result.reset();
        const std::vector<const Fluent*> fluents = m_problem->total_fluents();
        for (unsigned i = 0; i < m_problem->num_total_fluents(); ++i) {
            if (!fluent_set.isset(i)) continue;
            const Fluent* fluent = fluents[i];
            if (fluent->pddl_predicate_type() == m_predicate_type) {
                m_result.set(fluent->pddl_objs_idx()[m_position]);
            }
        }
    }
public:
    LeafConceptElement(
        const Sketch_STRIPS_Problem* problem, bool goal, std::string predicate_name, unsigned position)
        : BaseConceptElement(problem, goal), m_predicate_type(problem->predicate_index(predicate_name)), m_position(position) {
        assert(m_problem);
        // we also consider fluents that remain constant.
        m_result = Bit_Set(m_problem->num_objects());
        if (this->goal()) {
            set_result(m_problem->goal_fluents_set());
        }
    }
    virtual ~LeafConceptElement() = default;

    virtual const ObjectsResult& evaluate(const State* state) override {
        if (is_uninitialized(state)) {
            m_state = state;
            set_result(m_problem->state_fluents_set(state));
        }
        return m_result;
    }
};


class BaseRoleElement : public BaseElement<RelationsResult> {
public:
    BaseRoleElement(const Sketch_STRIPS_Problem* problem, bool goal) : BaseElement(problem, goal) {}
    virtual ~BaseRoleElement() = default;

    virtual const RelationsResult& evaluate(const State* state) override {
        return m_result;
    }
};

/**
 * Concrete concepts and roles.
 */

class IntersectConceptElement : public BaseConceptElement {
protected:
    BaseConceptElement* m_left;
    BaseConceptElement* m_right;

    void set_result(const State* state) {
        m_result = m_left->evaluate(state);
        m_result.set_intersection(m_right->evaluate(state));
    }

public:
    IntersectConceptElement(
        const Sketch_STRIPS_Problem* problem, bool goal, BaseConceptElement* left, BaseConceptElement* right)
        : BaseConceptElement(problem, goal), m_left(left), m_right(right) {
        if (this->goal()) {
            // children must be goal initialized as well
            assert(m_left->goal() && m_right->goal());
            set_result(nullptr);
        }
    }
    virtual ~IntersectConceptElement() = default;

    virtual const ObjectsResult& evaluate(const State* state) override {
        if (is_uninitialized(state)) {
            m_state = state;
            set_result(state);
        }
        return m_result;
    }
};

class SetminusConceptElement : public BaseConceptElement {
protected:
    BaseConceptElement* m_left;
    BaseConceptElement* m_right;

    void set_result(const State* state) {
        m_result = m_left->evaluate(state);
        const ObjectsResult &right_result = m_right->evaluate(state);
        for (unsigned i = 0; i < m_problem->num_objects(); ++i) {
            if (right_result.isset(i)) {
                m_result.unset(i);
            }
        }
    }

public:
    SetminusConceptElement(
        const Sketch_STRIPS_Problem* problem, bool goal, BaseConceptElement* left, BaseConceptElement* right)
        : BaseConceptElement(problem, goal), m_left(left), m_right(right) {
        if (this->goal()) {
            // children must be goal initialized as well
            assert(m_left->goal() && m_right->goal());
            set_result(nullptr);
        }
    }
    virtual ~SetminusConceptElement() = default;

    virtual const ObjectsResult& evaluate(const State* state) override {
        if (is_uninitialized(state)) {
            m_state = state;
            set_result(state);
        }
        return m_result;
    }
};

}

#endif
