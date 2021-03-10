#ifndef __ELEMENT_UTILS__
#define __ELEMENT_UTILS__

#include <map>
#include <vector>
#include <limits>
#include <queue>
#include <utility>
#include <tuple>
#include "role.hxx"

namespace aptk {
namespace elements {

const int INF = std::numeric_limits<int>::max();

using AdjacencyList = std::vector<std::vector<unsigned>>;
using Distances = std::vector<unsigned>;
using PairwiseDistances = std::vector<Distances>;

/**
 * Computes representation of roles sorted and grouped by the concept
 * that occurs the a specific position depending on parameter first.
 */
extern std::map<Concept, Roles> compute_concept_role_mapping(const Roles &roles, bool first);

/**
 * Compute all objects occuring in a role
 */
extern Index_Vec concepts_indices(
    const Sketch_STRIPS_Problem* problem, const Concepts &concepts);

extern std::pair<Concepts, Index_Vec> collect_concepts(
    const Sketch_STRIPS_Problem* problem, const Roles &roles);

/**
 * Compute distances from source to all other nodes using breadth-first search.
 */
extern Distances compute_distances(const AdjacencyList &edges, unsigned source);

/**
 * Compute all pairwise distances for nodes in a given adjacency list
 */
extern PairwiseDistances compute_pairwise_distances(const AdjacencyList &edges);

/**
 * Compute all pairwise distances between objects occuring in a given role.
 */
extern std::tuple<PairwiseDistances, Concepts, Index_Vec> compute_pairwise_distances(
    const Sketch_STRIPS_Problem* problem, const Roles &roles);

}
}

#endif
