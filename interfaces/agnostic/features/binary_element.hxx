#ifndef __BINARY_ELEMENT__
#define __BINARY_ELEMENT__

#include "element.hxx"

namespace aptk {

class BinaryElement : public BaseElement {
protected:
    BaseElement* m_left;
    BaseElement* m_right;

    /**
     * Evaluate.
     */
    virtual void compute_result(const Bit_Set& left_result, const Bit_Set& right_result) {
        std::cout << "BinaryElement::compute_result: not allowed to call abstract method" << std::endl;
        exit(1);
    }

public:
    BinaryElement(const Sketch_STRIPS_Problem* problem, bool goal,
    BaseElement* left, BaseElement* right)
    : BaseElement(problem, goal), m_left(left), m_right(right) {
        // Initialize results depending on goal.
        if (goal) {
            if (!left->goal() || !right->goal()) {
                std::cout << "BinaryElement::BinaryElement: children must evaluated at goal!" << std::endl;
                exit(1);
            }
            compute_result(left->evaluate(nullptr), right->evaluate(nullptr));
        }
    }
    ~BinaryElement() = default;

    /**
     * Evaluate for a given state if necessary
     * and return the result.
     */
    Bit_Set& evaluate(const State* state) {
        if (is_uninitialized(state)) {
            set_initialized(state);
            /*if (m_left->evaluate(state).max_index() != m_right->evaluate(state).max_index()) {
                std::cout << "BinaryElement::evaluate: incompatible results!\n" << std::endl;
                exit(1);
            }*/
            compute_result(m_left->evaluate(state), m_right->evaluate(state));
        }
        return m_result;
    }
};

}

#endif
