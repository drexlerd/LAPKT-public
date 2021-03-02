#ifndef __SUM_DISTANCE_FEATURE__
#define __SUM_DISTANCE_FEATURE__

#include "../numerical_feature.hxx"
#include "../../elements/role.hxx"
#include "../../elements/concept.hxx"
#include "../../elements/predicate.hxx"
#include "../../sketch.hxx"

namespace aptk {

class SumDistanceFeature : public NumericalFeature {
protected:
    RoleElement* m_role1;
    RoleElement* m_conn;
    RoleElement* m_role2;
    mutable int old_eval;
    int new_eval;

    using Node = unsigned;
    using Edge = std::pair<Node, Node>;

    class Graph {
        private:

    };
public:
    SumDistanceFeature(const BaseSketch* sketch, const std::string &name,
        RoleElement* role1, RoleElement* conn, RoleElement* role2) : NumericalFeature(sketch, name, nullptr),
        m_role1(role1), m_conn(conn), m_role2(role2), old_eval(0), new_eval(0) { }
    virtual ~SumDistanceFeature() = default;

    virtual void evaluate(const State* state) override {
        Roles result_role1 = m_role1->evaluate(state);
        Roles result_conn = m_conn->evaluate(state);
        Roles result_role2 = m_role2->evaluate(state);
        // 1. compute pairwise distance matrix of connection role.
        Concepts_Set nodes_set;
        for (const Role& r : result_conn) {
            nodes_set.insert(r.first);
            nodes_set.insert(r.second);
        }
        Concepts nodes(nodes_set.begin(), nodes_set.end());
        // mapping from concepts to indices for more compact storage.
        std::vector<unsigned> concept_indices(m_sketch->problem()->num_objects(), -1);
        for (unsigned i = 0; i < nodes.size(); ++i) {
            concept_indices[nodes[i]] = i;
        }
        // we use distance INF to indicate non existent paths
        const int INF = std::numeric_limits<int>::max();
        std::vector<std::vector<Node>> distances(nodes.size(), std::vector<Node>(nodes.size(), INF));

        // 2. compute sum of shortest paths for each role.
        Concepts_Set from_concepts;
        for (const Role& r : result_role1) {
            from_concepts.insert(r.first);
        }
        std::unordered_map<Concept, Concepts> to_concepts_mapping;
        for (const Role& r : result_role2) {
            if (to_concepts_mapping.find(r.first) == to_concepts_mapping.end()) {
                to_concepts_mapping.insert(make_pair(r.first, Concepts({r.second })));
            } else {
                to_concepts_mapping.at(r.first).push_back(r.second);
            }
        }
        new_eval = 0;
        for (Concept c1 : from_concepts) {
            if (to_concepts_mapping.find(c1) == to_concepts_mapping.end()) {
                std::cout << "SumDistanceFeature::evaluate: no matching target for concept: " << m_sketch->problem()->object_index_to_object_name().at(c1) << std::endl;
                exit(1);
            }
            unsigned min_distance = INF;
            for (Concept c2 : to_concepts_mapping.at(c1)) {
                min_distance = std::min(min_distance, distances[concept_indices[c1]][concept_indices[c2]]);
            }
            new_eval += min_distance;
        }
    }
};

}

#endif
