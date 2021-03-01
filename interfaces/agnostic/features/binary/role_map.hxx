#ifndef __ROLE_MAP_ELEMENT__
#define __ROLE_MAP_ELEMENT__

#include "../binary_element.hxx"
#include <unordered_set>

namespace aptk {

class RoleMapElement : public BinaryElement {
protected:
    const unsigned m_left_a;
    const unsigned m_left_b;
    const unsigned m_right_a;
    const unsigned m_right_b;

    // objects that can occur at position a
    std::unordered_set<unsigned> m_objs_a;
    // objects that can occur at position a
    std::unordered_set<unsigned> m_objs_b;
    // m_left_a_b=1 iff predicate with objects a and b is true in s
    std::vector<Bit_Set> m_left_a_b;
    // m_right_a_b=1 iff predicate with objects a and b is true in s
    std::vector<Bit_Set> m_right_a_b;

    virtual void compute_result(const Bit_Set &left_result, const Bit_Set &right_result) override {
        // initialize result
        m_result.reset();
        // collect objects that can possibly occur at a
        for (unsigned i = 0; i < m_problem->num_total_fluents(); ++i) {
            if (left_result.isset(i)) {
                const Fluent* fluent = m_problem->total_fluents()[i];
                m_objs_a.insert(fluent->pddl_objs_idx()[m_left_a]);
            }
        }
        // collect objects that can possibly occur at b


        for (unsigned i = 0; i < m_problem->num_total_fluents(); ++i) {
            for (unsigned j = 0; j < m_problem->num_total_fluents(); ++j) {

            }
        }
    }

public:
    RoleMapElement(
        const Sketch_STRIPS_Problem* problem, bool goal,
        BaseElement* left_role, unsigned left_a, unsigned left_b,
        BaseElement* right_role, unsigned right_a, unsigned right_b)
        : BinaryElement(problem, goal, left_role, right_role, RESULT_TYPE::PREDICATE),
        m_left_a(left_a), m_left_b(left_b), m_right_a(right_a), m_right_b(right_b) {
        if (m_left->result_type() != RESULT_TYPE::PREDICATE ||
            m_right->result_type() != RESULT_TYPE::PREDICATE) {
            std::cout << "RoleMapElement::RoleMapElement: Expecting predicates as input!\n";
            exit(1);
        }
    }
    virtual ~RoleMapElement() = default;

    virtual Bit_Set& evaluate(const State* state) override {

    }
};

}

#endif
