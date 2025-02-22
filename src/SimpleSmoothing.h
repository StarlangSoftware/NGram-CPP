//
// Created by LMF319018 on 08/01/2019.
//

#ifndef NGRAM_SIMPLESMOOTHING_H
#define NGRAM_SIMPLESMOOTHING_H

#include "NGram.h"

template <class Symbol> class SimpleSmoothing {
protected:
    virtual void setProbabilitiesWithLevel(NGram<Symbol>& nGram, int level) = 0;
public:
    void setProbabilities(NGram<Symbol>& nGram);
};

/**
 * Calculates the N-Gram probabilities with simple smoothing.
 * @param nGram N-Gram for which simple smoothing calculation is done.
 */
template<class Symbol> void SimpleSmoothing<Symbol>::setProbabilities(NGram<Symbol>& nGram) {
    setProbabilitiesWithLevel(nGram, nGram.getN());
}

#endif //NGRAM_SIMPLESMOOTHING_H
