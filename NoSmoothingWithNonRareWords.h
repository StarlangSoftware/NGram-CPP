//
// Created by LMF319018 on 08/01/2019.
//

#ifndef NGRAM_NOSMOOTHINGWITHNONRAREWORDS_H
#define NGRAM_NOSMOOTHINGWITHNONRAREWORDS_H

#include "NoSmoothing.h"

template <class Symbol> class NoSmoothingWithNonRareWords : public NoSmoothing<Symbol>{
private:
    unordered_set<Symbol> dictionary;
    double probability;
public:
    explicit NoSmoothingWithNonRareWords(double probability);
protected:
    void setProbabilities(NGram<Symbol> nGram, int level);
};

template<class Symbol> NoSmoothingWithNonRareWords<Symbol>::NoSmoothingWithNonRareWords(double probability) {
    this->probability = probability;
}

template<class Symbol> void NoSmoothingWithNonRareWords<Symbol>::setProbabilities(NGram<Symbol> nGram, int level) {
    dictionary = nGram.constructDictionaryWithNonRareWords(level, probability);
    nGram.replaceUnknownWords(dictionary);
    nGram.setProbabilityWithPseudoCount(0.0, level);
}

#endif //NGRAM_NOSMOOTHINGWITHNONRAREWORDS_H
