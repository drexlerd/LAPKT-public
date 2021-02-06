#ifndef __SSIW_Planner__
#define __SSIW_Planner__

#include <py_strips_prob.hxx>
#include <fwd_search_prob.hxx>
#include <novelty.hxx>
#include <aptk/siw.hxx>

typedef	aptk::search::SIW< aptk::agnostic::Fwd_Search_Problem >  SIW_Fwd;

class	SSIW_Planner : public STRIPS_Problem
{
public:

	SSIW_Planner( );
	SSIW_Planner( std::string, std::string, std::string );
	virtual ~SSIW_Planner();


	virtual void setup();
	void	solve();

	int		m_iw_bound;
	std::string	m_log_filename;
	std::string	m_plan_filename;

protected:

	float	do_search( SIW_Fwd& engine);

};

#endif
