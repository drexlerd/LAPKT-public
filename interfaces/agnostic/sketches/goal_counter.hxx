
#ifndef __GOAL_COUNTER__
#define __GOAL_COUNTER__


#include <sketch.hxx>
#include <landmark_graph.hxx>

namespace aptk {
class GoalCounterSketch;

/**
 * Counts the number of unsatisfied goal atoms.
 */
class GoalCounterFeature : public NumericalFeature {
private:
    aptk::agnostic::Reachability_Test* m_reachability;
    bool m_consistency_test;

    // Store achieved goal atoms
    Fluent_Vec m_goals_achieved;
    Fluent_Vec m_goal_candidates;

	// initial number of goal candidates
	unsigned m_initial_num_goal_candidates;

private:
    void exclude_actions( Bit_Set& excluded ){
		std::vector< const Action*>::const_iterator it_a = this->sketch()->problem()->actions().begin();
		unsigned asize = this->sketch()->problem()->num_actions();
		unsigned fsize = m_goals_achieved.size();
		const bool has_ceff = this->sketch()->problem()->has_conditional_effects();

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

public:
    GoalCounterFeature(const BaseSketch* sketch)
      : NumericalFeature(sketch),
        m_consistency_test(true),
        m_reachability(new aptk::agnostic::Reachability_Test( *(this->sketch()->problem()) )) {
		// initialize goal candidates
		this->m_goal_candidates = this->sketch()->problem()->goal();
		this->m_initial_num_goal_candidates = this->m_goal_candidates.size();
    }
    virtual ~GoalCounterFeature() = default;

    virtual void evaluate(const SketchState &sketch_state) override {
        // 1. the state has to satisfy at least the previously satisfied goal atoms.
        const State* s = sketch_state.state();
		for(Fluent_Vec::iterator it = m_goals_achieved.begin(); it != m_goals_achieved.end(); it++){
			if(! s->entails( *it )){
				return;
			}
		}
        // 2. check if there is an additional goal atom that becomes true in s.
		bool new_goal_achieved = false;
		Fluent_Vec unachieved;
		for(Fluent_Vec::iterator it = m_goal_candidates.begin(); it != m_goal_candidates.end(); it++){
			if(  s->entails( *it ) )  // goal atom satisfied in s
				{
					m_goals_achieved.push_back( *it );


					if(!m_consistency_test){
						new_goal_achieved = true;
						continue;
					}

					static Bit_Set excluded( sketch()->problem()->num_actions() );
					exclude_actions( excluded );

					/*#ifdef DEBUG
						if ( this->verbose() )
							debug_info( s, unachieved );
					#endif*/

					if(m_reachability->is_reachable( s->fluent_vec() , sketch()->problem()->goal() , excluded  ) )
						new_goal_achieved = true;
					else{
						unachieved.push_back( *it );
						m_goals_achieved.pop_back();
					}
				}
			else
				unachieved.push_back( *it );
		}
		// set goal candidates for next evaluation
		m_goal_candidates = unachieved;

        // set the result value
        new_eval = m_initial_num_goal_candidates - m_goals_achieved.size();
        // assert(new_eval > 0);
    }
};

/**
 * Sketch with single rule that decreases the number of achieved goal atoms.
 */
class GoalCounterSketch : public BaseSketch {
public:
    GoalCounterSketch(const Sketch_STRIPS_Problem *problem) : BaseSketch(problem) {
        add_numerical_feature("goal_counter", new GoalCounterFeature(this));
        add_rule(new Rule(this,
            {},
            { new NonzeroNumerical(get_numerical_feature("goal_counter")), },
            {},
            { new DecrementNumerical(get_numerical_feature("goal_counter")), }
        ));
    }
    virtual ~GoalCounterSketch() = default;
};

}

#endif