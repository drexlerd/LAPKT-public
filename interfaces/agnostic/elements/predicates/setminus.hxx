#ifndef __PREDICATE_SETMINUS__
#define __PREDICATE_SETMINUS__

#include "../predicate.hxx"

namespace aptk {


class PredicateSetminusElement : public PredicateElement {
protected:
    PredicateElement* m_left;
    PredicateElement* m_right;

    virtual void compute_result(const Predicates& left_result, const Predicates& right_result) {
        m_result.clear();
        Predicates_Set left_set(left_result.begin(), left_result.end());
        Predicates_Set right_set(right_result.begin(), right_result.end());
        for (Predicate p : left_set) {
            if (right_set.find(p) == right_set.end()) {
                m_result.push_back(p);
            }
        }
    }

    virtual void compute_result(const State* state) override {
        m_result.clear();
        compute_result(m_left->evaluate(state), m_right->evaluate(state));
    }

public:
    PredicateSetminusElement(const Sketch_STRIPS_Problem* problem, bool goal, PredicateElement* left, PredicateElement* right)
    : PredicateElement(problem, goal), m_left(left), m_right(right) {
        if (goal) {
            compute_result(m_left->result(), m_right->result());
        }
    }
    virtual ~PredicateSetminusElement() = default;
};

}

#endif
