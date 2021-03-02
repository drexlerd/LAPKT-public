#include "../role.hxx"
#include "../utils.hxx"
#include <map>

namespace aptk {

class RoleCompositionElement : public RoleElement {
protected:
    RoleElement* m_left;
    RoleElement* m_right;

    void compute_result(const Roles& left_roles, const Roles& right_roles) {
        // 1. collect right elements of left role O(NlogN)
        std::map<Concept, Roles> left_concept_role = aptk::elements::compute_concept_role_mapping(left_roles, false);
        // 2. collect left elements of right role O(NlogN)
        std::map<Concept, Roles> right_concept_role = aptk::elements::compute_concept_role_mapping(right_roles, true);
        // 3. connect pairwise O(N^2) or linear in size of result set
        Roles_Set result_set;
        auto it1 = left_concept_role.begin();
        auto it2 = right_concept_role.begin();
        while (it1 != left_concept_role.end() && it2 != right_concept_role.end()) {
            if (it1->first < it2->first) {
                ++it1;
            } else if (it1->first > it2->first) {
                ++it2;
            } else {
                // concepts are equal => compose rules.
                for (const Role &r1 : it1->second) {
                    for (const Role &r2 : it2->second) {
                        result_set.insert(std::make_pair(r1.first, r2.second));
                    }
                }
                ++it1;
                ++it2;
            }
        }
        m_result = Roles(result_set.begin(), result_set.end());
    }

    virtual void compute_result(const State* state) override {
        m_result.clear();
        compute_result(m_left->evaluate(state), m_right->evaluate(state));
    }

public:
    RoleCompositionElement(const Sketch_STRIPS_Problem* problem, bool goal, RoleElement* left, RoleElement* right)
    : RoleElement(problem, goal), m_left(left), m_right(right) {
        if (goal) {
            compute_result(left->result(), right->result());
        }
    }
    virtual ~RoleCompositionElement() = default;
};

}