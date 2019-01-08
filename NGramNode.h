//
// Created by LMF319018 on 07/01/2019.
//

#ifndef NGRAM_NGRAMNODE_H
#define NGRAM_NGRAMNODE_H

#include <map>
#include <random>
#include <CounterHashMap.h>
#include <unordered_set>
#include "UnseenCase.h"

using namespace std;

template <class Symbol> class NGramNode{
private:
    map<Symbol, NGramNode<Symbol>> children;
    Symbol symbol;
    int count = 0;
    double probability = 0.0;
    double probabilityOfUnseen = 0.0;
    NGramNode<Symbol>* unknown = nullptr;
    double childSum();
    void updateCountsOfCounts(int* countsOfCounts, int height);
    void setProbabilityWithPseudoCount(double pseudoCount, int height, double vocabularySize);
    void setAdjustedProbability(double* N, int height, double vocabularySize, double pZero);
    void addNGram(Symbol* s, int index, int height);
    double getUniGramProbability(Symbol w1);
    double getBiGramProbability(Symbol w1, Symbol w2);
    double getTriGramProbability(Symbol w1, Symbol w2, Symbol w3);
    void countWords(CounterHashMap<Symbol> wordCounter, int height);
    void replaceUnknownWords(unordered_set<Symbol> dictionary);
    int getCount(Symbol* s, int length, int index);
public:
    ~NGramNode();
    explicit NGramNode(Symbol symbol);
    NGramNode();
    int getCount();
    unsigned long size();
    int maximumOccurrence(int height);
    Symbol generateNextString(vector<Symbol> s, int index);
};

template<class Symbol> NGramNode<Symbol>::NGramNode(Symbol symbol) {
    this->symbol = symbol;
    count = 0;
}

template<class Symbol> NGramNode<Symbol>::NGramNode() {
    count = 0;
}

template<class Symbol> int NGramNode<Symbol>::getCount() {
    return count;
}

template<class Symbol> unsigned long NGramNode<Symbol>::size() {
    return children.size();
}

template<class Symbol> int NGramNode<Symbol>::maximumOccurrence(int height) {
    int current, max = 0;
    if (height == 0){
        return count;
    } else {
        for (auto const& it : children){
            NGramNode child = it.second;
            current = child.maximumOccurrence(height - 1);
            if (current > max){
                max = current;
            }
        }
        return max;
    }
}

template<class Symbol> double NGramNode<Symbol>::childSum() {
    double sum = 0;
    for (auto const& it : children){
        NGramNode child = it.second;
        sum += child.count;
    }
    if (unknown != nullptr){
        sum += unknown->count;
    }
    return sum;
}

template<class Symbol> void NGramNode<Symbol>::updateCountsOfCounts(int* countsOfCounts, int height) {
    if (height == 0){
        countsOfCounts[count]++;
    } else {
        for (auto const& it : children){
            NGramNode child = it.second;
            child.updateCountsOfCounts(countsOfCounts, height - 1);
        }
    }
}

template<class Symbol> void NGramNode<Symbol>::setProbabilityWithPseudoCount(double pseudoCount, int height, double vocabularySize) {
    if (height == 1){
        double sum = childSum() + pseudoCount * vocabularySize;
        for (auto const& it : children){
            NGramNode& child = it.second;
            child.probability = (child.count + pseudoCount) / sum;
        }
        if (unknown != nullptr){
            unknown->probability = (unknown->count + pseudoCount) / sum;
        }
        probabilityOfUnseen = pseudoCount / sum;
    } else {
        for (auto const& it : children) {
            NGramNode child = it.second;
            child.setProbabilityWithPseudoCount(pseudoCount, height - 1, vocabularySize);
        }
    }
}

template<class Symbol> void NGramNode<Symbol>::setAdjustedProbability(double *N, int height, double vocabularySize, double pZero) {
    if (height == 1){
        double sum = 0;
        for (auto const& it : children){
            NGramNode child = it.second;
            int r = child.count;
            if (r <= 5){
                double newR = ((r + 1) * N[r + 1]) / N[r];
                sum += newR;
            } else {
                sum += r;
            }
        }
        for (auto const& it : children){
            NGramNode& child = it.second;
            int r = child.count;
            if (r <= 5){
                double newR = ((r + 1) * N[r + 1]) / N[r];
                child.probability = (1 - pZero) * (newR / sum);
            } else {
                child.probability = (1 - pZero) * (r / sum);
            }
        }
        probabilityOfUnseen = pZero / (vocabularySize - children.size());
    } else {
        for (auto const& it : children){
            NGramNode child = it.second;
            child.setAdjustedProbability(N, height - 1, vocabularySize, pZero);
        }
    }
}

