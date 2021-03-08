#include "utils.hxx"

namespace aptk {
namespace elements {

std::map<Concept, Roles> compute_concept_role_mapping(const Roles &roles, bool first) {
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

std::vector<unsigned> concepts_indices(const Sketch_STRIPS_Problem* problem, const Concepts &concepts) {
    // mapping from concepts to indices for more compact storage.
    std::vector<unsigned> concept_indices(problem->num_objects(), -1);
    for (unsigned i = 0; i < concepts.size(); ++i) {
        concept_indices[concepts[i]] = i;
    }
    return concept_indices;
}

std::pair<Concepts, std::vector<unsigned>> collect_concepts(
    const Sketch_STRIPS_Problem* problem, const Roles &roles) {
    Concepts_Set concepts;
    for (const Role& r : roles) {
        concepts.insert(r.first);
        concepts.insert(r.second);
    }
    Concepts concepts_vec(concepts.begin(), concepts.end());
    return std::make_pair(concepts_vec, aptk::elements::concepts_indices(problem, concepts_vec));
}

Distances compute_distances(const AdjacencyList &edges, unsigned source) {
    unsigned num_nodes = edges.size();
    Distances distances(num_nodes, INF);
    distances[source] = 0;
    std::queue<unsigned> queue;
    queue.push(source);
    while(!queue.empty()) {
        unsigned node = queue.front();
        queue.pop();
        int distance = distances[node];
        for (unsigned succ : edges[node]) {
            if (distances[succ] == INF) {
                // in unit cost setting the shortest path corresponds
                // to the distance of visiting the node the first time.
                distances[succ] = distance + 1;
                queue.push(succ);
            }
        }
    }
    return distances;
}

PairwiseDistances compute_pairwise_distances(const AdjacencyList &edges) {
    unsigned num_nodes = edges.size();
    PairwiseDistances pairwise_distances;
    pairwise_distances.reserve(num_nodes);
    // perform breadth first search starting from each node in the graph
    for (unsigned i = 0; i < num_nodes; ++i) {
        pairwise_distances.push_back(compute_distances(edges, i));
    }
    return pairwise_distances;
}

std::pair<PairwiseDistances, std::vector<unsigned>> compute_pairwise_distances(
    const Sketch_STRIPS_Problem* problem, const Roles& roles) {
    std::pair<Concepts, std::vector<unsigned>> collected = aptk::elements::collect_concepts(problem, roles);
    Concepts& concepts = collected.first;
    std::vector<unsigned>& concept_indices = collected.second;
    // initialize adjacency matrix
    aptk::elements::AdjacencyList edges(concepts.size());
    for (const Role& r : roles) {
        edges[concept_indices[r.first]].push_back(concept_indices[r.second]);
    }
    // compute pairwise distances
    return std::make_pair(aptk::elements::compute_pairwise_distances(edges), concept_indices);
}

}
}
