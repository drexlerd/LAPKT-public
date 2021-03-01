#ifndef __ROLE_EXTRACTION__
#define __ROLE_EXTRACTION__

#include "../role.hxx"
#include "../predicate.hxx"

namespace aptk {

using Role = std::pair<unsigned, unsigned>;
using Roles = std::vector<Role>;

/**
 * The underlying result represents a set of predicates.
 */
class RoleExtractionElement : public RoleElement {
protected:
    PredicateElement* m_predicate_element;
    const unsigned m_a;
    const unsigned m_b;

    virtual void compute_result(const State* state) override {
        m_result.clear();
        for (const Predicate predicate : m_predicate_element->evaluate(state)) {
            const Fluent* fluent = m_problem->total_fluents()[predicate];
            m_result.emplace_back(fluent->pddl_objs_idx()[m_a], fluent->pddl_objs_idx()[m_b]);
        }
    }

public:
    RoleExtractionElement(const Sketch_STRIPS_Problem* problem, bool goal, PredicateElement* predicate_element, unsigned a, unsigned b)
    : RoleElement(problem, goal),
    m_predicate_element(predicate_element), m_a(a), m_b(b) {
        if (goal) {
            for (const Predicate predicate : predicate_element->result()) {
                const Fluent* fluent = problem->total_fluents()[predicate];
                m_result.emplace_back(fluent->pddl_objs_idx()[m_a], fluent->pddl_objs_idx()[m_b]);
            }
        }
    }
    virtual ~RoleExtractionElement() = default;
};

}

#endif
