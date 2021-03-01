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
        m_result.reset();
        for (int i = 0; i < m_problem->num_total_fluents(); ++i) {
            const Fluent* fluent = m_problem->total_fluents()[i];
            if (role.isset(i) &&
                concept.isset(fluent->pddl_objs_idx()[m_b])) {
                // TODO: we might want to be the result correspond to predicate instead of extracting just a.
                m_result.set(fluent->pddl_objs_idx()[m_a]);
            }
        }
    }

    virtual bool assert_parameters() const override {
        // sanity check
        if (m_left->result_type() != RESULT_TYPE::PREDICATE &&
            m_right->result_type() != RESULT_TYPE::OBJECT) {
            std::cout << "ExistentialAbstractionElement::ExistentialAbstractionElement: Expecting predicates as input!\n";
            exit(1);
        }
    }

public:
    ExistentialAbstractionElement(
        const Sketch_STRIPS_Problem* problem, bool goal, BaseElement* role, BaseElement* concept,
        unsigned a, unsigned b)
        : BinaryElement(problem, goal, role, concept, RESULT_TYPE::OBJECT),
        m_a(a), m_b(b) {
    }
    virtual ~ExistentialAbstractionElement() = default;
};

}

#endif
