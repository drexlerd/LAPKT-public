#ifndef __CONCEPT_SETMINUS__
#define __CONCEPT_SETMINUS__

#include "../concept.hxx"

namespace aptk {


class ConceptSetminusElement : public ConceptElement {
protected:
    ConceptElement* m_left;
    ConceptElement* m_right;

    virtual void compute_result(const Concepts& left_result, const Concepts& right_result) {
        m_result.clear();
        Concepts_Set left_set(left_result.begin(), left_result.end());
        Concepts_Set right_set(right_result.begin(), right_result.end());
        for (Concept c : left_set) {
            if (right_set.find(c) == right_set.end()) {
                m_result.push_back(c);
            }
        }
    }

    virtual void compute_result(const State* state) override {
        m_result.clear();
        compute_result(m_left->evaluate(state), m_right->evaluate(state));
    }

public:
    ConceptSetminusElement(const Sketch_STRIPS_Problem* problem, bool goal, ConceptElement* left, ConceptElement* right)
    : ConceptElement(problem, goal), m_left(left), m_right(right) {
        if (goal) {
            compute_result(left->result(), right->result());
        }
    }
    virtual ~ConceptSetminusElement() = default;
};

}

#endif
