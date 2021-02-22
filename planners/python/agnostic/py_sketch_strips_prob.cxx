#include <py_sketch_strips_prob.hxx>
#include <cstring>
#include <fstream>
#include <iostream>

using namespace boost::python;

	Sketch_STRIPS_Problem::Sketch_STRIPS_Problem( ) {
		m_parsing_time = 0.0f;
		m_ignore_action_costs = false;
		m_problem = new aptk::Sketch_STRIPS_Problem;

	}

	Sketch_STRIPS_Problem::Sketch_STRIPS_Problem( std::string domain, std::string instance, std::string sketch ) {
		m_parsing_time = 0.0f;
		m_ignore_action_costs = false;
		m_problem = new aptk::Sketch_STRIPS_Problem( domain, instance, sketch );

	}

	Sketch_STRIPS_Problem::~Sketch_STRIPS_Problem() {
	}

	void
	Sketch_STRIPS_Problem::add_atom_ext( std::string name, int predicate_type, std::string predicate_name, boost::python::list &objects) {
		aptk::Index_Vec objs_idx;
		aptk::Name_Vec objs_names;
		objs_idx.reserve(len(objects));
		objs_names.reserve(len(objects));
		for ( int i = 0; i < len(objects); i++ ) {
			boost::python::tuple li = extract< tuple >( objects[i] );
            int obj_idx = extract<int>(li[0]);
			std::string obj_name = extract<std::string>(li[1]);
			objs_idx.emplace_back(obj_idx);
			objs_names.emplace_back(obj_name);
		}
		assert(objs_idx.size() == objs_names.size());
        aptk::Sketch_STRIPS_Problem::add_fluent( *instance(), name, predicate_type, predicate_name, move(objs_idx), move(objs_names) );
	}

	void
	Sketch_STRIPS_Problem::add_init_atom_ext( std::string name, int predicate_type, std::string predicate_name, boost::python::list &objects) {
		aptk::Index_Vec objs_idx;
		aptk::Name_Vec objs_names;
		objs_idx.reserve(len(objects));
		objs_names.reserve(len(objects));
		for ( int i = 0; i < len(objects); i++ ) {
			boost::python::tuple li = extract< tuple >( objects[i] );
            int obj_idx = extract<int>(li[0]);
			std::string obj_name = extract<std::string>(li[1]);
			objs_idx.emplace_back(obj_idx);
			objs_names.emplace_back(obj_name);
		}
		assert(objs_idx.size() == objs_names.size());
        aptk::Sketch_STRIPS_Problem::add_other_fluent( *instance(), name, predicate_type, predicate_name, move(objs_idx), move(objs_names) );
	}

	void
	Sketch_STRIPS_Problem::create_negated_fluents_ext() {
		m_negated.resize( instance()->num_fluents() );
		unsigned count = 0;
		for ( auto fl_idx : m_negated_conditions ) {
			aptk::Fluent* fl = instance()->fluents()[fl_idx];
			assert( fl != nullptr );
			std::string negated_signature = "(not " + fl->signature() + ")";
			std::vector<unsigned> objs_idx = fl->pddl_objs_idx();
			std::vector<std::string> objs_names = fl->pddl_obj_names();
			unsigned neg_fl_idx = aptk::Sketch_STRIPS_Problem::add_fluent( *instance(), negated_signature, fl->pddl_predicate_type(), fl->pddl_predicate_name(), move(objs_idx), move(objs_names) );
			m_negated.at( fl_idx ) = instance()->fluents()[neg_fl_idx];
			count++;
		}
		std::cout << count << " negated fluents created" << std::endl;
	}

	void
	Sketch_STRIPS_Problem::set_sketch_name( std::string sketch_name ) {
		instance()->set_sketch_name( sketch_name );
	}
