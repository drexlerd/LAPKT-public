#include "utils.hxx"

namespace aptk {
namespace features {

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

}
}
