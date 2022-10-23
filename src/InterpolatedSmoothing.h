//
// Created by LMF319018 on 08/01/2019.
//

#ifndef NGRAM_INTERPOLATEDSMOOTHING_H
#define NGRAM_INTERPOLATEDSMOOTHING_H

#include <KFoldCrossValidation.h>
#include <cfloat>
#include "TrainedSmoothing.h"
#include "GoodTuringSmoothing.h"

template <class Symbol> class InterpolatedSmoothing : public TrainedSmoothing<Symbol>{
private:
    double lambda1, lambda2;
    double learnBestLambda(const vector<NGram<Symbol>*>& nGrams, const KFoldCrossValidation<vector<Symbol>>& kFoldCrossValidation, double lowerBound);
    vector<double> learnBestLambdas(const vector<NGram<Symbol>*>& nGrams, const KFoldCrossValidation<vector<Symbol>>& kFoldCrossValidation, double lowerBound1, double lowerBound2);
protected:
    void learnParameters(const vector<vector<Symbol>>& corpus, int N);
    void setProbabilitiesWithLevel(NGram<Symbol>& nGram, int level);
};

/**
 * The algorithm tries to optimize the best lambda for a given corpus. The algorithm uses perplexity on the validation
 * set as the optimization criterion.
 *
 * @param nGrams 10 N-Grams learned for different folds of the corpus. nGrams[i] is the N-Gram trained with i'th train fold of the corpus.
 * @param kFoldCrossValidation Cross-validation data used in training and testing the N-grams.
 * @param lowerBound Initial lower bound for optimizing the best lambda.
 * @return  Best lambda optimized with k-fold crossvalidation.
 */
template<class Symbol> double InterpolatedSmoothing<Symbol>::learnBestLambda(const vector<NGram<Symbol>*>& nGrams,
                                                                             const KFoldCrossValidation<vector<Symbol>>& kFoldCrossValidation,
                                                                             double lowerBound) {
    double bestPerplexity, bestPrevious = -1, upperBound = 0.999, perplexity, bestLambda = (lowerBound + upperBound) / 2;
    int numberOfParts = 5;
    vector<vector<Symbol>> testFolds[10];
    for (int i = 0; i < 10; i++){
        testFolds[i] = kFoldCrossValidation.getTestFold(i);
    }
    while (true){
        bestPerplexity = DBL_MAX;
        for (double value = lowerBound; value <= upperBound; value += (upperBound - lowerBound) / numberOfParts){
            perplexity = 0;
            for (int i = 0; i < 10; i++){
                nGrams.at(i)->setLambda(value);
                perplexity += nGrams[i]->getPerplexity(testFolds[i]);
            }
            if (perplexity < bestPerplexity){
                bestPerplexity = perplexity;
                bestLambda = value;
            }
        }
        lowerBound = this->newLowerBound(bestLambda, lowerBound, upperBound, numberOfParts);
        upperBound = this->newUpperBound(bestLambda, lowerBound, upperBound, numberOfParts);
        if (bestPrevious != -1){
            if (abs(bestPrevious - bestPerplexity) / bestPerplexity < 0.001){
                break;
            }
        }
        bestPrevious = bestPerplexity;
    }
    return bestLambda;
}

/**
 * The algorithm tries to optimize the best lambdas (lambda1, lambda2) for a given corpus. The algorithm uses perplexity on the validation
 * set as the optimization criterion.
 *
 * @param nGrams 10 N-Grams learned for different folds of the corpus. nGrams[i] is the N-Gram trained with i'th train fold of the corpus.
 * @param kFoldCrossValidation Cross-validation data used in training and testing the N-grams.
 * @param lowerBound1 Initial lower bound for optimizing the best lambda1.
 * @param lowerBound2 Initial lower bound for optimizing the best lambda2.
 * @return
 */
