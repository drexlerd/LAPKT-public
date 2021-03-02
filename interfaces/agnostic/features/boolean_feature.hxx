#ifndef __BOOLEAN_FEATURE__
#define __BOOLEAN_FEATURE__

#include "feature.hxx"

namespace aptk {

/**
 * The BooleanFeature implements the boolean variable that is true
 * iff the number of elements in the result is nonzero.
 */
class BooleanFeature : public BaseFeature {
protected:
    // the compositional algorithm used during evaluation
    BaseElement* const m_element;
    mutable bool old_eval;
    bool new_eval;
public:
    BooleanFeature(
        const BaseSketch* sketch,
        std::string name,
        BaseElement* const element) : BaseFeature(sketch, name), m_element(element), old_eval(false), new_eval(false) { }
    virtual ~BooleanFeature() = default;

    virtual void evaluate(const State* state) override {
        new_eval = (m_element->get_result_size(state) > 0) ? true : false;
    }

    virtual void backup_evaluation() const override {
        old_eval = new_eval;
    }

    bool get_old_eval() const {
        return old_eval;
    }

    bool get_new_eval() const {
        return new_eval;
    }

    /**
     * Pretty printer.
     */
    virtual void print() const override {
        std::cout << m_name << ": ";
        m_element->print();
    }
};

}

#endif
