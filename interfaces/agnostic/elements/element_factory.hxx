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
#include "concepts/role_value_equality.hxx"
#include "concepts/role_value_subset.hxx"
#include "roles/extraction.hxx"
#include "roles/intersection.hxx"
#include "roles/setminus.hxx"
#include "roles/union.hxx"
#include "roles/composition.hxx"


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
    static ElementCache<std::tuple<bool, RoleElement*, unsigned>, ConceptElement*> m_concept_role_extraction_cache;
    static ElementCache<std::tuple<bool, PredicateElement*, unsigned>, ConceptElement*> m_concept_predicate_extraction_cache;
    static ElementCache<std::tuple<bool, ConceptElement*, ConceptElement*>, ConceptElement*> m_concept_intersection_cache;
    static ElementCache<std::tuple<bool, ConceptElement*, ConceptElement*>, ConceptElement*> m_concept_setminus_cache;
    static ElementCache<std::tuple<bool, ConceptElement*, ConceptElement*>, ConceptElement*> m_concept_union_cache;
    static ElementCache<std::tuple<bool, RoleElement*, ConceptElement*>, ConceptElement*> m_concept_existential_abstraction_cache;
    static ElementCache<std::tuple<bool, RoleElement*, ConceptElement*>, ConceptElement*> m_concept_universal_abstraction_cache;
    static ElementCache<std::tuple<bool, RoleElement*, RoleElement*>, ConceptElement*> m_concept_role_value_equality_cache;
    static ElementCache<std::tuple<bool, RoleElement*, RoleElement*>, ConceptElement*> m_concept_role_value_subset_cache;

    // roles
    static ElementCache<std::tuple<bool, PredicateElement*, unsigned, unsigned>, RoleElement*> m_role_extraction_cache;
    static ElementCache<std::tuple<bool, RoleElement*, RoleElement*>, RoleElement*> m_role_intersection_cache;
    static ElementCache<std::tuple<bool, RoleElement*, RoleElement*>, RoleElement*> m_role_setminus_cache;
    static ElementCache<std::tuple<bool, RoleElement*, RoleElement*>, RoleElement*> m_role_union_cache;
    static ElementCache<std::tuple<bool, RoleElement*, RoleElement*>, RoleElement*> m_role_composition_cache;

    // custom
    static ElementCache<std::string, PredicateElement*> m_predicate_custom_cache;
    static ElementCache<std::string, ConceptElement*> m_concept_custom_cache;
    static ElementCache<std::string, RoleElement*> m_role_custom_cache;
public:
    ElementFactory() = default;
    virtual ~ElementFactory() = default;

    static PredicateElement* make_predicate_extraction(const Sketch_STRIPS_Problem* problem, bool goal, std::string predicate_name);
    static PredicateElement* make_predicate_intersection(const Sketch_STRIPS_Problem* problem, bool goal, PredicateElement* left, PredicateElement* right);
    static PredicateElement* make_predicate_setminus(const Sketch_STRIPS_Problem* problem, bool goal, PredicateElement* left, PredicateElement* right);
    static PredicateElement* make_predicate_union(const Sketch_STRIPS_Problem* problem, bool goal, PredicateElement* left, PredicateElement* right);

    static ConceptElement* make_concept_extraction(const Sketch_STRIPS_Problem* problem, bool goal, RoleElement* role, unsigned position);
    static ConceptElement* make_concept_extraction(const Sketch_STRIPS_Problem* problem, bool goal, PredicateElement* predicate, unsigned position);
    static ConceptElement* make_concept_intersection(const Sketch_STRIPS_Problem* problem, bool goal, ConceptElement* left, ConceptElement* right);
    static ConceptElement* make_concept_setminus(const Sketch_STRIPS_Problem* problem, bool goal, ConceptElement* left, ConceptElement* right);
    static ConceptElement* make_concept_union(const Sketch_STRIPS_Problem* problem, bool goal, ConceptElement* left, ConceptElement* right);
    static ConceptElement* make_concept_existential_abstraction(const Sketch_STRIPS_Problem* problem, bool goal, RoleElement* role, ConceptElement* concept);
    static ConceptElement* make_concept_universal_abstraction(const Sketch_STRIPS_Problem* problem, bool goal, RoleElement* role, ConceptElement* concept);
    static ConceptElement* make_concept_role_value_equality(const Sketch_STRIPS_Problem* problem, bool goal, RoleElement* left, RoleElement* right);
    static ConceptElement* make_concept_role_value_subset(const Sketch_STRIPS_Problem* problem, bool goal, RoleElement* left, RoleElement* right);

    static RoleElement* make_role_extraction(const Sketch_STRIPS_Problem* problem, bool goal, PredicateElement* predicate, unsigned a, unsigned b);
    static RoleElement* make_role_intersection(const Sketch_STRIPS_Problem* problem, bool goal, RoleElement* left, RoleElement* right);
    static RoleElement* make_role_setminus(const Sketch_STRIPS_Problem* problem, bool goal, RoleElement* left, RoleElement* right);
    static RoleElement* make_role_union(const Sketch_STRIPS_Problem* problem, bool goal, RoleElement* left, RoleElement* right);
    static RoleElement* make_role_composition(const Sketch_STRIPS_Problem* problem, bool goal, RoleElement* left, RoleElement* right);

    static PredicateElement* add_predicate_custom(std::string name, PredicateElement* custom);
    static PredicateElement* get_predicate_custom(std::string name);
    static ConceptElement* add_concept_custom(std::string name, ConceptElement* custom);
    static ConceptElement* get_concept_custom(std::string name);
    static RoleElement* add_role_custom(std::string name, RoleElement* custom);
    static RoleElement* get_role_custom(std::string name);
};

}

#endif
