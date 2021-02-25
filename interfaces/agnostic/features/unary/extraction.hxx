#ifndef __EXTRACTION_ELEMENT__
#define __EXTRACTION_ELEMENT__

#include "../unary_element.hxx"

namespace aptk {


class ExtractionElement : public UnaryElement {
protected:
    const unsigned m_position;

    void compute_result(const Bit_Set& result) {
        m_result.reset();
        for (int i = 0; i < m_problem->num_total_fluents(); ++i) {
            if (result.isset(i)) {
                m_result.set(m_problem->total_fluents()[i]->pddl_objs_idx()[m_position]);
            }
        }
    }

public:
    ExtractionElement(
        const Sketch_STRIPS_Problem* problem, bool goal, BaseElement* role, unsigned position)
        : UnaryElement(problem, goal, role), m_position(position) {
        m_result = Bit_Set(problem->num_objects());
        if (goal) {

        }
    }
    virtual ~ExtractionElement() = default;
};


}

#endif
