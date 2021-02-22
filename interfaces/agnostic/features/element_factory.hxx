#ifndef __ELEMENT_FACTORY__
#define __ELEMENT_FACTORY__

#include "boost/functional/hash.hpp"
#include "element.hxx"
#include <unordered_map>
#include <tuple>

namespace aptk {
class BaseConceptElement;
class Sketch_STRIPS_Problem;

/**
 * A simple cache.
 */
template<typename Key_T, typename Value_T>
class ConceptCache {
public:
    ConceptCache() = default;
    virtual ~ConceptCache() = default;

    struct KeyHash {
        std::size_t operator()(const Key_T &key) const {
            return boost::hash_value(key);
        }
    };

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
class ConceptFactory {
private:
    static ConceptCache<std::tuple<bool, std::string, unsigned>, BaseConceptElement*> m_leaf_concept_cache;
    static ConceptCache<std::tuple<bool, BaseConceptElement*, BaseConceptElement*>, BaseConceptElement*> m_intersect_cache;
    static ConceptCache<std::tuple<bool, BaseConceptElement*, BaseConceptElement*>, BaseConceptElement*> m_setminus_cache;
public:
    ConceptFactory() = default;
    virtual ~ConceptFactory() = default;

    static BaseConceptElement* make_leaf_concept(const Sketch_STRIPS_Problem* problem, bool goal, std::string predicate_name, unsigned position);
    static BaseConceptElement* make_intersect(const Sketch_STRIPS_Problem* problem, bool goal, BaseConceptElement* left, BaseConceptElement* right);
    static BaseConceptElement* make_setminus(const Sketch_STRIPS_Problem* problem, bool goal, BaseConceptElement* left, BaseConceptElement* right);
};

/**
 * Instantiate static members.
 */
ConceptCache<std::tuple<bool, std::string, unsigned>, BaseConceptElement*> ConceptFactory::m_leaf_concept_cache = ConceptCache<std::tuple<bool, std::string, unsigned>, BaseConceptElement*>();
ConceptCache<std::tuple<bool, BaseConceptElement*, BaseConceptElement*>, BaseConceptElement*> ConceptFactory::m_intersect_cache = ConceptCache<std::tuple<bool, BaseConceptElement*, BaseConceptElement*>, BaseConceptElement*>();
ConceptCache<std::tuple<bool, BaseConceptElement*, BaseConceptElement*>, BaseConceptElement*> ConceptFactory::m_setminus_cache = ConceptCache<std::tuple<bool, BaseConceptElement*, BaseConceptElement*>, BaseConceptElement*>();

BaseConceptElement* ConceptFactory::make_leaf_concept(const Sketch_STRIPS_Problem* problem, bool goal, std::string predicate_name, unsigned position) {
    std::tuple<bool, std::string, unsigned> key = std::tuple<bool, std::string, unsigned>(goal, predicate_name, position);
    if (!m_leaf_concept_cache.exists(key)) {
        BaseConceptElement* concept = new LeafConceptElement(problem, goal, predicate_name, position);
        m_leaf_concept_cache.insert(key, std::move(concept));
    }
    return m_leaf_concept_cache.get(key);
}

BaseConceptElement* ConceptFactory::make_intersect(const Sketch_STRIPS_Problem* problem, bool goal, BaseConceptElement* left, BaseConceptElement* right) {
    std::tuple<bool, BaseConceptElement*, BaseConceptElement*> key = std::tuple<bool, BaseConceptElement*, BaseConceptElement*>(goal, left, right);
    if (!m_intersect_cache.exists(key)) {
        BaseConceptElement* concept = new IntersectConceptElement(problem, goal, left, right);
        m_intersect_cache.insert(key, std::move(concept));
    }
    return m_intersect_cache.get(key);
}

BaseConceptElement* ConceptFactory::make_setminus(const Sketch_STRIPS_Problem* problem, bool goal, BaseConceptElement* left, BaseConceptElement* right) {
    std::tuple<bool, BaseConceptElement*, BaseConceptElement*> key = std::tuple<bool, BaseConceptElement*, BaseConceptElement*>(goal, left, right);
    if (!m_setminus_cache.exists(key)) {
        BaseConceptElement* concept = new SetminusConceptElement(problem, goal, left, right);
        m_setminus_cache.insert(key, std::move(concept));
    }
    return m_setminus_cache.get(key);
}

}

#endif
