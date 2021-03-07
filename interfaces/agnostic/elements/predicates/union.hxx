#ifndef __PREDICATE_UNION__
#define __PREDICATE_UNION__

#include "../predicate.hxx"

namespace aptk {


class PredicateUnionElement : public PredicateElement {
protected:
    PredicateElement* m_left;
    PredicateElement* m_right;

    virtual void compute_result(const Predicates& left_result, const Predicates& right_result) {
        m_result.clear();
        Predicates_Set left_set(left_result.begin(), left_result.end());
        Predicates_Set right_set(right_result.begin(), right_result.end());
        Predicates_Set result_set;
        for (Predicate p : left_set) {
            result_set.insert(p);
        }
        for (Predicate p : right_set) {
            result_set.insert(p);
        }
        m_result = Predicates(result_set.begin(), result_set.end());
    }

    virtual void compute_result(const State* state) override {
        m_result.clear();
        compute_result(m_left->evaluate(state), m_right->evaluate(state));
    }

public:
    PredicateUnionElement(const Sketch_STRIPS_Problem* problem, bool goal, PredicateElement* left, PredicateElement* right)
    : PredicateElement(problem, goal), m_left(left), m_right(right) {
        if (goal) {
            compute_result(m_left->result(), m_right->result());
        }
    }
    virtual ~PredicateUnionElement() = default;
};

}

#endif
