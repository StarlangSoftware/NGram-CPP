//
// Created by LMF319018 on 07/01/2019.
//

#ifndef NGRAM_NGRAM_H
#define NGRAM_NGRAM_H

#include <vector>
#include <iostream>
#include <fstream>
#include "NGramNode.h"

template <class Symbol> class NGram{
private:
    NGramNode<Symbol>* rootNode = nullptr;
    int N = 0;
    double lambda1 = 0.0, lambda2 = 0.0;
    bool interpolated = false;
    unordered_set<Symbol> vocabulary;
    double* probabilityOfUnseen;
    void loadNGram(ifstream &inputFile);
    int maximumOccurrence(int height);
    void updateCountsOfCounts(int* countsOfCounts, int height);
    double getUniGramPerplexity(vector<vector<Symbol>> corpus);
    double getBiGramPerplexity(vector<vector<Symbol>> corpus);
    double getTriGramPerplexity(vector<vector<Symbol>> corpus);
    double getUniGramProbability(Symbol w1);
    double getBiGramProbability(Symbol w1, Symbol w2);
    double getTriGramProbability(Symbol w1, Symbol w2, Symbol w3);
public:
    NGram(vector<vector<Symbol>> corpus, int N);
    explicit NGram(string fileName);
    explicit NGram(int N);
    explicit NGram(ifstream &inputFile);
    void merge(NGram<Symbol>& toBeMerged);
    void setN(int N);
    int getN();
    void addNGram(Symbol* symbols, int size);
    void addNGramSentence(Symbol* symbols, int size, int sentenceCount = 1);
    double vocabularySize();
    void setLambda(double lambda1);
    void setLambda(double lambda1, double lambda2);
    void setProbabilityWithPseudoCount(double pseudoCount, int height);
    void replaceUnknownWords(unordered_set<Symbol> dictionary);
    unordered_set<Symbol> constructDictionaryWithNonRareWords(int level, double probability);
    vector<int> calculateCountsOfCounts(int height);
    double getPerplexity(vector<vector<Symbol>> corpus);
    double getProbability(initializer_list<Symbol> symbols);
    int getCount(Symbol* symbols, int length);
    void setAdjustedProbability(double* countsOfCounts, int height, double pZero);
    ~NGram();
    void prune(double threshold);
    void serialize(ostream& outputFile);
    void save(const string &fileName);
};

template<class Symbol> NGram<Symbol>::~NGram() {
    delete rootNode;
    delete probabilityOfUnseen;
}

/**
 * Constructor of {@link NGram} class which takes a {@link vector<vector<Symbol>>} corpus and {@link Integer} size of ngram as input.
 * It adds all sentences of corpus as ngrams.
 *
 * @param corpus {@link vector<vector<Symbol>>} list of sentences whose ngrams are added.
 * @param N size of ngram.
 */
template<class Symbol> NGram<Symbol>::NGram(vector<vector<Symbol>> corpus, int N) {
    int i;
    this->N = N;
    probabilityOfUnseen = new double[N];
    for (int i = 0; i < N; i++){
        probabilityOfUnseen[i] = 0.0;
    }
    rootNode = new NGramNode<Symbol>(Symbol());
    for (i = 0; i < corpus.size(); i++){
        Symbol* data = corpus.at(i).data();
        addNGramSentence(data, corpus.at(i).size());
    }
}

/**
 * Constructor of {@link NGram} class which takes {@link Integer} size of ngram.
 *
 * @param N size of ngram.
 */
template<class Symbol> NGram<Symbol>::NGram(int N) {
    this->N = N;
    probabilityOfUnseen = new double[N];
    for (int i = 0; i < N; i++){
        probabilityOfUnseen[i] = 0.0;
    }
    rootNode = new NGramNode<Symbol>(Symbol());
}

/**
 *
 * @return size of ngram.
 */
template<class Symbol> int NGram<Symbol>::getN() {
    return N;
}

