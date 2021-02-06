#include <vector>
#include <utility>
#include <memory>
#include <cassert>

#include "elements.hxx"

namespace sketch {

class BasePrecondition {
protected:
    // the underlying feature
    std::shared_ptr<DLBaseElement> feature;
public:
    BasePrecondition(std::shared_ptr<DLBaseElement> feature) : feature(feature) {}
    /**
     * Evaluate the precondition for a given state
     */
    virtual bool evaluate(const std::vector<int> &state) const = 0;
};
/**
 * Precondition of boolean feature that evaluates to true
 * iff the underlying feature evaluated to some non zero value.
 */
class PositiveBooleanPrecondition : public BasePrecondition {
public:
    PositiveBooleanPrecondition(std::shared_ptr<DLBaseElement> feature) : BasePrecondition(feature) {}
    virtual bool evaluate(const std::vector<int> &state) const override {
        assert(feature);
        return feature->size() > 0;
    }
};
/**
 * Precondition of boolean feature that evaluates to true
 * iff the underlying feature evaluates to zero.
 */
class NegativeBooleanPrecondition : public BasePrecondition {
    virtual bool evaluate(const std::vector<int> &state) const override {
        assert(feature);
        return feature->size() == 0;
    }
};
/**
 * Precondition of numerical feature that evaluates to true
 * iff the underlying feature evaluates to some non zero value.
 */
class NonzeroNumericalPreconditon : public BasePrecondition {
    virtual bool evaluate(const std::vector<int> &state) const override {
        assert(feature);
        return feature->size() > 0;
    }
};
/**
 * Precondition of numerical feature that evaluates to true
 * iff the underlying feature evaluates to zero.
 */
class ZeroNumericalPrecondition : public BasePrecondition {
    virtual bool evaluate(const std::vector<int> &state) const override {
        assert(feature);
        return feature->size() == 0;
    }
};


/**
 * A sketch rule or simply rule consists of precondition and effect.
 */
class Rule {
private:
    std::vector<std::unique_ptr<BasePrecondition>> precondition;
    std::vector<std::unique_ptr<BaseEffect>> effect;

    /**
     * Evaluate the effect for two given evaluations.
     * evaluation1 corresponds to the subproblems initial state evaluation
     * evaluation2 corresponds to the currently generated state
     */
    virtual bool evaluate(const std::vector<int> &evaluation1, const std::vector<int> evaluation2) const = 0;
};

}