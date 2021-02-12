#include <sketch.hxx>

namespace aptk
{

BaseSketch::BaseSketch(
    std::unordered_map<std::string, int> &&predicate_name_to_idx,
    std::unordered_map<std::string, int> &&object_name_to_idx)
    : m_predicate_name_to_idx(std::move(predicate_name_to_idx)),
        m_object_name_to_idx(std::move(object_name_to_idx)) {
}

}