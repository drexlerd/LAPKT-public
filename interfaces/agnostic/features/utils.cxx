#include "utils.hxx"

#include <sstream>

namespace aptk {

std::string compute_derived_predicate_signature(
    const std::string& left_predicate_name,
    const std::string& right_predicate_name,
    const std::vector<std::string>& obj_names) {
    std::stringstream ss;
    ss << left_predicate_name << "_"
       << right_predicate_name << "_";
    unsigned i = 0;
    for (const std::string& obj_name : obj_names) {
        ss << obj_name;
        ++i;
        if (i < obj_names.size()) {
            ss << "_";
        }
    }
    return ss.str();
}

std::string compute_derived_predicate_name(
    const std::string& left_predicate_name,
    const std::string& right_predicate_name) {
    std::stringstream ss;
    ss << left_predicate_name << "_"
       << right_predicate_name;
    return ss.str();
}

}