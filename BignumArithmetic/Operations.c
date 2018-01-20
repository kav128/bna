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
	char *_a = calloc(2 * BufSize, sizeof(char));
	char *_b = calloc(2 * BufSize, sizeof(char));
	char *_r = calloc(2 * BufSize, sizeof(char));
	memcpy(_a, a, BufSize);
	memcpy(_b, b, BufSize);

	size_t lna = strlen(_a);
	size_t lnb = strlen(_b);
	size_t ln = (lna > lnb ? lna : lnb) + 1;
	AddZeros(_a, ln - lna);
	AddZeros(_b, ln - lnb);

	char *p = calloc(BufSize, sizeof(char));
	for (int i = ln - 1; i >= 0; i--)
	{
		*(_r + i) = *(_a + i) + *(_b + i) + *(p + i) - '0';
		if (*(_r + i) > '9')
		{
			*(_r + i) -= 10;
			if (i > 0)
			{
				*(p + i - 1) = 1;
			}
		}
	}
	ZeroTrim(_r);
	memcpy(res, _r, BufSize);
	*(res + BufSize - 1) = 0;

	free(p);
	free(_r);
	free(_b);
	free(_a);
}

// "Нормализованное" вычитание, 0 <= a <= b
void SubN(char *a, char *b, char *res)
{
	char *_a = calloc(BufSize, sizeof(char));
	char *_b = calloc(BufSize, sizeof(char));
	memcpy(_a, a, BufSize);
	memcpy(_b, b, BufSize);

	size_t ln = strlen(_a);
	size_t lnb = strlen(_b);
	AddZeros(_b, ln - lnb);

	shortint *_p = calloc(ln, sizeof(shortint));
	for (int i = ln - 1; i >= 0; i--)
	{
		shortint rs = *(_a + i) - *(_b + i) + *(_p + i);
		if (rs < 0)
		{
			rs += 10;
			int j = i;
			do
			{
				j--;
			} while (*(_a + j) == '0');
			*(_p + j++) = -1;
			for (; j < i; j++)
			{
				*(_p + j) = 9;
			}

		}
		*(res + i) = '0' + rs;
	}
	free(_p);

	free(_b);
	free(_a);
}

// Умножение на однозначное число и добавление нескольких нулей в конец. Служебная функция
void MulToIntN(const char *a, char b, char *res, size_t zeros)
{
	char *_a = calloc(2 * BufSize, sizeof(char));
	char *p = calloc(2 * BufSize, sizeof(char));
	char *_res = calloc(2 * BufSize, sizeof(char));
	memcpy(_a, a, 2 * BufSize);
	memset(_a + BufSize, 0, BufSize);
	memset(p, 0, 2 * BufSize);
	AddZeros(_a, 1);
	size_t ln = strlen(_a);
	for (int i = ln - 1; i >= 0; i--)
	{
		char rs = (*(_a + i) - '0') * b + *(p + i);
		*(_res + i) = rs % 10 + '0';
		if (i > 0)
		{
			*(p + i - 1) = rs / 10;
		}
	}
	ZeroTrim(_res);

	ln = strlen(_res);
	if (zeros > 0)
	{
		memset(_res + ln, '0', zeros);
	}
	memcpy(res, _res, BufSize);
	*(res + BufSize - 1) = 0;
	free(_res);
	free(p);
	free(_a);
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
		Sub(a, b, a);
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
void Add(char *a, char *b, char *res)
{
	signed char sga = 0, sgb = 0; // Знаки чисел. По дефолту все положительно
	if (*a == '-')
	{
		sga = 1;
		a++;
	}
	if (*b == '-')
	{
		sgb = 1;
		b++;
	}

	int sgn = sga << 1 | sgb;
	int cmp = Compare(a, b);
	switch (sgn)
	{
		case 0:
			AddN(a, b, res);
			break;
		case 1:
			if (cmp == 1)
			{
				SubN(a, b, res);
			}
			else
			{
				SubN(b, a, res);
				Negative(res);
			}
			break;
		case 2:
			if (cmp == 1)
			{
				SubN(a, b, res);
				Negative(res);
			}
			else
			{
				SubN(b, a, res);
			}
			break;
		case 3:
			AddN(a, b, res);
			Negative(res);
			break;
	}

	ZeroTrim(res);
}

// Вычитание
void Sub(char *a, char *b, char *res)
{
	signed char sga = 0, sgb = 0; // Знаки чисел. По дефолту все положительно
	if (*a == '-')
	{
		sga = 1;
		a++;
	}
	if (*b == '-')
	{
		sgb = 1;
		b++;
	}

	int sgn = sga << 1 | sgb;
	int cmp = Compare(a, b);
	switch (sgn)
	{
		case 0:
			if (cmp == 1)
			{
				SubN(a, b, res);
			}
			else
			{
				SubN(b, a, res);
				Negative(res);
			}
			break;
		case 1:
			AddN(a, b, res);
			break;
		case 2:
			AddN(a, b, res);
			Negative(res);
			break;
		case 3:
			if (cmp == 1)
			{
				SubN(a, b, res);
				Negative(res);
			}
			else
			{
				SubN(b, a, res);
			}
			break;
	}

	ZeroTrim(res);
}

// Умножение
int Mul(char *a, char *b, char *res)
{
	if (!CheckMultiplication(a, b))
	{
		return 0; // Не влезает в буфер
	}

	char *_a = a;
	char *_b = b;
	Erase(res);

	char sga = *_a == '-' ? 1 : 0;
	char sgb = *_b == '-' ? 1 : 0;
	if (sga)
	{
		_a++;
	}
	if (sgb)
	{
		_b++;
	}

	MulN(_a, _b, res);
	if (sga != sgb)
	{
		Negative(res);
	}

	return 1; // ОК
}

// Деление
void Div(char *a, char *b, char *res)
{
	Erase(res);

	char sga = 0, sgb = 0;
	if (*a == '-')
	{
		sga = 1;
		a++;
	}
	if (*b == '-')
	{
		sgb = 1;
		b++;
	}

	DivN(a, b, res);
	if (sga != sgb)
	{
		Negative(res);
	}
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
