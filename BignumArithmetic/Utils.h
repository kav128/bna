#ifndef BIGNUM_UTILS_H
#define BIGNUM_UTILS_H

void ZeroTrim(char *str);
void Erase(char *str);
void AddZeros(char *str, size_t zcount);
char Validate(char *str);
char Abs(char *a);
void Negative(char *a);
int Compare(const char *a, const char *b);
void memswap(char *a, char *b, size_t len);

#endif //BIGNUM_UTILS_H
