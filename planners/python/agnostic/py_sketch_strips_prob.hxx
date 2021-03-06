#ifndef __PY_FOD_PROBLEM__
#define __PY_FOD_PROBLEM__

#include <py_strips_prob.hxx>
#include <sketch_strips_prob.hxx>
#include <fluent.hxx>
#include <action.hxx>
#include <boost/python.hpp>
#include <string>
#include <set>


class Sketch_STRIPS_Problem : public STRIPS_Problem {
public:
	Sketch_STRIPS_Problem(  );
	Sketch_STRIPS_Problem( std::string, std::string, std::string, bool );
	virtual ~Sketch_STRIPS_Problem();

	aptk::Sketch_STRIPS_Problem*	instance() {
		return static_cast<aptk::Sketch_STRIPS_Problem*>(m_problem);
	}

    // add atoms that occur in effects of operators.
	void add_atom_ext(
		std::string name, int predicate_type,
	    std::string predicate_name, boost::python::list &objects);
    // add atoms that do not occur in effects of operators.
	void add_init_atom_ext(
		std::string name, int predicate_type,
		std::string predicate_name, boost::python::list &objects);

	void create_negated_fluents_ext();

	void set_sketch_name( std::string sketch_name );
	void set_use_goal_counter( bool use_goal_counter );
};

#endif // py_sketch_strips_problem.hxx
