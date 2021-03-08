#include <sketch.hxx>
#include "../features/numerical_feature.hxx"
#include "../features/boolean_feature.hxx"

namespace aptk {

class N_Shape : public NumericalFeature {
public:
    N_Shape(const BaseSketch* sketch, const std::string &name);
    virtual ~N_Shape() = default;
};

class N_Surface : public NumericalFeature {
public:
    N_Surface(const BaseSketch* sketch, const std::string &name);
    virtual ~N_Surface() = default;
};

class N_Color : public NumericalFeature {
public:
    N_Color(const BaseSketch* sketch, const std::string &name);
    virtual ~N_Color() = default;
};

class B_Scheduled : public BooleanFeature {
public:
    B_Scheduled(const BaseSketch* sketch, const std::string &name);
    virtual ~B_Scheduled() = default;
};

class B_Busy : public BooleanFeature {
public:
    B_Busy(const BaseSketch* sketch, const std::string &name);
    virtual ~B_Busy() = default;
};


class ScheduleSketch : public BaseSketch {
public:
    ScheduleSketch(const Sketch_STRIPS_Problem *problem);
    virtual ~ScheduleSketch() = default;
};

}
