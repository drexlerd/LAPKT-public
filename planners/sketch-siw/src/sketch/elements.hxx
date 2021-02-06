#ifndef ELEMENTS_H
#define ELEMENTS_H

#include <vector>


namespace sketch {

class DLBaseElement {
protected:
    // reuse memory for evaluation.
    std::vector<int> result;
public:
    /**
     * Concrete implementation modifies the result vector and returns reference.
     * This allows to allocate memory only once.
     * TODO: only reevaluate the element if the state has changed
     * in comparison to a previous evaluation.
     * This speeds up computation where elements occur multiple times
     * in the same or across different features.
     */
    virtual const std::vector<int> &evaluate(const std::vector<int> &state) const = 0;

    /**
     * Return the number of elements in the result.
     */
    size_t size() const {
        return result.size();
    }
};

}

#endif
