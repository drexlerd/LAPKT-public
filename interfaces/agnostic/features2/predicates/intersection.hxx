#ifndef __PREDICATE_INTERSECTION__
#define __PREDICATE_INTERSECTION__

#include "../predicate.hxx"
#include <unordered_set>

namespace aptk {


class PredicateIntersectionElement : public PredicateElement {
protected:
    PredicateElement* m_left;
    PredicateElement* m_right;

    virtual void compute_result(const State* state) override {
        m_result.clear();
        std::unordered_set<unsigned> left_set(m_left->evaluate(state).begin(), m_left->evaluate(state).end());
        std::unordered_set<unsigned> right_set(m_right->evaluate(state).begin(), m_right->evaluate(state).end());
        for (unsigned p : left_set) {
            if (right_set.find(p) != right_set.end()) {
                m_result.push_back(p);
            }
        }
    }

public:
    PredicateIntersectionElement(const Sketch_STRIPS_Problem* problem, bool goal, PredicateElement* left, PredicateElement* right)
    : PredicateElement(problem, goal), m_left(left), m_right(right) {
        if (goal) {
            std::unordered_set<unsigned> left_set(left->result().begin(), left->result().end());
            std::unordered_set<unsigned> right_set(right->result().begin(), right->result().end());
            for (unsigned p : left_set) {
                if (right_set.find(p) != right_set.end()) {
                    m_result.push_back(p);
                }
            }
        }
    }
};

}

#endif
