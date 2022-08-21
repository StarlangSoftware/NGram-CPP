//
// Created by Olcay Taner YILDIZ on 24.12.2020.
//

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <string>
#include "catch.hpp"
#include "../src/NGram.h"
#include "ReadCorpus.h"

TEST_CASE("NGramTest-testGetCountSimple") {
    vector<string> text1 = {"<s>", "ali", "topu", "at", "mehmet", "ayşeye", "gitti", "</s>"};
    vector<string> text2 = {"<s>", "ali", "top", "at", "ayşe", "eve", "gitti", "</s>"};
    vector<string> text3 = {"<s>", "ayşe", "kitabı", "ver", "</s>"};
    vector<string> text4 = {"<s>", "ali", "topu", "mehmete", "at", "</s>"};
    vector<string> text5 = {"<s>", "ali", "topu", "at", "mehmet", "ayşeyle", "gitti", "</s>"};
    vector<vector<string>> simpleCorpus = {text1, text2, text3, text4, text5};
    NGram<string> simpleUniGram = NGram<string>(simpleCorpus, 1);
    NGram<string> simpleBiGram =  NGram<string>(simpleCorpus, 2);
    NGram<string> simpleTriGram = NGram<string>(simpleCorpus, 3);
    REQUIRE(5 == simpleUniGram.getCount(new string[]{"<s>"}, 1));
    REQUIRE(0 == simpleUniGram.getCount(new string[]{"mahmut"}, 1));
    REQUIRE(1 == simpleUniGram.getCount(new string[]{"kitabı"}, 1));
    REQUIRE(4 == simpleBiGram.getCount(new string[]{"<s>", "ali"}, 2));
    REQUIRE(0 == simpleBiGram.getCount(new string[]{"ayşe", "ali"}, 2));
    REQUIRE(0 == simpleBiGram.getCount(new string[]{"mahmut", "ali"}, 2));
    REQUIRE(2 == simpleBiGram.getCount(new string[]{"at", "mehmet"}, 2));
    REQUIRE(1 == simpleTriGram.getCount(new string[]{"<s>", "ali", "top"}, 3));
    REQUIRE(0 == simpleTriGram.getCount(new string[]{"ayşe", "kitabı", "at"}, 3));
    REQUIRE(0 == simpleTriGram.getCount(new string[]{"ayşe", "topu", "at"}, 3));
    REQUIRE(0 == simpleTriGram.getCount(new string[]{"mahmut", "evde", "kal"}, 3));
    REQUIRE(2 == simpleTriGram.getCount(new string[]{"ali", "topu", "at"}, 3));
}

TEST_CASE("NGramTest-testGetCountComplex") {
    vector<vector<string>> trainCorpus = readCorpus("../Test/train.txt");
    NGram<string> complexUniGram = NGram<string>(trainCorpus, 1);
    NGram<string> complexBiGram = NGram<string>(trainCorpus, 2);
    NGram<string> complexTriGram = NGram<string>(trainCorpus, 3);
    REQUIRE(20000 == complexUniGram.getCount(new string[]{"<s>"}, 1));
    REQUIRE(50 == complexUniGram.getCount(new string[]{"atatürk"}, 1));
    REQUIRE(11 == complexBiGram.getCount(new string[]{"<s>", "mustafa"}, 2));
    REQUIRE(3 == complexBiGram.getCount(new string[]{"mustafa", "kemal"}, 2));
    REQUIRE(1 == complexTriGram.getCount(new string[]{"<s>", "mustafa", "kemal"}, 3));
    REQUIRE(1 == complexTriGram.getCount(new string[]{"mustafa", "kemal", "atatürk"}, 3));
}

TEST_CASE("NGramTest-testVocabularySizeSimple") {
    vector<string> text1 = {"<s>", "ali", "topu", "at", "mehmet", "ayşeye", "gitti", "</s>"};
    vector<string> text2 = {"<s>", "ali", "top", "at", "ayşe", "eve", "gitti", "</s>"};
    vector<string> text3 = {"<s>", "ayşe", "kitabı", "ver", "</s>"};
    vector<string> text4 = {"<s>", "ali", "topu", "mehmete", "at", "</s>"};
    vector<string> text5 = {"<s>", "ali", "topu", "at", "mehmet", "ayşeyle", "gitti", "</s>"};
    vector<vector<string>> simpleCorpus = {text1, text2, text3, text4, text5};
    NGram<string> simpleUniGram = NGram<string>(simpleCorpus, 1);
    REQUIRE(15 == simpleUniGram.vocabularySize());
}

TEST_CASE("NGramTest-testVocabularySizeComplex") {
    vector<vector<string>> trainCorpus = readCorpus("../Test/train.txt");
    NGram<string>  complexUniGram = NGram<string>(trainCorpus, 1);
    REQUIRE(57625 == complexUniGram.vocabularySize());
    vector<vector<string>> testCorpus = readCorpus("../Test/test.txt");
    NGram<string> complexUniGram2 = NGram<string>(testCorpus, 1);
    REQUIRE(55485 == complexUniGram2.vocabularySize());
    vector<vector<string>> validationCorpus = readCorpus("../Test/validation.txt");
    NGram<string> complexUniGram3 = NGram<string>(validationCorpus, 1);
    REQUIRE(35663 == complexUniGram3.vocabularySize());
}

