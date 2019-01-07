//
// Created by LMF319018 on 07/01/2019.
//

#ifndef NGRAM_UNSEENCASE_H
#define NGRAM_UNSEENCASE_H
#include <exception>
using namespace std;

class UnseenCase : public exception {
public:
    const char* what() const noexcept override;
};


#endif //NGRAM_UNSEENCASE_H
