#ifndef __OPEN_LIST__
#define __OPEN_LIST__

#include <vector>
#include <queue>
#include <aptk/ext_math.hxx>

namespace aptk
{

namespace search {

template <typename Node>
class Node_Comparer
{             
	/**
	* bigger g higher priority
	*/
	static bool WAStarComp( Node* a, Node* b) {
		return (dless(b->fn(), a->fn()) || (dequal(a->fn(), b->fn()) && dless(a->gn(), b->gn())));
	}
        
	/**
	* lower g higher priority
	*/
	static bool GBFSComp( Node* a, Node* b) {
		return (dless(b->fn(), a->fn()) || (dequal(a->fn(), b->fn()) && dless(b->gn(), a->gn())));
	}
public:
	bool operator()( Node* a, Node* b ) {
		return WAStarComp(a, b);
	}
};          
  
template < class Node_Comp, class Node >
class Open_List
{

        std::priority_queue< Node*, std::vector< Node* >, Node_Comp > m_queue;
	//std::priority< Node, std::vector< Node >, Node_Comp > m_queue;
public:
	Open_List();
	~Open_List();

	void 		insert( Node* );
	Node* 		pop();
	bool		empty() const;
	float		min() const;
	void		clear();
};

template < class Node_Comp, class Node >
Open_List<Node_Comp, Node>::Open_List()
{
}

template < typename Node_Comp, typename Node >
Open_List<Node_Comp, Node>::~Open_List()
{
}

template < typename Node_Comp, typename Node >
void	Open_List<Node_Comp, Node>::insert( Node* n )
{
	m_queue.push( n );
}

template <typename Node_Comp, typename Node >
Node*	Open_List<Node_Comp, Node>::pop()
{
        if( empty() ) return NULL;
	Node* elem = m_queue.top();
	m_queue.pop();
	return elem;
}

template < typename Node_Comp, typename Node >
bool	Open_List<Node_Comp, Node>::empty() const
{
	return m_queue.empty();
}

template < typename Node_Comp, typename Node >
float     Open_List<Node_Comp, Node>::min() const
{
	if ( empty() ) return 0.0f;
	return m_queue.top()->f;
}

template < typename Node_Comp, typename Node >
void	Open_List<Node_Comp, Node>::clear() 
{
	while ( !empty() )
	{
		Node* elem = pop();
		delete elem;
	}	
}

}

}

#endif // Open_List.hxx