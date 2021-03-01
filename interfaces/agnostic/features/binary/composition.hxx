#ifndef __COMPOSITION_ELEMENT__
#define __COMPOSITION_ELEMENT__

#include "../binary_element.hxx"

namespace aptk {

class CompositionElement : public BinaryElement {
protected:
    const unsigned m_left_a;
    const unsigned m_left_b;
    const unsigned m_right_a;
    const unsigned m_right_b;

    // The fluent indices relevant to derive truth value of composed fluent.
    // This allows computation linear in the number of derived fluents
    // instead of quadratic in the number of total fluents.
    std::vector<std::pair<unsigned, unsigned>> m_derived_fluents_mapping;
    Fluent_Vec m_derived_fluents;

    virtual void compute_result(const Bit_Set &left_result, const Bit_Set &right_result) override {
        m_result.reset();
    }

public:
    CompositionElement(
        const Sketch_STRIPS_Problem* problem, bool goal,
        BaseElement* left_role, unsigned left_a, unsigned left_b,
        BaseElement* right_role, unsigned right_a, unsigned right_b)
        : BinaryElement(problem, goal, left_role, right_role, RESULT_TYPE::PREDICATE),
        m_left_a(left_a), m_left_b(left_b),
        m_right_a(right_a), m_right_b(right_b) {
        if (m_left->result_type() != RESULT_TYPE::PREDICATE ||
            m_right->result_type() != RESULT_TYPE::PREDICATE ||
            !m_left->goal() || !m_right->goal()) {
            std::cout << "CompositionElement::CompositionElement: incompatible parameters!\n";
            exit(1);
        }
        // TODO(dominik):
        // 1. instantiate new fluents that are set to true in the result
        // for each composition of the predicates given with the role.
        // 2. Precompute fluent pairs that are relevant to derive truth value of derived fluent.
        // e.g. fluent_i = (a,b), fluent_j = (b,c)
        //      => fluent_i, fluent_j relevant to derive truth value
        //         of composition_fluent_ij = (a,c)
        const Bit_Set &left_result = left_role->evaluate(nullptr);
        const Bit_Set &right_result = right_role->evaluate(nullptr);
        /*for (const Fluent* left_fluent : left_fluents) {
            unsigned left_obj_b = left_fluent->pddl_objs_idx().at(1);
            for (const Fluent* right_fluent : right_fluents) {
                unsigned right_obj_a = right_fluent->pddl_objs_idx().at(0);
                if (left_obj_b == right_obj_a) {
                    // predicates can be composed
                }
            }
        }*/
    }
    virtual ~CompositionElement() = default;
};

}

#endif
