#ifndef __COUNT_FEATURE__
#define __COUNT_FEATURE__


#include "../numerical_feature.hxx"

namespace aptk {

class CountFeature : public NumericalFeature {
protected:
public:
    CountFeature(
        const BaseSketch* sketch,
        std::string name,
        BaseElement* const element)
        : NumericalFeature(sketch, name, element) {
    }

    virtual void evaluate(const State* state) override {
        new_eval = m_element->get_result_size(state);
    }

    virtual ~CountFeature() = default;

};

}

#endif
