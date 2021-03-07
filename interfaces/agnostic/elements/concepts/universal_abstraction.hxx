#ifndef __UNIVERSAL_ABSTRACTION__
#define __UNIVERSAL_ABSTRACTION__

#include "../concept.hxx"
#include "../role.hxx"

namespace aptk {

/**
 * For the role we pass the index of relevant objects.
 */
class UniversalAbstractionElement : public ConceptElement {
protected:
    RoleElement* m_role;
    ConceptElement* m_concept;

    virtual void compute_result(const Roles& role_result, const Concepts& concept_result) {
        m_result.clear();
        Concepts_Set concepts_set(concept_result.begin(), concept_result.end());
        Concepts_Set result_set;
        // 1. perform existential abstraction to find elements for which some relation to b exists.
        for (const Role& role : role_result) {
            if (concepts_set.find(role.second) != concepts_set.end()) {
                result_set.insert(role.first);
            }
        }
        // 2. remove objects for which not all relations contain object from b
        for (const Role& role : role_result) {
            if (result_set.find(role.first) != result_set.end() &&
                concepts_set.find(role.second) == concepts_set.end()) {
                result_set.erase(role.first);
            }
        }
        m_result = Concepts(result_set.begin(), result_set.end());
    }

    virtual void compute_result(const State* state) override {
        m_result.clear();
        compute_result(m_role->evaluate(state), m_concept->evaluate(state));
    }

public:
    UniversalAbstractionElement(
        const Sketch_STRIPS_Problem* problem, bool goal, RoleElement* role, ConceptElement* concept)
        : ConceptElement(problem, goal), m_role(role), m_concept(concept) {
        if (goal) {
            compute_result(role->result(), concept->result());
        }
    }
    virtual ~UniversalAbstractionElement() = default;
};

}

#endif
