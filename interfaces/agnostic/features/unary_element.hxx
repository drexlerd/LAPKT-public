#ifndef __UNARY_ELEMENT__
#define __UNARY_ELEMENT__

#include "element.hxx"

namespace aptk {

class UnaryElement : public BaseElement {
protected:
    BaseElement* m_element;

    /**
     * Evaluate.
     */
    virtual void compute_result(const Bit_Set& result) {
        std::cout << "UnaryConceptElement::compute_result: not allowed to call abstract method" << std::endl;
        exit(1);
    }

public:
    UnaryElement(const Sketch_STRIPS_Problem* problem, bool goal,
    BaseElement* element)
    : BaseElement(problem, goal), m_element(element) {
        // Initialize results depending on goal.
        if (goal) {
            if (!element->goal()) {
                std::cout << "UnaryConceptElement::BinaryConceptElement: children must evaluated at goal!\n";
                exit(1);
            }
            compute_result(element->evaluate(nullptr));
        }
    }
    ~UnaryElement() = default;

    /**
     * Evaluate for a given state if necessary
     * and return the result.
     */
    Bit_Set& evaluate(const State* state) {
        if (is_uninitialized(state)) {
            m_state = state;
            compute_result(m_element->evaluate(state));
        }
        return m_result;
    }
};

}

#endif
