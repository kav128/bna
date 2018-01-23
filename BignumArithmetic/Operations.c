#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "Operations.h"
#include "Utils.h"

typedef signed char shortint;

// Размер всех строк для хранения чисел и промежуточных вычислений
size_t BufSize = 512;

// "Нормализованное сложение", 0 <= a, b
void AddN(char *a, char *b, char *res)
{
	size_t lna = strlen(a);
	size_t lnb = strlen(b);
	char *ca = a + lna - 1;
	char *cb = b + lnb - 1;
	char *cr = res + (lna > lnb ? lna : lnb);
	unsigned char p = 0;
	unsigned char r = 0;
	do
	{
		r = (ca >= a ? (*ca - '0') : 0) + (cb >= b ? (*cb - '0') : 0) + p;
		*cr = r % 10 + '0';
		p = r / 10;
		cr--;
		cb--;
		ca--;
	}
	while (cr >= res);
}

// "Нормализованное" вычитание, 0 <= b <= a
void SubN(char *a, char *b, char *res)
{
	size_t lna = strlen(a);
	size_t lnb = strlen(b);
	char *ca = a + lna - 1;
	char *cb = b + lnb - 1;
	char *cr = res + (lna > lnb ? lna : lnb);
	shortint r = 0;
	shortint *p = calloc(BufSize, sizeof(shortint));
	shortint *cp = p + (lna > lnb ? lna : lnb);
	do
	{
		r = (ca >= a ? *ca - '0' : 0) - (cb >= b ? *cb - '0' : 0) + *cp;
		cp--;
		*cr = (r < 0 ? r + 10 : r) + '0';
		*cp = r < 0 ? -1 : 0;
		cr--;
		cb--;
		ca--;
	}
	while (cr >= res);
}

// Умножение на однозначное число и добавление нескольких нулей в конец. Служебная функция
void MulToIntN(const char *a, char b, char *res, size_t zeros)
{
	size_t lna = strlen(a);
	char *ca = a + lna - 1;
	char *cr = res + lna + zeros;
	while (cr > res + lna)
	{
		*cr = '0';
		cr--;
	}
	unsigned char p = 0;
	unsigned char r = 0;
	do
	{
		r = (ca >= a ? (*ca - '0') : 0) * b + p;
		*cr = r % 10 + '0';
		p = r / 10;
		cr--;
		ca--;
	}
	while (cr >= res);
	ZeroTrim(res);
}

// "Нормализованное" умножение, 0 <= a, b
void MulN(const char *a, const char *b, char *res)
{
	char *p = calloc(2 * BufSize, sizeof(char));
	Erase(res);
	*res = '0';
	size_t lnb = strlen(b);
	for (int i = lnb - 1; i >= 0; i--)
	{
		Erase(p);
		MulToIntN(a, *(b + i) - '0', p, lnb - i - 1);
		AddN(res, p, res);
	}
	free(p);
}

// Деление числа на число. Крайне медленный алгоритм. Должна применяться для деления на числа меньше 10. В a остается остаток
char DivSimple(char *a, char *b)
{
	char i = 0;
	while (Compare(a, b) >= 0)
	{
		SubN(a, b, a);
		i++;
	}

	return i;
}

// "Нормализованное" деление, 0 <= b <= a
void DivN(char *a, char *b, char *res)
{
	char *p = calloc(BufSize, sizeof(char));
	size_t lna = strlen(a);
	size_t lnb = strlen(b);
	char *rs = res;
	memcpy(p, a, lnb);
	*(rs++) = DivSimple(p, b) + '0';
	for (int i = lnb; i < lna; i++)
	{
		*(p + strlen(p)) = *(a + i);
		*(rs++) = DivSimple(p, b) + '0';
	}
	ZeroTrim(res);
	free(p);
}

// Сложение
void Add(bignum a, bignum b, bignum *res)
{
	int sgn = a.sign << 1 | b.sign;
	int cmp = Compare(a.absnum, b.absnum);
	switch (sgn)
	{
		case 0:
			AddN(a.absnum, b.absnum, res->absnum);
			res->sign = 0;
			break;
		case 1:
			if (cmp == 1)
			{
				SubN(a.absnum, b.absnum, res->absnum);
				res->sign = 0;
			}
			else
			{
				SubN(b.absnum, a.absnum, res->absnum);
				res->sign = 1;
			}
			break;
		case 2:
			if (cmp == 1)
			{
				SubN(a.absnum, b.absnum, res->absnum);
				res->sign = 1;
			}
			else
			{
				SubN(b.absnum, a.absnum, res->absnum);
				res->sign = 0;
			}
			break;
		case 3:
			AddN(a.absnum, b.absnum, res->absnum);
			res->sign = 1;
			break;
	}

	ZeroTrim(res->absnum);
}

// Вычитание
void Sub(bignum a, bignum b, bignum *res)
{
	int sgn = a.sign << 1 | b.sign;
	int cmp = Compare(a.absnum, b.absnum);
	switch (sgn)
	{
		case 0:
			if (cmp == 1)
			{
				SubN(a.absnum, b.absnum, res->absnum);
				res->sign = 0;
			}
			else
			{
				SubN(b.absnum, a.absnum, res->absnum);
				res->sign = 1;
			}
			break;
		case 1:
			AddN(a.absnum, b.absnum, res->absnum);
			res->sign = 0;
			break;
		case 2:
			AddN(a.absnum, b.absnum, res->absnum);
			res->sign = 1;
			break;
		case 3:
			if (cmp == 1)
			{
				SubN(a.absnum, b.absnum, res->absnum);
				res->sign = 1;
			}
			else
			{
				SubN(b.absnum, a.absnum, res->absnum);
				res->sign = 0;
			}
			break;
	}

	ZeroTrim(res->absnum);
}

// Умножение
int Mul(bignum a, bignum b, bignum *res)
{
	if (!CheckMultiplication(a.absnum, b.absnum))
	{
		return 0; // Не влезает в буфер
	}

	Erase(res->absnum);
	MulN(a.absnum, b.absnum, res->absnum);
	res->sign = (a.sign != b.sign);

	return 1; // ОК
}

// Деление
void Div(bignum a, bignum b, bignum *res)
{
	Erase(res->absnum);
	DivN(a.absnum, b.absnum, res->absnum);
	res->sign = (a.sign != b.sign);
}

// Проверка вместимости буфера для результата.
int CheckMultiplication(char *a, char *b)
{
	int ae = strlen(a) - 1;
	int be = strlen(b) - 1;
	int re = ae + be;
	double am = *a - '0' + (ae > 0 ? (*(a + 1) + 1 - '0') / 10.0 : 0);
	double bm = *b - '0' + (be > 0 ? (*(b + 1) + 1 - '0') / 10.0 : 0);
	double rm = am * bm;
	if (rm > 10)
	{
		rm /= 10;
		re++;
	}
	return re + 1 < BufSize;
}
