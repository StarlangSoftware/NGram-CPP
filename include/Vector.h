//
// Created by Olcay Taner Yıldız on 26.11.2018.
//

#ifndef MATH_VECTOR_H
#define MATH_VECTOR_H
#include <vector>
using namespace std;

class Vector {
private:
    unsigned long size;
    vector<double> values;
public:
    explicit Vector(vector<double> values);
    Vector(unsigned long size, double x);
    Vector(unsigned long size, int index, double x);
    Vector(double* values, unsigned long size);
    Vector biased();
    void add(double x);
    void insert(int pos, double x);
    void remove(int pos);
    void clear();
    unsigned long maxIndex();
    void sigmoid();
    Vector skipVector(unsigned long mod, unsigned long value);
    void add(Vector v);
    void subtract(Vector v);
    Vector difference(Vector v);
    double dotProduct(Vector v);
    double dotProduct();
    Vector elementProduct(Vector v);
    void divide(double value);
    void multiply(double value);
    Vector product(double value);
    void l1Normalize();
    double l2Norm();
    unsigned long getSize();
    double cosineSimilarity(Vector v);
    double getValue(unsigned long index);
    void setValue(unsigned long index, double value);
    void addValue(unsigned long index, double value);
    double sum();
    void swap(int index1, int index2);
};


#endif //MATH_VECTOR_H
