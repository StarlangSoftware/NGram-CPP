//
// Created by Olcay Taner Yıldız on 26.11.2018.
//

#ifndef MATH_MATRIXCOLUMNMISMATCH_H
#define MATH_MATRIXCOLUMNMISMATCH_H
#include <exception>
using namespace std;

class MatrixColumnMismatch : public exception{
public:
    const char* what() const noexcept override;
};


#endif //MATH_MATRIXCOLUMNMISMATCH_H
