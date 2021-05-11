#ifndef __NONEMPTY_FEATURE__
#define __NONEMPTY_FEATURE__

#include "../boolean_feature.hxx"

namespace aptk {

class NonemptyFeature : public BooleanFeature {
protected:
public:
    NonemptyFeature(
        const BaseSketch* sketch,
        std::string name,
        BaseElement* const element) : BooleanFeature(sketch, name, element) { }
    virtual ~NonemptyFeature() = default;

    virtual void evaluate(const State* state) override {
        new_eval = (m_element->get_result_size(state) > 0) ? true : false;
    }
};

}

#endif