#ifndef __ROLE__
#define __ROLE__

#include "element.hxx"
#include "boost/functional/hash.hpp"

namespace aptk {

using Role = std::pair<unsigned, unsigned>;
using Roles = std::vector<Role>;
struct RoleHash {
    std::size_t operator()(const Role &role) const {
        return boost::hash_value(role);
    }
};
using Roles_Set = std::unordered_set<Role, RoleHash>;

/**
 * The underlying result represents a set of predicates.
 */
class RoleElement : public BaseElement {
protected:
    Roles m_result;

public:
    RoleElement(const Sketch_STRIPS_Problem* problem, bool goal) : BaseElement(problem, goal) { }
    virtual ~RoleElement() = default;

    /**
     * A RoleElement returns a reference to Roles.
     */
    virtual const Roles& evaluate(const State* state) {
        if (is_uninitialized(state)) {
            set_initialized(state);
            compute_result(state);
        }
        return m_result;
    }

    /**
     * Getters
     */
    const Roles& result() const { return m_result; }

    virtual void print() const override {
    }
};

}

#endif
