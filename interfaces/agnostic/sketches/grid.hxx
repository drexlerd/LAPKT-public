#include <sketch.hxx>

namespace aptk {

class GridSketch : public BaseSketch {
public:
    GridSketch(const Sketch_STRIPS_Problem *problem);
    virtual ~GridSketch() = default;
};

}
