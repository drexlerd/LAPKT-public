#ifndef _ROLE_UNION__
#define _ROLE_UNION__

#include "../role.hxx"

namespace aptk {


class RoleUnionElement : public RoleElement {
protected:
    RoleElement* m_left;
    RoleElement* m_right;

    virtual void compute_result(const State* state) override {
        m_result.clear();
        Roles_Set left_set(m_left->evaluate(state).begin(), m_left->evaluate(state).end());
        Roles_Set right_set(m_right->evaluate(state).begin(), m_right->evaluate(state).end());
        Roles_Set result_set;
        for (Role r : left_set) {
            result_set.insert(r);
        }
        for (Role r : right_set) {
            result_set.insert(r);
        }
        m_result = Roles(result_set.begin(), result_set.end());
    }

public:
    RoleUnionElement(const Sketch_STRIPS_Problem* problem, bool goal, RoleElement* left, RoleElement* right)
    : RoleElement(problem, goal), m_left(left), m_right(right) {
        if (goal) {
            Roles_Set left_set(left->result().begin(), left->result().end());
            Roles_Set right_set(right->result().begin(), right->result().end());
            Roles_Set result_set;
            for (Role r : left_set) {
                result_set.insert(r);
            }
            for (Role r : right_set) {
                result_set.insert(r);
            }
            m_result = Roles(result_set.begin(), result_set.end());
        }
    }
    virtual ~RoleUnionElement() = default;
};

}

#endif