/**
 * Set size of ngram.
 * @param N size of ngram
 */
template<class Symbol> void NGram<Symbol>::setN(int N) {
    this->N = N;
}

/**
 * Adds {@link Symbol*} given array of symbols to {@link unordered_set} the vocabulary and to {@link NGramNode} the rootNode
 *
 * @param symbols {@link Symbol*} ngram added.
 * @param size size of symbols.
 */
template<class Symbol> void NGram<Symbol>::addNGram(Symbol *symbols, int size) {
    for (int i = 0; i < size; i++){
        vocabulary.emplace(symbols[i]);
    }
    rootNode->addNGram(symbols, 0, N);
}

/**
 * Adds given sentence count times to {@link unordered_set} the vocabulary and create and add ngrams of the sentence to {@link NGramNode} the rootNode
 *
 * @param symbols {@link Symbol*} sentence whose ngrams are added.
 * @param size size of symbols.
 * @param count Number of times this sentence is added.
 */
template<class Symbol> void NGram<Symbol>::addNGramSentence(Symbol *symbols, int size, int sentenceCount) {
    for (int i = 0; i < size; i++){
        vocabulary.emplace(symbols[i]);
    }
    for (int j = 0; j < size - N + 1; j++){
        rootNode->addNGram(symbols, j, N, sentenceCount);
    }
}

/**
 * @return vocabulary size.
 */
template<class Symbol> double NGram<Symbol>::vocabularySize() {
    return vocabulary.size();
}

/**
 * Sets lambda, interpolation ratio, for bigram and unigram probabilities.
 *
 * ie. lambda1 * bigramProbability + (1 - lambda1) * unigramProbability
 *
 * @param lambda1 interpolation ratio for bigram probabilities
 */
template<class Symbol> void NGram<Symbol>::setLambda(double lambda1) {
    if (N == 2){
        interpolated = true;
        this->lambda1 = lambda1;
    }
}

/**
 * Sets lambdas, interpolation ratios, for trigram, bigram and unigram probabilities.
 * ie. lambda1 * trigramProbability + lambda2 * bigramProbability  + (1 - lambda1 - lambda2) * unigramProbability
 *
 * @param lambda1 interpolation ratio for trigram probabilities
 * @param lambda2 interpolation ratio for bigram probabilities
 */
template<class Symbol> void NGram<Symbol>::setLambda(double lambda1, double lambda2) {
    if (N == 3){
        interpolated = true;
        this->lambda1 = lambda1;
        this->lambda2 = lambda2;
    }
}

/**
 * Sets probabilities by adding pseudocounts given height and pseudocount.
 * @param pseudoCount pseudocount added to all N-Grams.
 * @param height  height for N-Gram. If height= 1, N-Gram is treated as UniGram, if height = 2,
 *                N-Gram is treated as Bigram, etc.
 */
template<class Symbol> void NGram<Symbol>::setProbabilityWithPseudoCount(double pseudoCount, int height) {
    double sizeOfVocabulary;
    if (pseudoCount != 0){
        sizeOfVocabulary = vocabularySize() + 1;
    } else {
        sizeOfVocabulary = vocabularySize();
    }
    rootNode->setProbabilityWithPseudoCount(pseudoCount, height, sizeOfVocabulary);
    probabilityOfUnseen[height - 1] = 1.0 / sizeOfVocabulary;
}

/**
 * Replaces words not in {@link unordered_set} given dictionary.
 *
 * @param dictionary dictionary of known words.
 */
template<class Symbol> void NGram<Symbol>::replaceUnknownWords(unordered_set <Symbol> dictionary) {
    rootNode->replaceUnknownWords(dictionary);
}

