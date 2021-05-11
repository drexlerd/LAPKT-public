
#ifndef __SCHEDULE_SKETCH__
#define __SCHEDULE_SKETCH__

#include <sketch.hxx>
#include "../features/numerical_features/count.hxx"
#include "../features/boolean_features/nonempty.hxx"

namespace aptk {

class N_Shape : public CountFeature {
public:
    N_Shape(const BaseSketch* sketch, const std::string &name);
    virtual ~N_Shape() = default;
};

class N_Surface : public CountFeature {
public:
    N_Surface(const BaseSketch* sketch, const std::string &name);
    virtual ~N_Surface() = default;
};

class N_Color : public CountFeature {
public:
    N_Color(const BaseSketch* sketch, const std::string &name);
    virtual ~N_Color() = default;
};

class B_ScheduledOrBusy : public NonemptyFeature {
public:
    B_ScheduledOrBusy(const BaseSketch* sketch, const std::string &name);
    virtual ~B_ScheduledOrBusy() = default;
};


class N_Hot : public CountFeature {
public:
    N_Hot(const BaseSketch* sketch, const std::string &name);
    virtual ~N_Hot() = default;
};


class ScheduleSketch : public BaseSketch {
public:
    ScheduleSketch(const Sketch_STRIPS_Problem *problem);
    virtual ~ScheduleSketch() = default;
};

}

#endif
