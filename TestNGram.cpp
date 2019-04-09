//
// Created by LMF319018 on 07/01/2019.
//
#include "NGramNode.h"
#include "NGram.h"
#include <fstream>

int main(){
    string sentence1[] = {"ali", "topu", "at"};
    string sentence2[] = {"fatma", "topu", "aliye", "at"};
    string sentence3[] = {"ali", "topu", "fatmaya", "at"};
    string sentence4[] = {"ali", "at", "topu", "aliye"};
    string sentence5[] = {"fatma", "at", "topu", "fatmaya"};
    NGram<string> nGram = NGram<string>(2);
    nGram.addNGramSentence(sentence1, 3);
    nGram.addNGramSentence(sentence2, 4);
    nGram.addNGramSentence(sentence3, 4);
    nGram.addNGramSentence(sentence4, 4);
    nGram.addNGramSentence(sentence5, 4);
    nGram.save("ngram.txt");
    ifstream inputFile;
    inputFile.open("ngram.txt");
    NGram<string> nGram2 = NGram<string>(inputFile);
    inputFile.close();
    nGram2.save("ngram2.txt");
}