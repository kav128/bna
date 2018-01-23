#ifndef BIGNUM_UTILS_H
#define BIGNUM_UTILS_H

struct _bignum
{
	char sign; // 0 - положительное, 1 - отрицательное
	char *absnum; // Модуль числа
};
typedef struct _bignum bignum;

void ZeroTrim(char *str);
void Erase(char *str);
void AddZeros(char *str, size_t zcount);
char Validate(char *str);
char Abs(char *a);
void Negative(char *a);
int Compare(const char *a, const char *b);
void memswap(char *a, char *b, size_t len);
int CalcBufSize(char *a, char *b, char op);

#endif //BIGNUM_UTILS_H
