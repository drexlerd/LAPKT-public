#ifndef __EXISTENTIAL_ABSTRACTION_ELEMENT__
#define __EXISTENTIAL_ABSTRACTION_ELEMENT__

#include "../binary_element.hxx"

namespace aptk {

/**
 * For the role we pass the index of relevant objects.
 */
class ExistentialAbstractionElement : public BinaryElement {
protected:
    int m_a;
    int m_b;

    virtual void compute_result(const Bit_Set& role, const Bit_Set& concept) override {
        assert_role(role, "ExistentialAbstractionElement:");
        assert_concept(concept, "ExistentialAbstractionElement:");
        allocate_or_reset(m_problem->num_objects());
        for (int i = 0; i < m_problem->num_total_fluents(); ++i) {
            const Fluent* fluent = m_problem->total_fluents()[i];
            if (role.isset(i) &&
                concept.isset(fluent->pddl_objs_idx()[m_b])) {
                // TODO: we might want to be the result correspond to predicate instead of extracting just a.
                m_result.set(fluent->pddl_objs_idx()[m_a]);
            }
        }
    }

public:
    ExistentialAbstractionElement(
        const Sketch_STRIPS_Problem* problem, bool goal, BaseElement* role, BaseElement* concept,
        unsigned a, unsigned b)
        : BinaryElement(problem, goal, role, concept),
        m_a(a), m_b(b) {
        // sanity check
        if (role->result_type() != RESULT_TYPE::PREDICATE ||
            concept->result_type() != RESULT_TYPE::OBJECT) {
            std::cout << "ExistentialAbstractionElement::ExistentialAbstractionElement: incompatible parameters!" << std::endl;
            exit(1);
        }
        m_result_type = RESULT_TYPE::OBJECT;
    }
    virtual ~ExistentialAbstractionElement() = default;
};

}

#endif
