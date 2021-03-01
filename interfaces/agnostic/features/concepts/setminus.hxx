#ifndef __CONCEPT_SETMINUS__
#define __CONCEPT_SETMINUS__

#include "../concept.hxx"

namespace aptk {


class ConceptSetminusElement : public ConceptElement {
protected:
    ConceptElement* m_left;
    ConceptElement* m_right;

    virtual void compute_result(const State* state) override {
        m_result.clear();
        Concepts_Set left_set(m_left->evaluate(state).begin(), m_left->evaluate(state).end());
        Concepts_Set right_set(m_right->evaluate(state).begin(), m_right->evaluate(state).end());
        for (Concept c : left_set) {
            if (right_set.find(c) == right_set.end()) {
                m_result.push_back(c);
            }
        }
    }

public:
    ConceptSetminusElement(const Sketch_STRIPS_Problem* problem, bool goal, ConceptElement* left, ConceptElement* right)
    : ConceptElement(problem, goal), m_left(left), m_right(right) {
        if (goal) {
            Concepts_Set left_set(left->result().begin(), left->result().end());
            Concepts_Set right_set(right->result().begin(), right->result().end());
            for (Concept c : left_set) {
                if (right_set.find(c) == right_set.end()) {
                    m_result.push_back(c);
                }
            }
        }
    }
    virtual ~ConceptSetminusElement() = default;
};

}

#endif
