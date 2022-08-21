//
// Created by Olcay Taner YILDIZ on 24.12.2020.
//

#include "catch.hpp"
#include "../src/NoSmoothing.h"
#include "ReadCorpus.h"

TEST_CASE("NoSmoothingTest-testPerplexitySimple") {
    vector<string> text1 = {"<s>", "ali", "topu", "at", "mehmet", "ayşeye", "gitti", "</s>"};
    vector<string> text2 = {"<s>", "ali", "top", "at", "ayşe", "eve", "gitti", "</s>"};
    vector<string> text3 = {"<s>", "ayşe", "kitabı", "ver", "</s>"};
    vector<string> text4 = {"<s>", "ali", "topu", "mehmete", "at", "</s>"};
    vector<string> text5 = {"<s>", "ali", "topu", "at", "mehmet", "ayşeyle", "gitti", "</s>"};
    vector<vector<string>> simpleCorpus = {text1, text2, text3, text4, text5};
    NGram<string> simpleUniGram = NGram<string>(simpleCorpus, 1);
    NGram<string> simpleBiGram = NGram<string>(simpleCorpus, 2);
    NGram<string> simpleTriGram = NGram<string>(simpleCorpus, 3);
    NoSmoothing<string> noSmoothing;
    noSmoothing.setProbabilities(simpleUniGram);
    noSmoothing.setProbabilities(simpleBiGram);
    noSmoothing.setProbabilities(simpleTriGram);
    REQUIRE_THAT(12.318362, Catch::Matchers::WithinAbs(simpleUniGram.getPerplexity(simpleCorpus), 0.0001));
    REQUIRE_THAT(1.573148, Catch::Matchers::WithinAbs(simpleBiGram.getPerplexity(simpleCorpus), 0.0001));
    REQUIRE_THAT(1.248330, Catch::Matchers::WithinAbs(simpleTriGram.getPerplexity(simpleCorpus), 0.0001));
}

TEST_CASE("NoSmoothingTest-testPerplexityComplex") {
    vector<vector<string>> trainCorpus = readCorpus("../Test/train.txt");
    NGram<string> complexUniGram = NGram<string>(trainCorpus, 1);
    NGram<string> complexBiGram = NGram<string>(trainCorpus, 2);
    NGram<string> complexTriGram = NGram<string>(trainCorpus, 3);
    NoSmoothing<string> noSmoothing;
    noSmoothing.setProbabilities(complexUniGram);
    noSmoothing.setProbabilities(complexBiGram);
    noSmoothing.setProbabilities(complexTriGram);
    REQUIRE_THAT(3220.299369, Catch::Matchers::WithinAbs(complexUniGram.getPerplexity(trainCorpus), 0.0001));
    REQUIRE_THAT(32.362912, Catch::Matchers::WithinAbs(complexBiGram.getPerplexity(trainCorpus), 0.0001));
    REQUIRE_THAT(2.025259, Catch::Matchers::WithinAbs(complexTriGram.getPerplexity(trainCorpus), 0.0001));
}

TEST_CASE("NoSmoothingTest-testCalculateNGramProbabilitiesSimple") {
    vector<string> text1 = {"<s>", "ali", "topu", "at", "mehmet", "ayşeye", "gitti", "</s>"};
    vector<string> text2 = {"<s>", "ali", "top", "at", "ayşe", "eve", "gitti", "</s>"};
    vector<string> text3 = {"<s>", "ayşe", "kitabı", "ver", "</s>"};
    vector<string> text4 = {"<s>", "ali", "topu", "mehmete", "at", "</s>"};
    vector<string> text5 = {"<s>", "ali", "topu", "at", "mehmet", "ayşeyle", "gitti", "</s>"};
    vector<vector<string>> simpleCorpus = {text1, text2, text3, text4, text5};
    NGram<string> simpleUniGram = NGram<string>(simpleCorpus, 1);
    NGram<string> simpleBiGram = NGram<string>(simpleCorpus, 2);
    NGram<string> simpleTriGram = NGram<string>(simpleCorpus, 3);
    NoSmoothing<string> noSmoothing;
    noSmoothing.setProbabilities(simpleUniGram);
    noSmoothing.setProbabilities(simpleBiGram);
    noSmoothing.setProbabilities(simpleTriGram);
    REQUIRE(5 / 35.0 == simpleUniGram.getProbability({"<s>"}));
    REQUIRE(0.0 == simpleUniGram.getProbability({"mahmut"}));
    REQUIRE(1.0 / 35.0 == simpleUniGram.getProbability({"kitabı"}));
    REQUIRE(4 / 5.0 == simpleBiGram.getProbability({"<s>", "ali"}));
    REQUIRE(0 / 2.0 == simpleBiGram.getProbability({"ayşe", "ali"}));
    REQUIRE(0.0 == simpleBiGram.getProbability({"mahmut", "ali"}));
    REQUIRE(2 / 4.0 == simpleBiGram.getProbability({"at", "mehmet"}));
    REQUIRE(1 / 4.0 == simpleTriGram.getProbability({"<s>", "ali", "top"}));
    REQUIRE(0 / 1.0 == simpleTriGram.getProbability({"ayşe", "kitabı", "at"}));
    REQUIRE(0.0 == simpleTriGram.getProbability({"ayşe", "topu", "at"}));
    REQUIRE(0.0 == simpleTriGram.getProbability({"mahmut", "evde", "kal"}));
    REQUIRE(2 / 3.0 == simpleTriGram.getProbability({"ali", "topu", "at"}));
}

TEST_CASE("NoSmoothingTest-testCalculateNGramProbabilitiesComplex") {
    vector<vector<string>> trainCorpus = readCorpus("../Test/train.txt");
    NGram<string> complexUniGram = NGram<string>(trainCorpus, 1);
    NGram<string> complexBiGram = NGram<string>(trainCorpus, 2);
    NGram<string> complexTriGram = NGram<string>(trainCorpus, 3);
    NoSmoothing<string> noSmoothing;
    noSmoothing.setProbabilities(complexUniGram);
    noSmoothing.setProbabilities(complexBiGram);
    noSmoothing.setProbabilities(complexTriGram);
    REQUIRE(20000 / 376019.0 == complexUniGram.getProbability({"<s>"}));
    REQUIRE(50 / 376019.0 == complexUniGram.getProbability({"atatürk"}));
    REQUIRE(11 / 20000.0 == complexBiGram.getProbability({"<s>", "mustafa"}));
    REQUIRE(3 / 138.0 == complexBiGram.getProbability({"mustafa", "kemal"}));
    REQUIRE(1 / 11.0 == complexTriGram.getProbability({"<s>", "mustafa", "kemal"}));
    REQUIRE(1 / 3.0 == complexTriGram.getProbability({"mustafa", "kemal", "atatürk"}));
}