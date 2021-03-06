#ifndef _ROLE_INVERSE__
#define _ROLE_INVERSE__

#include "../role.hxx"

namespace aptk {


class RoleInverseElement : public RoleElement {
protected:
    RoleElement* m_role;

    virtual void compute_result(const State* state) override {
        m_result.clear();
        Roles role_result = m_role->evaluate(state);
        m_result.clear();
        for (Role& r : role_result) {
            m_result.push_back(Role(r.second, r.first));
        }
    }

public:
    RoleInverseElement(const Sketch_STRIPS_Problem* problem, bool goal, RoleElement* role)
    : RoleElement(problem, goal), m_role(role) {
        if (goal) {
            Roles role_result = role->result();
            m_result.clear();
            for (Role& r : role_result) {
                m_result.push_back(Role(r.second, r.first));
            }
        }
    }
    virtual ~RoleInverseElement() = default;
};

}

#endif
