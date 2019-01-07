//
// Created by LMF319018 on 07/01/2019.
//

#ifndef NGRAM_NGRAM_H
#define NGRAM_NGRAM_H

#include "NGramNode.h"

template <class Symbol> class NGram{
private:
    NGramNode<Symbol> rootNode;
    int N = 0;
    double lambda1 = 0.0, lambda2 = 0.0;
    bool interpolated = false;
    unordered_set<Symbol> vocabulary;
    double* probabilityOfUnseen;
public:
    NGram(vector<vector<Symbol>> corpus, int N);
    NGram(int N);
    void setN(int N);
    int getN();
    void addNGram(Symbol* symbols, int size);
    void addNGramSentence(Symbol* symbols, int size);
    double vocabularySize();
    void setLambda(double lambda1);
    void setLambda(double lambda1, double lambda2);
    ~NGram();
};

template<class Symbol> NGram<Symbol>::~NGram() {
    delete probabilityOfUnseen;
}

template<class Symbol> NGram<Symbol>::NGram(vector<vector<Symbol>> corpus, int N) {
    int i;
    this->N = N;
    probabilityOfUnseen = new double[N];
    rootNode = NGramNode<Symbol>{};
    for (i = 0; i < corpus.size(); i++){
        Symbol* data = corpus.at(i).data();
        addNGramSentence(data, corpus.at(i).size());
    }
}

template<class Symbol> NGram<Symbol>::NGram(int N) {
    this->N = N;
    probabilityOfUnseen = new double[N];
    rootNode = NGramNode<Symbol>{};
}

template<class Symbol> int NGram<Symbol>::getN() {
    return N;
}

template<class Symbol> void NGram<Symbol>::setN(int N) {
    this->N = N;
}

template<class Symbol> void NGram<Symbol>::addNGram(Symbol *symbols, int size) {
    for (int i = 0; i < size; i++){
        vocabulary.add(symbols[i]);
    }
    rootNode.addNGram(symbols, 0, N);
}

template<class Symbol> void NGram<Symbol>::addNGramSentence(Symbol *symbols, int size) {
    for (int i = 0; i < size; i++){
        vocabulary.add(symbols[i]);
    }
    for (int j = 0; j < size - N + 1; j++){
        rootNode.addNGram(symbols, j, N);
    }
}

template<class Symbol> double NGram<Symbol>::vocabularySize() {
    return vocabulary.size();
}

template<class Symbol> void NGram<Symbol>::setLambda(double lambda1) {
    if (N == 2){
        interpolated = true;
        this->lambda1 = lambda1;
    }
}

template<class Symbol> void NGram<Symbol>::setLambda(double lambda1, double lambda2) {
    if (N == 3){
        interpolated = true;
        this->lambda1 = lambda1;
        this->lambda2 = lambda2;
    }
}

#endif //NGRAM_NGRAM_H
