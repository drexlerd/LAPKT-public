
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

#ifndef __SKETCH_STRIPS_PROBLEM__
#define __SKETCH_STRIPS_PROBLEM__

#include "strips_prob.hxx"
#include <string>
#include <map>
#include <iosfwd>
#include <types.hxx>
#include <succ_gen.hxx>
#include <match_tree.hxx>
#include <algorithm>
#include <mutex_set.hxx>

namespace aptk
{

class Sketch_STRIPS_Problem : public STRIPS_Problem {
protected:
    std::string m_sketch_name;



public:
	Sketch_STRIPS_Problem( std::string dom_name = "Unnamed", std::string prob_name = "Unnamed ", std::string sketch_name = "Unnamed ");
	virtual ~Sketch_STRIPS_Problem();
};



}

#endif // Sketch_STRIPS_Problem.hxx
