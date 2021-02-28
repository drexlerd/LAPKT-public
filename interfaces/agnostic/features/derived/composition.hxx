#ifndef __COMPOSITION_ELEMENT__
#define __COMPOSITION_ELEMENT__

#include "../binary_element.hxx"

namespace aptk {

class CompositionElement : public BaseElement {
protected:
    const unsigned m_left_predicate_type;
    const unsigned m_right_predicate_type;

    // The fluent indices relevant to derive truth value of composed fluent.
    // This allows computation linear in the number of derived fluents
    // instead of quadratic in the number of total fluents.
    std::vector<std::pair<unsigned, unsigned>> m_derived_fluents_mapping;
    Fluent_Vec m_derived_fluents;

    void compute_result(const Bit_Set &fluent_set) {
    }

public:
    CompositionElement(
        const Sketch_STRIPS_Problem* problem, bool goal,
        const std::string &left_predicate_name, const std::string &right_predicate_name)
        : BaseElement(problem, goal),
        m_left_predicate_type(problem->predicate_type(left_predicate_name)),
        m_right_predicate_type(problem->predicate_type(right_predicate_name)) {
        if (problem->total_fluents()[m_left_predicate_type]->pddl_objs_idx().size() != 2 ||
            problem->total_fluents()[m_right_predicate_type]->pddl_objs_idx().size() != 2) {
            std::cout << "CompositionElement::CompositionElement: composition only allowed between predicates of arity 2!" << std::endl;
            exit(1);
        }
        m_result_type = RESULT_TYPE::ROLE;
        // TODO(dominik):
        // 1. instantiate new fluents that are set to true in the result
        // for each composition of the predicates given with the role.
        // 2. Precompute fluent pairs that are relevant to derive truth value of derived fluent.
        // e.g. fluent_i = (a,b), fluent_j = (b,c)
        //      => fluent_i, fluent_j relevant to derive truth value
        //         of composition_fluent_ij = (a,c)
        std::vector<const Fluent*> left_fluents;
        std::vector<const Fluent*> right_fluents;
        for (const Fluent* fluent : m_problem->total_fluents()) {
            if (fluent->pddl_predicate_type() == m_left_predicate_type) {
                left_fluents.push_back(fluent);
            }
            if (fluent->pddl_predicate_type() == m_right_predicate_type) {
                right_fluents.push_back(fluent);
            }
        }
        for (const Fluent* left_fluent : left_fluents) {
            unsigned left_obj_b = left_fluent->pddl_objs_idx().at(1);
            for (const Fluent* right_fluent : right_fluents) {
                unsigned right_obj_a = right_fluent->pddl_objs_idx().at(0);
                if (left_obj_b == right_obj_a) {
                    // predicates can be composed
                }
            }
        }
    }
    virtual ~CompositionElement() = default;
};

}

#endif