TEST_CASE("NGramTest-testPrune") {
    vector<string> text1 = {"<s>", "ali", "topu", "at", "mehmet", "ayşeye", "gitti", "</s>"};
    vector<string> text2 = {"<s>", "ali", "top", "at", "ayşe", "eve", "gitti", "</s>"};
    vector<string> text3 = {"<s>", "ayşe", "kitabı", "ver", "</s>"};
    vector<string> text4 = {"<s>", "ali", "topu", "mehmete", "at", "</s>"};
    vector<string> text5 = {"<s>", "ali", "topu", "at", "mehmet", "ayşeyle", "gitti", "</s>"};
    vector<vector<string>> simpleCorpus = {text1, text2, text3, text4, text5};
    NGram<string> simpleBiGram = NGram<string>(simpleCorpus, 2);
    simpleBiGram.prune(0.0);
    REQUIRE(4 == simpleBiGram.getCount(new string[]{"<s>", "ali"}, 2));
    REQUIRE(1 == simpleBiGram.getCount(new string[]{"<s>", "ayşe"}, 2));
    REQUIRE(3 == simpleBiGram.getCount(new string[]{"ali", "topu"}, 2));
    REQUIRE(1 == simpleBiGram.getCount(new string[]{"ali", "top"}, 2));
    REQUIRE(2 == simpleBiGram.getCount(new string[]{"topu", "at"}, 2));
    REQUIRE(1 == simpleBiGram.getCount(new string[]{"topu", "mehmete"}, 2));
    simpleBiGram.prune(0.6);
    REQUIRE(4 == simpleBiGram.getCount(new string[]{"<s>", "ali"}, 2));
    REQUIRE(0 == simpleBiGram.getCount(new string[]{"<s>", "ayşe"}, 2));
    REQUIRE(3 == simpleBiGram.getCount(new string[]{"ali", "topu"}, 2));
    REQUIRE(0 == simpleBiGram.getCount(new string[]{"ali", "top"}, 2));
    REQUIRE(2 == simpleBiGram.getCount(new string[]{"topu", "at"}, 2));
    REQUIRE(0 == simpleBiGram.getCount(new string[]{"topu", "mehmete"}, 2));
    simpleBiGram.prune(0.7);
    REQUIRE(4 == simpleBiGram.getCount(new string[]{"<s>", "ali"}, 2));
    REQUIRE(3 == simpleBiGram.getCount(new string[]{"ali", "topu"}, 2));
    REQUIRE(2 == simpleBiGram.getCount(new string[]{"topu", "at"}, 2));
    simpleBiGram.prune(0.8);
    REQUIRE(4 == simpleBiGram.getCount(new string[]{"<s>", "ali"}, 2));
    REQUIRE(3 == simpleBiGram.getCount(new string[]{"ali", "topu"}, 2));
    simpleBiGram.prune(0.9);
    REQUIRE(4 == simpleBiGram.getCount(new string[]{"<s>", "ali"}, 2));
}

TEST_CASE("NGramTest-loadNGram") {
    NGram<string> nGram1 = NGram<string>("simple1.txt");
    REQUIRE(5 == nGram1.getCount(new string[]{"<s>"}, 1));
    REQUIRE(4 == nGram1.getCount(new string[]{"at"}, 1));
    REQUIRE(2 == nGram1.getCount(new string[]{"mehmet"}, 1));
    NGram<string> nGram2 = NGram<string>("simple2.txt");
    REQUIRE(1 == nGram2.getCount(new string[]{"ver", "</s>"}, 2));
    REQUIRE(4 == nGram2.getCount(new string[]{"<s>", "ali"}, 2));
    REQUIRE(2 == nGram2.getCount(new string[]{"topu", "at"}, 2));
    NGram<string> nGram3 = NGram<string>("simple3.txt");
    REQUIRE(1 == nGram3.getCount(new string[]{"ayşe", "eve", "gitti"}, 3));
    REQUIRE(2 == nGram3.getCount(new string[]{"topu", "at", "mehmet"}, 3));
    REQUIRE(3 == nGram3.getCount(new string[]{"<s>", "ali", "topu"}, 3));
}

TEST_CASE("NGramTest-merge") {
    auto* nGram1 = new NGram<string>("simple1a.txt");
    nGram1->merge(*(new NGram<string>("simple1b.txt")));
    REQUIRE(18 == nGram1->vocabularySize());
    auto* nGram2 = new NGram<string>("simple2a.txt");
    nGram2->merge(*(new NGram<string>("simple2b.txt")));
    nGram2->merge(*(new NGram<string>("simple2c.txt")));
    nGram2->merge(*(new NGram<string>("simple2d.txt")));
    REQUIRE(21 == nGram2->vocabularySize());
    auto* nGram3 = new NGram<string>("simple3a.txt");
    nGram3->merge(*(new NGram<string>("simple3b.txt")));
    nGram3->merge(*(new NGram<string>("simple3c.txt")));
    REQUIRE(20 == nGram3->vocabularySize());
}