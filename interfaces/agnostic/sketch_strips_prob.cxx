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
#include <unordered_map>

namespace aptk
{
	Sketch_STRIPS_Problem::Sketch_STRIPS_Problem( std::string dom_name, std::string prob_name, std::string sketch_name )
		: STRIPS_Problem(dom_name, prob_name),
		m_sketch_name(sketch_name),
		m_num_init_fluents(0),
		m_num_predicates(0),
		m_num_objects(0) {
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
		p.m_predicate_name_to_index[new_fluent->pddl_predicate_name()] = new_fluent->pddl_predicate_type();
        for (int i = 0; i < static_cast<int>(new_fluent->pddl_objs_idx().size()); ++i) {
			p.m_object_name_to_index[new_fluent->pddl_obj_names()[i]] = new_fluent->pddl_objs_idx()[i];
		}
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
		new_fluent->set_index( p.fluents().size() + p.init_fluents().size() );
		new_fluent->set_signature( signature );
		new_fluent->set_predicate_type(predicate_type);
		new_fluent->set_predicate_name(predicate_name);
		new_fluent->set_objs_idx(move(objs_idx));
		new_fluent->set_objs_names(move(objs_names));
		assert(predicate_name != "");
		p.m_predicate_name_to_index[new_fluent->pddl_predicate_name()] = new_fluent->pddl_predicate_type();
		p.m_num_predicates = p.m_predicate_name_to_index.size();
        for (int i = 0; i < static_cast<int>(new_fluent->pddl_objs_idx().size()); ++i) {
			p.m_object_name_to_index[new_fluent->pddl_obj_names()[i]] = new_fluent->pddl_objs_idx()[i];
		}
		p.m_num_objects = p.m_object_name_to_index.size();
		p.m_num_init_fluents++;
		p.m_init_fluents.push_back( new_fluent);
		p.m_init_const_fluents.push_back( new_fluent );
		return p.m_init_fluents.size()-1;
	}

	void Sketch_STRIPS_Problem::print_init_fluents( std::ostream& os ) const {
		for ( unsigned k = 0; k < init_fluents().size(); k++ ) {
			os << k+1 << ". " << init_fluents().at(k)->signature() << std::endl;
		}
	}
}
