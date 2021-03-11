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

template <class Symbol> class NGramNode{
private:
    map<Symbol, NGramNode<Symbol>*> children;
    Symbol symbol;
    int count = 0;
    double probability = 0.0;
    double probabilityOfUnseen = 0.0;
    NGramNode<Symbol>* unknown = nullptr;
    double childSum();
    void countWords(CounterHashMap<Symbol> wordCounter, int height);
    void replaceUnknownWords(unordered_set<Symbol> dictionary);
public:
    ~NGramNode();
    explicit NGramNode(Symbol symbol);
    NGramNode();
    NGramNode(bool isRootNode, istream &inputFile);
    void merge(NGramNode<Symbol>* toBeMerged);
    void addNGram(Symbol* s, int index, int height, int sentenceCount = 1);
    int getCount();
    int getCount(Symbol* s, int length, int index);
    unsigned long size();
    void updateCountsOfCounts(int* countsOfCounts, int height);
    void setAdjustedProbability(double* N, int height, double vocabularySize, double pZero);
    void setProbabilityWithPseudoCount(double pseudoCount, int height, double vocabularySize);
    int maximumOccurrence(int height);
    Symbol generateNextString(vector<Symbol> s, int index);
    double getUniGramProbability(Symbol w1);
    double getBiGramProbability(Symbol w1, Symbol w2);
    double getTriGramProbability(Symbol w1, Symbol w2, Symbol w3);
    void prune(double threshold, int N);
    void serialize(bool isRootNode, ostream &outputFile, int level);
};

/**
 * Constructor of {@link NGramNode}
 * @param symbol symbol to be kept in this node.
 */
template<class Symbol> NGramNode<Symbol>::NGramNode(Symbol symbol) {
    this->symbol = symbol;
    count = 0;
}

template<class Symbol> NGramNode<Symbol>::NGramNode() {
    count = 0;
}

/**
 * Gets count of this node.
 * @return count of this node.
 */
template<class Symbol> int NGramNode<Symbol>::getCount() {
    return count;
}

/**
 * Gets the size of children of this node.
 * @return size of children of {@link NGramNode} this node.
 */
template<class Symbol> unsigned long NGramNode<Symbol>::size() {
    return children.size();
}

/**
 * Finds maximum occurrence. If height is 0, returns the count of this node.
 * Otherwise, traverses this nodes' children recursively and returns maximum occurrence.
 * @param height height for NGram.
 * @return maximum occurrence.
 */
template<class Symbol> int NGramNode<Symbol>::maximumOccurrence(int height) {
    int current, max = 0;
    if (height == 0){
        return count;
    } else {
        for (auto const& it : children){
            NGramNode<Symbol>* child = it.second;
            current = child->maximumOccurrence(height - 1);
            if (current > max){
                max = current;
            }
        }
        return max;
    }
}

/**
 *
 * @return sum of counts of children nodes.
 */
template<class Symbol> double NGramNode<Symbol>::childSum() {
    double sum = 0;
    for (auto const& it : children){
        NGramNode<Symbol>* child = it.second;
        sum += child->count;
    }
    if (unknown != nullptr){
        sum += unknown->count;
    }
    return sum;
}

/**
 * Traverses nodes and updates counts of counts for each node.
 * @param countsOfCounts counts of counts of NGrams.
 * @param height height for NGram. if height = 1, If level = 1, N-Gram is treated as UniGram, if level = 2,
 *               N-Gram is treated as Bigram, etc.
 */
template<class Symbol> void NGramNode<Symbol>::updateCountsOfCounts(int* countsOfCounts, int height) {
    if (height == 0){
        countsOfCounts[count]++;
    } else {
        for (auto const& it : children){
            NGramNode<Symbol>* child = it.second;
            child->updateCountsOfCounts(countsOfCounts, height - 1);
        }
    }
}

/**
 * Sets probabilities by traversing nodes and adding pseudocount for each NGram.
 * @param pseudoCount pseudocount added to each NGram.
 * @param height height for NGram. if height = 1, If level = 1, N-Gram is treated as UniGram, if level = 2,
 *               N-Gram is treated as Bigram, etc.
 * @param vocabularySize size of vocabulary
 */
template<class Symbol> void NGramNode<Symbol>::setProbabilityWithPseudoCount(double pseudoCount, int height, double vocabularySize) {
    if (height == 1){
        double sum = childSum() + pseudoCount * vocabularySize;
        for (auto& it : children){
            NGramNode<Symbol>* child = it.second;
            child->probability = (child->count + pseudoCount) / sum;
        }
        if (unknown != nullptr){
            unknown->probability = (unknown->count + pseudoCount) / sum;
        }
        probabilityOfUnseen = pseudoCount / sum;
    } else {
        for (auto& it : children) {
            NGramNode<Symbol>* child = it.second;
            child->setProbabilityWithPseudoCount(pseudoCount, height - 1, vocabularySize);
        }
    }
}

