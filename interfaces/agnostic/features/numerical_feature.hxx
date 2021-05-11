#ifndef __NUMERICAL_FEATURE__
#define __NUMERICAL_FEATURE__

#include "feature.hxx"

namespace aptk {

/**
 * The NumericalFeature implements counting of objects in the result.
 */
class NumericalFeature : public BaseFeature {
protected:
    // the compositional algorithm used during evaluation
    BaseElement* const m_element;
    mutable int old_eval;
    int new_eval;
public:
    NumericalFeature(
        const BaseSketch* sketch,
        std::string name,
        BaseElement* const element)
        : BaseFeature(sketch, name), m_element(element), old_eval(0), new_eval(0) {
    }
    virtual ~NumericalFeature() = default;

    virtual void evaluate(const State* state) override {
        new_eval = m_element->get_result_size(state);
        // std::cout << "n: " << new_eval << std::endl;
    }

    virtual void backup_evaluation() const override {
        old_eval = new_eval;
    }

    int get_old_eval() const {
        return old_eval;
    }

    int get_new_eval() const {
        return new_eval;
    }

    /**
     * Pretty printer.
     */
    virtual void print() const override {
        if (m_element == nullptr) {
            std::cout << "NumericalFeature::print: tried printing on nullptr!" << std::endl;
            exit(1);
        }
        std::cout << m_name << ": ";
        m_element->print();
    }
};

}

#endif
