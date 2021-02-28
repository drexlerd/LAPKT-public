#ifndef __FEATURE_UTILS__
#define __FEATURE_UTILS__

#include <string>
#include <vector>

namespace aptk {

std::string compute_derived_predicate_signature(
    const std::string& left_predicate_name,
    const std::string& right_predicate_name,
    const std::vector<std::string>& obj_names);

std::string compute_derived_predicate_name(
    const std::string& left_predicate_name,
    const std::string& right_predicate_name);
}

#endif
