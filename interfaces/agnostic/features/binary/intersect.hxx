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

    virtual bool assert_parameters() const override {
        // nothing to be checked
        if (m_left->result_type() != m_right->result_type()) {
            std::cout << "IntersectElement::IntersectElement: incompatible parameters!" << std::endl;
            exit(1);
        }
    }

public:
    IntersectElement(
        const Sketch_STRIPS_Problem* problem, bool goal, BaseElement* left, BaseElement* right)
        : BinaryElement(problem, goal, left, right, left->result_type()) {
        m_result_type = left->result_type();
    }
    virtual ~IntersectElement() = default;
};

}

#endif
