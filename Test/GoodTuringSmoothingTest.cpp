//
// Created by Olcay Taner YILDIZ on 25.12.2020.
//

#include "catch.hpp"
#include "../GoodTuringSmoothing.h"
#include "ReadCorpus.h"

TEST_CASE("GoodTuringSmoothingTest-testPerplexitySimple") {
    vector <string> text1 = {"<s>", "ali", "topu", "at", "mehmet", "ayşeye", "gitti", "</s>"};
    vector <string> text2 = {"<s>", "ali", "top", "at", "ayşe", "eve", "gitti", "</s>"};
    vector <string> text3 = {"<s>", "ayşe", "kitabı", "ver", "</s>"};
    vector <string> text4 = {"<s>", "ali", "topu", "mehmete", "at", "</s>"};
    vector <string> text5 = {"<s>", "ali", "topu", "at", "mehmet", "ayşeyle", "gitti", "</s>"};
    vector <vector<string>> simpleCorpus = {text1, text2, text3, text4, text5};
    NGram <string> simpleUniGram = NGram<string>(simpleCorpus, 1);
    NGram <string> simpleBiGram = NGram<string>(simpleCorpus, 2);
    NGram <string> simpleTriGram = NGram<string>(simpleCorpus, 3);
    GoodTuringSmoothing <string> goodTuringSmoothing;
    goodTuringSmoothing.setProbabilities(simpleUniGram);
    goodTuringSmoothing.setProbabilities(simpleBiGram);
    goodTuringSmoothing.setProbabilities(simpleTriGram);
    REQUIRE_THAT(14.500734, Catch::Matchers::WithinAbs(simpleUniGram.getPerplexity(simpleCorpus), 0.0001));
    REQUIRE_THAT(2.762526, Catch::Matchers::WithinAbs(simpleBiGram.getPerplexity(simpleCorpus), 0.0001));
    REQUIRE_THAT(3.685001, Catch::Matchers::WithinAbs(simpleTriGram.getPerplexity(simpleCorpus), 0.0001));
}

TEST_CASE("GoodTuringSmoothingTest-testPerplexityComplex") {
    vector<vector<string>> trainCorpus = readCorpus("../Test/train.txt");
    vector<vector<string>> testCorpus = readCorpus("../Test/test.txt");
    NGram<string> complexUniGram = NGram<string>(trainCorpus, 1);
    NGram<string> complexBiGram = NGram<string>(trainCorpus, 2);
    NGram<string> complexTriGram = NGram<string>(trainCorpus, 3);
    GoodTuringSmoothing <string> goodTuringSmoothing;
    goodTuringSmoothing.setProbabilities(complexUniGram);
    goodTuringSmoothing.setProbabilities(complexBiGram);
    goodTuringSmoothing.setProbabilities(complexTriGram);
    REQUIRE_THAT(1290.97916, Catch::Matchers::WithinAbs(complexUniGram.getPerplexity(testCorpus), 0.0001));
    REQUIRE_THAT(8331.518540, Catch::Matchers::WithinAbs(complexBiGram.getPerplexity(testCorpus), 0.0001));
    REQUIRE_THAT(39184.430078, Catch::Matchers::WithinAbs(complexTriGram.getPerplexity(testCorpus), 0.0001));
}

