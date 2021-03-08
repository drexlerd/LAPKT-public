
/*
Lightweight Automated Planning Toolkit
Copyright (C) 2012
Miquel Ramirez <miquel.ramirez@rmit.edu.au>
Nir Lipovetzky <nirlipo@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __SKETCH_STRIPS_PROBLEM__
#define __SKETCH_STRIPS_PROBLEM__

#include <strips_prob.hxx>
#include <strips_state.hxx>
#include <string>
#include <map>
#include <iosfwd>
#include <succ_gen.hxx>
#include <match_tree.hxx>
#include <algorithm>
#include <mutex_set.hxx>
#include <memory>
#include <unordered_map>
#include <unordered_set>

namespace aptk
{

struct StateData {

};

class Sketch_STRIPS_Problem : public STRIPS_Problem {
protected:
    std::string m_sketch_name;
    // init_fluents, we assume they are added after fluents
	Fluent_Ptr_Vec m_init_fluents;
	std::vector<const Fluent*> m_init_const_fluents;
	unsigned m_num_init_fluents;
	// all fluents that are ever added to the task
	Fluent_Ptr_Vec m_total_fluents;
	std::vector<const Fluent*> m_total_const_fluents;
	unsigned m_num_total_fluents;

    // the total number of predicates occuring in the instance
	unsigned m_num_predicates;
	// the total number of objects occuring in the instance
	unsigned m_num_objects;
    // name to index mappings of predicates and objects
	std::unordered_map<std::string, unsigned> m_predicate_name_to_predicate_type;
	std::unordered_map<std::string, unsigned> m_object_name_to_object_index;
	// index to name mappings of predicates and objects
	std::unordered_map<unsigned, std::string> m_predicate_type_to_predicate_name;
	std::unordered_map<unsigned, std::string> m_predicate_index_to_predicate_signature;
	std::unordered_map<unsigned, std::string> m_object_index_to_object_name;

public:
	Sketch_STRIPS_Problem( std::string dom_name = "Unnamed", std::string prob_name = "Unnamed ", std::string sketch_name = "Unnamed ");
	virtual ~Sketch_STRIPS_Problem();

    /**
	 * Fluents with more additional information about predicate index and object indices.
	 */
	// fluents that are changed by actions
	static unsigned	add_fluent( Sketch_STRIPS_Problem& p, std::string signature,
		unsigned predicate_type, std::string predicate_name, bool negated,
		Index_Vec &&objs_idx, Name_Vec &&objs_names );
	// fluents that remain constant during planning
	static unsigned	add_other_fluent( Sketch_STRIPS_Problem& p, std::string signature,
		unsigned predicate_type, std::string predicate_name, bool negated,
		Index_Vec &&objs_idx, Name_Vec &&objs_names );
	// fluents that are derived from other fluents during sketch initialization
	unsigned add_derived_fluent(std::string signature,
		unsigned predicate_type, std::string predicate_name, bool negated,
		Index_Vec &&objs_idx, Name_Vec &&objs_names );

    /**
	 * Compute fluent sets for feature evaluation.
	 * Fluent sets include fluents that remain constant.
	 * TODO: if this becomes a bottleneck we can decide to
	 * compute this only once for each problem instance or state.
	 */
	Fluent_Set init_fluents_set() const {
        Bit_Set init_fluents_set(m_num_total_fluents);
		for (const Fluent* fluent : m_init_const_fluents) {
			init_fluents_set.set(fluent->index());
		}
		return init_fluents_set;
	}
	Fluent_Set goal_fluents_set() const {
        Bit_Set goal_fluents_set = init_fluents_set();
		for (unsigned i : m_goal) {
			goal_fluents_set.set(i);
		}
		return goal_fluents_set;
	}
	Fluent_Set state_fluents_set(const State* state) const {
        Bit_Set state_fluents_set = init_fluents_set();
		for (unsigned i : state->fluent_vec()) {
			state_fluents_set.set(i);
		}
		return state_fluents_set;
	}


    /**
	 * Setters
	 */
    void set_sketch_name( std::string sketch_name ) { m_sketch_name = sketch_name; }

    /**
	 * Getters.
	 */
	const std::string& sketch_name() const { return m_sketch_name; }
	Fluent_Ptr_Vec& init_fluents() { return m_init_fluents; }
	const std::vector< const Fluent*>& init_fluents() const { return m_init_const_fluents; }
	Fluent_Ptr_Vec& total_fluents() { return m_total_fluents; }
	const std::vector< const Fluent*>& total_fluents() const { return m_total_const_fluents; }

    unsigned num_total_fluents() const { return m_num_total_fluents; }
    unsigned num_predicates() const { return m_num_predicates; }
	unsigned num_objects() const { return m_num_objects; }
	unsigned predicate_type(const std::string &predicate_name) const { return predicate_name_to_predicate_type().at(predicate_name); }
	const std::unordered_map<std::string, unsigned>& predicate_name_to_predicate_type() const { return m_predicate_name_to_predicate_type; }
	const std::unordered_map<std::string, unsigned>& object_name_to_object_index() const { return m_object_name_to_object_index; }
    const std::unordered_map<unsigned, std::string>& predicate_index_to_predicate_signature() const { return m_predicate_index_to_predicate_signature; }
	const std::unordered_map<unsigned, std::string>& object_index_to_object_name() const { return m_object_index_to_object_name; }


    /**
	 * Printers.
	 */
	void print_init_fluents( std::ostream& os ) const;
};

}

#endif // Sketch_STRIPS_Problem.hxx
