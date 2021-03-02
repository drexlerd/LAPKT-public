#include "../role.hxx"
#include <map>

namespace aptk {

/**
 * Computes representation of roles sorted and grouped by the concept
 * that occurs the a specific position depending on parameter first.
 */
static std::map<Concept, Roles> compute_concept_role_mapping(const Roles &roles, bool first) {
    std::map<Concept, Roles> result;
    for (const Role& r : roles) {
        Concept c = (first) ? r.first : r.second;
        if (result.find(c) == result.end()) {
            result.insert(std::make_pair(c, Roles( {r,} )));
        } else {
            result.at(c).push_back(r);
        }
    }
    return result;
}

class RoleCompositionElement : public RoleElement {
protected:
    RoleElement* m_left;
    RoleElement* m_right;

    virtual void compute_result(const State* state) override {
        m_result.clear();
        // 1. collect right elements of left role O(NlogN)
        std::map<Concept, Roles> left_concept_role = compute_concept_role_mapping(m_left->evaluate(state), true);
        // 2. collect left elements of right role O(NlogN)
        std::map<Concept, Roles> right_concept_role = compute_concept_role_mapping(m_right->evaluate(state), false);
        // 3. connect pairwise O(N^2) or linear in size of result set
        Roles_Set result_set;
        auto it1 = left_concept_role.begin();
        auto it2 = right_concept_role.begin();
        while (it1 != left_concept_role.end() && it2 != right_concept_role.end()) {
            if (it1->first < it2->first) {
                ++it1;
            } else if (it1->first < it2->first) {
                ++it2;
            } else {
                // compose roles
                for (const Role &r1 : it1->second) {
                    for (const Role &r2 : it2->second) {
                        assert(r1.second == r2.first);
                        result_set.insert(std::make_pair(r1.first, r2.second));
                    }
                }
                ++it1;
                ++it2;
            }
        }
        m_result = Roles(result_set.begin(), result_set.end());
    }

public:
    RoleCompositionElement(const Sketch_STRIPS_Problem* problem, bool goal, RoleElement* left, RoleElement* right)
    : RoleElement(problem, goal), m_left(left), m_right(right) {
        if (goal) {
            // 1. collect right elements of left role O(NlogN)
            std::map<Concept, Roles> left_concept_role = compute_concept_role_mapping(left->result(), true);
            // 2. collect left elements of right role O(NlogN)
            std::map<Concept, Roles> right_concept_role = compute_concept_role_mapping(right->result(), false);
            // 3. connect pairwise O(N^2) or linear in size of result set
            Roles_Set result_set;
            auto it1 = left_concept_role.begin();
            auto it2 = right_concept_role.begin();
            while (it1 != left_concept_role.end() && it2 != right_concept_role.end()) {
                if (it1->first < it2->first) {
                    ++it1;
                } else if (it1->first < it2->first) {
                    ++it2;
                } else {
                    // compose roles
                    for (const Role &r1 : it1->second) {
                        for (const Role &r2 : it2->second) {
                            assert(r1.second == r2.first);
                            result_set.insert(std::make_pair(r1.first, r2.second));
                        }
                    }
                    ++it1;
                    ++it2;
                }
            }
            m_result = Roles(result_set.begin(), result_set.end());
        }
    }
    virtual ~RoleCompositionElement() = default;
};

}