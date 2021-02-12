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

#ifndef __FLUENT__
#define __FLUENT__

#include <types.hxx>
#include <strips_prob.hxx>

namespace aptk
{

class Fluent
{
public:

	Fluent( STRIPS_Problem& p );
	~Fluent();

	unsigned	index() const;

    unsigned	pddl_predicate_type() const;
	const Index_Vec&	pddl_objs_idx() const;

	const std::string&	signature() const;
	const std::string&	pddl_predicate_name() const;
	const Name_Vec&   pddl_object_names() const;

	void		set_index( unsigned idx );
	void		set_signature( std::string signature );
	void		set_predicate_type( unsigned predicate_type );
	void		set_objs_idx( Index_Vec &&objs_idx );
	void		set_predicate_name ( std::string predicate_name );
	void		set_objs_names ( Name_Vec &&objs_names );

	STRIPS_Problem& problem();

protected:
	STRIPS_Problem&			m_problem;

	// grounded information
	unsigned			m_index;

    // fol information
	unsigned			m_predicate_type;
	Index_Vec			m_objs_idx;

    // human readable information
    std::string			m_signature;
	std::string			m_predicate_name;
    Name_Vec			m_objs_names;
};

inline unsigned		Fluent::index() const
{
	return m_index;
}

inline unsigned	Fluent::pddl_predicate_type() const {
    return m_predicate_type;
}

inline const Index_Vec& Fluent::pddl_objs_idx() const {
    return m_objs_idx;
}

inline const std::string& Fluent::signature() const
{
	return m_signature;
}

inline const std::string& Fluent::pddl_predicate_name() const {
    return m_predicate_name;
}

inline const Name_Vec& Fluent::pddl_object_names() const {
    return m_objs_names;
}

inline void	Fluent::set_index( unsigned idx )
{
	m_index = idx;
}

inline void	Fluent::set_signature( std::string sig )
{
	m_signature = sig;
}

inline void Fluent::set_predicate_type( unsigned predicate_type)
{
    m_predicate_type = predicate_type;
}

inline void Fluent::set_objs_idx( Index_Vec &&objs_idx)
{
    m_objs_idx = move(objs_idx);
}

inline void Fluent::set_predicate_name ( std::string predicate_name )
{
    m_predicate_name = predicate_name;
}

inline void Fluent::set_objs_names ( Name_Vec &&objs_names)
{
    m_objs_names = move(objs_names);
}

}

#endif // Fluent.hxx
