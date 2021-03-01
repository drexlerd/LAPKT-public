#ifndef __PREDICATE_UNION__
#define __PREDICATE_UNION__

#include "../predicate.hxx"

namespace aptk {


class PredicateUnionElement : public PredicateElement {
protected:
    PredicateElement* m_left;
    PredicateElement* m_right;

    virtual void compute_result(const State* state) override {
        m_result.clear();
        Predicates_Set left_set(m_left->evaluate(state).begin(), m_left->evaluate(state).end());
        Predicates_Set right_set(m_right->evaluate(state).begin(), m_right->evaluate(state).end());
        Predicates_Set result_set;
        for (Predicate p : left_set) {
            result_set.insert(p);
        }
        for (Predicate p : right_set) {
            result_set.insert(p);
        }
        m_result = Predicates(result_set.begin(), result_set.end());
    }

public:
    PredicateUnionElement(const Sketch_STRIPS_Problem* problem, bool goal, PredicateElement* left, PredicateElement* right)
    : PredicateElement(problem, goal), m_left(left), m_right(right) {
        if (goal) {
            Predicates_Set left_set(left->result().begin(), left->result().end());
            Predicates_Set right_set(right->result().begin(), right->result().end());
            Predicates_Set result_set;
            for (Predicate p : left_set) {
                result_set.insert(p);
            }
            for (Predicate p : right_set) {
                result_set.insert(p);
            }
            m_result = Predicates(result_set.begin(), result_set.end());
        }
    }
    virtual ~PredicateUnionElement() = default;
};

}

#endif
