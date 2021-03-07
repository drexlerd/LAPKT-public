#ifndef __EXISTENTIAL_ABSTRACTION__
#define __EXISTENTIAL_ABSTRACTION__

#include "../concept.hxx"
#include "../role.hxx"

namespace aptk {

class ExistentialAbstractionElement : public ConceptElement {
protected:
    RoleElement* m_role;
    ConceptElement* m_concept;

    virtual void compute_result(const Roles& role_result, const Concepts& concept_result) {
        m_result.clear();
        Concepts_Set concepts_set(concept_result.begin(), concept_result.end());
        for (const Role& role : role_result) {
            if (concepts_set.find(role.second) != concepts_set.end()) {
                m_result.push_back(role.first);
            }
        }
    }

    virtual void compute_result(const State* state) override {
        m_result.clear();
        compute_result(m_role->evaluate(state), m_concept->evaluate(state));
    }

public:
    ExistentialAbstractionElement(
        const Sketch_STRIPS_Problem* problem, bool goal, RoleElement* role, ConceptElement* concept)
        : ConceptElement(problem, goal), m_role(role), m_concept(concept) {
        if (goal) {
            compute_result(role->result(), concept->result());
        }
    }
    virtual ~ExistentialAbstractionElement() = default;
};

}

#endif
