#ifndef BIGNUM_OPERATIONS_H
#define BIGNUM_OPERATIONS_H

void Initialize();
void Finalize();

void Add(const char *a, const char *b, char *res);
void Sub(const char *a, const char *b, char *res);
int Mul(const char *a, const char *b, char *res);
void Div(const char *a, const char *b, char *res);
int CheckMultiplication(const char *a, const char *b);

#endif //BIGNUM_OPERATIONS_H
