#ifndef __EMPTY_FEATURE__
#define __EMPTY_FEATURE__


#include "../boolean_feature.hxx"

namespace aptk {

class EmptyFeature : public BooleanFeature {
protected:
public:
    EmptyFeature(
        const BaseSketch* sketch,
        std::string name,
        BaseElement* const element) : BooleanFeature(sketch, name, element) { }
    virtual ~EmptyFeature() = default;

    virtual void evaluate(const State* state) override {
        new_eval = (m_element->get_result_size(state) == 0) ? true : false;
    }
};

}

#endif