/**
 * Constructs a dictionary of nonrare words with given N-Gram level and probability threshold.
 *
 * @param level Level for counting words. Counts for different levels of the N-Gram can be set. If level = 1, N-Gram is treated as UniGram, if level = 2,
 *              N-Gram is treated as Bigram, etc.
 * @param probability probability threshold for nonrare words.
 * @return {@link unordered_set} nonrare words.
 */
template<class Symbol> unordered_set<Symbol> NGram<Symbol>::constructDictionaryWithNonRareWords(int level, double probability) {
    unordered_set<Symbol> result;
    CounterHashMap<Symbol> wordCounter;
    rootNode->countWords(wordCounter, level);
    int sum = wordCounter.sumOfCounts();
    for (auto const& it : wordCounter){
        if (it.second / (sum + 0.0) > probability){
            result.emplace(it.first);
        }
    }
    return result;
}

/**
 * Calculates counts of counts of NGrams.
 * @param height  height for NGram. If height = 1, N-Gram is treated as UniGram, if height = 2,
 *                N-Gram is treated as Bigram, etc.
 * @return counts of counts of NGrams.
 */
template<class Symbol> vector<int> NGram<Symbol>::calculateCountsOfCounts(int height) {
    int maxCount;
    maxCount = maximumOccurrence(height);
    int* countsOfCounts = new int[maxCount + 2];
    for (int i = 0; i < maxCount + 2; i++){
        countsOfCounts[i] = 0;
    }
    updateCountsOfCounts(countsOfCounts, height);
    vector<int> result(maxCount + 2);
    for (int i = 0; i < maxCount + 2; i++){
        result[i] = countsOfCounts[i];
    }
    return result;
}

/**
 * Find maximum occurrence in given height.
 * @param height height for occurrences. If height = 1, N-Gram is treated as UniGram, if height = 2,
 *               N-Gram is treated as Bigram, etc.
 * @return maximum occurrence in given height.
 */
template<class Symbol> int NGram<Symbol>::maximumOccurrence(int height) {
    return rootNode->maximumOccurrence(height);
}

/**
 * Update counts of counts of N-Grams with given counts of counts and given height.
 * @param countsOfCounts updated counts of counts.
 * @param height  height for NGram. If height = 1, N-Gram is treated as UniGram, if height = 2,
 *                N-Gram is treated as Bigram, etc.
 */
template<class Symbol> void NGram<Symbol>::updateCountsOfCounts(int* countsOfCounts, int height) {
    rootNode->updateCountsOfCounts(countsOfCounts, height);
}

/**
 * Calculates the perplexity of given corpus depending on N-Gram model (unigram, bigram, trigram, etc.)
 *
 * @param corpus corpus whose perplexity is calculated.
 * @return perplexity of given corpus
 */
template<class Symbol> double NGram<Symbol>::getPerplexity(vector<vector<Symbol>> corpus) {
    switch (N){
        case 1:
            return getUniGramPerplexity(corpus);
        case 2:
            return getBiGramPerplexity(corpus);
        case 3:
            return getTriGramPerplexity(corpus);
        default:
            return 0;
    }
}

/**
 * Calculates unigram perplexity of given corpus. First sums negative log likelihoods of all unigrams in corpus.
 * Then returns exp of average negative log likelihood.
 *
 * @param corpus corpus whose unigram perplexity is calculated.
 *
 * @return unigram perplexity of corpus.
 */
template<class Symbol> double NGram<Symbol>::getUniGramPerplexity(vector<vector<Symbol>> corpus) {
    double sum = 0;
    int count = 0;
    for (int i = 0; i < corpus.size(); i++){
        for (int j = 0; j < corpus.at(i).size(); j++){
            double p = getProbability({corpus.at(i).at(j)});
            sum -= log(p);
            count++;
        }
    }
    return exp(sum / count);
}

/**
 * Calculates bigram perplexity of given corpus. First sums negative log likelihoods of all bigrams in corpus.
 * Then returns exp of average negative log likelihood.
 *
 * @param corpus corpus whose bigram perplexity is calculated.
 *
 * @return bigram perplexity of given corpus.
 */
