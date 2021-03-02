#ifndef __CONCEPT_EXTRACTION__
#define __CONCEPT_EXTRACTION__

#include "../concept.hxx"
#include "../role.hxx"
#include "../predicate.hxx"

namespace aptk {


class ConceptRoleExtractionElement : public ConceptElement {
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
                std::cout << "ConceptRoleExtractionElement::compute_result: only position 0 or 1 allowed!" << std::endl;
                exit(1);
            }
        }
    }

public:
    ConceptRoleExtractionElement(const Sketch_STRIPS_Problem* problem, bool goal, RoleElement* role, unsigned position)
    : ConceptElement(problem, goal), m_role(role), m_position(position) {
        if (goal) {
            for (const Role& r : role->result()) {
                if (position == 0) {
                    m_result.push_back(r.first);
                } else if (position == 1) {
                    m_result.push_back(r.second);
                } else {
                    std::cout << "ConceptRoleExtractionElement::ConceptRoleExtractionElement: only position 0 or 1 allowed!" << std::endl;
                    exit(1);
                }
            }
        }
    }
    virtual ~ConceptRoleExtractionElement() = default;
};

class ConceptPredicateExtractionElement : public ConceptElement {
protected:
    PredicateElement* m_predicate;
    const unsigned m_position;

    void compute_result(const State* state) {
        m_result.clear();
        for (const Predicate& p : m_predicate->evaluate(state)) {
            m_result.push_back(m_problem->total_fluents()[p]->pddl_objs_idx()[m_position]);
        }
    }

public:
    ConceptPredicateExtractionElement(const Sketch_STRIPS_Problem* problem, bool goal, PredicateElement* predicate, unsigned position)
    : ConceptElement(problem, goal), m_predicate(predicate), m_position(position) {
        if (goal) {
            for (const Predicate& p : m_predicate->result()) {
                m_result.push_back(m_problem->total_fluents()[p]->pddl_objs_idx()[m_position]);
            }
        }
    }
    virtual ~ConceptPredicateExtractionElement() = default;
};

}

#endif
