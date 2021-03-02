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

    virtual void compute_result(const State* state) override {
        m_result.clear();
        m_concept->evaluate(state);
        Concepts_Set concepts_set(m_concept->evaluate(state).begin(), m_concept->evaluate(state).end());
        Concepts_Set result_set;
        // 1. perform existential abstraction to find elements for which some relation to b exists.
        for (const Role& role : m_role->evaluate(state)) {
            if (concepts_set.find(role.second) != concepts_set.end()) {
                result_set.insert(role.first);
            }
        }
        // 2. remove objects for which not all relations contain object from b
        for (const Role& role : m_role->evaluate(state)) {
            if (result_set.find(role.first) != result_set.end() &&
                concepts_set.find(role.second) == concepts_set.end()) {
                result_set.erase(role.first);
            }
        }
        m_result = Concepts(result_set.begin(), result_set.end());
    }

public:
    UniversalAbstractionElement(
        const Sketch_STRIPS_Problem* problem, bool goal, RoleElement* role, ConceptElement* concept)
        : ConceptElement(problem, goal), m_role(role), m_concept(concept) {
        if (role == nullptr || concept == nullptr) {
            std::cout << "UniversalAbstractionElement::UniversalAbstractionElement: nullptr in parameters - " << role << " " << concept << std::endl;
            exit(1);
        }
        if (goal) {
            Concepts_Set concepts_set(concept->result().begin(), concept->result().end());
            Concepts_Set result_set;
            // 1. perform existential abstraction to find elements for which some relation to b exists.
            for (const Role& role : role->result()) {
                if (concepts_set.find(role.second) != concepts_set.end()) {
                    result_set.insert(role.first);
                }
            }
            // 2. remove objects for which not all relations contain object from b
            for (const Role& role : role->result()) {
                if (result_set.find(role.first) != result_set.end() &&
                    concepts_set.find(role.second) == concepts_set.end()) {
                    result_set.erase(role.first);
                }
            }
            m_result = Concepts(result_set.begin(), result_set.end());
        }
    }
    virtual ~UniversalAbstractionElement() = default;
};

}

#endif
