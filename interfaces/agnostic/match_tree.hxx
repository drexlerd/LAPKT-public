/*
Lightweight Automated Planning Toolkit
Copyright (C) 2012
Miquel Ramirez <miquel.ramirez@rmit.edu.au>
Nir Lipovetzky <nirlipo@gmail.com>
Christian Muise <christian.muise@gmail.com>

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

#ifndef __MATCH_TREE__
#define __MATCH_TREE__

#include <types.hxx>
#include <vector>
#include <string>
#include <set>

namespace aptk {

class STRIPS_Problem;
class State;
class Action;

namespace agnostic {

// Match tree data structure from PRP ( https://bitbucket.org/haz/planner-for-relevant-policies )
	

class BaseNode {
public:
	virtual ~BaseNode() {}
	virtual void dump( std::string indent ) const = 0;
	virtual void generate_applicable_items( const State& s, std::vector<int>& actions, const STRIPS_Problem& prob ) = 0;
	
	BaseNode *create_tree( std::vector<int>& actions, std::set<int> &vars_seen, const STRIPS_Problem& prob );
	int get_best_var( std::vector<int>& actions, std::set<int> &vars_seen, const STRIPS_Problem& prob );
	bool reg_item_done( int action_id, std::set<int> &vars_seen, const STRIPS_Problem& prob );
};


class SwitchNode : public BaseNode {
	int switch_var;
	std::vector<int> immediate_items;
	std::vector<BaseNode *> children;
	BaseNode * default_child;
	
public:
	~SwitchNode();
	SwitchNode( std::vector<int>& actions, std::set<int> &vars_seen, const STRIPS_Problem& prob );
	virtual void generate_applicable_items( const State& s, std::vector<int>& actions, const STRIPS_Problem& prob );
	virtual void dump( std::string indent ) const;
};


class LeafNode : public BaseNode {
	std::vector<int> applicable_items;
public:
	LeafNode( std::vector<int>& actions );
	virtual void generate_applicable_items( const State& s, std::vector<int>& actions, const STRIPS_Problem& prob );
	virtual void dump( std::string indent ) const;
};


class EmptyNode : public BaseNode {
public:
	virtual void generate_applicable_items( const State &, std::vector<int>&, const STRIPS_Problem& ) {}
	virtual void dump( std::string indent ) const;
};


class Match_Tree {

public:

	Match_Tree ( const STRIPS_Problem& prob ) : m_problem( prob ) {}

	~Match_Tree() { delete root_node; };

	void build();
	void retrieve_applicable( const State& s, std::vector<int>& actions ) const;

private:

	const STRIPS_Problem& m_problem;
	BaseNode * root_node;

};


}

}

#endif // match_tree.hxx
