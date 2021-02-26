#ifndef __EXTRACTION_ELEMENT__
#define __EXTRACTION_ELEMENT__

#include "../unary_element.hxx"

namespace aptk {


class ExtractionElement : public UnaryElement {
protected:
    const unsigned m_position;

    void compute_result(const Bit_Set& result) {
        if (m_result.max_index() == 0) {
            // allocate memory once
            m_result = Bit_Set(m_problem->num_objects());
        } else {
            // otherwise reset.
            m_result.reset();
        }
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
    }
    virtual ~ExtractionElement() = default;
};


}

#endif
