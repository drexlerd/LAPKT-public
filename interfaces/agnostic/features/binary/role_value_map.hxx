#ifndef __ROLE_VALUE_MAP_ELEMENT__
#define __ROLE_VALUE_MAP_ELEMENT__

#include "../binary_element.hxx"

namespace aptk {

class RoleValueMapElement : public BinaryElement {
protected:
    const unsigned m_left_a;
    const unsigned m_left_b;
    const unsigned m_right_a;
    const unsigned m_right_b;

    virtual void compute_result(const Bit_Set &left_result, const Bit_Set &right_result) override {
    }

public:
    RoleValueMapElement(
        const Sketch_STRIPS_Problem* problem, bool goal,
        BaseElement* left_role, unsigned left_a, unsigned left_b,
        BaseElement* right_role, unsigned right_a, unsigned right_b)
        : BinaryElement(problem, goal, left_role, right_role),
        m_left_a(left_a), m_left_b(left_b), m_right_a(right_a), m_right_b(right_b) {
        if (left_role->result_type() != RESULT_TYPE::PREDICATE &&
            right_role->result_type() != RESULT_TYPE::PREDICATE) {
            std::cout << "CompositionElement::CompositionElement: Expecting predicates as input!\n";
            exit(1);
        }
        // the result is the predicate itself, not just an object
        // this is slighly more general.
        m_result_type = RESULT_TYPE::PREDICATE;
    }
    virtual ~RoleValueMapElement() = default;

    virtual Bit_Set& evaluate(const State* state) override {

    }
};

}

#endif
