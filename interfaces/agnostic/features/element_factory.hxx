#ifndef __ELEMENT_FACTORY__
#define __ELEMENT_FACTORY__

#include "boost/functional/hash.hpp"
#include <unordered_map>
#include <tuple>
#include <iostream>
#include "predicates/extraction.hxx"
#include "predicates/intersection.hxx"
#include "predicates/setminus.hxx"
#include "predicates/union.hxx"
#include "concepts/extraction.hxx"
#include "concepts/intersection.hxx"
#include "concepts/setminus.hxx"
#include "concepts/union.hxx"
#include "concepts/existential_abstraction.hxx"
#include "concepts/universal_abstraction.hxx"
#include "roles/extraction.hxx"
#include "roles/intersection.hxx"
#include "roles/setminus.hxx"
#include "roles/union.hxx"


namespace aptk {
class BaseElement;
class ConceptElement;
class RoleElement;
class PredicateElement;
class Sketch_STRIPS_Problem;

/**
 * A simple cache.
 */
template<typename Key_T, typename Value_T>
class ElementCache {
protected:
    struct KeyHash {
        std::size_t operator()(const Key_T &key) const {
            return boost::hash_value(key);
        }
    };
public:
    ElementCache() = default;
    virtual ~ElementCache() = default;

    bool exists(const Key_T &key) const {
        if (m_cache.find(key) != m_cache.end()) {
            return true;
        }
        return false;
    }

    Value_T& get(const Key_T &key) {
        return m_cache.find(key)->second;
    }

    void insert(const Key_T &key, Value_T &&value) {
        m_cache.insert(make_pair(key, std::move(value)));
    }
private:
    std::unordered_map<Key_T, Value_T, KeyHash> m_cache;
};

/**
 * Factory to generate concepts
 */
class ElementFactory {
private:
    // predicates
    static ElementCache<std::tuple<bool, std::string>, PredicateElement*> m_predicate_extraction_cache;
    static ElementCache<std::tuple<bool, PredicateElement*, PredicateElement*>, PredicateElement*> m_predicate_intersection_cache;
    static ElementCache<std::tuple<bool, PredicateElement*, PredicateElement*>, PredicateElement*> m_predicate_setminus_cache;
    static ElementCache<std::tuple<bool, PredicateElement*, PredicateElement*>, PredicateElement*> m_predicate_union_cache;
    // concepts
    static ElementCache<std::tuple<bool, RoleElement*, unsigned>, ConceptElement*> m_concept_extraction_cache;
    static ElementCache<std::tuple<bool, ConceptElement*, ConceptElement*>, ConceptElement*> m_concept_intersection_cache;
    static ElementCache<std::tuple<bool, ConceptElement*, ConceptElement*>, ConceptElement*> m_concept_setminus_cache;
    static ElementCache<std::tuple<bool, ConceptElement*, ConceptElement*>, ConceptElement*> m_concept_union_cache;
    static ElementCache<std::tuple<bool, RoleElement*, ConceptElement*>, ConceptElement*> m_concept_existential_abstraction_cache;
    static ElementCache<std::tuple<bool, RoleElement*, ConceptElement*>, ConceptElement*> m_concept_universal_abstraction_cache;
    // roles
    static ElementCache<std::tuple<bool, PredicateElement*, unsigned, unsigned>, RoleElement*> m_role_extraction_cache;
    static ElementCache<std::tuple<bool, RoleElement*, RoleElement*>, RoleElement*> m_role_intersection_cache;
    static ElementCache<std::tuple<bool, RoleElement*, RoleElement*>, RoleElement*> m_role_setminus_cache;
    static ElementCache<std::tuple<bool, RoleElement*, RoleElement*>, RoleElement*> m_role_union_cache;
    // custom
    static ElementCache<std::string, BaseElement*> m_custom_cache;
public:
    ElementFactory() = default;
    virtual ~ElementFactory() = default;

    static BaseElement* make_predicate_extraction(const Sketch_STRIPS_Problem* problem, bool goal, std::string predicate_name);
    static BaseElement* make_predicate_intersection(const Sketch_STRIPS_Problem* problem, bool goal, PredicateElement* left, PredicateElement* right);
    static BaseElement* make_predicate_setminus(const Sketch_STRIPS_Problem* problem, bool goal, PredicateElement* left, PredicateElement* right);
    static BaseElement* make_predicate_union(const Sketch_STRIPS_Problem* problem, bool goal, PredicateElement* left, PredicateElement* right);

