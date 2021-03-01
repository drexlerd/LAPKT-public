#ifndef __EXISTENTIAL_ABSTRACTION__
#define __EXISTENTIAL_ABSTRACTION__

#include "../concept.hxx"
#include "../role.hxx"

namespace aptk {

class ExistentialAbstractionElement : public ConceptElement {
protected:
    RoleElement* m_role;
    ConceptElement* m_concept;

    virtual void compute_result(const State* state) override {
        m_result.clear();
        Concepts_Set concepts_set(m_concept->evaluate(state).begin(), m_concept->evaluate(state).end());
        for (const Role& role : m_role->evaluate(state)) {
            if (concepts_set.find(role.second) != concepts_set.end()) {
                m_result.push_back(role.first);
            }
        }
    }


public:
    ExistentialAbstractionElement(
        const Sketch_STRIPS_Problem* problem, bool goal, RoleElement* role, ConceptElement* concept)
        : ConceptElement(problem, goal), m_role(role), m_concept(concept) {
        if (goal) {
            Concepts_Set concepts_set(concept->result().begin(), concept->result().end());
            for (const Role& role : role->result()) {
                if (concepts_set.find(role.second) != concepts_set.end()) {
                    m_result.push_back(role.first);
                }
            }
        }
    }
    virtual ~ExistentialAbstractionElement() = default;
};

}

#endif
