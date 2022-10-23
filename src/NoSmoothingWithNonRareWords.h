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
    void setProbabilitiesWithLevel(NGram<Symbol>& nGram, int level);
};

/**
 * Constructor of {@link NoSmoothingWithNonRareWords}
 *
 * @param _probability
 */
template<class Symbol> NoSmoothingWithNonRareWords<Symbol>::NoSmoothingWithNonRareWords(double _probability) {
    this->probability = _probability;
}

/**
 * Wrapper function to set the N-gram probabilities with no smoothing and replacing unknown words not found in nonrare words.
 * @param nGram N-Gram for which the probabilities will be set.
 * @param level Level for which N-Gram probabilities will be set. Probabilities for different levels of the
 *              N-gram can be set with this function. If level = 1, N-Gram is treated as UniGram, if level = 2,
 *              N-Gram is treated as Bigram, etc.
 *
 */
template<class Symbol> void NoSmoothingWithNonRareWords<Symbol>::setProbabilitiesWithLevel(NGram<Symbol>& nGram, int level) {
    dictionary = nGram.constructDictionaryWithNonRareWords(level, probability);
    nGram.replaceUnknownWords(dictionary);
    nGram.setProbabilityWithPseudoCount(0.0, level);
}

#endif //NGRAM_NOSMOOTHINGWITHNONRAREWORDS_H
