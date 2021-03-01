#ifndef __PREDICATE_SETMINUS__
#define __PREDICATE_SETMINUS__

#include "../predicate.hxx"

namespace aptk {


class PredicateSetminusElement : public PredicateElement {
protected:
    PredicateElement* m_left;
    PredicateElement* m_right;

    virtual void compute_result(const State* state) override {
        m_result.clear();
        Predicates_Set left_set(m_left->evaluate(state).begin(), m_left->evaluate(state).end());
        Predicates_Set right_set(m_right->evaluate(state).begin(), m_right->evaluate(state).end());
        for (Predicate p : left_set) {
            if (right_set.find(p) == right_set.end()) {
                m_result.push_back(p);
            }
        }
    }

public:
    PredicateSetminusElement(const Sketch_STRIPS_Problem* problem, bool goal, PredicateElement* left, PredicateElement* right)
    : PredicateElement(problem, goal), m_left(left), m_right(right) {
        if (goal) {
            Predicates_Set left_set(left->result().begin(), left->result().end());
            Predicates_Set right_set(right->result().begin(), right->result().end());
            for (Predicate p : left_set) {
                if (right_set.find(p) == right_set.end()) {
                    m_result.push_back(p);
                }
            }
        }
    }
    virtual ~PredicateSetminusElement() = default;
};

}

#endif
