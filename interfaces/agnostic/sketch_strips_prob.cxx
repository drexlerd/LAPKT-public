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

#include <sketch_strips_prob.hxx>
#include <action.hxx>
#include <fluent.hxx>
#include <cassert>
#include <map>
#include <iostream>
#include <unordered_set>
#include <unordered_map>

namespace aptk
{
    static void add_predicate_information(
		const Fluent_Ptr_Vec &fluents,
		std::unordered_map<std::string, int> &out_predicate_name_to_idx,
		std::unordered_map<std::string, int> &out_object_name_to_idx) {
		for (const Fluent *x : fluents) {
			out_predicate_name_to_idx[x->pddl_predicate_name()] = x->pddl_predicate_type();
			const Name_Vec &object_names = x->pddl_object_names();
			const Index_Vec &object_idx = x->pddl_objs_idx();
			assert(object_names.size() == object_idx.size());
			for (int i = 0; i < static_cast<int>(object_idx.size()); ++i) {
				out_object_name_to_idx[object_names[i]] = object_idx[i];
			}
		}
	}

	Sketch_STRIPS_Problem::Sketch_STRIPS_Problem( std::string dom_name, std::string prob_name, std::string sketch_name )
		: STRIPS_Problem(dom_name, prob_name),
		m_sketch_name(sketch_name),
		m_num_predicates(-1),
		m_num_objects(-1)

	{
	}

	Sketch_STRIPS_Problem::~Sketch_STRIPS_Problem()
	{
	}

	unsigned Sketch_STRIPS_Problem::add_fluent( Sketch_STRIPS_Problem& p, std::string signature,
		unsigned predicate_type, std::string predicate_name, Index_Vec &&objs_idx, Name_Vec &&objs_names )
	{
		Fluent* new_fluent = new Fluent( p );
		new_fluent->set_index( p.fluents().size() );
		new_fluent->set_signature( signature );
		new_fluent->set_predicate_type(predicate_type);
		new_fluent->set_predicate_name(predicate_name);
		new_fluent->set_objs_idx(move(objs_idx));
		new_fluent->set_objs_names(move(objs_names));
		p.m_fluents_map[signature] = new_fluent->index();
		p.increase_num_fluents();
		p.fluents().push_back( new_fluent );
		p.m_const_fluents.push_back( new_fluent );
		return p.fluents().size()-1;
	}

	unsigned Sketch_STRIPS_Problem::add_other_fluent( Sketch_STRIPS_Problem& p, std::string signature,
		unsigned predicate_type, std::string predicate_name, Index_Vec &&objs_idx, Name_Vec &&objs_names )
	{
		Fluent* new_fluent = new Fluent( p );
		new_fluent->set_index( p.fluents().size() );
		new_fluent->set_signature( signature );
		new_fluent->set_predicate_type(predicate_type);
		new_fluent->set_predicate_name(predicate_name);
		new_fluent->set_objs_idx(move(objs_idx));
		new_fluent->set_objs_names(move(objs_names));
		p.m_init_fluents.push_back( new_fluent);
		p.m_init_const_fluents.push_back( new_fluent );
		return p.m_init_fluents.size()-1;
	}

	void Sketch_STRIPS_Problem::set_num_predicates(Sketch_STRIPS_Problem& p, unsigned num_predicates) {
        p.m_num_predicates = num_predicates;
	}
	void Sketch_STRIPS_Problem::set_num_objects(Sketch_STRIPS_Problem& p, unsigned num_objects) {
        p.m_num_objects = num_objects;
	}

	void Sketch_STRIPS_Problem::initialize_sketch_information() {
		// 1. fill init fluents
        m_first_order_state.resize(m_num_predicates);
		for (const Fluent *x : init_fluents()) {
            m_first_order_state[x->pddl_predicate_type()].push_back(x);
		}
		// 2. store predicate indices that may occur in fluents.
		std::unordered_set<int> indices;
		for (const Fluent *x : fluents()) {
            indices.insert(x->pddl_predicate_type());
		}
		m_state_predicate_idx = std::vector<int>(indices.begin(), indices.end());
		// 3. initialize mapping of predicate and object names
		add_predicate_information(fluents(), m_predicate_name_to_index, m_object_name_to_index);
		add_predicate_information(init_fluents(), m_predicate_name_to_index, m_object_name_to_index);
	}

	const std::vector<std::vector<const Fluent*>>
	&Sketch_STRIPS_Problem::get_first_order_state(const State *state) {
		const Fluent_Vec &state_fluents = state->fluent_vec();
		// 1. clear old state fluents
		for (int predicate_idx : m_state_predicate_idx) {
			m_first_order_state[predicate_idx].clear();
		}
		// 2. fill new state fluents
		for (unsigned i : state_fluents) {
			const Fluent *fluent = fluents()[i];
            m_first_order_state[fluent->pddl_predicate_type()].push_back(fluent);
		}
        return m_first_order_state;
	}

	const std::vector<std::vector<const Fluent*>>
	&Sketch_STRIPS_Problem::get_first_order_goal() const {
		return m_first_order_goal;
	}

	void Sketch_STRIPS_Problem::print_init_fluents( std::ostream& os ) const {
		for ( unsigned k = 0; k < init_fluents().size(); k++ ) {
			os << k+1 << ". " << init_fluents().at(k)->signature() << std::endl;
		}
	}
}
