#ifndef _TRANSITIVE_CLOSURE_ELEMENT__
#define _TRANSITIVE_CLOSURE_ELEMENT__

#include "../unary_element.hxx"

namespace aptk {

class TransitiveClosureElement : public UnaryElement {
protected:

    void compute_result(const Bit_Set &fluent_set) {
    }

public:
    TransitiveClosureElement(
        const Sketch_STRIPS_Problem* problem, bool goal, BaseElement* role)
        : UnaryElement(problem, goal, role) {
        if (role->result_type() != RESULT_TYPE::PREDICATE) {
            std::cout << "TransitiveClosureElement::TransitiveClosureElement: Expecting predicate as input!\n";
            exit(1);
        }
        // the result is the predicate itself, not just an object
        // this is slighly more general.
        m_result_type = RESULT_TYPE::PREDICATE;
    }
    virtual ~TransitiveClosureElement() = default;

    virtual Bit_Set& evaluate(const State* state) override {

    }
};

}

#endif
