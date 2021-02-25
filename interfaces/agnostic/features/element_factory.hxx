#ifndef __ELEMENT_FACTORY__
#define __ELEMENT_FACTORY__

#include "boost/functional/hash.hpp"
#include <unordered_map>
#include <tuple>

namespace aptk {
class BaseElement;
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
    static ElementCache<std::tuple<bool, std::string, unsigned>, BaseElement*> m_leaf_concept_cache;
    static ElementCache<std::tuple<bool, BaseElement*, BaseElement*>, BaseElement*> m_intersect_concept_cache;
    static ElementCache<std::tuple<bool, BaseElement*, BaseElement*>, BaseElement*> m_setminus_concept_cache;
public:
    ElementFactory() = default;
    virtual ~ElementFactory() = default;

    static BaseElement* make_leaf_concept(const Sketch_STRIPS_Problem* problem, bool goal, std::string predicate_name, unsigned position);
    static BaseElement* make_intersect_concept(const Sketch_STRIPS_Problem* problem, bool goal, BaseElement* left, BaseElement* right);
    static BaseElement* make_setminus_concept(const Sketch_STRIPS_Problem* problem, bool goal, BaseElement* left, BaseElement* right);
    static BaseElement* make_extraction_concept(const Sketch_STRIPS_Problem* problem, bool goal, BaseElement* role);

    static BaseElement* make_leaf_role(const Sketch_STRIPS_Problem* problem, bool goal, std::string predicate_name);
    static BaseElement* make_intersect_role(const Sketch_STRIPS_Problem* problem, bool goal, BaseElement* left, BaseElement* right);
    static BaseElement* make_setminus_role(const Sketch_STRIPS_Problem* problem, bool goal, BaseElement* left, BaseElement* right);
};

}

#endif
