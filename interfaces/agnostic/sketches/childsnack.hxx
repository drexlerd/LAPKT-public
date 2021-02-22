#include <sketch.hxx>

namespace aptk {

class GlutenAllergicChildrenToBeServed : public NumericalFeature {
public:
    GlutenAllergicChildrenToBeServed(const BaseSketch* sketch);
    virtual ~GlutenAllergicChildrenToBeServed() = default;

    virtual void evaluate(const State* state) override;
};

class RegularChildrenToBeServed : public NumericalFeature {
public:
    RegularChildrenToBeServed(const BaseSketch* sketch);
    virtual ~RegularChildrenToBeServed() = default;

    virtual void evaluate(const State* state) override;
};

class GlutenFreeSandwichesAtKitchen : public NumericalFeature {
public:
    GlutenFreeSandwichesAtKitchen(const BaseSketch* sketch);
    virtual ~GlutenFreeSandwichesAtKitchen() = default;

    virtual void evaluate(const State* state) override;
};

class RegularSandwichesAtKitchen : public NumericalFeature {
public:
    RegularSandwichesAtKitchen(const BaseSketch* sketch);
    virtual ~RegularSandwichesAtKitchen() = default;

    virtual void evaluate(const State* state) override;
};

class GlutenFreeSandwichesBeingServed : public NumericalFeature {
public:
    GlutenFreeSandwichesBeingServed(const BaseSketch* sketch);
    virtual ~GlutenFreeSandwichesBeingServed() = default;

    virtual void evaluate(const State* state) override;
};


class ChildsnackSketch : public BaseSketch {
public:
    ChildsnackSketch(const Sketch_STRIPS_Problem *problem);
    virtual ~ChildsnackSketch() = default;
};

}
