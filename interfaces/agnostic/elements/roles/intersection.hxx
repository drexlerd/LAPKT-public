#ifndef _ROLE_INTERSECTION__
#define _ROLE_INTERSECTION__

#include "../role.hxx"

namespace aptk {


class RoleIntersectionElement : public RoleElement {
protected:
    RoleElement* m_left;
    RoleElement* m_right;

    virtual void compute_result(const State* state) override {
        m_result.clear();
        Roles_Set left_set(m_left->evaluate(state).begin(), m_left->evaluate(state).end());
        Roles_Set right_set(m_right->evaluate(state).begin(), m_right->evaluate(state).end());
        for (Role r : left_set) {
            if (right_set.find(r) != right_set.end()) {
                m_result.push_back(r);
            }
        }
    }

public:
    RoleIntersectionElement(const Sketch_STRIPS_Problem* problem, bool goal, RoleElement* left, RoleElement* right)
    : RoleElement(problem, goal), m_left(left), m_right(right) {
        if (goal) {
            Roles_Set left_set(left->result().begin(), left->result().end());
            Roles_Set right_set(right->result().begin(), right->result().end());
            for (Role r : left_set) {
                if (right_set.find(r) != right_set.end()) {
                    m_result.push_back(r);
                }
            }
        }
    }
    virtual ~RoleIntersectionElement() = default;
};

}

#endif
