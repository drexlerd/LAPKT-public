#include <siw_plus_bfs_f.hxx>
using namespace boost::python;

BOOST_PYTHON_MODULE( libatlapkt )
{
	class_<SIW_PLUS_BFS_F_Planner>("SIW_PLUS_BFS_F_Planner")
		.def( init< std::string, std::string >() )
		.def( "add_atom", &SIW_PLUS_BFS_F_Planner::add_atom )
		.def( "add_action", &SIW_PLUS_BFS_F_Planner::add_action )
		.def( "add_mutex_group", &SIW_PLUS_BFS_F_Planner::add_mutex_group )
		.def( "num_atoms", &SIW_PLUS_BFS_F_Planner::n_atoms )
		.def( "num_actions", &SIW_PLUS_BFS_F_Planner::n_actions )
		.def( "get_atom_name", &SIW_PLUS_BFS_F_Planner::get_atom_name )
		.def( "get_domain_name", &SIW_PLUS_BFS_F_Planner::get_domain_name )
		.def( "get_problem_name", &SIW_PLUS_BFS_F_Planner::get_problem_name )
		.def( "add_precondition", &SIW_PLUS_BFS_F_Planner::add_precondition )
		.def( "add_effect", &SIW_PLUS_BFS_F_Planner::add_effect )
		.def( "add_cond_effect", &SIW_PLUS_BFS_F_Planner::add_cond_effect )
		.def( "set_cost", &SIW_PLUS_BFS_F_Planner::set_cost )
		.def( "notify_negated_conditions", &SIW_PLUS_BFS_F_Planner::notify_negated_conditions )
		.def( "create_negated_fluents", &SIW_PLUS_BFS_F_Planner::create_negated_fluents )
		.def( "set_init", &SIW_PLUS_BFS_F_Planner::set_init )
		.def( "set_goal", &SIW_PLUS_BFS_F_Planner::set_goal )
		.def( "set_domain_name", &SIW_PLUS_BFS_F_Planner::set_domain_name )
		.def( "set_problem_name", &SIW_PLUS_BFS_F_Planner::set_problem_name )
		.def( "write_ground_pddl", &SIW_PLUS_BFS_F_Planner::write_ground_pddl )
		.def( "print_action", &SIW_PLUS_BFS_F_Planner::print_action )
		.def( "setup", &SIW_PLUS_BFS_F_Planner::setup )
		.def( "solve", &SIW_PLUS_BFS_F_Planner::solve )
		.def_readwrite( "ignore_action_costs", &SIW_PLUS_BFS_F_Planner::m_ignore_action_costs )
		.def_readwrite( "parsing_time", &SIW_PLUS_BFS_F_Planner::m_parsing_time )
		.def_readwrite( "iw_bound", &SIW_PLUS_BFS_F_Planner::m_iw_bound )
		.def_readwrite( "max_novelty", &SIW_PLUS_BFS_F_Planner::m_max_novelty )
		.def_readwrite( "log_filename", &SIW_PLUS_BFS_F_Planner::m_log_filename )
		.def_readwrite( "enable_siw_plus", &SIW_PLUS_BFS_F_Planner::m_enable_siw_plus )
		.def_readwrite( "enable_bfs_f", &SIW_PLUS_BFS_F_Planner::m_enable_bfs_f )
	;
}

