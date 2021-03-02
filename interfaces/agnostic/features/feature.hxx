#ifndef __FEATURE__
#define __FEATURE__

#include "../elements/element.hxx"

namespace aptk {
class BaseSketch;

class BaseFeature {
protected:
    const BaseSketch* m_sketch;
    const std::string m_name;
public:
    BaseFeature(
        const BaseSketch* sketch,
        std::string name)
        : m_sketch(sketch), m_name(name) {
    }
    virtual ~BaseFeature() = default;

    /**
     * Evaluate the feature for a given state.
     * The problem provides additional information.
     */
    virtual void evaluate(const State* state) = 0;

    virtual void backup_evaluation() const = 0;

    const BaseSketch* sketch() const { return m_sketch; }
    const std::string& name() const { return m_name; }

    /**
     * Pretty printer.
     */
    virtual void print() const = 0;
};

}

#endif
