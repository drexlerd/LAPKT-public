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

#ifndef __SKETCH_SERIALIZED_SEARCH__
#define __SKETCH_SERIALIZED_SEARCH__

#include <aptk/search_prob.hxx>
#include <aptk/resources_control.hxx>
#include <aptk/closed_list.hxx>
// #include <aptk/iw.hxx>
#include <reachability.hxx>
#include <vector>
#include <algorithm>
#include <iostream>
#include "../../interfaces/agnostic/sketches/goal_counter.hxx"
#include "../../interfaces/agnostic/sketches/childsnack.hxx"
#include "../../interfaces/agnostic/sketches/driverlog.hxx"
#include "../../interfaces/agnostic/sketches/barman.hxx"
#include "../../interfaces/agnostic/sketches/hiking.hxx"

namespace aptk {

namespace search {

template < typename Search_Model, typename Search_Strategy, typename Search_Node >
class Sketch_Serialized_Search : public Search_Strategy {

public:
	typedef		typename Search_Model::State_Type		                          State;
	typedef 	Closed_List< Search_Node >			                          Closed_List_Type;

	Sketch_Serialized_Search( 	const Search_Model& search_problem )
		: Search_Strategy( search_problem ), m_closed_goal_states( NULL )  {
		const Sketch_STRIPS_Problem* sketch_problem = static_cast<const Sketch_STRIPS_Problem*>(&search_problem.task());
		if (sketch_problem->domain_name() == "child-snack") {
            m_sketch = new aptk::ChildsnackSketch(sketch_problem);
		} else if (sketch_problem->domain_name() == "driverlog") {
            m_sketch = new aptk::DriverlogSketch(sketch_problem);
		} else if (sketch_problem->domain_name() == "barman") {
			m_sketch = new aptk::BarmanSketch(sketch_problem);
		} else if (sketch_problem->domain_name() == "hiking") {
            m_sketch = new aptk::HikingSketch(sketch_problem);
		} else {
			// default is goal sketch.
			m_sketch = new aptk::GoalCounterSketch(sketch_problem);
		}
	}

	virtual ~Sketch_Serialized_Search() {
		m_closed_goal_states = NULL;
	}

	void            	set_closed_goal_states( Closed_List_Type* c ){ m_closed_goal_states = c; }
	void 			close_goal_state( Search_Node* n ) 	 {
			if( closed_goal_states() ){
				//m_closed_goal_states->put( n );
				State* new_state = new State( this->problem().task() );
				new_state->set( n->state()->fluent_vec() );
				new_state->update_hash();
				Search_Node* new_node = new Search_Node( new_state, n->action() );
				new_node->gn() = n->gn();
				m_closed_goal_states->put( new_node );
			}
	}
	Closed_List_Type*	closed_goal_states() 			 { return m_closed_goal_states; }


	void                    reset_closed_goal_states( ) {
		if( closed_goal_states() ){
			// for ( typename Closed_List_Type::iterator i = m_closed_goal_states->begin();
			//       i != m_closed_goal_states->end(); i++ ) {
			// 	i->second = NULL;
			// }
			m_closed_goal_states->clear();
		}
	}


	bool is_goal_state_closed( Search_Node* n ) {
		if( !closed_goal_states() ) return false;

		n->compare_only_state( true );
		Search_Node* n2 = this->closed_goal_states()->retrieve(n);
		n->compare_only_state( false );

		if ( n2 != NULL )
			return true;

		return false;
	}

    /**
	 * Starts a new IW search if new subproblem is encountered.
	 */
	virtual bool  is_goal( Search_Node* n ) {
		State* s = n->state();
		assert(s != NULL);
        // if goal state is closed then don't waste time with expensive computation
		if( is_goal_state_closed( n ) )
			return false;

		// evaluate sketch!
		if (m_sketch->process_state(s)) {
			close_goal_state( n );
			return true;
		} else {
			return false;
		}
	}

protected:
	Closed_List_Type*			m_closed_goal_states;
	BaseSketch*					m_sketch;
};

}

}



#endif // Sketch_serialized_search.hxx
