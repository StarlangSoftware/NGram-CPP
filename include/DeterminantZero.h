//
// Created by Olcay Taner Yıldız on 26.11.2018.
//

#ifndef MATH_DETERMINANTZERO_H
#define MATH_DETERMINANTZERO_H
#include <exception>
using namespace std;

class DeterminantZero : public exception {
public:
    const char* what() const noexcept override;
};


#endif //MATH_DETERMINANTZERO_H
