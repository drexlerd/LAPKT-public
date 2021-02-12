
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

#include "strips_prob.hxx"
#include <string>
#include <map>
#include <iosfwd>
#include <types.hxx>
#include <succ_gen.hxx>
#include <match_tree.hxx>
#include <algorithm>
#include <mutex_set.hxx>

namespace aptk
{

class Sketch_STRIPS_Problem : public STRIPS_Problem {
protected:
    std::string m_sketch_name;

    // fluents that do not change when applying actions
	// these fluents are needed for feature evaluation
	Fluent_Ptr_Vec									m_init_fluents;
	std::vector<const Fluent*>						m_init_const_fluents;

    // the total number of predicates occuring in the instance
	unsigned m_num_predicates;
	// the total number of objects occuring in the instance
	unsigned m_num_objects;

	// preallocated memory to store state information ordered by predicate types
	std::vector<std::vector<const Fluent*>> first_order_state;
public:
	Sketch_STRIPS_Problem( std::string dom_name = "Unnamed", std::string prob_name = "Unnamed ", std::string sketch_name = "Unnamed ");
	virtual ~Sketch_STRIPS_Problem();

    /**
	 * Fluents with more additional information about predicate index and object indices.
	 */
	// fluents that are changed by actions
	static unsigned		add_fluent( Sketch_STRIPS_Problem& p, std::string signature,
							unsigned predicate_type, std::string predicate_name,
							Index_Vec &&objs_idx, Name_Vec &&objs_names );
	// fluents that remain constant during planning
	static unsigned		add_other_fluent( Sketch_STRIPS_Problem& p, std::string signature,
						unsigned predicate_type, std::string predicate_name,
						Index_Vec &&objs_idx, Name_Vec &&objs_names );

	/**
	 * Set fol related task information
	 */
	static void set_num_predicates(Sketch_STRIPS_Problem& p, unsigned num_predicates);
	static void set_num_objects(Sketch_STRIPS_Problem& p, unsigned num_objects);

    /**
	 * Compute sorted first order state information in the preallocated memory
	 * and return a const reference to it.
	 */
    const std::vector<std::vector<const Fluent*>> &get_first_order_state(const Fluent_Vec &state_fluents);

    /**
	 * Getters.
	 */
	Fluent_Ptr_Vec& init_fluents() { return m_init_fluents; }
	const std::vector< const Fluent*>&
		init_fluents() const { return m_init_const_fluents; }

    /**
	 * Printers.
	 */
	void print_init_fluents( std::ostream& os ) const;
};



}

#endif // Sketch_STRIPS_Problem.hxx
