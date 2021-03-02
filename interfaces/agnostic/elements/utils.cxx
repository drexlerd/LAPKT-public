#include "utils.hxx"

namespace aptk {
namespace elements {

std::map<Concept, Roles> compute_concept_role_mapping(const Roles &roles, bool first) {
    std::map<Concept, Roles> result;
    for (const Role& r : roles) {
        Concept c = (first) ? r.first : r.second;
        if (result.find(c) == result.end()) {
            result.insert(std::make_pair(c, Roles( {r,} )));
        } else {
            result.at(c).push_back(r);
        }
    }
    return result;
}

}
}
