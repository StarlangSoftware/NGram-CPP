//
// Created by Olcay Taner YILDIZ on 25.12.2020.
//

#include "catch.hpp"
#include "../AdditiveSmoothing.h"
#include "ReadCorpus.h"

TEST_CASE("AdditiveSmoothingTest-testPerplexityComplex") {
    AdditiveSmoothing<string> additiveSmoothing;
    vector<vector<string>> trainCorpus = readCorpus("../Test/train.txt");
    vector<vector<string>> validationCorpus = readCorpus("../Test/validation.txt");
    vector<vector<string>> testCorpus = readCorpus("../Test/test.txt");
    NGram<string> complexUniGram = NGram<string>(trainCorpus, 1);
    additiveSmoothing.train(validationCorpus, complexUniGram);
    REQUIRE_THAT(4043.947022, Catch::Matchers::WithinAbs(complexUniGram.getPerplexity(testCorpus), 0.0001));
    NGram<string> complexBiGram = NGram<string>(trainCorpus, 2);
    additiveSmoothing.train(validationCorpus, complexBiGram);
    REQUIRE_THAT(9220.218871, Catch::Matchers::WithinAbs(complexBiGram.getPerplexity(testCorpus), 0.0001));
    NGram<string> complexTriGram = NGram<string>(trainCorpus, 3);
    additiveSmoothing.train(validationCorpus, complexTriGram);
    REQUIRE_THAT(30695.701941, Catch::Matchers::WithinAbs(complexTriGram.getPerplexity(testCorpus), 0.0001));
}

TEST_CASE("AdditiveSmoothingTest-testCalculateNGramProbabilitiesComplex") {
    AdditiveSmoothing<string> additiveSmoothing;
    vector<vector<string>> trainCorpus = readCorpus("../Test/train.txt");
    vector<vector<string>> validationCorpus = readCorpus("../Test/validation.txt");
    NGram<string> complexUniGram = NGram<string>(trainCorpus, 1);
    additiveSmoothing.train(validationCorpus, complexUniGram);
    double delta1 = additiveSmoothing.getDelta();
    REQUIRE((20000 + delta1) / (376019.0 + delta1 * (complexUniGram.vocabularySize() + 1)) == complexUniGram.getProbability({"<s>"}));
    REQUIRE((50 + delta1) / (376019.0 + delta1 * (complexUniGram.vocabularySize() + 1)) == complexUniGram.getProbability({"atatürk"}));
    NGram<string> complexBiGram = NGram<string>(trainCorpus, 2);
    additiveSmoothing.train(validationCorpus, complexBiGram);
    double delta2 = additiveSmoothing.getDelta();
    REQUIRE((11 + delta2) / (20000.0 + delta2 * (complexBiGram.vocabularySize() + 1)) == complexBiGram.getProbability({"<s>", "mustafa"}));
    REQUIRE((3 + delta2) / (138.0 + delta2 * (complexBiGram.vocabularySize() + 1)) == complexBiGram.getProbability({"mustafa", "kemal"}));
    NGram<string> complexTriGram = NGram<string>(trainCorpus, 3);
    additiveSmoothing.train(validationCorpus, complexTriGram);
    double delta3 = additiveSmoothing.getDelta();
    REQUIRE((1 + delta3) / (11.0 + delta3 * (complexTriGram.vocabularySize() + 1)) == complexTriGram.getProbability({"<s>", "mustafa", "kemal"}));
    REQUIRE((1 + delta3) / (3.0 + delta3 * (complexTriGram.vocabularySize() + 1)) == complexTriGram.getProbability({"mustafa", "kemal", "atatürk"}));
}