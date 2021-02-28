#include "element_factory.hxx"
#include "binary/intersect.hxx"
#include "binary/setminus.hxx"
#include "binary/union.hxx"
#include "binary/existential_abstraction.hxx"
#include "binary/universal_abstraction.hxx"
#include "unary/extraction.hxx"
#include "derived/composition.hxx"
#include "concept.hxx"
#include "role.hxx"

namespace aptk
{

/**
 * Instantiate static members.
 */
ElementCache<std::tuple<bool, std::string, unsigned>, BaseElement*> ElementFactory::m_concept_cache = ElementCache<std::tuple<bool, std::string, unsigned>, BaseElement*>();
ElementCache<std::tuple<bool, std::string>, BaseElement*> ElementFactory::m_role_cache = ElementCache<std::tuple<bool, std::string>, BaseElement*>();
ElementCache<std::tuple<bool, BaseElement*, BaseElement*>, BaseElement*> ElementFactory::m_intersect_cache = ElementCache<std::tuple<bool, BaseElement*, BaseElement*>, BaseElement*>();
ElementCache<std::tuple<bool, BaseElement*, BaseElement*>, BaseElement*> ElementFactory::m_setminus_cache = ElementCache<std::tuple<bool, BaseElement*, BaseElement*>, BaseElement*>();
ElementCache<std::tuple<bool, BaseElement*, unsigned>, BaseElement*> ElementFactory::m_extraction_cache = ElementCache<std::tuple<bool, BaseElement*, unsigned>, BaseElement*>();
ElementCache<std::tuple<bool, BaseElement*, BaseElement*, unsigned, unsigned>, BaseElement*> ElementFactory::m_existential_abstraction_cache = ElementCache<std::tuple<bool, BaseElement*, BaseElement*, unsigned, unsigned>, BaseElement*>();
ElementCache<std::tuple<bool, BaseElement*, BaseElement*, unsigned, unsigned>, BaseElement*> ElementFactory::m_universal_abstraction_cache = ElementCache<std::tuple<bool, BaseElement*, BaseElement*, unsigned, unsigned>, BaseElement*>();
ElementCache<std::tuple<bool, std::string, std::string>, BaseElement*> ElementFactory::m_composition_cache = ElementCache<std::tuple<bool, std::string, std::string>, BaseElement*>();

ElementCache<std::string, BaseElement*> ElementFactory::m_custom_cache = ElementCache<std::string, BaseElement*>();

// TODO: remove duplicate code, e.g. by creating unary, binary caches.
BaseElement* ElementFactory::make_concept(const Sketch_STRIPS_Problem* problem, bool goal, std::string predicate_name, unsigned position) {
    std::tuple<bool, std::string, unsigned> key = std::tuple<bool, std::string, unsigned>(goal, predicate_name, position);
    if (!m_concept_cache.exists(key)) {
        BaseElement* concept = new ConceptElement(problem, goal, predicate_name, position);
        m_concept_cache.insert(key, std::move(concept));
    }
    return m_concept_cache.get(key);
}

BaseElement* ElementFactory::make_role(const Sketch_STRIPS_Problem* problem, bool goal, std::string predicate_name) {
    std::tuple<bool, std::string> key = std::tuple<bool, std::string>(goal, predicate_name);
    if (!m_role_cache.exists(key)) {
        BaseElement* concept = new RoleElement(problem, goal, predicate_name);
        m_role_cache.insert(key, std::move(concept));
    }
    return m_role_cache.get(key);
}

BaseElement* ElementFactory::make_intersect(const Sketch_STRIPS_Problem* problem, bool goal, BaseElement* left, BaseElement* right) {
    std::tuple<bool, BaseElement*, BaseElement*> key = std::tuple<bool, BaseElement*, BaseElement*>(goal, left, right);
    if (!m_intersect_cache.exists(key)) {
        BaseElement* element = new IntersectElement(problem, goal, left, right);
        m_intersect_cache.insert(key, std::move(element));
    }
    return m_intersect_cache.get(key);
}

BaseElement* ElementFactory::make_setminus(const Sketch_STRIPS_Problem* problem, bool goal, BaseElement* left, BaseElement* right) {
    std::tuple<bool, BaseElement*, BaseElement*> key = std::tuple<bool, BaseElement*, BaseElement*>(goal, left, right);
    if (!m_setminus_cache.exists(key)) {
        BaseElement* element = new SetminusElement(problem, goal, left, right);
        m_setminus_cache.insert(key, std::move(element));
    }
    return m_setminus_cache.get(key);
}

BaseElement* ElementFactory::make_extraction(const Sketch_STRIPS_Problem* problem, bool goal, BaseElement* role, unsigned position) {
    std::tuple<bool, BaseElement*, unsigned> key = std::tuple<bool, BaseElement*, unsigned>(goal, role, position);
    if (!m_extraction_cache.exists(key)) {
        BaseElement* element = new ExtractionElement(problem, goal, role, position);
        m_extraction_cache.insert(key, std::move(element));
    }
    return m_extraction_cache.get(key);
}

BaseElement* ElementFactory::make_existential_abstraction(const Sketch_STRIPS_Problem* problem, bool goal, BaseElement* role, BaseElement* concept, unsigned a, unsigned b) {
    std::tuple<bool, BaseElement*, BaseElement*, unsigned, unsigned> key = std::tuple<bool, BaseElement*, BaseElement*, unsigned, unsigned>(goal, role, concept, a, b);
    if (!m_existential_abstraction_cache.exists(key)) {
        BaseElement* element = new ExistentialAbstractionElement(problem, goal, role, concept, a, b);
        m_existential_abstraction_cache.insert(key, std::move(element));
    }
    return m_existential_abstraction_cache.get(key);
}

BaseElement* ElementFactory::make_universal_abstraction(const Sketch_STRIPS_Problem* problem, bool goal, BaseElement* role, BaseElement* concept, unsigned a, unsigned b) {
    std::tuple<bool, BaseElement*, BaseElement*, unsigned, unsigned> key = std::tuple<bool, BaseElement*, BaseElement*, unsigned, unsigned>(goal, role, concept, a, b);
    if (!m_universal_abstraction_cache.exists(key)) {
        BaseElement* element = new UniversalAbstractionElement(problem, goal, role, concept, a, b);
        m_universal_abstraction_cache.insert(key, std::move(element));
    }
    return m_universal_abstraction_cache.get(key);
}

BaseElement* ElementFactory::make_composition(const Sketch_STRIPS_Problem* problem, bool goal, std::string left_predicate_name, std::string right_predicate_name) {
    std::tuple<bool, std::string, std::string> key = std::tuple<bool, std::string, std::string>(goal, left_predicate_name, right_predicate_name);
    if (!m_composition_cache.exists(key)) {
        BaseElement* element = new CompositionElement(problem, goal, left_predicate_name, right_predicate_name);
        m_composition_cache.insert(key, std::move(element));
    }
    return m_composition_cache.get(key);
}

BaseElement* ElementFactory::add_custom(std::string name, BaseElement* custom) {
    if (m_custom_cache.exists(name)) {
        std::cout << "ElementFactory::add_custom: key already exists!\n";
        exit(1);
    }
    m_custom_cache.insert(name, std::move(custom));
    return m_custom_cache.get(name);
}

BaseElement* ElementFactory::get_custom(std::string name) {
    if (!m_custom_cache.exists(name)) {
        std::cout << "ElementFactory::get_custom: key does not exists!\n";
        exit(1);
    }
    return m_custom_cache.get(name);
}


} // namespace aptk
