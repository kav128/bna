#include <malloc.h>
#include <string.h>
#include "Utils.h"

// Размер всех строк для хранения чисел и промежуточных вычислений
extern const size_t BufSize;

// Избавляемся от лидирующих нулей
void ZeroTrim(char *str)
{
	size_t shift = 0;
	// Вдруг число отрицательное
	int start = *str == '-' ? 1 : 0;
	size_t len = strlen(str) + 1;
	while (*(str + start + shift) == '0')
	{
		shift++;
	}
	if (shift == len - start - 1)
	{
		shift--;
	}

	memcpy(str + start, str + start + shift, len - shift);
	len -= shift;
	// Зануляем байты в конце строки
	memset(str + start + len, 0, len);
}

// Добавляем лидирующие нули
void AddZeros(char *str, size_t zcount)
{
	if (zcount < 1)
	{
		return;
	}
	size_t len = strlen(str);
	memcpy(str + zcount, str, len);
	memset(str, '0', zcount);
}

// Проверяем, является ли строка правильным числом
char Validate(char *str)
{
	char res = 1;
	if (*str == '-')
	{
		str++;
	}

	do
	{
		if (*str < '0' || *str > '9')
		{
			res = 0;
		}
	}
	while (*(++str) != 0 && res == 1);

	return res;
}

// Подчищаем всю строку
void Erase(char *str)
{
	memset(str, 0, BufSize);
}

// Берем модуль числа и возвращаем 1, если число было отрицательным или 0, если положительным
char Abs(char *a)
{
	char res = *a == '-' ? 1 : 0;
	if (res)
	{
		*a = '0';
	}
	ZeroTrim(a);
	return res;
}

// Меняем знак у числа
void Negative(char *a)
{
	ZeroTrim(a);
	if (*a != '0')
	{
		if (*a == '-')
		{
			Abs(a);
		}
		else
		{
			size_t ln = strlen(a);
			memcpy(a + 1, a, ln);
			*a = '-';
		}
	}
}

// Сравниваем два числа
int Compare(const char *a, const char *b)
{
	char *_a, *_b;
	_a = calloc(BufSize, 1);
	_b = calloc(BufSize, 1);
	memcpy(_a, a, BufSize);
	memcpy(_b, b, BufSize);
	ZeroTrim(_a);
	ZeroTrim(_b);

	int sna = Abs(_a);
	int snb = Abs(_b);

	size_t lna = strlen(_a);
	size_t lnb = strlen(_b);
	int res;

	if (sna == snb)
	{
		if (lna != lnb)
		{
			res = lna > lnb ? 1 : -1;
		}
		else
		{
			int i = 0;
			while (i < lna && *(_a + i) == *(_b + i))
			{
				i++;
			}
			res = *(_a + i) == *(_b + i) ? 0 : *(_a + i) > *(_b + i) ? 1 : -1;
		}

		if (sna)
		{
			res = -res;
		}
	}
	else
	{
		res = sna ? -1 : 1;
	}


	free(_b);
	free(_a);
	return res;
}

void memswap(char *a, char *b, size_t len)
{
	for (size_t i = 0; i < len; i++)
	{
		*(a + i) ^= *(b + i);
		*(b + i) ^= *(a + i);
		*(a + i) ^= *(b + i);
	}
}
