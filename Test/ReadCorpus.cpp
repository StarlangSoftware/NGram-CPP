//
// Created by Olcay Taner YILDIZ on 25.12.2020.
//

#include <sstream>
#include <vector>
#include <fstream>
using namespace std;

vector<vector <string>> readCorpus(string fileName){
    vector<vector <string>> result;
    string line;
    ifstream inputFile;
    inputFile.open(fileName, ifstream :: in);
    while (inputFile.good()) {
        getline(inputFile, line);
        std::istringstream stringStream(line);
        vector<string> tokens{istream_iterator<string>{stringStream}, istream_iterator<string>{}};
        result.emplace_back(tokens);
    }
    inputFile.close();
    return result;
}
