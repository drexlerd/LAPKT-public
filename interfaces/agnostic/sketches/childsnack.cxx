#include "childsnack.hxx"

namespace aptk {

GlutenAllergicChildrenToBeServed::GlutenAllergicChildrenToBeServed(
    const BaseSketch* sketch) : NumericalFeature(sketch) {
}

void GlutenAllergicChildrenToBeServed::evaluate(const SketchState &sketch_state) {
    sketch_state.print();
    // number of children that still have to be served
    int num_children = 0;

    assert(false);
}

ChildsnackSketch::ChildsnackSketch(
    const Sketch_STRIPS_Problem *problem) : BaseSketch(problem) {
    add_numerical_feature("gluten_allergic_children_to_be_served", new GlutenAllergicChildrenToBeServed(this));
}



}
