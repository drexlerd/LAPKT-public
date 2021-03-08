#ifndef _ROLE_MINIMAL__
#define _ROLE_MINIMAL__

#include "../role.hxx"
#include "../utils.hxx"

namespace aptk {

/**
 * Computes the set of all roles R'
 * such that (a,b)\in R' and (a,b)\in R
 * and b is the maximum element of all (a,b)\in R with respect to C.
 *
 */
class RoleMinimalElement : public RoleElement {
protected:
    RoleElement* m_role;
    RoleElement* m_conn;

    virtual void compute_result(const Roles& roles_result, const Roles& conn_result) {
        // Construct graph DAG G=(V,E) that represents conn_result.
        // All nodes v in V are initially unmarked.
        // While the graph is non empty:
        //   1. Pick any node u from G such u has no incoming edge.
        //   2. If u was not marked and there exists a corresponding role r in roles_result
        //      then add r to the final result, and otherwise, mark all successors.
        //   3. Remove u together with all incident edges.
        std::map<Concept, Roles> concept_role = aptk::elements::compute_concept_role_mapping(roles_result, true);
        std::pair<Concepts, std::vector<unsigned>> collected = aptk::elements::collect_concepts(m_problem, conn_result);
        Concepts& concepts = collected.first;
        std::vector<unsigned>& concept_indices = collected.second;
        // Initialize graph G.
        std::vector<std::vector<unsigned>> adj_list(concepts.size());
        std::vector<unsigned> in_degree(concepts.size(), 0);
        for (const Role& r : conn_result) {
            // add forward edge
            adj_list[concept_indices[r.first]].push_back(concept_indices[r.second]);
            // increment indegree
            ++in_degree[concept_indices[r.second]];
        }
        // Perform Kahn's algorithm to compute topological sorting
        // to obtain minimal concepts with respect to conn.
        std::vector<bool> marked(concepts.size(), false);
        std::vector<bool> visited(concepts.size(), false);
        std::queue<unsigned> queue;
        for (unsigned i = 0; i < concepts.size(); ++i) {
            if (in_degree[i] == 0) {
                queue.push(concepts[i]);
                visited[i] = true;
            }
        }
        // ensure that is a minimal element
        if (queue.empty()) {
            std::cout << "RoleFurthestElement::compute_result: there exists no minimal conn!" << std::endl;
            exit(1);
        }
        // clear previous solution.
        m_result.clear();
        while (!queue.empty()) {
            unsigned c = queue.front();
            queue.pop();
            // handle current node.
            if (!marked[concept_indices[c]]) {
                auto it = concept_role.find(c);
                if (it != concept_role.end()) {
                    // append elements
                    m_result.insert(m_result.end(), it->second.begin(), it->second.end());
                    // mark current node to notify successor nodes that minimum was found
                    marked[concept_indices[c]] = true;
                }
            }
            // expand successors
            for (const unsigned succ : adj_list[concept_indices[c]]) {
                // remove edge from G
                --in_degree[succ];
                // add unvisited successor with no incoming edge to the queue
                if (!visited[succ] && in_degree[succ] == 0) {
                    visited[succ] = true;
                    queue.push(concepts[succ]);
                    // mark successor if current node is marked.
                    if (marked[concept_indices[c]]) {
                        marked[succ] = true;
                    }
                }
            }
        }
        if (m_result.empty()) {
            std::cout << "RoleFurthestElement::compute_result: there is no minial role!" << std::endl;
            exit(1);
        }
        if (!std::all_of(visited.begin(), visited.end(), [](bool x){ return x; })) {
            std::cout << "RoleFurthestElement::compute_result: cycle detected in conn!" << std::endl;
            exit(1);
        }
    }

protected:
    virtual void compute_result(const State* state) override {
        m_result.clear();
        compute_result(m_role->evaluate(state), m_conn->evaluate(state));
    }

public:
    RoleMinimalElement(const Sketch_STRIPS_Problem* problem, bool goal, RoleElement* role, RoleElement* conn)
    : RoleElement(problem, goal), m_role(role), m_conn(conn) {
        if (goal) {
            compute_result(role->result(), conn->result());
        }
    }
};

}

#endif
