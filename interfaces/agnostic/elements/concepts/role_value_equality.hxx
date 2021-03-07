#ifndef __CONCEPT_ROLE_VALUE_MAP__
#define __CONCEPT_ROLE_VALUE_MAP__

#include "../concept.hxx"
#include "../role.hxx"

namespace aptk {


class ConceptRoleValueEqualityElement : public ConceptElement {
private:
    RoleElement* m_left;
    RoleElement* m_right;

    virtual void compute_result(const Roles& left_roles, const Roles& right_roles) {
        Roles_Set left_set(left_roles.begin(), left_roles.end());
        Roles_Set right_set(right_roles.begin(), right_roles.end());
        // 1. Collect all element that occur somewhere at a and b
        // because these elements are candidates to be checked
        Concepts_Set a_set;
        Concepts_Set b_set;
        for (const Role& r : left_roles) {
            a_set.insert(r.first);
            b_set.insert(r.second);
        }
        for (const Role& r : right_roles) {
            a_set.insert(r.first);
            b_set.insert(r.second);
        }
        // 2. Initialize result to contain the whole universe
        Concepts_Set result_set;
        for (unsigned i = 0; i < m_problem->num_objects(); ++i) {
            result_set.insert(i);
        }

        // 3. Exclude concepts from result that fail the test: forall b:(a,b)\in R <-> (a,b)\in S
        for (Concept c1 : a_set) {
            for (Concept c2 : b_set) {
                Role r(c1, c2);
                bool left_exists = (left_set.find(r) != left_set.end());
                bool right_exists = (right_set.find(r) != left_set.end());
                if (left_exists != right_exists) {
                    result_set.erase(c1);
                    break;
                }
            }
        }
        m_result = Concepts(result_set.begin(), result_set.end());
    }

protected:
    virtual void compute_result(const State* state) override {
        m_result.clear();
        compute_result(m_left->evaluate(state), m_right->evaluate(state));
    }

public:
    ConceptRoleValueEqualityElement(const Sketch_STRIPS_Problem* problem, bool goal, RoleElement* left, RoleElement* right)
    : ConceptElement(problem, goal), m_left(left), m_right(right) {
        if (goal) {
            compute_result(left->result(), right->result());
        }
    }
    virtual ~ConceptRoleValueEqualityElement() = default;
};

}

#endif
