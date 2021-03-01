#ifndef __CONCEPT_UNION__
#define __CONCEPT_UNION__

#include "../concept.hxx"

namespace aptk {


class ConceptUnionElement : public ConceptElement {
protected:
    ConceptElement* m_left;
    ConceptElement* m_right;

    virtual void compute_result(const State* state) override {
        m_result.clear();
        Concepts_Set left_set(m_left->evaluate(state).begin(), m_left->evaluate(state).end());
        Concepts_Set right_set(m_right->evaluate(state).begin(), m_right->evaluate(state).end());
        Concepts_Set result_set;
        for (Concept c : left_set) {
            result_set.insert(c);
        }
        for (Concept c : right_set) {
            result_set.insert(c);
        }
        m_result = Concepts(result_set.begin(), result_set.end());
    }

public:
    ConceptUnionElement(const Sketch_STRIPS_Problem* problem, bool goal, ConceptElement* left, ConceptElement* right)
    : ConceptElement(problem, goal), m_left(left), m_right(right) {
        if (goal) {
            Concepts_Set left_set(left->result().begin(), left->result().end());
            Concepts_Set right_set(right->result().begin(), right->result().end());
            Concepts_Set result_set;
            for (Concept c : left_set) {
                result_set.insert(c);
            }
            for (Concept c : right_set) {
                result_set.insert(c);
            }
            m_result = Concepts(result_set.begin(), result_set.end());
        }
    }
    virtual ~ConceptUnionElement() = default;
};

}

#endif
