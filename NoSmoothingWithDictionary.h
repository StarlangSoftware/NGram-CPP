//
// Created by LMF319018 on 08/01/2019.
//

#ifndef NGRAM_NOSMOOTHINGWITHDICTIONARY_H
#define NGRAM_NOSMOOTHINGWITHDICTIONARY_H

#include "NoSmoothing.h"

template <class Symbol> class NoSmoothingWithDictionary : public NoSmoothing<Symbol>{
private:
    unordered_set<Symbol> dictionary;
public:
    NoSmoothingWithDictionary(unordered_set<Symbol> dictionary);
protected:
    void setProbabilities(NGram<Symbol> nGram, int level);
};

template<class Symbol> NoSmoothingWithDictionary<Symbol>::NoSmoothingWithDictionary(unordered_set<Symbol> dictionary) {
    this->dictionary = dictionary;
}

template<class Symbol> void NoSmoothingWithDictionary<Symbol>::setProbabilities(NGram<Symbol> nGram, int level) {
    nGram.replaceUnknownWords(dictionary);
    nGram.setProbabilityWithPseudoCount(0.0, level);
}

#endif //NGRAM_NOSMOOTHINGWITHDICTIONARY_H
