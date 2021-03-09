#ifndef _ROLE_RESTRICTION__
#define _ROLE_RESTRICTION__

#include "../role.hxx"
#include "../concept.hxx"

namespace aptk {


class RoleRestrictionElement : public RoleElement {
protected:
    RoleElement* m_role;
    ConceptElement* m_concept;

    virtual void compute_result(const Roles& role_result, const Concepts& concept_result) {
        Concepts_Set concept_result_set(concept_result.begin(), concept_result.end());
        for (const Role& r : role_result) {
            if (concept_result_set.find(r.second) != concept_result_set.end()) {
                m_result.push_back(r);
            }
        }
    }

    virtual void compute_result(const State* state) override {
        m_result.clear();
        compute_result(m_role->evaluate(state), m_concept->evaluate(state));
    }

public:
    RoleRestrictionElement(const Sketch_STRIPS_Problem* problem, bool goal, RoleElement* role, ConceptElement* concept)
    : RoleElement(problem, goal), m_role(role), m_concept(concept) {
        if (goal) {
            compute_result(m_role->result(), m_concept->result());
        }
    }
    virtual ~RoleRestrictionElement() = default;
};

}

#endif
