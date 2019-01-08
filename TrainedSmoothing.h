//
// Created by LMF319018 on 08/01/2019.
//

#ifndef NGRAM_TRAINEDSMOOTHING_H
#define NGRAM_TRAINEDSMOOTHING_H

#include "SimpleSmoothing.h"

template <class Symbol> class TrainedSmoothing : public SimpleSmoothing<Symbol>{
protected:
    virtual void learnParameters(vector<vector<Symbol>> corpus, int N) = 0;
    double newLowerBound(double current, double currentLowerBound, double currentUpperBound, int numberOfParts);
    double newUpperBound(double current, double currentLowerBound, double currentUpperBound, int numberOfParts);
public:
    void train(vector<vector<Symbol>> corpus, NGram<Symbol> nGram);
};

template<class Symbol> double TrainedSmoothing<Symbol>::newLowerBound(double current, double currentLowerBound, double currentUpperBound, int numberOfParts) {
    if (current != currentLowerBound){
        return current - (currentUpperBound - currentLowerBound) / numberOfParts;
    } else {
        return current / numberOfParts;
    }
}

template<class Symbol> double TrainedSmoothing<Symbol>::newUpperBound(double current, double currentLowerBound, double currentUpperBound,
                                               int numberOfParts) {
    if (current != currentUpperBound){
        return current + (currentUpperBound - currentLowerBound) / numberOfParts;
    } else {
        return current * numberOfParts;
    }
}

template<class Symbol> void TrainedSmoothing<Symbol>::train(vector<vector<Symbol>> corpus, NGram<Symbol> nGram) {
    learnParameters(corpus, nGram.getN());
    setProbabilities(nGram);
}

#endif //NGRAM_TRAINEDSMOOTHING_H
