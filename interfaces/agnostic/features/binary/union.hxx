#ifndef __UNION_ELEMENT__
#define __UNION_ELEMENT__

#include "../binary_element.hxx"

namespace aptk {

class UnionElement : public BinaryElement {
protected:
    virtual void compute_result(const Bit_Set& left_result, const Bit_Set& right_result) override {
        m_result = left_result;
        m_result.set_union(right_result);
    }

public:
    UnionElement(
        const Sketch_STRIPS_Problem* problem, bool goal, BaseElement* left, BaseElement* right)
        : BinaryElement(problem, goal, left, right) {
    }
    virtual ~UnionElement() = default;

    virtual void print_result() const override {

    }
};

}

#endif
