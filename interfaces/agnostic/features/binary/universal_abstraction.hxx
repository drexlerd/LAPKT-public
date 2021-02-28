#ifndef __UNIVERSAL_ABSTRACTION_ELEMENT__
#define __UNIVERSAL_ABSTRACTION_ELEMENT__

#include "../binary_element.hxx"

namespace aptk {

/**
 * For the role we pass the index of relevant objects.
 */
class UniversalAbstractionElement : public BinaryElement {
protected:
    int m_a;
    int m_b;

    virtual void compute_result(const Bit_Set& role, const Bit_Set& concept) override {
        // sanity check
        assert_role(role, "UniversalAbstractionElement:");
        assert_concept(concept, "UniversalAbstractionElement:");
        // allocate memory
        allocate_or_reset(m_problem->num_objects());
        // 1. perform existential abstraction to find elements for which some relation to b exists.
        for (int i = 0; i < m_problem->num_total_fluents(); ++i) {
            const Fluent* fluent = m_problem->total_fluents()[i];
            unsigned obj_b = fluent->pddl_objs_idx()[m_b];
            if (role.isset(i) &&
                concept.isset(obj_b)) {
                // TODO: we might want to be the result correspond to predicate instead of extracting just a.
                unsigned obj_a = fluent->pddl_objs_idx()[m_a];
                m_result.set(obj_a);
            }
        }
        // 2. remove objects for which not all relations contain object from b
        for (int i = 0; i < m_problem->num_total_fluents(); ++i) {
            const Fluent* fluent = m_problem->total_fluents()[i];
            if (role.isset(i)) {
                unsigned obj_a = fluent->pddl_objs_idx()[m_a];
                unsigned obj_b = fluent->pddl_objs_idx()[m_b];
                if (m_result.isset(obj_a) &&
                    !concept.isset(obj_b)) {
                    m_result.unset(obj_a);
                }
            }
        }
    }

public:
    UniversalAbstractionElement(
        const Sketch_STRIPS_Problem* problem, bool goal, BaseElement* role, BaseElement* concept,
        unsigned a, unsigned b)
        : BinaryElement(problem, goal, role, concept),
        m_a(a), m_b(b) {
        // sanity check
        if (role->result_type() != RESULT_TYPE::ROLE ||
            concept->result_type() != RESULT_TYPE::CONCEPT) {
            std::cout << "UniversalAbstractionElement::UniversalAbstractionElement: incompatible parameters!" << std::endl;
            exit(1);
        }
        // TODO(dominik): must check that a and b are valid as well
        m_result_type = RESULT_TYPE::CONCEPT;
    }
    virtual ~UniversalAbstractionElement() = default;
};

}

#endif
