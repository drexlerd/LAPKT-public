#ifndef _ROLE_IDENTITY__
#define _ROLE_IDENTITY__

#include "../role.hxx"

namespace aptk {


class RoleIdentityElement : public RoleElement {
protected:
    ConceptElement* m_concept;

    virtual void compute_result(const Concepts& concepts_result) {
        m_result.clear();
        for (Concept c : concepts_result) {
            m_result.emplace_back(c, c);
        }
    }

    virtual void compute_result(const State* state) override {
        m_result.clear();
        compute_result(m_concept->evaluate(state));
    }

public:
    RoleIdentityElement(const Sketch_STRIPS_Problem* problem, bool goal, ConceptElement* concept)
    : RoleElement(problem, goal), m_concept(concept) {
        if (goal) {
            compute_result(m_concept->result());
        }
    }
    virtual ~RoleIdentityElement() = default;
};

}

#endif
