#ifndef __INTERSECT_ELEMENT__
#define __INTERSECT_ELEMENT__

#include "../binary_element.hxx"

namespace aptk {


class IntersectElement : public BinaryElement {
protected:
    virtual void compute_result(const Bit_Set& left_result, const Bit_Set& right_result) override {
        m_result = left_result;
        m_result.set_intersection(right_result);
    }

public:
    IntersectElement(
        const Sketch_STRIPS_Problem* problem, bool goal, BaseElement* left, BaseElement* right)
        : BinaryElement(problem, goal, left, right, left->result_type()) {
        if (m_left->result_type() != m_right->result_type()) {
            std::cout << "IntersectElement::IntersectElement: incompatible parameters!" << std::endl;
            exit(1);
        }
    }
    virtual ~IntersectElement() = default;
};

}

#endif
