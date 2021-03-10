#ifndef _ROLE_TRANSITIVE_CLOSURE__
#define _ROLE_TRANSITIVE_CLOSURE__

#include "../role.hxx"
#include "../concept.hxx"
#include "../utils.hxx"

namespace aptk {

class RoleTransitiveClosureElement : public RoleElement {
protected:
    // reachability relation
    RoleElement* m_reachability;

    virtual void compute_result(const Roles& role_result) {
        // 1. compute pairwise reachability over role_result.
        // 2. extract all roles with first object in initial.
        //std::pair<aptk::elements::PairwiseDistances, Index_Vec> result = aptk::elements::compute_pairwise_distances(m_problem, role_result);
        //for (aptk::elements::Distances& distances : result.first) {

        //}
    }

protected:
    virtual void compute_result(const State* state) override {
        m_result.clear();
        compute_result(m_reachability->evaluate(state));
    }

public:
    RoleTransitiveClosureElement(const Sketch_STRIPS_Problem* problem, bool goal, RoleElement* reachability)
    : RoleElement(problem, goal), m_reachability(reachability) {
        if (goal) {
            compute_result(reachability->result());
        }
    }
};

}

#endif
