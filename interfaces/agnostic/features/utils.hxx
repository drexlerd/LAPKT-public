#ifndef __FEATURE_UTILS__
#define __FEATURE_UTILS__

#include <vector>
#include <limits>
#include <queue>

const int INF = std::numeric_limits<int>::max();

namespace aptk {
namespace features {

using AdjacencyList = std::vector<std::vector<unsigned>>;
using Distances = std::vector<unsigned>;
using PairwiseDistances = std::vector<Distances>;

/**
 * Compute distances from source to all other nodes using breadth-first search.
 */
extern Distances compute_distances(const AdjacencyList &edges, unsigned source);

/**
 * Compute all pairwise distances for nodes in a given adjacency list
 */
extern PairwiseDistances compute_pairwise_distances(const AdjacencyList &edges);

}
}

#endif