template<class Symbol> double NGram<Symbol>::getBiGramPerplexity(vector<vector<Symbol>> corpus) {
    double sum = 0;
    int count = 0;
    for (int i = 0; i < corpus.size(); i++){
        for (int j = 0; j < corpus.at(i).size() - 1; j++){
            double p = getProbability({corpus.at(i).at(j), corpus.at(i).at(j + 1)});
            if (p == 0){
                cout << "Zero probability";
            }
            sum -= log(p);
            count++;
        }
    }
    return exp(sum / count);
}

/**
 * Calculates trigram perplexity of given corpus. First sums negative log likelihoods of all trigrams in corpus.
 * Then returns exp of average negative log likelihood.
 *
 * @param corpus corpus whose trigram perplexity is calculated.
 * @return trigram perplexity of given corpus.
 */
template<class Symbol> double NGram<Symbol>::getTriGramPerplexity(vector<vector<Symbol>> corpus) {
    double sum = 0;
    int count = 0;
    for (int i = 0; i < corpus.size(); i++){
        for (int j = 0; j < corpus.at(i).size() - 2; j++){
            double p = getProbability({corpus.at(i).at(j), corpus.at(i).at(j + 1), corpus.at(i).at(j + 2)});
            sum -= log(p);
            count++;
        }
    }
    return exp(sum / count);
}

/**
 * Gets probability of sequence of symbols depending on N in N-Gram. If N is 1, returns unigram probability.
 * If N is 2, if interpolated is true, then returns interpolated bigram and unigram probability, otherwise returns only bigram probability.
 * If N is 3, if interpolated is true, then returns interpolated trigram, bigram and unigram probability, otherwise returns only trigram probability.
 * @param symbols sequence of symbol.
 * @return probability of given sequence.
 */
template<class Symbol> double NGram<Symbol>::getProbability(initializer_list<Symbol> symbols) {
    switch (N){
        case 1:
            return getUniGramProbability(*(symbols.begin()));
        case 2:
            if (symbols.size() == 1){
                return getUniGramProbability(*(symbols.begin()));
            }
            if (interpolated){
                return lambda1 * getBiGramProbability(*(symbols.begin()), *(symbols.begin() + 1)) + (1 - lambda1) * getUniGramProbability(*(symbols.begin() + 1));
            } else {
                return getBiGramProbability(*(symbols.begin()), *(symbols.begin() + 1));
            }
        case 3:
            if (symbols.size() == 1){
                return getUniGramProbability(*(symbols.begin()));
            } else {
                if (symbols.size() == 2){
                    return getBiGramProbability(*(symbols.begin()), *(symbols.begin() + 1));
                }
            }
            if (interpolated){
                return lambda1 * getTriGramProbability(*(symbols.begin()), *(symbols.begin() + 1), *(symbols.begin() + 2)) + lambda2 * getBiGramProbability(*(symbols.begin() + 1), *(symbols.begin() + 2)) + (1 - lambda1 - lambda2) * getUniGramProbability(*(symbols.begin() + 2));
            } else {
                return getTriGramProbability(*(symbols.begin()), *(symbols.begin() + 1), *(symbols.begin() + 2));
            }
    }
    return 0.0;
}

/**
 * Gets unigram probability of given symbol.
 * @param w1 a unigram symbol.
 * @return probability of given unigram.
 */
template<class Symbol> double NGram<Symbol>::getUniGramProbability(Symbol w1) {
    return rootNode->getUniGramProbability(w1);
}

/**
 * Gets bigram probability of given symbols.
 * @param w1 first gram of bigram
 * @param w2 second gram of bigram
 * @return probability of bigram formed by w1 and w2.
 */
template<class Symbol> double NGram<Symbol>::getBiGramProbability(Symbol w1, Symbol w2) {
    try {
        return rootNode->getBiGramProbability(w1, w2);
    } catch (UnseenCase& unseenCase) {
        return probabilityOfUnseen[1];
    }
}

