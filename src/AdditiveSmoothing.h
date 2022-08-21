//
// Created by LMF319018 on 08/01/2019.
//

#ifndef NGRAM_ADDITIVESMOOTHINGTEST_H
#define NGRAM_ADDITIVESMOOTHINGTEST_H

#include <cfloat>
#include "TrainedSmoothing.h"
#include "KFoldCrossValidation.h"

template <class Symbol> class AdditiveSmoothing : public TrainedSmoothing<Symbol>{
private:
    /**
     * Additive pseudocount parameter used in Additive Smoothing. The parameter will be learned using 10-fold cross
     * validation.
     */
    double delta;
    double learnBestDelta(vector<NGram<Symbol>*> nGrams, KFoldCrossValidation<vector<Symbol>> kFoldCrossValidation, double lowerBound);
    void learnParameters(vector<vector<Symbol>> corpus, int N);

protected:
    void setProbabilitiesWithLevel(NGram<Symbol>& nGram, int level);
public:
    double getDelta();
};

/**
 * The algorithm tries to optimize the best delta for a given corpus. The algorithm uses perplexity on the validation
 * set as the optimization criterion.
 * @param nGrams 10 N-Grams learned for different folds of the corpus. nGrams[i] is the N-Gram trained with i'th train
 *               fold of the corpus.
 * @param kFoldCrossValidation Cross-validation data used in training and testing the N-grams.
 * @param lowerBound Initial lower bound for optimizing the best delta.
 * @return Best delta optimized with k-fold crossvalidation.
 */
template<class Symbol>
double AdditiveSmoothing<Symbol>::learnBestDelta(vector<NGram<Symbol>*> nGrams,
                                                 KFoldCrossValidation<vector<Symbol>> kFoldCrossValidation,
                                                 double lowerBound) {
    double bestPerplexity, bestPrevious = -1, upperBound = 1, perplexity, bestDelta = (lowerBound + upperBound) / 2;
    int numberOfParts = 5;
    while (true){
        bestPerplexity = DBL_MAX;
        for (double value = lowerBound; value <= upperBound; value += (upperBound - lowerBound) / numberOfParts){
            perplexity = 0;
            for (int i = 0; i < 10; i++){
                nGrams.at(i)->setProbabilityWithPseudoCount(value, nGrams.at(i)->getN());
                perplexity += nGrams.at(i)->getPerplexity(kFoldCrossValidation.getTestFold(i));
            }
            if (perplexity < bestPerplexity){
                bestPerplexity = perplexity;
                bestDelta = value;
            }
        }
        lowerBound = this->newLowerBound(bestDelta, lowerBound, upperBound, numberOfParts);
        upperBound = this->newUpperBound(bestDelta, lowerBound, upperBound, numberOfParts);
        if (bestPrevious != -1){
            if (abs(bestPrevious - bestPerplexity) / bestPerplexity < 0.001){
                break;
            }
        }
        bestPrevious = bestPerplexity;
    }
    return bestDelta;
}

/**
 * Wrapper function to learn the parameter (delta) in additive smoothing. The function first creates K NGrams
 * with the train folds of the corpus. Then optimizes delta with respect to the test folds of the corpus.
 * @param corpus Train corpus used to optimize delta parameter
 * @param N N in N-Gram.
 */
template<class Symbol> void AdditiveSmoothing<Symbol>::learnParameters(vector<vector<Symbol>> corpus, int N) {
    int K = 10;
    vector<NGram<Symbol>*> nGrams;
    KFoldCrossValidation<vector<Symbol>> kFoldCrossValidation = KFoldCrossValidation(corpus, K, 0);
    for (int i = 0; i < K; i++){
        NGram<Symbol>* nGram = new NGram<Symbol>(kFoldCrossValidation.getTrainFold(i), N);
        nGrams.push_back(nGram);
    }
    delta = learnBestDelta(nGrams, kFoldCrossValidation, 0.1);
}

/**
 * Wrapper function to set the N-gram probabilities with additive smoothing.
 * @param nGram N-Gram for which the probabilities will be set.
 * @param level Level for which N-Gram probabilities will be set. Probabilities for different levels of the
 *              N-gram can be set with this function. If level = 1, N-Gram is treated as UniGram, if level = 2,
 *              N-Gram is treated as Bigram, etc.
 */
template<class Symbol> void AdditiveSmoothing<Symbol>::setProbabilitiesWithLevel(NGram<Symbol>& nGram, int level) {
    nGram.setProbabilityWithPseudoCount(delta, level);
}

/**
 * Gets the best delta.
 * @return Learned best delta.
 */
template<class Symbol> double AdditiveSmoothing<Symbol>::getDelta(){
    return delta;
}

#endif //NGRAM_ADDITIVESMOOTHINGTEST_H
