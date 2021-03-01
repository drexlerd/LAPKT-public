#ifndef _TRANSITIVE_CLOSURE_ELEMENT__
#define _TRANSITIVE_CLOSURE_ELEMENT__

#include "../unary_element.hxx"

namespace aptk {

class TransitiveClosureElement : public UnaryElement {
protected:

    void compute_result(const Bit_Set &fluent_set) {
    }

    virtual bool assert_parameters() const override {
        if (m_element->result_type() != RESULT_TYPE::PREDICATE) {
            std::cout << "TransitiveClosureElement::TransitiveClosureElement: incompatible parameters!\n";
            exit(1);
        }
    }

public:
    TransitiveClosureElement(
        const Sketch_STRIPS_Problem* problem, bool goal, BaseElement* role)
        : UnaryElement(problem, goal, role, RESULT_TYPE::PREDICATE) {
    }
    virtual ~TransitiveClosureElement() = default;
};

}

#endif
