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

#ifndef __H_C__
#define __H_C__

#include <aptk/search_prob.hxx>
#include <aptk/heuristic.hxx>
#include <strips_state.hxx>
#include <conj_comp_prob.hxx>
#include <vector>
#include <deque>
#include <fstream>

namespace aptk {

namespace agnostic {

class HC_Max_Evaluation_Function {

public:
	HC_Max_Evaluation_Function( std::vector<float>& value_table ) 
	: m_values( value_table ) {
	}	

	float	operator()( Fluent_Vec::const_iterator begin, Fluent_Vec::const_iterator end ) const {
		float v = 0.0f;
		for ( Fluent_Vec::const_iterator it = begin; it != end; it++ ) {
			v = ( v < m_values[*it] ? m_values[*it] : v );
			if ( v == infty ) return v;
		}
		return v;	
	}

private:

	const std::vector<float>&	m_values;
		
};

enum class HC_Cost_Function { Ignore_Costs, Use_Costs };

template <typename Fluent_Set_Eval_Func = HC_Max_Evaluation_Function, HC_Cost_Function cost_opt = HC_Cost_Function::Use_Costs >
class HC_Heuristic {

public:
	HC_Heuristic( const CC_Problem& prob ) 
	: m_model( prob ), eval_func(m_values) {
		m_values.resize( m_model.num_fluents() );
		m_best_supporters.resize(  m_model.num_fluents() );
		m_already_updated.resize( m_model.num_fluents() );
	}

	virtual ~HC_Heuristic() {
	}

	virtual void eval( const State& s, float& h_val ) {

		m_already_updated.reset();
		m_updated.clear();
		initialize(s);
		compute();
		h_val = eval_func( m_model.goal().begin(), m_model.goal().end() );
		/*
		h_val = 0.0f;
		for ( Fluent_Vec::const_iterator it = m_model.goal().begin(); it != m_model.goal().end(); it++ ) {
			h_val= ( h_val < m_values[*it] ? m_values[*it] : h_val );
			std::cout << "Evaluation goal fluent ";
			m_model.print_fluent( *it, std::cout );
			std::cout << "which has value: " <<  m_values[*it] << std::endl;
		}
		*/

	}

	virtual void eval( const State& s, float& h_val,  std::vector<Action_Idx>& pref_ops ) {
		eval( s, h_val );
	}

	const Action*	best_supporter( unsigned f ) const {
		if ( m_best_supporters[f].first == nullptr ) return nullptr;
		return &(m_best_supporters[f].first->original());
	}

	const std::pair< const CC_Problem::CC_Action*, unsigned >& best_effect( unsigned p ) const {
		return m_best_supporters[p];
	} 

	void	print_values( std::ofstream& os ) const {
		for ( unsigned p = 0; p < m_model.num_fluents(); p++ ) {
			os << "h^C( ";
			m_model.print_fluent( p, os );
			os << ") = " << m_values[p] << ", ";
			os << "a_{p,q} = ";
			if ( m_best_supporters[p].first != nullptr ) {
				os << m_best_supporters[p].first->signature() << ", ";
				os << "via cond. eff.? " << ( m_best_supporters[p].second ? "yes" : "no" )  << std::endl;
			}
			else 
				os << "NONE" << std::endl;
		}
	}

protected:

	void	update( unsigned p, float v ) {
		if ( v >= m_values[p] ) return;
		m_values[p] = v;
		if ( !m_already_updated.isset( p ) ) {
			m_updated.push_back( p );
			m_already_updated.set( p );
		}		
	}

	void	update( unsigned p, float v, const CC_Problem::CC_Action* a, unsigned eff_index = 0 ) {
		if ( v >= m_values[p] ) return;
		m_values[p] = v;
		if ( !m_already_updated.isset( p ) ) {
			m_updated.push_back( p );
			m_already_updated.set( p );
		}
		m_best_supporters[p] = std::make_pair(a, eff_index);
	}

	void	set( unsigned p, float v ) {
		m_values[p] = v;
		if ( !m_already_updated.isset( p ) ) {
			m_updated.push_back( p );
			m_already_updated.set( p );
		}		
	}

