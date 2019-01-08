//
// Created by Olcay Taner Yıldız on 28.09.2018.
//

#ifndef SAMPLING_KFOLDCROSSVALIDATION_H
#define SAMPLING_KFOLDCROSSVALIDATION_H
#include "CrossValidation.h"
#include <random>
using namespace std;

template <class T> class KFoldCrossValidation : public CrossValidation<T>{
private:
    unsigned long N;
    vector<T> instanceList;
public:
    KFoldCrossValidation(const vector<T>& instanceList, int K, unsigned int seed);
    vector<T> getTrainFold(int k);
    vector<T> getTestFold(int k);
};

template<class T> KFoldCrossValidation<T>::KFoldCrossValidation(const vector<T> &instanceList, int K, unsigned int seed) {
    this->instanceList = instanceList;
    shuffle(this->instanceList.begin(), this->instanceList.end(), default_random_engine(seed));
    N = instanceList.capacity();
    this->K = K;
}

template<class T> vector<T> KFoldCrossValidation<T>::getTrainFold(int k){
    vector<T> trainFold;
    for (int i = 0; i < (k * N) / this->K; i++){
        trainFold.push_back(instanceList.at(i));
    }
    for (unsigned long i = ((k + 1) * N) / this->K; i < N; i++){
        trainFold.push_back(instanceList.at(i));
    }
    return trainFold;
}

template<class T> vector<T> KFoldCrossValidation<T>::getTestFold(int k){
    vector<T> testFold;
    for (unsigned long i = (k * N) / this->K; i < ((k + 1) * N) / this->K; i++){
        testFold.push_back(instanceList.at(i));
    }
    return testFold;
}

#endif //SAMPLING_KFOLDCROSSVALIDATION_H
