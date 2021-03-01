#ifndef __CONCEPT_EXTRACTION__
#define __CONCEPT_EXTRACTION__

#include "../concept.hxx"
#include "../role.hxx"

namespace aptk {


class ConceptExtractionElement : public ConceptElement {
protected:
    RoleElement* m_role;
    const unsigned m_position;

    void compute_result(const State* state) {
        m_result.clear();
        for (const Role& role : m_role->evaluate(state)) {
            if (m_position == 0) {
                m_result.push_back(role.first);
            } else if (m_position == 1) {
                m_result.push_back(role.second);
            } else {
                std::cout << "ConceptExtractionElement::compute_result: only position 0 or 1 allowed!" << std::endl;
                exit(1);
            }
        }
    }

public:
    ConceptExtractionElement(const Sketch_STRIPS_Problem* problem, bool goal, RoleElement* role, unsigned position)
    : ConceptElement(problem, goal), m_role(role), m_position(position) {
        if (goal) {
            for (const Role& role : role->result()) {
                if (position == 0) {
                    m_result.push_back(role.first);
                } else if (position == 1) {
                    m_result.push_back(role.second);
                } else {
                    std::cout << "ConceptExtractionElement::ConceptExtractionElement: only position 0 or 1 allowed!" << std::endl;
                    exit(1);
                }
            }
        }
    }
    virtual ~ConceptExtractionElement() = default;
};

}

#endif