/**
 * Sets adjusted probabilities with counts of counts of NGrams.
 * For count < 5, count is considered as ((r + 1) * N[r + 1]) / N[r]), otherwise, count is considered as it is.
 * Sum of children counts are computed. Then, probability of a child node is (1 - pZero) * (r / sum) if r > 5
 * otherwise, r is replaced with ((r + 1) * N[r + 1]) / N[r]) and calculated the same.
 * @param N counts of counts of NGrams.
 * @param height height for NGram. if height = 1, If level = 1, N-Gram is treated as UniGram, if level = 2,
 *               N-Gram is treated as Bigram, etc.
 * @param vocabularySize size of vocabulary.
 * @param pZero probability of zero.
 */
template<class Symbol> void NGramNode<Symbol>::setAdjustedProbability(double *N, int height, double vocabularySize, double pZero) {
    if (height == 1){
        double sum = 0;
        for (auto const& it : children){
            NGramNode<Symbol>* child = it.second;
            int r = child->count;
            if (r <= 5){
                double newR = ((r + 1) * N[r + 1]) / N[r];
                sum += newR;
            } else {
                sum += r;
            }
        }
        for (auto const& it : children){
            NGramNode<Symbol>* child = it.second;
            int r = child->count;
            if (r <= 5){
                double newR = ((r + 1) * N[r + 1]) / N[r];
                child->probability = (1 - pZero) * (newR / sum);
            } else {
                child->probability = (1 - pZero) * (r / sum);
            }
        }
        probabilityOfUnseen = pZero / (vocabularySize - children.size());
    } else {
        for (auto const& it : children){
            NGramNode<Symbol>* child = it.second;
            child->setAdjustedProbability(N, height - 1, vocabularySize, pZero);
        }
    }
}

/**
 * Adds NGram given as array of symbols to the node as a child.
 * @param s array of symbols
 * @param index  start index of NGram
 * @param height height for NGram. if height = 1, If level = 1, N-Gram is treated as UniGram, if level = 2,
 *               N-Gram is treated as Bigram, etc.
 */
template<class Symbol> void NGramNode<Symbol>::addNGram(Symbol *s, int index, int height, int sentenceCount) {
    NGramNode<Symbol>* child;
    if (height == 0){
        return;
    }
    Symbol symbol = s[index];
    if (children.size() != 0 && children.find(symbol) != children.end()){
        child = children.find(symbol)->second;
    } else {
        child = new NGramNode<Symbol>(symbol);
        children.emplace(symbol, child);
    }
    child->count += sentenceCount;
    child->addNGram(s, index + 1, height - 1, sentenceCount);
}

/**
 * Gets unigram probability of given symbol.
 * @param w1 unigram.
 * @return unigram probability of given symbol.
 */
template<class Symbol> double NGramNode<Symbol>::getUniGramProbability(Symbol w1) {
    if (children.find(w1) != children.end()){
        return children.find(w1)->second->probability;
    } else {
        if (unknown != nullptr){
            return unknown->probability;
        }
        return probabilityOfUnseen;
    }
}

/**
 * Gets bigram probability of given symbols w1 and w2
 * @param w1 first gram of bigram.
 * @param w2 second gram of bigram.
 * @return probability of given bigram
 * @throws UnseenCase
 */
template<class Symbol> double NGramNode<Symbol>::getBiGramProbability(Symbol w1, Symbol w2) {
    NGramNode<Symbol>* child;
    if (children.find(w1) != children.end()){
        child = children.find(w1)->second;
        return child->getUniGramProbability(w2);
    } else {
        if (unknown != nullptr){
            return unknown->getUniGramProbability(w2);
        }
        throw UnseenCase();
    }
}

/**
 * Gets trigram probability of given symbols w1, w2 and w3.
 * @param w1 first gram of trigram
 * @param w2 second gram of trigram
 * @param w3 third gram of trigram
 * @return probability of given trigram.
 * @throws UnseenCase
 */
template<class Symbol> double NGramNode<Symbol>::getTriGramProbability(Symbol w1, Symbol w2, Symbol w3) {
    NGramNode<Symbol>* child;
    if (children.find(w1) != children.end()){
        child = children.find(w1)->second;
        return child->getBiGramProbability(w2, w3);
    } else {
        if (unknown != nullptr){
            return unknown->getBiGramProbability(w2, w3);
        }
        throw UnseenCase();
    }
}

/**
 * Counts words recursively given height and wordCounter.
 * @param wordCounter word counter keeping symbols and their counts.
 * @param height height for NGram. if height = 1, If level = 1, N-Gram is treated as UniGram, if level = 2,
 *               N-Gram is treated as Bigram, etc.
 */
template<class Symbol> void NGramNode<Symbol>::countWords(CounterHashMap<Symbol> wordCounter, int height) {
    if (height == 0){
        wordCounter.putNTimes(symbol, count);
    } else {
        for (auto const& it : children){
            NGramNode* child = it.second;
            child->countWords(wordCounter, height - 1);
        }
    }
}

