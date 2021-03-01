#ifndef __SETMINUS_ELEMENT__
#define __SETMINUS_ELEMENT__

#include "../binary_element.hxx"


namespace aptk {

class SetminusElement : public BinaryElement {
protected:
    virtual void compute_result(const Bit_Set& left_result, const Bit_Set& right_result) override {
        m_result = left_result;
        for (unsigned i = 0; i < left_result.max_index(); ++i) {
            if (right_result.isset(i)) {
                m_result.unset(i);
            }
        }
    }

public:
    SetminusElement(
        const Sketch_STRIPS_Problem* problem, bool goal, BaseElement* left, BaseElement* right)
        : BinaryElement(problem, goal, left, right, left->result_type()) {
        if (m_left->result_type() != m_right->result_type()) {
            std::cout << "SetminusElement::SetminusElement: incompatible parameters!" << std::endl;
            exit(1);
        }
    }
    virtual ~SetminusElement() = default;
};


}

#endif
