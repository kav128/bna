#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "Operations.h"
#include "Utils.h"

typedef signed char shortint;

// Размер всех строк для хранения чисел и промежуточных вычислений
size_t BufSize = 512;
// Строки для локального хранения аргументов (чтобы не подпортить внешние данные)
char *_a, *_b;
// Вспомогательная строка
char *p;

void Initialize()
{
	_a = calloc(BufSize, 1);
	_b = calloc(BufSize, 1);
	p = malloc(BufSize);
}

void Finalize()
{
	free(p);
	free(_b);
	free(_a);
}

// "Нормализованное сложение", 0 <= a, b
void AddN(char *res)
{
	size_t lna = strlen(_a);
	size_t lnb = strlen(_b);
	size_t ln = (lna > lnb ? lna : lnb) + 1;
	AddZeros(_a, ln - lna);
	AddZeros(_b, ln - lnb);

	memset(p, 0, BufSize);
	for (int i = ln - 1; i >= 0; i--)
	{
		res[i] = *(_a + i) + *(_b + i) + *(p + i) - '0';
		if (res[i] > '9')
		{
			res[i] -= 10;
			if (i > 0)
			{
				*(p + i - 1) = 1;
			}
		}
	}
}

// "Нормализованное" вычитание, 0 <= a <= b
void SubN(char *res)
{
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
}

// Умножение на однозначное число и добавление нескольких нулей в конец. Служебная функция
void MulToIntN(const char *a, char b, char *res, size_t zeros)
{
	memcpy(_a, a, BufSize);
	memset(p, 0, BufSize);
	AddZeros(_a, 1);
	size_t ln = strlen(_a);
	for (int i = ln - 1; i >= 0; i--)
	{
		char rs = (*(_a + i) - '0') * b + *(p + i);
		*(res + i) = rs % 10 + '0';
		if (i > 0)
		{
			*(p + i - 1) = rs / 10;
		}
	}
	ZeroTrim(res);

	ln = strlen(res);
	memset(res + ln, '0', zeros);
}

// "Нормализованное" умножение, 0 <= a, b
void MulN(const char *a, const char *b, char *res)
{
	Erase(res);
	*res = '0';
	size_t lnb = strlen(b);
	memset(p, 0, BufSize);
	for (int i = lnb - 1; i >= 0; i--)
	{
		MulToIntN(a, *(b + i) - '0', p, lnb - i - 1);
		Add(res, p, res);
	}
	ZeroTrim(res);
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
void DivN(const char *a, const char *b, char *res)
{
	memset(p, 0, BufSize);
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
}

// Сложение
void Add(const char *a, const char *b, char *res)
{
	memcpy(_a, a, BufSize);
	memcpy(_b, b, BufSize);
	Erase(res);

	ZeroTrim(_a);
	ZeroTrim(_b);

	int sgn = Abs(_a) << 1 | Abs(_b);
	int cmp = Compare(_a, _b);
	switch (sgn)
	{
		case 0:
			AddN(res);
			break;
		case 1:
			if (cmp == 1)
			{
				SubN(res);
			}
			else
			{
				memswap(_a, _b, BufSize);
				SubN(res);
				Negative(res);
			}
			break;
		case 2:
			if (cmp == 1)
			{
				SubN(res);
				Negative(res);
			}
			else
			{
				memswap(_a, _b, BufSize);
				SubN(res);
			}
			break;
		case 3:
			AddN(res);
			Negative(res);
			break;
	}

	ZeroTrim(res);
}

// Вычитание
void Sub(const char *a, const char *b, char *res)
{
	memcpy(_a, a, BufSize);
	memcpy(_b, b, BufSize);
	Erase(res);

	ZeroTrim(_a);
	ZeroTrim(_b);

	int sgn = Abs(_a) << 1 | Abs(_b);
	int cmp = Compare(_a, _b);
	switch (sgn)
	{
		case 0:
			if (cmp == 1)
			{
				SubN(res);
			}
			else
			{
				memswap(_a, _b, BufSize);
				SubN(res);
				Negative(res);
			}
			break;
		case 1:
			AddN(res);
			break;
		case 2:
			AddN(res);
			Negative(res);
			break;
		case 3:
			if (cmp == 1)
			{
				SubN(res);
				Negative(res);
			}
			else
			{
				memswap(_a, _b, BufSize);
				SubN(res);
			}
			break;
	}

	ZeroTrim(res);
}

// Умножение
int Mul(const char *a, const char *b, char *res)
{
	char *__a = calloc(BufSize, 1);
	char *__b = calloc(BufSize, 1);
	memcpy(__a, a, BufSize);
	memcpy(__b, b, BufSize);
	if (!CheckMultiplication(a, b))
	{
		return 0; // Не влезает в буфер
	}

	Erase(res);

	char sga = Abs(__a);
	char sgb = Abs(__b);

	MulN(__a, __b, res);
	if (sga != sgb)
	{
		Negative(res);
	}
	free(__b);
	free(__a);

	return 1; // ОК
}

// Деление
void Div(const char *a, const char *b, char *res)
{
	char *__a = calloc(BufSize, 1);
	char *__b = calloc(BufSize, 1);
	memcpy(__a, a, BufSize);
	memcpy(__b, b, BufSize);
	Erase(res);

	char sga = Abs(__a);
	char sgb = Abs(__b);

	DivN(__a, __b, res);
	if (sga != sgb)
	{
		Negative(res);
	}
	free(__b);
	free(__a);
}

// Проверка вместимости буфера для результата.
int CheckMultiplication(const char *a, const char *b)
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
	return re + 1 <= BufSize;
}
