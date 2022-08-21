//
// Created by Olcay Taner YILDIZ on 25.12.2020.
//

#include "catch.hpp"
#include "../src/LaplaceSmoothing.h"
#include "ReadCorpus.h"

TEST_CASE("LaplaceSmoothingTest-testPerplexitySimple") {
    vector <string> text1 = {"<s>", "ali", "topu", "at", "mehmet", "ayşeye", "gitti", "</s>"};
    vector <string> text2 = {"<s>", "ali", "top", "at", "ayşe", "eve", "gitti", "</s>"};
    vector <string> text3 = {"<s>", "ayşe", "kitabı", "ver", "</s>"};
    vector <string> text4 = {"<s>", "ali", "topu", "mehmete", "at", "</s>"};
    vector <string> text5 = {"<s>", "ali", "topu", "at", "mehmet", "ayşeyle", "gitti", "</s>"};
    vector <vector<string>> simpleCorpus = {text1, text2, text3, text4, text5};
    NGram <string> simpleUniGram = NGram<string>(simpleCorpus, 1);
    NGram <string> simpleBiGram = NGram<string>(simpleCorpus, 2);
    NGram <string> simpleTriGram = NGram<string>(simpleCorpus, 3);
    LaplaceSmoothing <string> laplaceSmoothing;
    laplaceSmoothing.setProbabilities(simpleUniGram);
    laplaceSmoothing.setProbabilities(simpleBiGram);
    laplaceSmoothing.setProbabilities(simpleTriGram);
    REQUIRE_THAT(12.809502, Catch::Matchers::WithinAbs(simpleUniGram.getPerplexity(simpleCorpus), 0.0001));
    REQUIRE_THAT(6.914532, Catch::Matchers::WithinAbs(simpleBiGram.getPerplexity(simpleCorpus), 0.0001));
    REQUIRE_THAT(7.694528, Catch::Matchers::WithinAbs(simpleTriGram.getPerplexity(simpleCorpus), 0.0001));
}

TEST_CASE("LaplaceSmoothingTest-testPerplexityComplex") {
    vector<vector<string>> trainCorpus = readCorpus("../Test/train.txt");
    vector<vector<string>> testCorpus = readCorpus("../Test/test.txt");
    NGram<string> complexUniGram = NGram<string>(trainCorpus, 1);
    NGram<string> complexBiGram = NGram<string>(trainCorpus, 2);
    NGram<string> complexTriGram = NGram<string>(trainCorpus, 3);
    LaplaceSmoothing <string> laplaceSmoothing;
    laplaceSmoothing.setProbabilities(complexUniGram);
    laplaceSmoothing.setProbabilities(complexBiGram);
    laplaceSmoothing.setProbabilities(complexTriGram);
    REQUIRE_THAT(4085.763010, Catch::Matchers::WithinAbs(complexUniGram.getPerplexity(testCorpus), 0.0001));
    REQUIRE_THAT(24763.660225, Catch::Matchers::WithinAbs(complexBiGram.getPerplexity(testCorpus), 0.0001));
    REQUIRE_THAT(49579.187475, Catch::Matchers::WithinAbs(complexTriGram.getPerplexity(testCorpus), 0.0001));
}

