//
// Created by LMF319018 on 08/01/2019.
//

#ifndef NGRAM_SIMPLESMOOTHING_H
#define NGRAM_SIMPLESMOOTHING_H

#include "NGram.h"

template <class Symbol> class SimpleSmoothing {
protected:
    virtual void setProbabilities(NGram<Symbol> nGram, int level) = 0;
    void setProbabilities(NGram<Symbol> nGram);
};

template<class Symbol> void SimpleSmoothing<Symbol>::setProbabilities(NGram<Symbol> nGram) {
    setProbabilities(nGram, nGram.getN());
}

#endif //NGRAM_SIMPLESMOOTHING_H
