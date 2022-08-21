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
    explicit NoSmoothingWithDictionary(unordered_set<Symbol> dictionary);
protected:
    void setProbabilitiesWithLevel(NGram<Symbol>& nGram, int level);
};

/**
 * No argument constructor of {@link NoSmoothingWithDictionary}
 * @param dictionary
 */
template<class Symbol> NoSmoothingWithDictionary<Symbol>::NoSmoothingWithDictionary(unordered_set<Symbol> dictionary) {
    this->dictionary = dictionary;
}

/**
 * Wrapper function to set the N-gram probabilities with no smoothing and replacing unknown words not found in {@link HashSet} the dictionary.
 * @param nGram N-Gram for which the probabilities will be set.
 * @param level Level for which N-Gram probabilities will be set. Probabilities for different levels of the
 *              N-gram can be set with this function. If level = 1, N-Gram is treated as UniGram, if level = 2,
 *              N-Gram is treated as Bigram, etc.
 *
 */
template<class Symbol> void NoSmoothingWithDictionary<Symbol>::setProbabilitiesWithLevel(NGram<Symbol>& nGram, int level) {
    nGram.replaceUnknownWords(dictionary);
    nGram.setProbabilityWithPseudoCount(0.0, level);
}

#endif //NGRAM_NOSMOOTHINGWITHDICTIONARY_H
