//
// Created by LMF319018 on 08/01/2019.
//

#ifndef NGRAM_TRAINEDSMOOTHING_H
#define NGRAM_TRAINEDSMOOTHING_H

#include "SimpleSmoothing.h"

template <class Symbol> class TrainedSmoothing : public SimpleSmoothing<Symbol>{
protected:
    virtual void learnParameters(const vector<vector<Symbol>>& corpus, int N) = 0;
    double newLowerBound(double current, double currentLowerBound, double currentUpperBound, int numberOfParts);
    double newUpperBound(double current, double currentLowerBound, double currentUpperBound, int numberOfParts);
public:
    void train(const vector<vector<Symbol>>& corpus, NGram<Symbol>& nGram);
};

/**
 * Calculates new lower bound.
 * @param current current value.
 * @param currentLowerBound current lower bound
 * @param currentUpperBound current upper bound
 * @param numberOfParts number of parts between lower and upper bound.
 * @return new lower bound
 */
template<class Symbol> double TrainedSmoothing<Symbol>::newLowerBound(double current, double currentLowerBound, double currentUpperBound, int numberOfParts) {
    if (current != currentLowerBound){
        return current - (currentUpperBound - currentLowerBound) / numberOfParts;
    } else {
        return current / numberOfParts;
    }
}

/**
 * Calculates new upper bound.
 * @param current current value.
 * @param currentLowerBound current lower bound
 * @param currentUpperBound current upper bound
 * @param numberOfParts number of parts between lower and upper bound.
 * @return new upper bound
 */
template<class Symbol> double TrainedSmoothing<Symbol>::newUpperBound(double current, double currentLowerBound, double currentUpperBound,
                                               int numberOfParts) {
    if (current != currentUpperBound){
        return current + (currentUpperBound - currentLowerBound) / numberOfParts;
    } else {
        return current * numberOfParts;
    }
}

/**
 * Wrapper function to learn parameters of the smoothing method and set the N-gram probabilities.
 *
 * @param corpus Train corpus used to optimize parameters of the smoothing method.
 * @param nGram N-Gram for which the probabilities will be set.
 */
template<class Symbol> void TrainedSmoothing<Symbol>::train(const vector<vector<Symbol>>& corpus, NGram<Symbol>& nGram) {
    learnParameters(corpus, nGram.getN());
    this->setProbabilities(nGram);
}

#endif //NGRAM_TRAINEDSMOOTHING_H
