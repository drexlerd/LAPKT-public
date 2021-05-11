#ifndef __CHILDSNACK_SKETCH__
#define __CHILDSNACK_SKETCH__

#include <sketch.hxx>
#include "../features/boolean_features/nonempty.hxx"
#include "../features/numerical_features/count.hxx"

namespace aptk {
class BaseElement;

class N_GlutenAllergicChildrenToBeServed : public CountFeature {
public:
    N_GlutenAllergicChildrenToBeServed(const BaseSketch* sketch, const std::string &name);
    virtual ~N_GlutenAllergicChildrenToBeServed() = default;
};

class N_RegularChildrenToBeServed : public CountFeature {
public:
    N_RegularChildrenToBeServed(const BaseSketch* sketch, const std::string &name);
    virtual ~N_RegularChildrenToBeServed() = default;
};

class B_GlutenFreeSandwichAtKitchen : public NonemptyFeature {
public:
    B_GlutenFreeSandwichAtKitchen(const BaseSketch* sketch, const std::string &name);
    virtual ~B_GlutenFreeSandwichAtKitchen() = default;
};

class B_RegularSandwichAtKitchen : public NonemptyFeature {
public:
    B_RegularSandwichAtKitchen(const BaseSketch* sketch, const std::string &name);
    virtual ~B_RegularSandwichAtKitchen() = default;
};

class B_GlutenFreeSandwichOnTray : public NonemptyFeature {
public:
    B_GlutenFreeSandwichOnTray(const BaseSketch* sketch, const std::string &name);
    virtual ~B_GlutenFreeSandwichOnTray() = default;
};

class B_RegularSandwichOnTray : public NonemptyFeature {
public:
    B_RegularSandwichOnTray(const BaseSketch* sketch, const std::string &name);
    virtual ~B_RegularSandwichOnTray() = default;
};

class B_SandwichOnTray : public NonemptyFeature {
public:
    B_SandwichOnTray(const BaseSketch* sketch, const std::string &name);
    virtual ~B_SandwichOnTray() = default;
};


class ChildsnackSketch : public BaseSketch {
public:
    ChildsnackSketch(const Sketch_STRIPS_Problem *problem);
    virtual ~ChildsnackSketch() = default;
};

}

#endif
