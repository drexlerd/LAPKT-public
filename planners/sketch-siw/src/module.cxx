#include <ssiw_planner.hxx>
using namespace boost::python;

BOOST_PYTHON_MODULE( libsiw )
{
	class_<SSIW_Planner>("SSIW_Planner")
		.def( init< std::string, std::string, std::string >() )
		.def( "add_atom", &SSIW_Planner::add_atom )
		.def( "add_action", &SSIW_Planner::add_action )
		.def( "add_mutex_group", &SSIW_Planner::add_mutex_group )
		.def( "num_atoms", &SSIW_Planner::n_atoms )
		.def( "num_actions", &SSIW_Planner::n_actions )
		.def( "get_atom_name", &SSIW_Planner::get_atom_name )
		.def( "get_domain_name", &SSIW_Planner::get_domain_name )
		.def( "get_problem_name", &SSIW_Planner::get_problem_name )
		.def( "add_precondition", &SSIW_Planner::add_precondition )
		.def( "add_effect", &SSIW_Planner::add_effect )
		.def( "add_cond_effect", &SSIW_Planner::add_cond_effect )
		.def( "set_cost", &SSIW_Planner::set_cost )
		.def( "notify_negated_conditions", &SSIW_Planner::notify_negated_conditions )
		.def( "create_negated_fluents", &SSIW_Planner::create_negated_fluents )
		.def( "set_init", &SSIW_Planner::set_init )
		.def( "set_goal", &SSIW_Planner::set_goal )
		.def( "set_domain_name", &SSIW_Planner::set_domain_name )
		.def( "set_problem_name", &SSIW_Planner::set_problem_name )
		.def( "write_ground_pddl", &SSIW_Planner::write_ground_pddl )
		.def( "print_action", &SSIW_Planner::print_action )
		.def( "setup", &SSIW_Planner::setup )
		.def( "solve", &SSIW_Planner::solve )
		.def_readwrite( "parsing_time", &SSIW_Planner::m_parsing_time )
		.def_readwrite( "ignore_action_costs", &SSIW_Planner::m_ignore_action_costs )
		.def_readwrite( "iw_bound", &SSIW_Planner::m_iw_bound )
		.def_readwrite( "log_filename", &SSIW_Planner::m_log_filename )
	;
}
