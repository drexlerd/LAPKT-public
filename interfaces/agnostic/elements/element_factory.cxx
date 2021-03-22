#include "element_factory.hxx"

namespace aptk {

ElementCache<std::tuple<bool, std::string>, PredicateElement*> ElementFactory::m_predicate_extraction_cache = ElementCache<std::tuple<bool, std::string>, PredicateElement*>();
ElementCache<std::tuple<bool, PredicateElement*, PredicateElement*>, PredicateElement*> ElementFactory::m_predicate_intersection_cache = ElementCache<std::tuple<bool, PredicateElement*, PredicateElement*>, PredicateElement*>();
ElementCache<std::tuple<bool, PredicateElement*, PredicateElement*>, PredicateElement*> ElementFactory::m_predicate_setminus_cache = ElementCache<std::tuple<bool, PredicateElement*, PredicateElement*>, PredicateElement*>();
ElementCache<std::tuple<bool, PredicateElement*, PredicateElement*>, PredicateElement*> ElementFactory::m_predicate_union_cache = ElementCache<std::tuple<bool, PredicateElement*, PredicateElement*>, PredicateElement*>();
// concepts
ElementCache<std::tuple<bool, RoleElement*, unsigned>, ConceptElement*> ElementFactory::m_concept_role_extraction_cache = ElementCache<std::tuple<bool, RoleElement*, unsigned>, ConceptElement*>();
ElementCache<std::tuple<bool, RoleElement*>, ConceptElement*> ElementFactory::m_concept_object_extraction_cache = ElementCache<std::tuple<bool, RoleElement*>, ConceptElement*>();
ElementCache<std::tuple<std::string>, ConceptElement*> ElementFactory::m_concept_constant_extraction_cache = ElementCache<std::tuple<std::string>, ConceptElement*>();
ElementCache<std::tuple<bool, PredicateElement*, unsigned>, ConceptElement*> ElementFactory::m_concept_predicate_extraction_cache = ElementCache<std::tuple<bool, PredicateElement*, unsigned>, ConceptElement*>();
ElementCache<std::tuple<bool, ConceptElement*, ConceptElement*>, ConceptElement*> ElementFactory::m_concept_intersection_cache = ElementCache<std::tuple<bool, ConceptElement*, ConceptElement*>, ConceptElement*>();
ElementCache<std::tuple<bool, ConceptElement*, ConceptElement*>, ConceptElement*> ElementFactory::m_concept_setminus_cache = ElementCache<std::tuple<bool, ConceptElement*, ConceptElement*>, ConceptElement*>();
ElementCache<std::tuple<bool, ConceptElement*, ConceptElement*>, ConceptElement*> ElementFactory::m_concept_union_cache = ElementCache<std::tuple<bool, ConceptElement*, ConceptElement*>, ConceptElement*>();
ElementCache<std::tuple<bool, RoleElement*, ConceptElement*>, ConceptElement*> ElementFactory::m_concept_existential_abstraction_cache = ElementCache<std::tuple<bool, RoleElement*, ConceptElement*>, ConceptElement*>();
ElementCache<std::tuple<bool, RoleElement*, ConceptElement*>, ConceptElement*> ElementFactory::m_concept_universal_abstraction_cache = ElementCache<std::tuple<bool, RoleElement*, ConceptElement*>, ConceptElement*>();
ElementCache<std::tuple<bool, RoleElement*, RoleElement*>, ConceptElement*> ElementFactory::m_concept_role_value_equality_cache = ElementCache<std::tuple<bool, RoleElement*, RoleElement*>, ConceptElement*>();
ElementCache<std::tuple<bool, RoleElement*, RoleElement*>, ConceptElement*> ElementFactory::m_concept_role_value_subset_cache = ElementCache<std::tuple<bool, RoleElement*, RoleElement*>, ConceptElement*>();
ElementCache<std::tuple<bool, ConceptElement*, RoleElement*>, ConceptElement*> ElementFactory::m_concept_minimal_cache = ElementCache<std::tuple<bool, ConceptElement*, RoleElement*>, ConceptElement*>();
// roles
ElementCache<std::tuple<bool, PredicateElement*, unsigned, unsigned>, RoleElement*> ElementFactory::m_role_extraction_cache = ElementCache<std::tuple<bool, PredicateElement*, unsigned, unsigned>, RoleElement*>();
ElementCache<std::tuple<bool, RoleElement*, RoleElement*>, RoleElement*> ElementFactory::m_role_intersection_cache = ElementCache<std::tuple<bool, RoleElement*, RoleElement*>, RoleElement*>();
ElementCache<std::tuple<bool, RoleElement*, RoleElement*>, RoleElement*> ElementFactory::m_role_setminus_cache = ElementCache<std::tuple<bool, RoleElement*, RoleElement*>, RoleElement*>();
ElementCache<std::tuple<bool, RoleElement*, RoleElement*>, RoleElement*> ElementFactory::m_role_union_cache = ElementCache<std::tuple<bool, RoleElement*, RoleElement*>, RoleElement*>();
ElementCache<std::tuple<bool, RoleElement*, RoleElement*>, RoleElement*> ElementFactory::m_role_composition_cache = ElementCache<std::tuple<bool, RoleElement*, RoleElement*>, RoleElement*>();
ElementCache<std::tuple<bool, RoleElement*, RoleElement*>, RoleElement*> ElementFactory::m_role_minimal_cache = ElementCache<std::tuple<bool, RoleElement*, RoleElement*>, RoleElement*>();
ElementCache<std::tuple<bool, RoleElement*, ConceptElement*>, RoleElement*> ElementFactory::m_role_restriction_cache = ElementCache<std::tuple<bool, RoleElement*, ConceptElement*>, RoleElement*>();
ElementCache<std::tuple<bool, RoleElement*>, RoleElement*> ElementFactory::m_role_inverse_cache = ElementCache<std::tuple<bool, RoleElement*>, RoleElement*>();
ElementCache<std::tuple<bool, RoleElement*>, RoleElement*> ElementFactory::m_role_transitive_closure_cache = ElementCache<std::tuple<bool, RoleElement*>, RoleElement*>();
ElementCache<std::tuple<bool, RoleElement*>, RoleElement*> ElementFactory::m_role_reflexive_transitive_closure_cache = ElementCache<std::tuple<bool, RoleElement*>, RoleElement*>();
ElementCache<std::tuple<bool, ConceptElement*>, RoleElement*> ElementFactory::m_role_identity_cache = ElementCache<std::tuple<bool, ConceptElement*>, RoleElement*>();
// custom
ElementCache<std::string, PredicateElement*> ElementFactory::m_predicate_custom_cache = ElementCache<std::string, PredicateElement*>();
ElementCache<std::string, ConceptElement*> ElementFactory::m_concept_custom_cache = ElementCache<std::string, ConceptElement*>();
ElementCache<std::string, RoleElement*> ElementFactory::m_role_custom_cache = ElementCache<std::string, RoleElement*>();

PredicateElement* ElementFactory::make_predicate_extraction(const Sketch_STRIPS_Problem* problem, bool goal, std::string predicate_name) {
    std::tuple<bool, std::string> key = std::tuple<bool, std::string>(goal, predicate_name);
    if (!m_predicate_extraction_cache.exists(key)) {
        PredicateElement* result = new PredicateExtractionElement(problem, goal, predicate_name);
        m_predicate_extraction_cache.insert(key, std::move(result));
    }
    return m_predicate_extraction_cache.get(key);
}

PredicateElement* ElementFactory::make_predicate_intersection(const Sketch_STRIPS_Problem* problem, bool goal, PredicateElement* left, PredicateElement* right) {
    std::tuple<bool, PredicateElement*, PredicateElement*> key = std::tuple<bool, PredicateElement*, PredicateElement*>(goal, left, right);
    if (!m_predicate_intersection_cache.exists(key)) {
        PredicateElement* result = new PredicateIntersectionElement(problem, goal, left, right);
        m_predicate_intersection_cache.insert(key, std::move(result));
    }
    return m_predicate_intersection_cache.get(key);
}

PredicateElement* ElementFactory::make_predicate_setminus(const Sketch_STRIPS_Problem* problem, bool goal, PredicateElement* left, PredicateElement* right) {
    std::tuple<bool, PredicateElement*, PredicateElement*> key = std::tuple<bool, PredicateElement*, PredicateElement*>(goal, left, right);
    if (!m_predicate_setminus_cache.exists(key)) {
        PredicateElement* result = new PredicateSetminusElement(problem, goal, left, right);
        m_predicate_setminus_cache.insert(key, std::move(result));
    }
    return m_predicate_setminus_cache.get(key);
}

PredicateElement* ElementFactory::make_predicate_union(const Sketch_STRIPS_Problem* problem, bool goal, PredicateElement* left, PredicateElement* right) {
    std::tuple<bool, PredicateElement*, PredicateElement*> key = std::tuple<bool, PredicateElement*, PredicateElement*>(goal, left, right);
    if (!m_predicate_union_cache.exists(key)) {
        PredicateElement* result = new PredicateUnionElement(problem, goal, left, right);
        m_predicate_union_cache.insert(key, std::move(result));
    }
    return m_predicate_union_cache.get(key);
}

ConceptElement* ElementFactory::make_concept_extraction(const Sketch_STRIPS_Problem* problem, bool goal, RoleElement* role, unsigned position) {
    std::tuple<bool, RoleElement*, unsigned> key = std::tuple<bool, RoleElement*, unsigned>(goal, role, position);
    if (!m_concept_role_extraction_cache.exists(key)) {
        ConceptElement* result = new ConceptRoleExtractionElement(problem, goal, role, position);
        m_concept_role_extraction_cache.insert(key, std::move(result));
    }
    return m_concept_role_extraction_cache.get(key);
}

ConceptElement* ElementFactory::make_concept_extraction(const Sketch_STRIPS_Problem* problem, bool goal, RoleElement* role) {
    std::tuple<bool, RoleElement*> key = std::tuple<bool, RoleElement*>(goal, role);
    if (!m_concept_object_extraction_cache.exists(key)) {
        ConceptElement* result = new ConceptObjectExtractionElement(problem, goal, role);
        m_concept_object_extraction_cache.insert(key, std::move(result));
    }
    return m_concept_object_extraction_cache.get(key);
}

ConceptElement* ElementFactory::make_concept_extraction(const Sketch_STRIPS_Problem* problem, std::string constant_name) {
    std::tuple<std::string> key = std::tuple<std::string>(constant_name);
    if (!m_concept_constant_extraction_cache.exists(key)) {
        ConceptElement* result = new ConceptConstantExtractionElement(problem, constant_name);
        m_concept_constant_extraction_cache.insert(key, std::move(result));
    }
    return m_concept_constant_extraction_cache.get(key);
}

ConceptElement* ElementFactory::make_concept_extraction(const Sketch_STRIPS_Problem* problem, bool goal, PredicateElement* predicate, unsigned position) {
    std::tuple<bool, PredicateElement*, unsigned> key = std::tuple<bool, PredicateElement*, unsigned>(goal, predicate, position);
    if (!m_concept_predicate_extraction_cache.exists(key)) {
        ConceptElement* result = new ConceptPredicateExtractionElement(problem, goal, predicate, position);
        m_concept_predicate_extraction_cache.insert(key, std::move(result));
    }
    return m_concept_predicate_extraction_cache.get(key);
}


ConceptElement* ElementFactory::make_concept_intersection(const Sketch_STRIPS_Problem* problem, bool goal, ConceptElement* left, ConceptElement* right) {
    std::tuple<bool, ConceptElement*, ConceptElement*> key = std::tuple<bool, ConceptElement*, ConceptElement*>(goal, left, right);
    if (!m_concept_intersection_cache.exists(key)) {
        ConceptElement* result = new ConceptIntersectionElement(problem, goal, left, right);
        m_concept_intersection_cache.insert(key, std::move(result));
    }
    return m_concept_intersection_cache.get(key);
}

ConceptElement* ElementFactory::make_concept_setminus(const Sketch_STRIPS_Problem* problem, bool goal, ConceptElement* left, ConceptElement* right) {
    std::tuple<bool, ConceptElement*, ConceptElement*> key = std::tuple<bool, ConceptElement*, ConceptElement*>(goal, left, right);
    if (!m_concept_setminus_cache.exists(key)) {
        ConceptElement* result = new ConceptSetminusElement(problem, goal, left, right);
        m_concept_setminus_cache.insert(key, std::move(result));
    }
    return m_concept_setminus_cache.get(key);
}

ConceptElement* ElementFactory::make_concept_union(const Sketch_STRIPS_Problem* problem, bool goal, ConceptElement* left, ConceptElement* right) {
    std::tuple<bool, ConceptElement*, ConceptElement*> key = std::tuple<bool, ConceptElement*, ConceptElement*>(goal, left, right);
    if (!m_concept_union_cache.exists(key)) {
        ConceptElement* result = new ConceptUnionElement(problem, goal, left, right);
        m_concept_union_cache.insert(key, std::move(result));
    }
    return m_concept_union_cache.get(key);
}

ConceptElement* ElementFactory::make_concept_existential_abstraction(const Sketch_STRIPS_Problem* problem, bool goal, RoleElement* role, ConceptElement* concept) {
    std::tuple<bool, RoleElement*, ConceptElement*> key = std::tuple<bool, RoleElement*, ConceptElement*>(goal, role, concept);
    if (!m_concept_existential_abstraction_cache.exists(key)) {
        ConceptElement* result = new ExistentialAbstractionElement(problem, goal, role, concept);
        m_concept_existential_abstraction_cache.insert(key, std::move(result));
    }
    return m_concept_existential_abstraction_cache.get(key);
}

ConceptElement* ElementFactory::make_concept_universal_abstraction(const Sketch_STRIPS_Problem* problem, bool goal, RoleElement* role, ConceptElement* concept) {
    std::tuple<bool, RoleElement*, ConceptElement*> key = std::tuple<bool, RoleElement*, ConceptElement*>(goal, role, concept);
    if (!m_concept_universal_abstraction_cache.exists(key)) {
        ConceptElement* result = new UniversalAbstractionElement(problem, goal, role, concept);
        m_concept_universal_abstraction_cache.insert(key, std::move(result));
    }
    return m_concept_universal_abstraction_cache.get(key);
}

ConceptElement* ElementFactory::make_concept_role_value_equality(const Sketch_STRIPS_Problem* problem, bool goal, RoleElement* left, RoleElement* right) {
    std::tuple<bool, RoleElement*, RoleElement*> key = std::tuple<bool, RoleElement*, RoleElement*>(goal, left, right);
    if (!m_concept_role_value_equality_cache.exists(key)) {
        ConceptElement* result = new ConceptRoleValueEqualityElement(problem, goal, left, right);
        m_concept_role_value_equality_cache.insert(key, std::move(result));
    }
    return m_concept_role_value_equality_cache.get(key);
}

ConceptElement* ElementFactory::make_concept_role_value_subset(const Sketch_STRIPS_Problem* problem, bool goal, RoleElement* left, RoleElement* right) {
    std::tuple<bool, RoleElement*, RoleElement*> key = std::tuple<bool, RoleElement*, RoleElement*>(goal, left, right);
    if (!m_concept_role_value_subset_cache.exists(key)) {
        ConceptElement* result = new ConceptRoleValueSubsetElement(problem, goal, left, right);
        m_concept_role_value_subset_cache.insert(key, std::move(result));
    }
    return m_concept_role_value_subset_cache.get(key);
}

ConceptElement* ElementFactory::make_concept_minimal(const Sketch_STRIPS_Problem* problem, bool goal, ConceptElement* concept, RoleElement* role) {
    std::tuple<bool, ConceptElement*, RoleElement*> key = std::tuple<bool, ConceptElement*, RoleElement*>(goal, concept, role);
    if (!m_concept_minimal_cache.exists(key)) {
        ConceptElement* result = new ConceptMinimalElement(problem, goal, concept, role);
        m_concept_minimal_cache.insert(key, std::move(result));
    }
    return m_concept_minimal_cache.get(key);
}


RoleElement* ElementFactory::make_role_extraction(const Sketch_STRIPS_Problem* problem, bool goal, PredicateElement* predicate, unsigned a, unsigned b) {
    std::tuple<bool, PredicateElement*, unsigned, unsigned> key = std::tuple<bool, PredicateElement*, unsigned, unsigned>(goal, predicate, a, b);
    if (!m_role_extraction_cache.exists(key)) {
        RoleElement* result = new RoleExtractionElement(problem, goal, predicate, a, b);
        m_role_extraction_cache.insert(key, std::move(result));
    }
    return m_role_extraction_cache.get(key);
}

RoleElement* ElementFactory::make_role_intersection(const Sketch_STRIPS_Problem* problem, bool goal, RoleElement* left, RoleElement* right) {
    std::tuple<bool, RoleElement*, RoleElement*> key = std::tuple<bool, RoleElement*, RoleElement*>(goal, left, right);
    if (!m_role_intersection_cache.exists(key)) {
        RoleElement* result = new RoleIntersectionElement(problem, goal, left, right);
        m_role_intersection_cache.insert(key, std::move(result));
    }
    return m_role_intersection_cache.get(key);
}

RoleElement* ElementFactory::make_role_setminus(const Sketch_STRIPS_Problem* problem, bool goal, RoleElement* left, RoleElement* right) {
    std::tuple<bool, RoleElement*, RoleElement*> key = std::tuple<bool, RoleElement*, RoleElement*>(goal, left, right);
    if (!m_role_setminus_cache.exists(key)) {
        RoleElement* result = new RoleSetminusElement(problem, goal, left, right);
        m_role_setminus_cache.insert(key, std::move(result));
    }
    return m_role_setminus_cache.get(key);
}

RoleElement* ElementFactory::make_role_union(const Sketch_STRIPS_Problem* problem, bool goal, RoleElement* left, RoleElement* right) {
    std::tuple<bool, RoleElement*, RoleElement*> key = std::tuple<bool, RoleElement*, RoleElement*>(goal, left, right);
    if (!m_role_union_cache.exists(key)) {
        RoleElement* result = new RoleUnionElement(problem, goal, left, right);
        m_role_union_cache.insert(key, std::move(result));
    }
    return m_role_union_cache.get(key);
}

RoleElement* ElementFactory::make_role_composition(const Sketch_STRIPS_Problem* problem, bool goal, RoleElement* left, RoleElement* right) {
    std::tuple<bool, RoleElement*, RoleElement*> key = std::tuple<bool, RoleElement*, RoleElement*>(goal, left, right);
    if (!m_role_composition_cache.exists(key)) {
        RoleElement* result = new RoleCompositionElement(problem, goal, left, right);
        m_role_composition_cache.insert(key, std::move(result));
    }
    return m_role_composition_cache.get(key);
}

RoleElement* ElementFactory::make_role_minimal(const Sketch_STRIPS_Problem* problem, bool goal, RoleElement* left, RoleElement* right) {
    std::tuple<bool, RoleElement*, RoleElement*> key = std::tuple<bool, RoleElement*, RoleElement*>(goal, left, right);
    if (!m_role_minimal_cache.exists(key)) {
        RoleElement* result = new RoleMinimalElement(problem, goal, left, right);
        m_role_minimal_cache.insert(key, std::move(result));
    }
    return m_role_minimal_cache.get(key);
}

RoleElement* ElementFactory::make_role_restriction(const Sketch_STRIPS_Problem* problem, bool goal, RoleElement* role, ConceptElement* concept) {
    std::tuple<bool, RoleElement*, ConceptElement*> key = std::tuple<bool, RoleElement*, ConceptElement*>(goal, role, concept);
    if (!m_role_restriction_cache.exists(key)) {
        RoleElement* result = new RoleRestrictionElement(problem, goal, role, concept);
        m_role_restriction_cache.insert(key, std::move(result));
    }
    return m_role_restriction_cache.get(key);
}

RoleElement* ElementFactory::make_role_inverse(const Sketch_STRIPS_Problem* problem, bool goal, RoleElement* role) {
    std::tuple<bool, RoleElement*> key = std::tuple<bool, RoleElement*>(goal, role);
    if (!m_role_inverse_cache.exists(key)) {
        RoleElement* result = new RoleInverseElement(problem, goal, role);
        m_role_inverse_cache.insert(key, std::move(result));
    }
    return m_role_inverse_cache.get(key);
}

RoleElement* ElementFactory::make_role_transitive_closure(const Sketch_STRIPS_Problem* problem, bool goal, RoleElement* role) {
    std::tuple<bool, RoleElement*> key = std::tuple<bool, RoleElement*>(goal, role);
    if (!m_role_transitive_closure_cache.exists(key)) {
        RoleElement* result = new RoleTransitiveClosureElement(problem, goal, role);
        m_role_transitive_closure_cache.insert(key, std::move(result));
    }
    return m_role_transitive_closure_cache.get(key);
}

RoleElement* ElementFactory::make_role_reflexive_transitive_closure(const Sketch_STRIPS_Problem* problem, bool goal, RoleElement* role) {
    std::tuple<bool, RoleElement*> key = std::tuple<bool, RoleElement*>(goal, role);
    if (!m_role_reflexive_transitive_closure_cache.exists(key)) {
        RoleElement* result = new RoleReflexiveTransitiveClosureElement(problem, goal, role);
        m_role_reflexive_transitive_closure_cache.insert(key, std::move(result));
    }
    return m_role_reflexive_transitive_closure_cache.get(key);
}

RoleElement* ElementFactory::make_role_identity(const Sketch_STRIPS_Problem* problem, bool goal, ConceptElement* concept) {
    std::tuple<bool, ConceptElement*> key = std::tuple<bool, ConceptElement*>(goal, concept);
    if (!m_role_identity_cache.exists(key)) {
        RoleElement* result = new RoleIdentityElement(problem, goal, concept);
        m_role_identity_cache.insert(key, std::move(result));
    }
    return m_role_identity_cache.get(key);
}

PredicateElement* ElementFactory::add_predicate_custom(std::string name, PredicateElement* custom) {
    if (m_predicate_custom_cache.exists(name)) {
        std::cout << "ElementFactory::add_predicate_custom: key already exists!\n";
        exit(1);
    }
    m_predicate_custom_cache.insert(name, std::move(custom));
    return m_predicate_custom_cache.get(name);
}

PredicateElement* ElementFactory::get_predicate_custom(std::string name) {
    if (!m_predicate_custom_cache.exists(name)) {
        std::cout << "ElementFactory::get_predicate_custom: key " << name << " does not exists!\n";
        exit(1);
    }
    return m_predicate_custom_cache.get(name);
}

ConceptElement* ElementFactory::add_concept_custom(std::string name, ConceptElement* custom) {
    if (m_concept_custom_cache.exists(name)) {
        std::cout << "ElementFactory::add_concept_custom: key already exists!\n";
        exit(1);
    }
    m_concept_custom_cache.insert(name, std::move(custom));
    return m_concept_custom_cache.get(name);
}

ConceptElement* ElementFactory::get_concept_custom(std::string name) {
    if (!m_concept_custom_cache.exists(name)) {
        std::cout << "ElementFactory::get_concept_custom: key " << name << " does not exists!\n";
        exit(1);
    }
    return m_concept_custom_cache.get(name);
}

RoleElement* ElementFactory::add_role_custom(std::string name, RoleElement* custom) {
    if (m_role_custom_cache.exists(name)) {
        std::cout << "ElementFactory::add_role_custom: key already exists!\n";
        exit(1);
    }
    m_role_custom_cache.insert(name, std::move(custom));
    return m_role_custom_cache.get(name);
}

RoleElement* ElementFactory::get_role_custom(std::string name) {
    if (!m_role_custom_cache.exists(name)) {
        std::cout << "ElementFactory::get_role_custom: key " << name << " does not exists!\n";
        exit(1);
    }
    return m_role_custom_cache.get(name);
}

}