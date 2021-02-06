#ifndef __SKETCH_SIW_Planner__
#define __SKETCH_SIW_Planner__

#include <py_strips_prob.hxx>
#include <fwd_search_prob.hxx>
#include <novelty.hxx>
#include <aptk/sketch_siw.hxx>

typedef	aptk::search::Sketch_SIW< aptk::agnostic::Fwd_Search_Problem >  Sketch_SIW_Fwd;

class	Sketch_SIW_Planner : public STRIPS_Problem
{
public:

	Sketch_SIW_Planner( );
	Sketch_SIW_Planner( std::string, std::string, std::string );
	virtual ~Sketch_SIW_Planner();


	virtual void setup();
	void	solve();

	int		m_iw_bound;
	std::string	m_log_filename;
	std::string	m_plan_filename;

protected:

	float	do_search( Sketch_SIW_Fwd& engine);

};

#endif
