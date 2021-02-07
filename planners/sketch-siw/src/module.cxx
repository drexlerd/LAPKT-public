#include <sketch_siw_planner.hxx>
using namespace boost::python;

BOOST_PYTHON_MODULE( libsiw )
{
	class_<Sketch_SIW_Planner>("Sketch_SIW_Planner")
		.def( init< std::string, std::string, std::string >() )
		.def( "add_atom", &Sketch_SIW_Planner::add_atom )
		.def( "add_atom_ext", &Sketch_SIW_Planner::add_atom_ext )  // for adding fol information
		.def( "add_action", &Sketch_SIW_Planner::add_action )
		.def( "add_mutex_group", &Sketch_SIW_Planner::add_mutex_group )
		.def( "num_atoms", &Sketch_SIW_Planner::n_atoms )
		.def( "num_actions", &Sketch_SIW_Planner::n_actions )
		.def( "get_atom_name", &Sketch_SIW_Planner::get_atom_name )
		.def( "get_domain_name", &Sketch_SIW_Planner::get_domain_name )
		.def( "get_problem_name", &Sketch_SIW_Planner::get_problem_name )
		.def( "add_precondition", &Sketch_SIW_Planner::add_precondition )
		.def( "add_effect", &Sketch_SIW_Planner::add_effect )
		.def( "add_cond_effect", &Sketch_SIW_Planner::add_cond_effect )
		.def( "set_cost", &Sketch_SIW_Planner::set_cost )
		.def( "notify_negated_conditions", &Sketch_SIW_Planner::notify_negated_conditions )
		.def( "create_negated_fluents", &Sketch_SIW_Planner::create_negated_fluents )
		.def( "set_init", &Sketch_SIW_Planner::set_init )
		.def( "set_goal", &Sketch_SIW_Planner::set_goal )
		.def( "set_domain_name", &Sketch_SIW_Planner::set_domain_name )
		.def( "set_problem_name", &Sketch_SIW_Planner::set_problem_name )
		.def( "write_ground_pddl", &Sketch_SIW_Planner::write_ground_pddl )
		.def( "print_action", &Sketch_SIW_Planner::print_action )
		.def( "setup", &Sketch_SIW_Planner::setup )
		.def( "solve", &Sketch_SIW_Planner::solve )
		.def_readwrite( "parsing_time", &Sketch_SIW_Planner::m_parsing_time )
		.def_readwrite( "ignore_action_costs", &Sketch_SIW_Planner::m_ignore_action_costs )
		.def_readwrite( "iw_bound", &Sketch_SIW_Planner::m_iw_bound )
		.def_readwrite( "log_filename", &Sketch_SIW_Planner::m_log_filename )
	;
}
