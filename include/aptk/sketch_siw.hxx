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

#ifndef __SKETCH_SIW__
#define __SKETCH_SIW__

#include <aptk/search_prob.hxx>
#include <aptk/resources_control.hxx>
#include <aptk/closed_list.hxx>
#include <aptk/iw.hxx>
#include <landmark_graph.hxx>
#include <vector>
#include <algorithm>
#include <iostream>

#include "../../interfaces/agnostic/sketches/goal_counter_dec.hxx"
#include "../../interfaces/agnostic/sketches/goal_counter.hxx"
#include "../../interfaces/agnostic/sketches/childsnack.hxx"
#include "../../interfaces/agnostic/sketches/driverlog.hxx"
#include "../../interfaces/agnostic/sketches/barman.hxx"
#include "../../interfaces/agnostic/sketches/hiking.hxx"
#include "../../interfaces/agnostic/sketches/schedule.hxx"
#include "../../interfaces/agnostic/sketches/tpp.hxx"
#include "../../interfaces/agnostic/sketches/grid.hxx"
#include "../../interfaces/agnostic/sketches/philosophers.hxx"

namespace aptk {

namespace search {


template < typename Search_Model >
class Sketch_SIW : public brfs::IW<Search_Model, aptk::agnostic::Novelty<Search_Model, aptk::search::brfs::Node< aptk::State >>> {

public:

	typedef		aptk::search::brfs::Node< aptk::State >		Search_Node;
	typedef		aptk::agnostic::Landmarks_Graph				Landmarks_Graph;
    typedef		aptk::BaseSketch							BaseSketch;
	typedef		typename Search_Model::State_Type		                          State;
	typedef 	Closed_List< Search_Node >			                          Closed_List_Type;

	Sketch_SIW( const Search_Model& search_problem )
		: brfs::IW<Search_Model, aptk::agnostic::Novelty<Search_Model, Search_Node>>( search_problem ), m_pruned_sum_B_count(0), m_sum_B_count(0), m_max_B_count(0), m_iw_calls(0), m_max_bound( std::numeric_limits<unsigned>::max() ), m_closed_goal_states( NULL ) {
		m_goal_agenda = NULL;
		const Sketch_STRIPS_Problem* sketch_problem = static_cast<const Sketch_STRIPS_Problem*>(&search_problem.task());
		if (sketch_problem->domain_name() == "child-snack") {
            m_sketch = new aptk::ChildsnackSketch(sketch_problem);
		} else if (sketch_problem->domain_name() == "driverlog") {
            m_sketch = new aptk::DriverlogSketch(sketch_problem);
		} else if (sketch_problem->domain_name() == "barman") {
			m_sketch = new aptk::BarmanSketch(sketch_problem);
		} else if (sketch_problem->domain_name() == "hiking") {
            m_sketch = new aptk::HikingSketch(sketch_problem);
		} else if (sketch_problem->domain_name() == "schedule") {
            m_sketch = new aptk::ScheduleSketch(sketch_problem);
		} else if (sketch_problem->domain_name() == "tpp-propositional") {
            m_sketch = new aptk::TppSketch(sketch_problem);
		} else if (sketch_problem->domain_name() == "grid") {
            m_sketch = new aptk::GridSketch(sketch_problem);
		} else if (sketch_problem->domain_name() == "protocol") {
            m_sketch = new aptk::PhilosophersSketch(sketch_problem);
		} else {
			// default is goal sketch.
			m_sketch = new aptk::GoalCounterDecSketch(sketch_problem);
		}
	}

	virtual ~Sketch_SIW() {
	}
	void            set_goal_agenda( Landmarks_Graph* lg ) { m_goal_agenda = lg; }

    /**
	 * Calls IW for each subproblem encountered
	 */
	virtual bool	find_solution( float& cost, std::vector<Action_Idx>& plan, std::vector<std::vector<Action_Idx>>& partial_plans, std::vector<std::string>& sketch_plan, std::vector<unsigned>& subproblem_widths) {

		unsigned gsize = this->problem().task().goal().size();
		Search_Node* end = NULL;
		State* new_init_state = NULL;

		cost = 0;

        /**
		 * Initialize the sketch for the task's initial state.
		 */
		new_init_state = new State( this->problem().task() );
		new_init_state->set( this->m_root->state()->fluent_vec() );
		m_sketch->initialize_first_subproblem( new_init_state );

		do{
			if ( this->verbose() )
				//std::cout << std::endl << "{" << gsize << "/" << this->m_goal_candidates.size() << "/" << this->m_goals_achieved.size() << "}:IW(" << this->bound() << ") -> ";
			end = this->do_search();
			m_pruned_sum_B_count += this->pruned_by_bound();


			if ( end == NULL ) {


				/**
				 * If no partial plan to achieve any goal is  found,
				 * throw IW(b+1) from same root node
				 *
				 * If no state has been pruned by bound, then IW is in a dead-end,
				 * return NO-PLAN
				 */
				if( this->pruned_by_bound() == 0)
					return false;

				new_init_state = new State( this->problem().task() );
				new_init_state->set( this->m_root->state()->fluent_vec() );
				new_init_state->update_hash();

				if ( this->bound() > this->max_bound() ) // Hard cap on width exceeded
					return false;

				this->set_bound( this->bound()+1 );
				this->start( new_init_state );

				// Memory exceeded to reserve data structures for novelty
				if(this->m_novelty->arity() != this->bound() )
					return false;
			}
			else{

				/**
				 * If a partial plan extending the achieved goals set is found,
				 * IW(1) is thrown from end_state
				 */

				m_max_B_count = m_max_B_count < this->bound() ? this->bound() : m_max_B_count;
				m_sum_B_count += this->bound();
				m_iw_calls++;

				std::vector<Action_Idx> partial_plan;
				float partial_cost = 0.0f;
				this->extract_plan( this->m_root, end, partial_plan, partial_cost );
				plan.insert( plan.end(), partial_plan.begin(), partial_plan.end() );
				partial_plans.push_back(partial_plan);
				sketch_plan.push_back(m_sketch->applied_rules().back()->name());
				subproblem_widths.push_back(this->bound());
				cost += partial_cost;

				new_init_state = new State( this->problem().task() );
				new_init_state->set( end->state()->fluent_vec() );
				new_init_state->update_hash();

				this->set_bound( 1 );
				this->start( new_init_state );
			}


		} while( !this->problem().goal( *new_init_state ) );

		return true;
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

	void set_closed_goal_states( Closed_List_Type* c ){ m_closed_goal_states = c; }
	void close_goal_state( Search_Node* n ) 	 {
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
	Closed_List_Type* closed_goal_states() { return m_closed_goal_states; }


	void reset_closed_goal_states( ) {
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

	unsigned		sum_pruned_by_bound() const		{ return m_pruned_sum_B_count; }
	float                   avg_B() const { return (float)(m_sum_B_count) / m_iw_calls; }
	unsigned                max_B() const { return m_max_B_count; }
	void			set_max_bound( unsigned v ) { m_max_bound = v; }
	unsigned		max_bound( ) { return m_max_bound; }

protected:
	unsigned		m_pruned_sum_B_count;
	unsigned		m_sum_B_count;
	unsigned		m_max_B_count;
	unsigned		m_iw_calls;
	Landmarks_Graph*        m_goal_agenda;
	unsigned		m_max_bound;

	Closed_List_Type*			m_closed_goal_states;
	BaseSketch*					m_sketch;
};

}

}



#endif // Sketch_siw.hxx
