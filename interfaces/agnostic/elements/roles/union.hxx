#ifndef _ROLE_UNION__
#define _ROLE_UNION__

#include "../role.hxx"

namespace aptk {


class RoleUnionElement : public RoleElement {
protected:
    RoleElement* m_left;
    RoleElement* m_right;

    virtual void compute_result(const Roles& left_result, const Roles& right_result) {
        m_result.clear();
        Roles_Set left_set(left_result.begin(), left_result.end());
        Roles_Set right_set(right_result.begin(), right_result.end());
        Roles_Set result_set;
        for (Role r : left_set) {
            result_set.insert(r);
        }
        for (Role r : right_set) {
            result_set.insert(r);
        }
        m_result = Roles(result_set.begin(), result_set.end());
    }

    virtual void compute_result(const State* state) override {
        m_result.clear();
        compute_result(m_left->evaluate(state), m_right->evaluate(state));
    }

public:
    RoleUnionElement(const Sketch_STRIPS_Problem* problem, bool goal, RoleElement* left, RoleElement* right)
    : RoleElement(problem, goal), m_left(left), m_right(right) {
        if (goal) {
            compute_result(m_left->result(), m_right->result());
        }
    }
    virtual ~RoleUnionElement() = default;
};

}

#endif
