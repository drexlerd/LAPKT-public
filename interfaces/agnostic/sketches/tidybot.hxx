#include <sketch.hxx>

namespace aptk {

class TidybotSketch : public BaseSketch {
public:
    TidybotSketch(const Sketch_STRIPS_Problem *problem);
    virtual ~TidybotSketch() = default;
};

}
