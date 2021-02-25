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
ConceptCache<std::tuple<bool, std::string, unsigned>, BaseElement*> ConceptFactory::m_leaf_concept_cache = ConceptCache<std::tuple<bool, std::string, unsigned>, BaseElement*>();
ConceptCache<std::tuple<bool, BaseElement*, BaseElement*>, BaseElement*> ConceptFactory::m_intersect_concept_cache = ConceptCache<std::tuple<bool, BaseElement*, BaseElement*>, BaseElement*>();
ConceptCache<std::tuple<bool, BaseElement*, BaseElement*>, BaseElement*> ConceptFactory::m_setminus_concept_cache = ConceptCache<std::tuple<bool, BaseElement*, BaseElement*>, BaseElement*>();

// TODO: remove duplicate code, e.g. by creating unary, binary, leaf caches.
BaseElement* ConceptFactory::make_leaf_concept(const Sketch_STRIPS_Problem* problem, bool goal, std::string predicate_name, unsigned position) {
    std::tuple<bool, std::string, unsigned> key = std::tuple<bool, std::string, unsigned>(goal, predicate_name, position);
    if (!m_leaf_concept_cache.exists(key)) {
        BaseElement* concept = new ConceptElement(problem, goal, predicate_name, position);
        m_leaf_concept_cache.insert(key, std::move(concept));
    }
    return m_leaf_concept_cache.get(key);
}

BaseElement* ConceptFactory::make_intersect_concept(const Sketch_STRIPS_Problem* problem, bool goal, BaseElement* left, BaseElement* right) {
    std::tuple<bool, BaseElement*, BaseElement*> key = std::tuple<bool, BaseElement*, BaseElement*>(goal, left, right);
    if (!m_intersect_concept_cache.exists(key)) {
        BaseElement* concept = new IntersectElement(problem, goal, left, right);
        m_intersect_concept_cache.insert(key, std::move(concept));
    }
    return m_intersect_concept_cache.get(key);
}

BaseElement* ConceptFactory::make_setminus_concept(const Sketch_STRIPS_Problem* problem, bool goal, BaseElement* left, BaseElement* right) {
    std::tuple<bool, BaseElement*, BaseElement*> key = std::tuple<bool, BaseElement*, BaseElement*>(goal, left, right);
    if (!m_setminus_concept_cache.exists(key)) {
        BaseElement* concept = new SetminusElement(problem, goal, left, right);
        m_setminus_concept_cache.insert(key, std::move(concept));
    }
    return m_setminus_concept_cache.get(key);
}

} // namespace aptk
