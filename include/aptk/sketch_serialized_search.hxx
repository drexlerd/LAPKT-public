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

namespace aptk {

namespace search {


template < typename Search_Model, typename Search_Strategy, typename Search_Node >
class Sketch_Serialized_Search : public Search_Strategy {

public:

	typedef		typename Search_Model::State_Type		                          State;
	typedef 	Closed_List< Search_Node >			                          Closed_List_Type;

	Sketch_Serialized_Search( 	const Search_Model& search_problem )
		: Search_Strategy( search_problem ), m_consistency_test(true), m_closed_goal_states( NULL )  {
		m_reachability = new aptk::agnostic::Reachability_Test( this->problem().task() );
		const Sketch_STRIPS_Problem* sketch_problem = static_cast<const Sketch_STRIPS_Problem*>(&search_problem.task());
		if (sketch_problem->sketch_name() == "grid.sketch") {

		} else {
			// default is goal sketch.
			m_sketch = new aptk::GoalCounterSketch(sketch_problem);
		}
	}

	virtual ~Sketch_Serialized_Search() {
		delete m_reachability;
		m_closed_goal_states = NULL;
	}

	void                    set_consistency_test( bool b )           { m_consistency_test = b; }
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


	bool 		        is_goal_state_closed( Search_Node* n ) 	 {
		if( !closed_goal_states() ) return false;

		n->compare_only_state( true );
		Search_Node* n2 = this->closed_goal_states()->retrieve(n);
		n->compare_only_state( false );

		if ( n2 != NULL )
			return true;

		return false;
	}

	void debug_info( State*s, Fluent_Vec& unachieved ){

			std::cout << std::endl;
			std::cout << "Goals Achieved: ";


			for(Fluent_Vec::iterator it = m_goals_achieved.begin(); it != m_goals_achieved.end(); it++){
				std::cout << " " << this->problem().task().fluents()[ *it ]->signature();
			}

			std::cout << std::endl;
			std::cout << "Unachieved Goals: ";
			for(Fluent_Vec::iterator it = unachieved.begin(); it != unachieved.end(); it++){
				std::cout << " " << this->problem().task().fluents()[ *it ]->signature();
			}
			std::cout << std::endl;
			std::cout << "Current State: ";
			s->print( std::cout );

	}

	void exclude_actions( Bit_Set& excluded ){
		std::vector< const Action*>::const_iterator it_a =  this->problem().task().actions().begin();
		unsigned asize = this->problem().num_actions();
		unsigned fsize = m_goals_achieved.size();
		const bool has_ceff = this->problem().task().has_conditional_effects();

		for ( unsigned i = 0; i < asize ; i++, it_a++ ) {

			/**
			 * If actions edel or adds fluent that has to persist, exclude action.
			 */
			unsigned p = 0;
			for(; p < fsize; p++){
				unsigned fl = m_goals_achieved.at(p);

				if(has_ceff){
					if( (*it_a)->consumes( fl ) ){
						excluded.set( i );
						break;
					}
				}
				else if( (*it_a)->edeletes( fl ) ){
					excluded.set( i );
					break;
				}

			}
			if( p == fsize )
				excluded.unset( i );

		}


	}

    /**
	 * Starts a new IW search if new subproblem is encountered.
	 * TODO: change this goal check to work with sketches
	 */
	virtual bool  is_goal( Search_Node* n ) {
		const bool has_state = n->has_state();
		static Fluent_Vec added_fluents;
		static Fluent_Vec deleted_fluents;

		State* s = has_state ? n->state() : n->parent()->state();

		// evaluate sketch!
		m_sketch->process_state(s);

		if( ! has_state ){
			added_fluents.clear();
			deleted_fluents.clear();
			n->parent()->state()->progress_lazy_state(  this->problem().task().actions()[ n->action() ], &added_fluents, &deleted_fluents );
		}


		for(Fluent_Vec::iterator it =  m_goals_achieved.begin(); it != m_goals_achieved.end(); it++){
			if(  ! s->entails( *it ) ){
				if( ! has_state )
					n->parent()->state()->regress_lazy_state( this->problem().task().actions()[ n->action() ], &added_fluents, &deleted_fluents );

				return false;
			}

		}


		if( is_goal_state_closed( n ) )
			return false;

		bool new_goal_achieved = false;
		Fluent_Vec unachieved;
		for(Fluent_Vec::iterator it = m_goal_candidates.begin(); it != m_goal_candidates.end(); it++){
			if(  s->entails( *it ) )
				{
					m_goals_achieved.push_back( *it );


					if(!m_consistency_test){
						new_goal_achieved = true;
						continue;
					}

					static Bit_Set excluded( this->problem().num_actions() );
					exclude_actions( excluded );

					#ifdef DEBUG
						if ( this->verbose() )
							debug_info( s, unachieved );
					#endif

					if(m_reachability->is_reachable( s->fluent_vec() , this->problem().task().goal() , excluded  ) )
						new_goal_achieved = true;
					else{
						unachieved.push_back( *it );
						m_goals_achieved.pop_back();
					}

				}
			else
				unachieved.push_back( *it );
		}

		if( ! has_state )
			n->parent()->state()->regress_lazy_state( this->problem().task().actions()[ n->action() ], &added_fluents, &deleted_fluents );

		if ( new_goal_achieved ){
			m_goal_candidates = unachieved;

			if( ! has_state )
				n->set_state( n->parent()->state()->progress_through( *(this->problem().task().actions()[ n->action() ]) ));

			close_goal_state( n );
			return true;
		}
		else
			return false;

	}

	Fluent_Vec&        goal_candidates(){ return m_goal_candidates; }
	Fluent_Vec&        goals_achieved(){ return m_goals_achieved; }

protected:
	aptk::agnostic::Reachability_Test*      m_reachability;

	Fluent_Vec                              m_goals_achieved;
	Fluent_Vec                              m_goal_candidates;
	bool                                    m_consistency_test;
	Closed_List_Type*			m_closed_goal_states;
	BaseSketch*					m_sketch;
};

}

}



#endif // Sketch_serialized_search.hxx
