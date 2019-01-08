//
// Created by LMF319018 on 08/01/2019.
//

#ifndef NGRAM_LAPLACESMOOTHING_H
#define NGRAM_LAPLACESMOOTHING_H

#include "SimpleSmoothing.h"

template <class Symbol> class LaplaceSmoothing : public SimpleSmoothing<Symbol>{
private:
    double delta;
public:
    LaplaceSmoothing();
    explicit LaplaceSmoothing(double delta);
protected:
    void setProbabilities(NGram<Symbol> nGram, int level);
};

template<class Symbol> LaplaceSmoothing<Symbol>::LaplaceSmoothing() {
    delta = 1.0;
}

template<class Symbol> LaplaceSmoothing<Symbol>::LaplaceSmoothing(double delta) {
    this->delta = delta;
}

template<class Symbol> void LaplaceSmoothing<Symbol>::setProbabilities(NGram<Symbol> nGram, int level) {
    nGram.setProbabilityWithPseudoCount(delta, level);
}

#endif //NGRAM_LAPLACESMOOTHING_H
