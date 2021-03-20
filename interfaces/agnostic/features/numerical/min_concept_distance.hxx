#ifndef __MIN_CONCEPT_DISTANCE_FEATURE__
#define __MIN_CONCEPT_DISTANCE_FEATURE__

#include "../numerical_feature.hxx"
#include "../../elements/role.hxx"
#include "../../elements/concept.hxx"
#include "../../elements/predicate.hxx"
#include "../../sketch.hxx"
#include "../utils.hxx"
#include "../../elements/utils.hxx"
#include <iostream>
#include <vector>
#include <numeric>
#include <string>
#include <functional>

namespace aptk {

/**
 * The SumDistanceFeature implements the minimization over shortest pairwise distances
 * of roles that are connected over some other role.
 * The evaluation throws an error if there is no connection for some relevant concepts.
 */
class MinConceptDistanceFeature : public NumericalFeature {
protected:
    ConceptElement* m_concept1;
    RoleElement* m_conn;
    ConceptElement* m_concept2;
public:
    MinConceptDistanceFeature(const BaseSketch* sketch, const std::string &name,
        ConceptElement* concept1, RoleElement* conn, ConceptElement* concept2) : NumericalFeature(sketch, name, nullptr),
        m_concept1(concept1), m_conn(conn), m_concept2(concept2) {
    }
    virtual ~MinConceptDistanceFeature() = default;

    virtual void evaluate(const State* state) override {
        Concepts result_concept1 = m_concept1->evaluate(state);
        Roles result_conn = m_conn->evaluate(state);
        Concepts result_concept2 = m_concept2->evaluate(state);
        /*
        m_concept1->print();
        m_conn->print();
        m_concept2->print();
        */

        // compute pairwise distances over role.
        std::tuple<aptk::elements::PairwiseDistances, Concepts, Index_Vec> result = aptk::elements::compute_pairwise_distances(m_sketch->problem(), result_conn);
        aptk::elements::PairwiseDistances& pairwise_distances = std::get<0>(result);
        Index_Vec& conn_concept_indices = std::get<2>(result);

        if (result_concept1.empty() || result_concept2.empty()) {
            // empty source or target trivially evaluates to 0.
            new_eval = 0;
        } else {
            new_eval = INF;
            for (Concept c1 : result_concept1) {
                for (Concept c2 : result_concept2) {
                    new_eval = std::min(new_eval, static_cast<int>(pairwise_distances[conn_concept_indices[c1]][conn_concept_indices[c2]]));
                }
            }
        }
        /*if (new_eval == INF) {
            std::cout << "MinConceptDistanceFeature::evaluate: minimal distance is INF!" << std::endl;
            exit(1);
        }*/
        // std::cout << new_eval << std::endl;
    }

    /**
     * Pretty printer.
     */
    virtual void print() const override {
        std::cout << m_name << ": new_eval=" << new_eval << ": ";
        m_concept1->print();
        m_conn->print();
        m_concept2->print();
    }
};

}

#endif
