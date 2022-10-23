//
// Created by LMF319018 on 08/01/2019.
//

#ifndef NGRAM_GOODTURINGSMOOTHING_H
#define NGRAM_GOODTURINGSMOOTHING_H

#include <Matrix.h>
#include <DeterminantZero.h>
#include <MatrixColumnMismatch.h>
#include "SimpleSmoothing.h"

template <class Symbol> class GoodTuringSmoothing : public SimpleSmoothing<Symbol>{
private:
    double* linearRegressionOnCountsOfCounts(const vector<int>& countsOfCounts) const;

public:
    void setProbabilitiesWithLevel(NGram<Symbol>& nGram, int level);
};

/**
 * Given counts of counts, this function will calculate the estimated counts of counts c$^*$ with
 * Good-Turing smoothing. First, the algorithm filters the non-zero counts from counts of counts array and constructs
 * c and r arrays. Then it constructs Z_n array with Z_n = (2C_n / (r_{n+1} - r_{n-1})). The algorithm then uses
 * simple linear regression on Z_n values to estimate w_1 and w_0, where log(N[i]) = w_1log(i) + w_0
 * @param countsOfCounts Counts of counts. countsOfCounts[1] is the number of words occurred once in the corpus.
 *                       countsOfCounts[i] is the number of words occurred i times in the corpus.
 * @return Estimated counts of counts array. N[1] is the estimated count for out of vocabulary words.
 */
template<class Symbol> double *GoodTuringSmoothing<Symbol>::linearRegressionOnCountsOfCounts(const vector<int>& countsOfCounts) const{
    auto* N = new double[countsOfCounts.size()];
    vector<int> r;
    vector<int> c;
    double xt, rt;
    for (int i = 1; i < countsOfCounts.size(); i++) {
        if (countsOfCounts[i] != 0) {
            r.push_back(i);
            c.push_back(countsOfCounts[i]);
        }
    }
    Matrix A = Matrix(2, 2);
    Vector y = Vector(2, 0);
    for (int i = 0; i < r.size(); i++){
        xt = log(r.at(i));
        if (i == 0){
            rt = log(c.at(i));
        } else {
            if (i == r.size() - 1){
                rt = log((1.0 * c.at(i)) / (r.at(i) - r.at(i - 1)));
            } else {
                rt = log((2.0 * c.at(i)) / (r.at(i + 1) - r.at(i - 1)));
            }
        }
        A.addValue(0, 0, 1.0);
        A.addValue(0, 1, xt);
        A.addValue(1, 0, xt);
        A.addValue(1, 1, xt * xt);
        y.addValue(0, rt);
        y.addValue(1, rt * xt);
    }
    try {
        A.inverse();
        Vector w = A.multiplyWithVectorFromRight(y);
        double w0 = w.getValue(0);
        double w1 = w.getValue(1);
        for (int i = 1; i < countsOfCounts.size(); i++){
            N[i] = exp(log(i) * w1 + w0);
        }
    } catch (DeterminantZero& determinantZero) {
    } catch (MatrixColumnMismatch& columnMismatch){
    }
    return N;
}

/**
 * Wrapper function to set the N-gram probabilities with Good-Turing smoothing. N[1] / \sum_{i=1}^infty N_i is
 * the out of vocabulary probability.
 * @param nGram N-Gram for which the probabilities will be set.
 * @param level Level for which N-Gram probabilities will be set. Probabilities for different levels of the
 *              N-gram can be set with this function. If level = 1, N-Gram is treated as UniGram, if level = 2,
 *              N-Gram is treated as Bigram, etc.
 */
template<class Symbol> void GoodTuringSmoothing<Symbol>::setProbabilitiesWithLevel(NGram<Symbol>& nGram, int level) {
    vector<int> countsOfCounts = nGram.calculateCountsOfCounts(level);
    double* N = linearRegressionOnCountsOfCounts(countsOfCounts);
    double sum = 0;
    for (int r = 1; r < countsOfCounts.size(); r++){
        sum += countsOfCounts[r] * r;
    }
    nGram.setAdjustedProbability(N, level, N[1] / sum);
    delete N;
}

#endif //NGRAM_GOODTURINGSMOOTHING_H
