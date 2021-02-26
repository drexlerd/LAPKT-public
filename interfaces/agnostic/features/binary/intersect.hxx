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
        : BinaryElement(problem, goal, left, right) {
    }
    virtual ~IntersectElement() = default;
};

}

#endif
