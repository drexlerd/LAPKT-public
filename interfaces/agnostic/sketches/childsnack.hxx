#ifndef __CHILDSNACK_DRIVERLOG__
#define __CHILDSNACK_DRIVERLOG__

#include <sketch.hxx>

namespace aptk {
class BaseElement;


class N_GlutenAllergicChildrenToBeServed : public NumericalFeature {
private:
    BaseElement* m_allergic_children_to_be_served;
public:
    N_GlutenAllergicChildrenToBeServed(const BaseSketch* sketch, const std::string &name);
    virtual ~N_GlutenAllergicChildrenToBeServed() = default;

    virtual void evaluate(const State* state) override;

    virtual void print() const override;
};

class N_RegularChildrenToBeServed : public NumericalFeature {
private:
    BaseElement* m_regular_children_to_be_served;
public:
    N_RegularChildrenToBeServed(const BaseSketch* sketch, const std::string &name);
    virtual ~N_RegularChildrenToBeServed() = default;

    virtual void evaluate(const State* state) override;

    virtual void print() const override;
};

class B_GlutenFreeSandwichAtKitchen : public BooleanFeature {
private:
    BaseElement* m_gluten_free_sandwich_at_kitchen;
public:
    B_GlutenFreeSandwichAtKitchen(const BaseSketch* sketch, const std::string &name);
    virtual ~B_GlutenFreeSandwichAtKitchen() = default;

    virtual void evaluate(const State* state) override;

    virtual void print() const override;
};

class B_RegularSandwichAtKitchen : public BooleanFeature {
private:
    BaseElement* m_regular_sandwich_at_kitchen;
public:
    B_RegularSandwichAtKitchen(const BaseSketch* sketch, const std::string &name);
    virtual ~B_RegularSandwichAtKitchen() = default;

    virtual void evaluate(const State* state) override;

    virtual void print() const override;
};

class B_GlutenFreeSandwichOnTray : public BooleanFeature {
private:
    BaseElement* m_gluten_free_sandwiches_on_tray;
public:
    B_GlutenFreeSandwichOnTray(const BaseSketch* sketch, const std::string &name);
    virtual ~B_GlutenFreeSandwichOnTray() = default;

    virtual void evaluate(const State* state) override;

    virtual void print() const override;
};

class B_RegularSandwichOnTray : public BooleanFeature {
private:
    BaseElement* m_regular_sandwiches_on_tray;
public:
    B_RegularSandwichOnTray(const BaseSketch* sketch, const std::string &name);
    virtual ~B_RegularSandwichOnTray() = default;

    virtual void evaluate(const State* state) override;

    virtual void print() const override;
};


class ChildsnackSketch : public BaseSketch {
public:
    ChildsnackSketch(const Sketch_STRIPS_Problem *problem);
    virtual ~ChildsnackSketch() = default;
};

}

#endif