    static BaseElement* make_concept_extraction(const Sketch_STRIPS_Problem* problem, bool goal, RoleElement* role, unsigned position);
    static BaseElement* make_concept_intersection(const Sketch_STRIPS_Problem* problem, bool goal, ConceptElement* left, ConceptElement* right);
    static BaseElement* make_concept_setminus(const Sketch_STRIPS_Problem* problem, bool goal, ConceptElement* left, ConceptElement* right);
    static BaseElement* make_concept_union(const Sketch_STRIPS_Problem* problem, bool goal, ConceptElement* left, ConceptElement* right);
    static BaseElement* make_concept_existential_abstraction(const Sketch_STRIPS_Problem* problem, bool goal, RoleElement* role, ConceptElement* concept);
    static BaseElement* make_concept_universal_abstraction(const Sketch_STRIPS_Problem* problem, bool goal, RoleElement* role, ConceptElement* concept);

    static BaseElement* make_role_extraction(const Sketch_STRIPS_Problem* problem, bool goal, PredicateElement* predicate, unsigned a, unsigned b);
    static BaseElement* make_role_intersection(const Sketch_STRIPS_Problem* problem, bool goal, RoleElement* left, RoleElement* right);
    static BaseElement* make_role_setminus(const Sketch_STRIPS_Problem* problem, bool goal, RoleElement* left, RoleElement* right);
    static BaseElement* make_role_union(const Sketch_STRIPS_Problem* problem, bool goal, RoleElement* left, RoleElement* right);