/**
 * Replace words not in given dictionary.
 * Deletes unknown words from children nodes and adds them to {@link NGramNode#unknown} unknown node as children recursively.
 *
 * @param dictionary dictionary of known words.
 */
template<class Symbol> void NGramNode<Symbol>::replaceUnknownWords(unordered_set<Symbol> dictionary) {
    if (children.size() != 0){
        vector<NGramNode<Symbol>*> childList;
        for (auto const& it : children){
            Symbol symbol = it.first;
            if (!dictionary.find(symbol) != dictionary.end()){
                childList.push_back(children.find(symbol).second);
            }
        }
        if (childList.size() > 0){
            unknown = new NGramNode<Symbol>();
            int sum = 0;
            for (NGramNode<Symbol>* child : childList){
                if (child->children.size() != 0){
                    unknown->children.putAll(child->children.begin(), child->children.end());
                }
                sum += child->count;
                children.erase(child->symbol);
            }
            unknown->count = sum;
            unknown->replaceUnknownWords(dictionary);
        }
        for (auto const& it : children){
            NGramNode<Symbol>* child = it.second;
            child->replaceUnknownWords(dictionary);
        }
    }

}

/**
 * Gets count of symbol given array of symbols and index of symbol in this array.
 * @param s array of symbols
 * @param index index of symbol whose count is returned
 * @return count of the symbol.
 */
template<class Symbol> int NGramNode<Symbol>::getCount(Symbol *s, int length, int index) {
    if (index < length){
        if (children.find(s[index]) != children.end()){
            return children.find(s[index])->second->getCount(s, length, index + 1);
        } else {
            return 0;
        }
    } else {
        return getCount();
    }
}

/**
 * Generates next string for given list of symbol and index
 * @param s list of symbol
 * @param index index index of generated string
 * @return generated string.
 */
template<class Symbol> Symbol NGramNode<Symbol>::generateNextString(vector<Symbol> s, int index) {
    double sum = 0.0, prob;
    if (index == s.size()){
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<>distribution (0.0, 1.0);
        prob = distribution(gen);
        for (auto const& it : children){
            NGramNode<Symbol>* node = it.second;
            if (prob < node->probability + sum){
                return node->symbol;
            } else {
                sum += node->probability;
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
    for (auto& iterator : children){
        delete iterator.second;
    }
}

template<class Symbol> void NGramNode<Symbol>::serialize(bool isRootNode, ostream &outputFile, int level) {
    if (!isRootNode){
        for (int i = 0; i < level; i++){
            outputFile << "\t";
        }
        outputFile << symbol << "\n";
    }
    for (int i = 0; i < level; i++){
        outputFile << "\t";
    }
    outputFile << count << " " << probability << " " << probabilityOfUnseen << " " << size() << "\n";
    for (auto& iterator : children){
        iterator.second->serialize(false, outputFile, level + 1);
    }
}

template<class Symbol> NGramNode<Symbol>::NGramNode(bool isRootNode, istream &inputFile){
    int numberOfChildren;
    if (!isRootNode){
        inputFile >> symbol;
    }
    inputFile >> count;
    inputFile >> probability;
    inputFile >> probabilityOfUnseen;
    inputFile >> numberOfChildren;
    unknown = nullptr;
    for (int i = 0; i < numberOfChildren; i++){
        auto* childNode = new NGramNode(false, inputFile);
        children.emplace(childNode->symbol, childNode);
    }
}

template<class Symbol> void NGramNode<Symbol>::prune(double threshold, int N) {
    if (N == 0){
        Symbol maxElement = Symbol();
        NGramNode<Symbol>* maxNode = nullptr;
        vector<Symbol> toBeDeleted;
        for (auto const& it : children){
            NGramNode<Symbol>* node = it.second;
            if (node->count / (count + 0.0) < threshold){
                toBeDeleted.emplace_back(it.first);
            }
            if (maxElement == Symbol() || node->count > children[maxElement]->count){
                maxElement = it.first;
                maxNode = node;
            }
        }
        for (Symbol symbol1 : toBeDeleted){
            children.erase(symbol1);
        }
        if (children.size() == 0){
            children[maxElement] = maxNode;
        }
    } else {
        for (auto const& it : children){
            NGramNode<Symbol>* node = it.second;
            node->prune(threshold, N - 1);
        }
    }
}

template<class Symbol> void NGramNode<Symbol>::merge(NGramNode<Symbol>* toBeMerged) {
    for (auto const& it : children){
        if (toBeMerged->children.find(it.first) != toBeMerged->children.end()){
            it.second->merge(toBeMerged->children[it.first]);
        }
    }
    for (auto const& it : toBeMerged->children){
        if (children.find(it.first) == children.end()){
            children[it.first] = toBeMerged->children[it.first];
        }
    }
    count += toBeMerged->getCount();
}

#endif //NGRAM_NGRAMNODE_H