template<class Symbol> void NGramNode<Symbol>::addNGram(Symbol *s, int index, int height) {
    NGramNode<Symbol> child;
    if (height == 0){
        return;
    }
    Symbol symbol = s[index];
    if (children.size() != 0 && children.find(symbol) != children.end()){
        child = children.find(symbol).second;
    } else {
        child = new NGramNode<Symbol>(symbol);
        children.emplace(symbol, child);
    }
    child.count++;
    child.addNGram(s, index + 1, height - 1);
}

template<class Symbol> double NGramNode<Symbol>::getUniGramProbability(Symbol w1) {
    if (children.find(w1) != children.end()){
        return children.find(w1).second.probability;
    } else {
        if (unknown != nullptr){
            return unknown->probability;
        }
        return probabilityOfUnseen;
    }
}

template<class Symbol> double NGramNode<Symbol>::getBiGramProbability(Symbol w1, Symbol w2) {
    NGramNode<Symbol> child;
    if (children.find(w1) != children.end()){
        child = children.find(w1).second;
        return child.getUniGramProbability(w2);
    } else {
        if (unknown != nullptr){
            return unknown->getUniGramProbability(w2);
        }
        throw UnseenCase();
    }
}

template<class Symbol> double NGramNode<Symbol>::getTriGramProbability(Symbol w1, Symbol w2, Symbol w3) {
    NGramNode<Symbol> child;
    if (children.find(w1) != children.end()){
        child = children.find(w1).second;
        return child.getBiGramProbability(w2, w3);
    } else {
        if (unknown != nullptr){
            return unknown->getBiGramProbability(w2, w3);
        }
        throw UnseenCase();
    }
}

template<class Symbol> void NGramNode<Symbol>::countWords(CounterHashMap<Symbol> wordCounter, int height) {
    if (height == 0){
        wordCounter.putNTimes(symbol, count);
    } else {
        for (auto const& it : children){
            NGramNode child = it.second;
            child.countWords(wordCounter, height - 1);
        }
    }
}

template<class Symbol> void NGramNode<Symbol>::replaceUnknownWords(unordered_set<Symbol> dictionary) {
    if (children.size() != 0){
        vector<NGramNode<Symbol>> childList;
        for (auto const& it : children){
            Symbol symbol = it.first;
            if (!dictionary.find(symbol) != dictionary.end()){
                childList.push_back(children.find(symbol).second);
            }
        }
        if (childList.size() > 0){
            unknown = new NGramNode<Symbol>();
            int sum = 0;
            for (NGramNode<Symbol> child : childList){
                if (child.children.size() != 0){
                    unknown->children.putAll(child.children.begin(), child.children.end());
                }
                sum += child.count;
                children.erase(child.symbol);
            }
            unknown->count = sum;
            unknown->replaceUnknownWords(dictionary);
        }
        for (auto const& it : children){
            NGramNode child = it.second;
            child.replaceUnknownWords(dictionary);
        }
    }

}

template<class Symbol> int NGramNode<Symbol>::getCount(Symbol *s, int length, int index) {
    if (index < length){
        if (children.find(s[index]) != children.end()){
            return children.find(s[index]).second.getCount(s, index + 1);
        } else {
            return 0;
        }
    } else {
        return getCount();
    }
}

template<class Symbol> Symbol NGramNode<Symbol>::generateNextString(vector<Symbol> s, int index) {
    double sum = 0.0, prob;
    if (index == s.size()){
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<>distribution (0.0, 1.0);
        prob = distribution(gen);
        for (auto const& it : children){
            NGramNode node = it.second;
            if (prob < node.probability + sum){
                return node.symbol;
            } else {
                sum += node.probability;
            }
        }
    } else {
        return children.find(s.get(index)).second.generateNextString(s, index + 1);
    }
}

template<class Symbol> NGramNode<Symbol>::~NGramNode() {
    if (unknown != nullptr){
        delete unknown;
    }
}

#endif //NGRAM_NGRAMNODE_H
