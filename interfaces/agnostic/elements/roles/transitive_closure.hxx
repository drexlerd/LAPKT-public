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
        m_result.clear();
        // 1. compute pairwise reachability over role_result.
        // 2. extract all roles with first object in initial.
        // compute pairwise distances over role.
        std::tuple<aptk::elements::PairwiseDistances, Concepts, Index_Vec> result = aptk::elements::compute_pairwise_distances(m_problem, role_result);
        aptk::elements::PairwiseDistances& pairwise_distances = std::get<0>(result);
        Concepts& concepts = std::get<1>(result);
        Index_Vec& conn_concept_indices = std::get<2>(result);
        for (Concept c1 : concepts) {
            for (Concept c2 : concepts) {
                if (c1 == c2) continue;  // not reflexive
                if (pairwise_distances[conn_concept_indices[c1]][conn_concept_indices[c2]] != aptk::elements::INF) {
                    // there exists a connection from c1 to c2
                    m_result.emplace_back(c1, c2);
                }
            }
        }
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
