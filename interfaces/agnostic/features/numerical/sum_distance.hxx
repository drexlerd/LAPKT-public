#ifndef __SUM_DISTANCE_FEATURE__
#define __SUM_DISTANCE_FEATURE__

#include "../numerical_feature.hxx"
#include "../../elements/role.hxx"
#include "../../elements/concept.hxx"
#include "../../elements/predicate.hxx"
#include "../../sketch.hxx"
#include "../utils.hxx"
#include "../../elements/utils.hxx"
#include <iostream>
#include <vector>
#include <numeric>
#include <string>
#include <functional>

namespace aptk {

/**
 * The SumDistanceFeature implements the summation over shortest pairwise distances
 * of roles that are connected over some other role.
 * The evaluation throws an error if there is no connection for some relevant concepts.
 */
class SumDistanceFeature : public NumericalFeature {
protected:
    RoleElement* m_role1;
    RoleElement* m_conn;
    RoleElement* m_role2;
public:
    SumDistanceFeature(const BaseSketch* sketch, const std::string &name,
        RoleElement* role1, RoleElement* conn, RoleElement* role2) : NumericalFeature(sketch, name, nullptr),
        m_role1(role1), m_conn(conn), m_role2(role2) {
    }
    virtual ~SumDistanceFeature() = default;

    virtual void evaluate(const State* state) override {
        // std::cout << "SumDistanceFeature::evaluate" << std::endl;
        Roles result_role1 = m_role1->evaluate(state);
        Roles result_conn = m_conn->evaluate(state);
        Roles result_role2 = m_role2->evaluate(state);
        /*
        m_role1->print();
        m_conn->print();
        m_role2->print();
        */
        // 1. compute pairwise distance matrix of connection role.
        Concepts_Set nodes_set;
        for (const Role& r : result_conn) {
            nodes_set.insert(r.first);
            nodes_set.insert(r.second);
        }
        Concepts nodes(nodes_set.begin(), nodes_set.end());
        // mapping from concepts to indices for more compact storage.
        std::vector<unsigned> conn_concept_indices(m_sketch->problem()->num_objects(), -1);
        for (unsigned i = 0; i < nodes.size(); ++i) {
            conn_concept_indices[nodes[i]] = i;
        }
        // initialize adjacency matrix
        aptk::features::AdjacencyList edges(nodes.size());
        for (const Role& r : result_conn) {
            edges[conn_concept_indices[r.first]].push_back(conn_concept_indices[r.second]);
        }
        // compute pairwise distances
        aptk::features::PairwiseDistances pairwise_distances = aptk::features::compute_pairwise_distances(edges);
        /*
        for (auto x : pairwise_distances) {
            for (auto y : x) {
                std::cout << y << " ";
            }
            std::cout << std::endl;
        }
        */

        // collect elements of role O(NlogN)
        std::map<Concept, Roles> left_concept_role = aptk::elements::compute_concept_role_mapping(result_role1, true);
        std::map<Concept, Roles> right_concept_role = aptk::elements::compute_concept_role_mapping(result_role2, true);
        aptk::features::Distances minimum_distances(left_concept_role.size(), INF);
        auto it1 = left_concept_role.begin();
        auto it2 = right_concept_role.begin();
        int role1_a = 0;
        while (it1 != left_concept_role.end() && it2 != right_concept_role.end()) {
            if (it1->first < it2->first) {
                ++it1;
                ++role1_a;
            } else if (it1->first < it2->first) {
                ++it2;
            } else {
                // compose roles
                for (const Role &r1 : it1->second) {
                    for (const Role &r2 : it2->second) {
                        assert(r1.second == r2.first);
                        minimum_distances[role1_a] = std::min(
                            minimum_distances[role1_a],
                            pairwise_distances[conn_concept_indices[r1.second]][conn_concept_indices[r2.second]]);
                    }
                }
                ++it1;
                ++role1_a;
                ++it2;
            }
        }

        new_eval = 0;
        for (int distance : minimum_distances) {
            new_eval += distance;
        }
        // std::cout << new_eval << std::endl;
    }

    /**
     * Pretty printer.
     */
    virtual void print() const override {
        std::cout << m_name << ": new_eval=" << new_eval << ": ";
        m_role1->print();
        m_conn->print();
        m_role2->print();
    }
};

}

#endif
