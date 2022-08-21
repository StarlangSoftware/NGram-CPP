//
// Created by Olcay Taner YILDIZ on 25.12.2020.
//

#include "catch.hpp"
#include "../src/InterpolatedSmoothing.h"
#include "ReadCorpus.h"

TEST_CASE("InterpolatedSmoothingTest-testPerplexityComplex") {
    InterpolatedSmoothing<string> interpolatedSmoothing;
    vector<vector<string>> trainCorpus = readCorpus("../Test/train.txt");
    vector<vector<string>> validationCorpus = readCorpus("../Test/validation.txt");
    vector<vector<string>> testCorpus = readCorpus("../Test/test.txt");
    NGram<string> complexBiGram = NGram<string>(trainCorpus, 2);
    interpolatedSmoothing.train(validationCorpus, complexBiGram);
    REQUIRE_THAT(917.214864, Catch::Matchers::WithinAbs(complexBiGram.getPerplexity(testCorpus), 0.0001));
    NGram<string> complexTriGram = NGram<string>(trainCorpus, 3);
    interpolatedSmoothing.train(validationCorpus, complexTriGram);
    REQUIRE_THAT(3000.451177, Catch::Matchers::WithinAbs(complexTriGram.getPerplexity(testCorpus), 0.0001));
}

TEST_CASE("InterpolatedSmoothingTest-testCalculateNGramProbabilitiesComplex") {
    InterpolatedSmoothing<string> interpolatedSmoothing;
    vector<vector<string>> trainCorpus = readCorpus("../Test/train.txt");
    vector<vector<string>> validationCorpus = readCorpus("../Test/validation.txt");
    NGram<string> complexBiGram = NGram<string>(trainCorpus, 2);
    interpolatedSmoothing.train(validationCorpus, complexBiGram);
    REQUIRE_THAT(0.000418, Catch::Matchers::WithinAbs(complexBiGram.getProbability({"<s>", "mustafa"}), 0.0001));
    REQUIRE_THAT(0.005555, Catch::Matchers::WithinAbs(complexBiGram.getProbability({"mustafa", "kemal"}), 0.0001));
    NGram<string> complexTriGram = NGram<string>(trainCorpus, 3);
    interpolatedSmoothing.train(validationCorpus, complexTriGram);
    REQUIRE_THAT(0.014406, Catch::Matchers::WithinAbs(complexTriGram.getProbability({"<s>", "mustafa", "kemal"}), 0.0001));
    REQUIRE_THAT(0.058765, Catch::Matchers::WithinAbs(complexTriGram.getProbability({"mustafa", "kemal", "atatürk"}), 0.0001));
}