TEST_CASE("GoodTuringSmoothingTest-testCalculateNGramProbabilitiesSimple") {
    vector<string> text1 = {"<s>", "ali", "topu", "at", "mehmet", "ayşeye", "gitti", "</s>"};
    vector<string> text2 = {"<s>", "ali", "top", "at", "ayşe", "eve", "gitti", "</s>"};
    vector<string> text3 = {"<s>", "ayşe", "kitabı", "ver", "</s>"};
    vector<string> text4 = {"<s>", "ali", "topu", "mehmete", "at", "</s>"};
    vector<string> text5 = {"<s>", "ali", "topu", "at", "mehmet", "ayşeyle", "gitti", "</s>"};
    vector<vector<string>> simpleCorpus = {text1, text2, text3, text4, text5};
    NGram<string> simpleUniGram = NGram<string>(simpleCorpus, 1);
    NGram<string> simpleBiGram = NGram<string>(simpleCorpus, 2);
    NGram<string> simpleTriGram = NGram<string>(simpleCorpus, 3);
    GoodTuringSmoothing <string> goodTuringSmoothing;
    goodTuringSmoothing.setProbabilities(simpleUniGram);
    goodTuringSmoothing.setProbabilities(simpleBiGram);
    goodTuringSmoothing.setProbabilities(simpleTriGram);
    REQUIRE_THAT(0.116607, Catch::Matchers::WithinAbs(simpleUniGram.getProbability({"<s>"}), 0.0001));
    REQUIRE_THAT(0.149464,  Catch::Matchers::WithinAbs(simpleUniGram.getProbability({"mahmut"}), 0.0001));
    REQUIRE_THAT(0.026599, Catch::Matchers::WithinAbs(simpleUniGram.getProbability({"kitabı"}), 0.0001));
    REQUIRE_THAT(0.492147, Catch::Matchers::WithinAbs(simpleBiGram.getProbability({"<s>", "ali"}), 0.0001));
    REQUIRE_THAT(0.030523, Catch::Matchers::WithinAbs(simpleBiGram.getProbability({"ayşe", "ali"}), 0.0001));
    REQUIRE_THAT(0.0625, Catch::Matchers::WithinAbs(simpleBiGram.getProbability({"mahmut", "ali"}), 0.0001));
    REQUIRE_THAT(0.323281, Catch::Matchers::WithinAbs(simpleBiGram.getProbability({"at", "mehmet"}), 0.0001));
    REQUIRE_THAT(0.049190, Catch::Matchers::WithinAbs(simpleTriGram.getProbability({"<s>", "ali", "top"}), 0.0001));
    REQUIRE_THAT(0.043874, Catch::Matchers::WithinAbs(simpleTriGram.getProbability({"ayşe", "kitabı", "at"}), 0.0001));
    REQUIRE_THAT(0.0625, Catch::Matchers::WithinAbs(simpleTriGram.getProbability({"ayşe", "topu", "at"}), 0.0001));
    REQUIRE_THAT(0.0625, Catch::Matchers::WithinAbs(simpleTriGram.getProbability({"mahmut", "evde", "kal"}), 0.0001));
    REQUIRE_THAT(0.261463, Catch::Matchers::WithinAbs(simpleTriGram.getProbability({"ali", "topu", "at"}), 0.0001));
}

TEST_CASE("GoodTuringSmoothingTest-testCalculateNGramProbabilitiesComplex") {
    vector<vector<string>> trainCorpus = readCorpus("../Test/train.txt");
    NGram<string> complexUniGram = NGram<string>(trainCorpus, 1);
    NGram<string> complexBiGram = NGram<string>(trainCorpus, 2);
    NGram<string> complexTriGram = NGram<string>(trainCorpus, 3);
    GoodTuringSmoothing <string> goodTuringSmoothing;
    goodTuringSmoothing.setProbabilities(complexUniGram);
    goodTuringSmoothing.setProbabilities(complexBiGram);
    goodTuringSmoothing.setProbabilities(complexTriGram);
    REQUIRE_THAT(0.050745, Catch::Matchers::WithinAbs(complexUniGram.getProbability({"<s>"}), 0.0001));
    REQUIRE_THAT(0.000126, Catch::Matchers::WithinAbs(complexUniGram.getProbability({"atatürk"}), 0.0001));
    REQUIRE_THAT(0.000497, Catch::Matchers::WithinAbs(complexBiGram.getProbability({"<s>", "mustafa"}), 0.0001));
    REQUIRE_THAT(0.014000, Catch::Matchers::WithinAbs(complexBiGram.getProbability({"mustafa", "kemal"}), 0.0001));
    REQUIRE_THAT(0.061028, Catch::Matchers::WithinAbs(complexTriGram.getProbability({"<s>", "mustafa", "kemal"}), 0.0001));
    REQUIRE_THAT(0.283532, Catch::Matchers::WithinAbs(complexTriGram.getProbability({"mustafa", "kemal", "atatürk"}), 0.0001));
}