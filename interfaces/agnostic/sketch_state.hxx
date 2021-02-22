
#include <strips_state.hxx>
#include <sketch_strips_prob.hxx>
#include <unordered_set>

namespace aptk {

static const std::vector< const Fluent*>
merge_fluents_ordered(const Sketch_STRIPS_Problem* problem) {
    std::vector< const Fluent*> result;
    result.reserve(problem->num_fluents() + problem->num_init_fluents());
    for (const Fluent* fluent : problem->fluents()) {
        //std::cout << "[" << fluent->pddl_predicate_name() << "] [" << fluent->signature() << "] [" << fluent->index() << "]" << std::endl;
        result.push_back(fluent);
    }
    for (const Fluent* fluent : problem->init_fluents()) {
        //std::cout << "[" << fluent->pddl_predicate_name() << "] [" << fluent->signature() << "] [" << fluent->index() << "]" << std::endl;
        result.push_back(fluent);
    }
    // sanity check
    for (unsigned i = 0; i < result.size(); ++i) {
        assert(result[i]->index() == i);
    }
    return result;
}

static std::vector<std::vector<const Fluent*>>
initialize_view_state_by_predicate(const Sketch_STRIPS_Problem* problem) {
    std::vector<std::vector<const Fluent*>> result(problem->num_predicates());
    for (const Fluent *x : problem->init_fluents()) {
        result[x->pddl_predicate_type()].push_back(x);
    }
    return result;
}

static const std::vector<unsigned>
initialize_state_predicate_idx(const Sketch_STRIPS_Problem* problem) {
    std::unordered_set<unsigned> indices;
    for (const Fluent *x : problem->fluents()) {
        indices.insert(x->pddl_predicate_type());
    }
    return std::vector<unsigned>(indices.begin(), indices.end());
}

/**
 * Unpacked version of a sta
 */
class SketchState {
private:
    // the parent task.
    const Sketch_STRIPS_Problem* m_problem;
    // (1) task information
    // m_fluents[i] is the fluent with index i from the set of all fluents = fluents + init_fluents
    const std::vector<const Fluent*> m_fluents;
    const Fluent_Vec m_goal;
    // predicate indices that occur in state_fluents
	const std::vector<unsigned> m_state_predicate_idx;
    // (2) state information
    const State* m_state;
    // (3) additional view for fast access of fluents by predicate type.
    std::vector<std::vector<const Fluent*>> m_state_by_predicate;
    // --- add more views here if necessary ---

public:
    SketchState(const Sketch_STRIPS_Problem* problem)
        : m_problem(problem),
          m_fluents(merge_fluents_ordered(problem)),
          m_goal(problem->goal()),
          m_state_predicate_idx(initialize_state_predicate_idx(problem)),
          m_state_by_predicate(initialize_view_state_by_predicate(problem)) {
    }

    /**
     * Initialize views for a given state.
     */
    void set_state(const State* state) {
        // set state and reevaluate views.
        m_state = state;
		// 1. clear old state fluents
		for (int predicate_idx : m_state_predicate_idx) {
			m_state_by_predicate[predicate_idx].clear();
		}
		// 2. fill new state fluents
		for (unsigned i : m_state->fluent_vec()) {
			const Fluent *fluent = m_fluents[i];
            m_state_by_predicate[fluent->pddl_predicate_type()].push_back(fluent);
		}
    }

    /**
     * Getters.
     */
    const Sketch_STRIPS_Problem* problem() const { return m_problem; }
    const std::vector<const Fluent*>& fluents() const { return m_fluents; }
    const Fluent_Vec& goal() const { return m_goal; }
    const std::vector<unsigned>& state_predicate_idx() const { return m_state_predicate_idx; }
    const State* state() const { return m_state; }
    const std::vector<std::vector<const Fluent*>>& state_by_predicate() const { return m_state_by_predicate; }

    /**
     * Print nicely.
     */
    void print() const {
        std::cout << "Merged fluents: \n";
        for (const Fluent* f : fluents()) {
            std::cout << "\t" << f->signature() << "\n";
        }
    }
};

}