TEST_CASE("LaplaceSmoothingTest-testCalculateNGramProbabilitiesSimple") {
    vector<string> text1 = {"<s>", "ali", "topu", "at", "mehmet", "ayşeye", "gitti", "</s>"};
    vector<string> text2 = {"<s>", "ali", "top", "at", "ayşe", "eve", "gitti", "</s>"};
    vector<string> text3 = {"<s>", "ayşe", "kitabı", "ver", "</s>"};
    vector<string> text4 = {"<s>", "ali", "topu", "mehmete", "at", "</s>"};
    vector<string> text5 = {"<s>", "ali", "topu", "at", "mehmet", "ayşeyle", "gitti", "</s>"};
    vector<vector<string>> simpleCorpus = {text1, text2, text3, text4, text5};
    NGram<string> simpleUniGram = NGram<string>(simpleCorpus, 1);
    NGram<string> simpleBiGram = NGram<string>(simpleCorpus, 2);
    NGram<string> simpleTriGram = NGram<string>(simpleCorpus, 3);
    LaplaceSmoothing<string> laplaceSmoothing;
    laplaceSmoothing.setProbabilities(simpleUniGram);
    laplaceSmoothing.setProbabilities(simpleBiGram);
    laplaceSmoothing.setProbabilities(simpleTriGram);
    REQUIRE((5 + 1) / (35 + simpleUniGram.vocabularySize() + 1) == simpleUniGram.getProbability({"<s>"}));
    REQUIRE((0 + 1) / (35 + simpleUniGram.vocabularySize() + 1) == simpleUniGram.getProbability({"mahmut"}));
    REQUIRE((1 + 1) / (35 + simpleUniGram.vocabularySize() + 1) == simpleUniGram.getProbability({"kitabı"}));
    REQUIRE((4 + 1) / (5 + simpleBiGram.vocabularySize() + 1) == simpleBiGram.getProbability({"<s>", "ali"}));
    REQUIRE((0 + 1) / (2 + simpleBiGram.vocabularySize() + 1) == simpleBiGram.getProbability({"ayşe", "ali"}));
    REQUIRE(1 / (simpleBiGram.vocabularySize() + 1) == simpleBiGram.getProbability({"mahmut", "ali"}));
    REQUIRE((2 + 1) / (4 + simpleBiGram.vocabularySize() + 1) == simpleBiGram.getProbability({"at", "mehmet"}));
    REQUIRE((1 + 1) / (4.0 + simpleTriGram.vocabularySize() + 1) == simpleTriGram.getProbability({"<s>", "ali", "top"}));
    REQUIRE((0 + 1) / (1.0 + simpleTriGram.vocabularySize() + 1) == simpleTriGram.getProbability({"ayşe", "kitabı", "at"}));
    REQUIRE(1 / (simpleTriGram.vocabularySize() + 1) == simpleTriGram.getProbability({"ayşe", "topu", "at"}));
    REQUIRE(1 / (simpleTriGram.vocabularySize() + 1) == simpleTriGram.getProbability({"mahmut", "evde", "kal"}));
    REQUIRE((2 + 1) / (3.0 + simpleTriGram.vocabularySize() + 1) == simpleTriGram.getProbability({"ali", "topu", "at"}));
}

TEST_CASE("LaplaceSmoothingTest-testCalculateNGramProbabilitiesComplex") {
    vector<vector<string>> trainCorpus = readCorpus("../Test/train.txt");
    NGram<string> complexUniGram = NGram<string>(trainCorpus, 1);
    NGram<string> complexBiGram = NGram<string>(trainCorpus, 2);
    NGram<string> complexTriGram = NGram<string>(trainCorpus, 3);
    LaplaceSmoothing <string> laplaceSmoothing;
    laplaceSmoothing.setProbabilities(complexUniGram);
    laplaceSmoothing.setProbabilities(complexBiGram);
    laplaceSmoothing.setProbabilities(complexTriGram);
    REQUIRE((20000 + 1) / (376019.0 + complexUniGram.vocabularySize() + 1) == complexUniGram.getProbability({"<s>"}));
    REQUIRE((50 + 1) / (376019.0 + complexUniGram.vocabularySize() + 1) == complexUniGram.getProbability({"atatürk"}));
    REQUIRE((11 + 1) / (20000.0 + complexBiGram.vocabularySize() + 1) == complexBiGram.getProbability({"<s>", "mustafa"}));
    REQUIRE((3 + 1) / (138.0 + complexBiGram.vocabularySize() + 1) == complexBiGram.getProbability({"mustafa", "kemal"}));
    REQUIRE((1 + 1) / (11.0 + complexTriGram.vocabularySize() + 1) == complexTriGram.getProbability({"<s>", "mustafa", "kemal"}));
    REQUIRE((1 + 1) / (3.0 + complexTriGram.vocabularySize() + 1) == complexTriGram.getProbability({"mustafa", "kemal", "atatürk"}));
}