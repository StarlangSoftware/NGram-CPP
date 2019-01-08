//
// Created by LMF319018 on 08/01/2019.
//

#ifndef NGRAM_NOSMOOTHING_H
#define NGRAM_NOSMOOTHING_H

#include "SimpleSmoothing.h"

template <class Symbol> class NoSmoothing : public SimpleSmoothing<Symbol> {
protected:
    void setProbabilities(NGram<Symbol> nGram, int level);
};

template<class Symbol> void NoSmoothing<Symbol>::setProbabilities(NGram<Symbol> nGram, int level) {
    nGram.setProbabilityWithPseudoCount(0.0, level);
}

#endif //NGRAM_NOSMOOTHING_H
