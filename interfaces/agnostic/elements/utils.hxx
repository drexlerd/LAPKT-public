#ifndef __ELEMENT_UTILS__
#define __ELEMENT_UTILS__

#include <map>
#include "role.hxx"

namespace aptk {
namespace elements {

/**
 * Computes representation of roles sorted and grouped by the concept
 * that occurs the a specific position depending on parameter first.
 */
extern std::map<Concept, Roles> compute_concept_role_mapping(const Roles &roles, bool first);

}
}

#endif