    static BaseElement* add_custom(std::string name, BaseElement* custom);
    static BaseElement* get_custom(std::string name);
};

ElementCache<std::tuple<bool, std::string>, PredicateElement*> ElementFactory::m_predicate_extraction_cache = ElementCache<std::tuple<bool, std::string>, PredicateElement*>();
ElementCache<std::tuple<bool, PredicateElement*, PredicateElement*>, PredicateElement*> ElementFactory::m_predicate_intersection_cache = ElementCache<std::tuple<bool, PredicateElement*, PredicateElement*>, PredicateElement*>();
ElementCache<std::tuple<bool, PredicateElement*, PredicateElement*>, PredicateElement*> ElementFactory::m_predicate_setminus_cache = ElementCache<std::tuple<bool, PredicateElement*, PredicateElement*>, PredicateElement*>();
ElementCache<std::tuple<bool, PredicateElement*, PredicateElement*>, PredicateElement*> ElementFactory::m_predicate_union_cache = ElementCache<std::tuple<bool, PredicateElement*, PredicateElement*>, PredicateElement*>();
// concepts
ElementCache<std::tuple<bool, RoleElement*, unsigned>, ConceptElement*> ElementFactory::m_concept_extraction_cache = ElementCache<std::tuple<bool, RoleElement*, unsigned>, ConceptElement*>();
ElementCache<std::tuple<bool, ConceptElement*, ConceptElement*>, ConceptElement*> ElementFactory::m_concept_intersection_cache = ElementCache<std::tuple<bool, ConceptElement*, ConceptElement*>, ConceptElement*>();
ElementCache<std::tuple<bool, ConceptElement*, ConceptElement*>, ConceptElement*> ElementFactory::m_concept_setminus_cache = ElementCache<std::tuple<bool, ConceptElement*, ConceptElement*>, ConceptElement*>();
ElementCache<std::tuple<bool, ConceptElement*, ConceptElement*>, ConceptElement*> ElementFactory::m_concept_union_cache = ElementCache<std::tuple<bool, ConceptElement*, ConceptElement*>, ConceptElement*>();
ElementCache<std::tuple<bool, RoleElement*, ConceptElement*>, ConceptElement*> ElementFactory::m_concept_existential_abstraction_cache = ElementCache<std::tuple<bool, RoleElement*, ConceptElement*>, ConceptElement*>();
ElementCache<std::tuple<bool, RoleElement*, ConceptElement*>, ConceptElement*> ElementFactory::m_concept_universal_abstraction_cache = ElementCache<std::tuple<bool, RoleElement*, ConceptElement*>, ConceptElement*>();
// roles
ElementCache<std::tuple<bool, PredicateElement*, unsigned, unsigned>, RoleElement*> ElementFactory::m_role_extraction_cache = ElementCache<std::tuple<bool, PredicateElement*, unsigned, unsigned>, RoleElement*>();
ElementCache<std::tuple<bool, RoleElement*, RoleElement*>, RoleElement*> ElementFactory::m_role_intersection_cache = ElementCache<std::tuple<bool, RoleElement*, RoleElement*>, RoleElement*>();
ElementCache<std::tuple<bool, RoleElement*, RoleElement*>, RoleElement*> ElementFactory::m_role_setminus_cache = ElementCache<std::tuple<bool, RoleElement*, RoleElement*>, RoleElement*>();
ElementCache<std::tuple<bool, RoleElement*, RoleElement*>, RoleElement*> ElementFactory::m_role_union_cache = ElementCache<std::tuple<bool, RoleElement*, RoleElement*>, RoleElement*>();
// custom
ElementCache<std::string, BaseElement*> m_custom_cache;


BaseElement* ElementFactory::make_predicate_extraction(const Sketch_STRIPS_Problem* problem, bool goal, std::string predicate_name) {
    std::tuple<bool, std::string> key = std::tuple<bool, std::string>(goal, predicate_name);
    if (!m_predicate_extraction_cache.exists(key)) {
        PredicateElement* predicate = new PredicateExtractionElement(problem, goal, predicate_name);
        m_predicate_extraction_cache.insert(key, std::move(predicate));
    }
    return m_predicate_extraction_cache.get(key);
}

BaseElement* ElementFactory::make_predicate_intersection(const Sketch_STRIPS_Problem* problem, bool goal, PredicateElement* left, PredicateElement* right) {
    std::tuple<bool, PredicateElement*, PredicateElement*> key = std::tuple<bool, PredicateElement*, PredicateElement*>(goal, left, right);
    if (!m_predicate_intersection_cache.exists(key)) {
        PredicateElement* predicate = new PredicateIntersectionElement(problem, goal, left, right);
        m_predicate_intersection_cache.insert(key, std::move(predicate));
    }
    return m_predicate_intersection_cache.get(key);
}

BaseElement* ElementFactory::make_predicate_setminus(const Sketch_STRIPS_Problem* problem, bool goal, PredicateElement* left, PredicateElement* right) {
    std::tuple<bool, PredicateElement*, PredicateElement*> key = std::tuple<bool, PredicateElement*, PredicateElement*>(goal, left, right);
    if (!m_predicate_setminus_cache.exists(key)) {
        PredicateElement* predicate = new PredicateSetminusElement(problem, goal, left, right);
        m_predicate_setminus_cache.insert(key, std::move(predicate));
    }
    return m_predicate_setminus_cache.get(key);
}

BaseElement* ElementFactory::make_predicate_union(const Sketch_STRIPS_Problem* problem, bool goal, PredicateElement* left, PredicateElement* right) {
    std::tuple<bool, PredicateElement*, PredicateElement*> key = std::tuple<bool, PredicateElement*, PredicateElement*>(goal, left, right);
    if (!m_predicate_union_cache.exists(key)) {
        PredicateElement* predicate = new PredicateUnionElement(problem, goal, left, right);
        m_predicate_union_cache.insert(key, std::move(predicate));
    }
    return m_predicate_union_cache.get(key);
}

BaseElement* ElementFactory::make_concept_extraction(const Sketch_STRIPS_Problem* problem, bool goal, RoleElement* role, unsigned position) {
    std::tuple<bool, RoleElement*, unsigned> key = std::tuple<bool, RoleElement*, unsigned>(goal, role, position);
    if (!m_concept_extraction_cache.exists(key)) {
        ConceptElement* concept = new ConceptExtractionElement(problem, goal, role, position);
        m_concept_extraction_cache.insert(key, std::move(concept));
    }
    return m_concept_extraction_cache.get(key);
}

BaseElement* ElementFactory::make_concept_intersection(const Sketch_STRIPS_Problem* problem, bool goal, ConceptElement* left, ConceptElement* right) {
    std::tuple<bool, ConceptElement*, ConceptElement*> key = std::tuple<bool, ConceptElement*, ConceptElement*>(goal, left, right);
    if (!m_concept_intersection_cache.exists(key)) {
        ConceptElement* concept = new ConceptIntersectionElement(problem, goal, left, right);
        m_concept_intersection_cache.insert(key, std::move(concept));
    }
    return m_concept_intersection_cache.get(key);
}

BaseElement* ElementFactory::make_concept_setminus(const Sketch_STRIPS_Problem* problem, bool goal, ConceptElement* left, ConceptElement* right) {
    std::tuple<bool, ConceptElement*, ConceptElement*> key = std::tuple<bool, ConceptElement*, ConceptElement*>(goal, left, right);
    if (!m_concept_setminus_cache.exists(key)) {
        ConceptElement* concept = new ConceptSetminusElement(problem, goal, left, right);
        m_concept_setminus_cache.insert(key, std::move(concept));
    }
    return m_concept_setminus_cache.get(key);
}
BaseElement* ElementFactory::make_concept_union(const Sketch_STRIPS_Problem* problem, bool goal, ConceptElement* left, ConceptElement* right) {
    std::tuple<bool, ConceptElement*, ConceptElement*> key = std::tuple<bool, ConceptElement*, ConceptElement*>(goal, left, right);
    if (!m_concept_union_cache.exists(key)) {
        ConceptElement* concept = new ConceptUnionElement(problem, goal, left, right);
        m_concept_union_cache.insert(key, std::move(concept));
    }
    return m_concept_union_cache.get(key);
}

BaseElement* ElementFactory::make_concept_existential_abstraction(const Sketch_STRIPS_Problem* problem, bool goal, RoleElement* role, ConceptElement* concept) {
    std::tuple<bool, RoleElement*, ConceptElement*> key = std::tuple<bool, RoleElement*, ConceptElement*>(goal, role, concept);
    if (!m_concept_existential_abstraction_cache.exists(key)) {
        ConceptElement* concept = new ExistentialAbstractionElement(problem, goal, role, concept);
        m_concept_existential_abstraction_cache.insert(key, std::move(concept));
    }
    return m_concept_existential_abstraction_cache.get(key);
}

BaseElement* ElementFactory::make_concept_universal_abstraction(const Sketch_STRIPS_Problem* problem, bool goal, RoleElement* role, ConceptElement* concept) {
    std::tuple<bool, RoleElement*, ConceptElement*> key = std::tuple<bool, RoleElement*, ConceptElement*>(goal, role, concept);
    if (!m_concept_universal_abstraction_cache.exists(key)) {
        ConceptElement* concept = new UniversalAbstractionElement(problem, goal, role, concept);
        m_concept_universal_abstraction_cache.insert(key, std::move(concept));
    }
    return m_concept_universal_abstraction_cache.get(key);
}

BaseElement* ElementFactory::make_role_extraction(const Sketch_STRIPS_Problem* problem, bool goal, PredicateElement* predicate, unsigned a, unsigned b) {
    std::tuple<bool, PredicateElement*, unsigned, unsigned> key = std::tuple<bool, PredicateElement*, unsigned, unsigned>(goal, predicate, a, b);
    if (!m_role_extraction_cache.exists(key)) {
        RoleElement* concept = new RoleExtractionElement(problem, goal, predicate, a, b);
        m_role_extraction_cache.insert(key, std::move(concept));
    }
    return m_role_extraction_cache.get(key);
}

BaseElement* ElementFactory::make_role_intersection(const Sketch_STRIPS_Problem* problem, bool goal, RoleElement* left, RoleElement* right) {
    std::tuple<bool, RoleElement*, RoleElement*> key = std::tuple<bool, RoleElement*, RoleElement*>(goal, left, right);
    if (!m_role_intersection_cache.exists(key)) {
        RoleElement* predicate = new RoleIntersectionElement(problem, goal, left, right);
        m_role_intersection_cache.insert(key, std::move(predicate));
    }
    return m_role_intersection_cache.get(key);
}

BaseElement* ElementFactory::make_role_setminus(const Sketch_STRIPS_Problem* problem, bool goal, RoleElement* left, RoleElement* right) {
    std::tuple<bool, RoleElement*, RoleElement*> key = std::tuple<bool, RoleElement*, RoleElement*>(goal, left, right);
    if (!m_role_setminus_cache.exists(key)) {
        RoleElement* predicate = new RoleSetminusElement(problem, goal, left, right);
        m_role_setminus_cache.insert(key, std::move(predicate));
    }
    return m_role_setminus_cache.get(key);
}

BaseElement* ElementFactory::make_role_union(const Sketch_STRIPS_Problem* problem, bool goal, RoleElement* left, RoleElement* right) {
    std::tuple<bool, RoleElement*, RoleElement*> key = std::tuple<bool, RoleElement*, RoleElement*>(goal, left, right);
    if (!m_role_union_cache.exists(key)) {
        RoleElement* predicate = new RoleUnionElement(problem, goal, left, right);
        m_role_union_cache.insert(key, std::move(predicate));
    }
    return m_role_union_cache.get(key);
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

}

#endif