/**
 * Gets trigram probability of given symbols.
 * @param w1 first gram of trigram
 * @param w2 second gram of trigram
 * @param w3 third gram of trigram
 * @return probability of bigram formed by w1, w2, w3.
 */
template<class Symbol> double NGram<Symbol>::getTriGramProbability(Symbol w1, Symbol w2, Symbol w3) {
    try {
        return rootNode->getTriGramProbability(w1, w2, w3);
    } catch (UnseenCase& unseenCase) {
        return probabilityOfUnseen[2];
    }
}

/**
 * Gets count of given sequence of symbol.
 * @param symbols sequence of symbol.
 * @return count of symbols.
 */
template<class Symbol> int NGram<Symbol>::getCount(Symbol *symbols, int length) {
    return rootNode->getCount(symbols, length, 0);
}

/**
 * Sets probability with given counts of counts and pZero.
 * @param countsOfCounts counts of counts of NGrams.
 * @param height  height for NGram. If height = 1, N-Gram is treated as UniGram, if height = 2,
 *                N-Gram is treated as Bigram, etc.
 * @param pZero probability of zero.
 */
template<class Symbol>
void NGram<Symbol>::setAdjustedProbability(double* countsOfCounts, int height, double pZero) {
    rootNode->setAdjustedProbability(countsOfCounts, height, vocabularySize() + 1, pZero);
    probabilityOfUnseen[height - 1] = 1.0 / (vocabularySize() + 1);
}

template<class Symbol> void NGram<Symbol>::serialize(ostream &outputFile) {
    outputFile << N << " " << lambda1 << " " << lambda2 << "\n";
    for (int i = 0; i < N; i++){
        outputFile << probabilityOfUnseen[i] << " ";
    }
    outputFile << "\n";
    outputFile << vocabularySize() << "\n";
    for (auto & item : vocabulary){
        outputFile << item << "\n";
    }
    rootNode->serialize(true, outputFile, 0);
}

/**
 * Save this NGram to a file.
 *
 * @param fileName {@link String} name of file where NGram is saved.
 */
template<class Symbol> void NGram<Symbol>::save(const string &fileName){
    ofstream outputFile;
    outputFile.open(fileName, ofstream::out);
    serialize(outputFile);
    outputFile.close();
}

template<class Symbol> void NGram<Symbol>::loadNGram(ifstream &inputFile){
    Symbol s;
    int vocabularySize;
    inputFile >> N;
    inputFile >> lambda1;
    inputFile >> lambda2;
    probabilityOfUnseen = new double[N];
    for (int i = 0; i < N; i++){
        inputFile >> probabilityOfUnseen[i];
    }
    inputFile >> vocabularySize;
    for (int i = 0; i < vocabularySize; i++){
        inputFile >> s;
        vocabulary.emplace(s);
    }
    rootNode = new NGramNode<Symbol>(true, inputFile);
}

template<class Symbol>NGram<Symbol>::NGram(ifstream &inputFile) {
    loadNGram(inputFile);
}

template<class Symbol>NGram<Symbol>::NGram(string fileName) {
    ifstream inputFile;
    inputFile.open(fileName, ifstream::in);
    loadNGram(inputFile);
    inputFile.close();
}

template<class Symbol> void NGram<Symbol>::prune(double threshold) {
    if (threshold > 0.0 && threshold <= 1.0){
        rootNode->prune(threshold, N - 1);
    }
}

template<class Symbol> void NGram<Symbol>::merge(NGram<Symbol>& toBeMerged) {
    if (N != toBeMerged.getN()){
        return;
    }
    vocabulary.template merge(toBeMerged.vocabulary);
    rootNode->merge(toBeMerged.rootNode);
}

#endif //NGRAM_NGRAM_H
