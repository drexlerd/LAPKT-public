#include <strips_state.hxx>
#include <sketch_state.hxx>
#include <aptk/bit_set.hxx>

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
    // Reference to state used during evaluation.
    const State* m_state;
    // Preallocated memory to store evaluation result
    T m_result;
public:
    BaseElement() = default;
    virtual ~BaseElement() = default;
    /**
     * Evaluate the Element for a given state
     * and return a reference to the result.
     */
    virtual const T& evaluate(const SketchState &sketch_state) = 0;
};

class ConceptElement : public BaseElement<ObjectsResult> {
public:
    // Empty constructor used in more complex concept elements.
    ConceptElement() : BaseElement() {}
    // Construct concept element from fluent vector.
    // The fluents can be goal or state fluents.
    ConceptElement(
        const Sketch_STRIPS_Problem* problem,
        const std::vector<const Fluent*> &fluents,
        unsigned predicate_type,
        unsigned position) : BaseElement() {
        m_result = Bit_Set(problem->fluents().size());
        // to ensure that the bit set is reset
        m_result.reset();
        // set the corresponding bits
        for (const Fluent* fluent : fluents) {
            assert(fluent->pddl_objs_idx().size() > position);
            if (fluent->pddl_predicate_type() == predicate_type) {
                m_result.set(fluent->pddl_objs_idx()[position]);
            }
        }
    }
    virtual ~ConceptElement() = default;

    virtual const ObjectsResult& evaluate(const SketchState &sketch_state) override {
        return m_result;
    }
};

class RoleElement : public BaseElement<RelationsResult> {
public:
    RoleElement() : BaseElement() {}
    virtual ~RoleElement() = default;

    virtual const RelationsResult& evaluate(const SketchState &sketch_state) override {
        return m_result;
    }
};

/**
 * Concrete concepts and roles.
 */

class IntersectConceptElement : public ConceptElement {
    ConceptElement* m_left;
    ConceptElement* m_right;
public:
    IntersectConceptElement(
        ConceptElement* left, ConceptElement* right)
        : m_left(left), m_right(right) { }
    virtual ~IntersectConceptElement();

    virtual const ObjectsResult& evaluate(const SketchState &sketch_state) override {
        if (m_state != sketch_state.state()) {
            m_state = sketch_state.state();
            m_result = m_left->evaluate(sketch_state);
            m_result.set_intersection(m_right->evaluate(sketch_state));
        }
        return m_result;
    }
};

class SetminusConceptElement : public ConceptElement {
    ConceptElement* m_left;
    ConceptElement* m_right;
public:
    SetminusConceptElement(
        ConceptElement* left, ConceptElement* right)
        : m_left(left), m_right(right) { }
    virtual ~SetminusConceptElement();

    virtual const ObjectsResult& evaluate(const SketchState &sketch_state) override {
        if (m_state != sketch_state.state()) {
            m_result = m_left->evaluate(sketch_state);
            const ObjectsResult &right_result = m_right->evaluate(sketch_state);
            for (int i = 0; i < m_result.size(); ++i) {
                if (right_result.isset(i)) {
                    m_result.unset(i);
                }
            }
        }
        return m_result;
    }
};

}
