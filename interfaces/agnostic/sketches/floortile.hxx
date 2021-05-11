#include <sketch.hxx>
#include "../features/boolean_features/nonempty.hxx"
#include "../features/numerical_features/count.hxx"

namespace aptk {

class N_Unpainted : public CountFeature {
public:
    N_Unpainted(const BaseSketch* sketch, const std::string &name);
    virtual ~N_Unpainted() = default;
};

class B_InvariantPaintable : public NonemptyFeature {
public:
    B_InvariantPaintable(const BaseSketch* sketch, const std::string &name);
    virtual ~B_InvariantPaintable() = default;
};

class FloortileSketch : public BaseSketch {
public:
    FloortileSketch(const Sketch_STRIPS_Problem *problem);
    virtual ~FloortileSketch() = default;
};

}
