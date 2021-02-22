#include <sketch.hxx>

namespace aptk {

class FloortileSketch : public BaseSketch {
public:
    FloortileSketch(const Sketch_STRIPS_Problem *problem);
    virtual ~FloortileSketch() = default;
};

}
