#include <sketch.hxx>

namespace aptk {

class TppSketch : public BaseSketch {
public:
    TppSketch(const Sketch_STRIPS_Problem *problem);
    virtual ~TppSketch() = default;
};

}
