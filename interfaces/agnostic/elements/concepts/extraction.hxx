#ifndef __CONCEPT_EXTRACTION__
#define __CONCEPT_EXTRACTION__

#include "../concept.hxx"
#include "../role.hxx"
#include "../predicate.hxx"
#include "../utils.hxx"

namespace aptk {


class ConceptRoleExtractionElement : public ConceptElement {
protected:
    RoleElement* m_role;
    const unsigned m_position;

    virtual void compute_result(const Roles& role_result) {
        for (const Role& role : role_result) {
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

    virtual void compute_result(const State* state) override {
        m_result.clear();
        compute_result(m_role->evaluate(state));
    }

public:
    ConceptRoleExtractionElement(const Sketch_STRIPS_Problem* problem, bool goal, RoleElement* role, unsigned position)
    : ConceptElement(problem, goal), m_role(role), m_position(position) {
        if (goal) {
            compute_result(role->result());
        }
    }
    virtual ~ConceptRoleExtractionElement() = default;
};


class ConceptObjectExtractionElement : public ConceptElement {
protected:
    RoleElement* m_role;

    virtual void compute_result(const Roles& role_result) {
        std::pair<Concepts, std::vector<unsigned>> collected = aptk::elements::collect_concepts(m_problem, role_result);
        m_result = collected.first;
    }

    virtual void compute_result(const State* state) override {
        m_result.clear();
        compute_result(m_role->evaluate(state));
    }

public:
    ConceptObjectExtractionElement(const Sketch_STRIPS_Problem* problem, bool goal, RoleElement* role)
    : ConceptElement(problem, goal), m_role(role) {
        if (goal) {
            compute_result(role->result());
        }
    }
    virtual ~ConceptObjectExtractionElement() = default;
};


class ConceptPredicateExtractionElement : public ConceptElement {
protected:
    PredicateElement* m_predicate;
    const unsigned m_position;

    virtual void compute_result(const Predicates& predicate_result) {
        for (const Predicate& p : predicate_result) {
            m_result.push_back(m_problem->total_fluents()[p]->pddl_objs_idx()[m_position]);
        }
    }

    virtual void compute_result(const State* state) override {
        m_result.clear();
        compute_result(m_predicate->evaluate(state));
    }

public:
    ConceptPredicateExtractionElement(const Sketch_STRIPS_Problem* problem, bool goal, PredicateElement* predicate, unsigned position)
    : ConceptElement(problem, goal), m_predicate(predicate), m_position(position) {
        if (goal) {
            compute_result(m_predicate->result());
        }
    }
    virtual ~ConceptPredicateExtractionElement() = default;
};

}

#endif
