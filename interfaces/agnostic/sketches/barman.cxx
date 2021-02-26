#include "barman.hxx"

namespace aptk {

N_CocktailsAndShotsToBeProduced::N_CocktailsAndShotsToBeProduced(const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name) {
}

void N_CocktailsAndShotsToBeProduced::evaluate(const State* state) {
}


N_IngredientsInShaker::N_IngredientsInShaker(const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name) {
}

void N_IngredientsInShaker::evaluate(const State* state) {
}


N_CocktailsConsistentWithPart1::N_CocktailsConsistentWithPart1(const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name) {
}

void N_CocktailsConsistentWithPart1::evaluate(const State* state) {
}


N_CocktailsConsistentWithPart2::N_CocktailsConsistentWithPart2(const BaseSketch* sketch, const std::string &name)
    : NumericalFeature(sketch, name) {
}

void N_CocktailsConsistentWithPart2::evaluate(const State* state) {
}


BarmanSketch::BarmanSketch(
    const Sketch_STRIPS_Problem *problem) : BaseSketch(problem) {
}

}
