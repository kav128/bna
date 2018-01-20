#ifndef BIGNUM_OPERATIONS_H
#define BIGNUM_OPERATIONS_H

#include "Utils.h"
void Add(bignum a, bignum b, bignum *res);
void Sub(bignum a, bignum b, bignum *res);
int Mul(bignum a, bignum b, bignum *res);
void Div(bignum a, bignum b, bignum *res);
int CheckMultiplication(char *a, char *b);

#endif //BIGNUM_OPERATIONS_H