template<class Symbol>
vector<double> InterpolatedSmoothing<Symbol>::learnBestLambdas(const vector<NGram<Symbol>*>& nGrams,
                                                               const KFoldCrossValidation<vector<Symbol>>& kFoldCrossValidation,
                                                               double lowerBound1,
                                                               double lowerBound2) {
    double bestPerplexity, upperBound1 = 0.999, upperBound2 = 0.999, bestPrevious = -1, perplexity, bestLambda1 = (lowerBound1 + upperBound1) / 2, bestLambda2 = (lowerBound1 + upperBound1) / 2;
    vector<vector<Symbol>> testFolds[10];
    int numberOfParts = 5;
    for (int i = 0; i < 10; i++){
        testFolds[i] = kFoldCrossValidation.getTestFold(i);
    }
    while (true){
        bestPerplexity = DBL_MAX;
        for (double value1 = lowerBound1; value1 <= upperBound1; value1 += (upperBound1 - lowerBound1) / numberOfParts){
            for (double value2 = lowerBound2; value2 <= upperBound2 && value1 + value2 < 1; value2 += (upperBound2 - lowerBound2) / numberOfParts){
                perplexity = 0;
                for (int i = 0; i < 10; i++){
                    nGrams.at(i)->setLambda(value1, value2);
                    perplexity += nGrams.at(i)->getPerplexity(testFolds[i]);
                }
                if (perplexity < bestPerplexity){
                    bestPerplexity = perplexity;
                    bestLambda1 = value1;
                    bestLambda2 = value2;
                }
            }
        }
        lowerBound1 = this->newLowerBound(bestLambda1, lowerBound1, upperBound1, numberOfParts);
        upperBound1 = this->newUpperBound(bestLambda1, lowerBound1, upperBound1, numberOfParts);
        lowerBound2 = this->newLowerBound(bestLambda2, lowerBound2, upperBound2, numberOfParts);
        upperBound2 = this->newUpperBound(bestLambda2, lowerBound2, upperBound2, numberOfParts);
        if (bestPrevious != -1){
            if (abs(bestPrevious - bestPerplexity) / bestPerplexity < 0.001){
                break;
            }
        }
        bestPrevious = bestPerplexity;
    }
    vector<double> result = vector<double>(2);
    result.emplace_back(bestLambda1);
    result.emplace_back(bestLambda2);
    return result;
}

/**
 * Wrapper function to learn the parameters (lambda1 and lambda2) in interpolated smoothing. The function first creates K NGrams
 * with the train folds of the corpus. Then optimizes lambdas with respect to the test folds of the corpus depending on given N.
 * @param corpus Train corpus used to optimize lambda parameters
 * @param N N in N-Gram.
 */
template<class Symbol> void InterpolatedSmoothing<Symbol>::learnParameters(const vector<vector<Symbol>>& corpus, int N) {
    if (N <= 1){
        return;
    }
    int K = 10;
    vector<NGram<Symbol>*> nGrams;
    KFoldCrossValidation<vector<Symbol>> kFoldCrossValidation = KFoldCrossValidation(corpus, K, 0);
    for (int i = 0; i < K; i++){
        auto* nGram = new NGram<Symbol>(kFoldCrossValidation.getTrainFold(i), N);
        nGrams.push_back(nGram);
        GoodTuringSmoothing<string> goodTuringSmoothing;
        for (int j = 2; j<= N; j++){
            goodTuringSmoothing.setProbabilitiesWithLevel(*(nGrams[i]), j);
        }
        goodTuringSmoothing.setProbabilitiesWithLevel(*(nGrams[i]), 1);
    }
    if (N == 2){
        lambda1 = learnBestLambda(nGrams, kFoldCrossValidation, 0.1);
    } else {
        if (N == 3){
            vector<double> bestLambdas = learnBestLambdas(nGrams, kFoldCrossValidation, 0.1, 0.1);
            lambda1 = bestLambdas[0];
            lambda2 = bestLambdas[1];
        }
    }
}

/**
 * Wrapper function to set the N-gram probabilities with interpolated smoothing.
 * @param nGram N-Gram for which the probabilities will be set.
 * @param level Level for which N-Gram probabilities will be set. Probabilities for different levels of the
 *              N-gram can be set with this function. If level = 1, N-Gram is treated as UniGram, if level = 2,
 *              N-Gram is treated as Bigram, etc.
 *
 */
template<class Symbol> void InterpolatedSmoothing<Symbol>::setProbabilitiesWithLevel(NGram<Symbol>& nGram, int level) {
    GoodTuringSmoothing<string> goodTuringSmoothing;
    for (int j = 2; j<= nGram.getN(); j++){
        goodTuringSmoothing.setProbabilitiesWithLevel(nGram, j);
    }
    goodTuringSmoothing.setProbabilitiesWithLevel(nGram, 1);
    switch (nGram.getN()){
        case 2:
            nGram.setLambda(lambda1);
            break;
        case 3:
            nGram.setLambda(lambda1, lambda2);
            break;
    }
}

#endif //NGRAM_INTERPOLATEDSMOOTHING_H