	float	action_cost( const CC_Problem::CC_Action& a ) const {
		return cost_opt == HC_Cost_Function::Ignore_Costs ? 1.0f : a.cost();
	}

	void	initialize( const State& s ) 
	{
		for ( unsigned k = 0; k < m_model.num_fluents(); k++ ) {
			m_values[k] = infty;
			m_best_supporters[k] = std::make_pair( nullptr, false );
		}

		for ( unsigned k = 0; k < m_model.empty_prec_actions().size(); k++ ) {
			const CC_Problem::CC_Action& a = *(m_model.empty_prec_actions()[k]);
			float v =  action_cost(a); 
			for ( auto it = a.add().begin(); it != a.add().end(); it++ )
				update( *it, v, &a );
			// Conditional effects
			for ( unsigned j = 0; j < a.cond_effs().size(); j++ )
			{
				const CC_Problem::CC_Cond_Eff& ceff = a.cond_effs()[j];
				if ( !ceff.first.empty() ) continue;
				float v_eff = v;
				for ( auto it = ceff.second.begin();
					it != ceff.second.end(); it++ )
					update( *it, v_eff, &a, j+1 );
			}
		}

		for ( auto it = m_model.fluents().begin(); it != m_model.fluents().end(); it++ )
			if ( (*it)->in_set( s.fluent_vec() ) ) {
				set( (*it)->index(), 0.0f ); 
			}
	}

	void	compute(  ) 
	{
		while ( !m_updated.empty() ) {

			unsigned p = m_updated.front();
			//std::cout << p << ". "; 
			//m_model.print_fluent( p, std::cout );
			//std::cout << " h: " << m_values[p] << " required by: " << m_model.requiring(p).size() << std::endl;
			m_updated.pop_front();
			m_already_updated.unset(p);

			//Successor_Generator::Heuristic_Iterator it( m_values, m_model.successor_generator().nodes() );
			//int i = it.first();
			//std::cout << "First action: " << i << std::endl;
			//while ( i != -1 ) {
			for ( unsigned i = 0; i < m_model.num_actions(); i++ ) {
				const CC_Problem::CC_Action& a = *(m_model.actions()[i]);

				//std::cout << "Action considered: " << a.signature() << std::endl;
				bool relevant =  a.requires(p);
				
				for ( unsigned j = 0; j < a.cond_effs().size() && !relevant; j++ ) {
					const CC_Problem::CC_Cond_Eff& ceff = a.cond_effs()[j];
					relevant = relevant || (std::find(ceff.first.begin(), ceff.first.end(), p) != ceff.first.end() );
				}
				
				if ( !relevant ) {
					//i = it.next();
					continue;
				}

				float h_pre = eval_func( a.pre().begin(), a.pre().end() );

				if ( h_pre == infty ) continue;
				//assert( h_pre != infty );

				//std::cout << "Action " << i << ". " << a.signature() << " relevant" << std::endl;

				float v = action_cost(a) + h_pre; 

				for ( auto it = a.add().begin(); it != a.add().end(); it++ )
					update( *it, v, m_model.actions()[i] );
				// Conditional effects
				for ( unsigned j = 0; j < a.cond_effs().size(); j++ )
				{
					const CC_Problem::CC_Cond_Eff& ceff = a.cond_effs()[j];
					float h_cond = std::max( eval_func( ceff.first.begin(), ceff.first.end() ), h_pre);
					if ( h_cond == infty ) continue;
					float v_eff = action_cost(a) + h_cond;
					for ( auto it = ceff.second.begin();
						it != ceff.second.end(); it++ )
						update( *it, v_eff, m_model.actions()[i], j + 1 );
				}

				//i = it.next();
			}
		}
	}
	
	
protected:

	const CC_Problem&								m_model;
	std::vector<float>								m_values;
	Fluent_Set_Eval_Func								eval_func;
	std::vector< std::pair< const CC_Problem::CC_Action*, unsigned > >		m_best_supporters;
	std::vector<const CC_Problem::CC_Action*>					m_app_set;
	std::deque<unsigned> 								m_updated;
	Bit_Set										m_already_updated;
};
	
}

}

#endif // h_C.hxx