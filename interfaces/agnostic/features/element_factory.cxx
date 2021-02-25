#include "element_factory.hxx"
#include "binary/intersect.hxx"
#include "binary/setminus.hxx"
#include "binary/union.hxx"
#include "concept.hxx"
#include "role.hxx"

namespace aptk
{

/**
 * Instantiate static members.
 */
ElementCache<std::tuple<bool, std::string, unsigned>, BaseElement*> ElementFactory::m_leaf_concept_cache = ElementCache<std::tuple<bool, std::string, unsigned>, BaseElement*>();
ElementCache<std::tuple<bool, BaseElement*, BaseElement*>, BaseElement*> ElementFactory::m_intersect_concept_cache = ElementCache<std::tuple<bool, BaseElement*, BaseElement*>, BaseElement*>();
ElementCache<std::tuple<bool, BaseElement*, BaseElement*>, BaseElement*> ElementFactory::m_setminus_concept_cache = ElementCache<std::tuple<bool, BaseElement*, BaseElement*>, BaseElement*>();

// TODO: remove duplicate code, e.g. by creating unary, binary, leaf caches.
BaseElement* ElementFactory::make_leaf_concept(const Sketch_STRIPS_Problem* problem, bool goal, std::string predicate_name, unsigned position) {
    std::tuple<bool, std::string, unsigned> key = std::tuple<bool, std::string, unsigned>(goal, predicate_name, position);
    if (!m_leaf_concept_cache.exists(key)) {
        BaseElement* concept = new ConceptElement(problem, goal, predicate_name, position);
        m_leaf_concept_cache.insert(key, std::move(concept));
    }
    return m_leaf_concept_cache.get(key);
}

BaseElement* ElementFactory::make_intersect_concept(const Sketch_STRIPS_Problem* problem, bool goal, BaseElement* left, BaseElement* right) {
    std::tuple<bool, BaseElement*, BaseElement*> key = std::tuple<bool, BaseElement*, BaseElement*>(goal, left, right);
    if (!m_intersect_concept_cache.exists(key)) {
        BaseElement* concept = new IntersectElement(problem, goal, left, right);
        m_intersect_concept_cache.insert(key, std::move(concept));
    }
    return m_intersect_concept_cache.get(key);
}

BaseElement* ElementFactory::make_setminus_concept(const Sketch_STRIPS_Problem* problem, bool goal, BaseElement* left, BaseElement* right) {
    std::tuple<bool, BaseElement*, BaseElement*> key = std::tuple<bool, BaseElement*, BaseElement*>(goal, left, right);
    if (!m_setminus_concept_cache.exists(key)) {
        BaseElement* concept = new SetminusElement(problem, goal, left, right);
        m_setminus_concept_cache.insert(key, std::move(concept));
    }
    return m_setminus_concept_cache.get(key);
}

} // namespace aptk
