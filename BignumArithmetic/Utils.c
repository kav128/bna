#include <malloc.h>
#include <string.h>
#include "Utils.h"

// Размер всех строк для хранения чисел и промежуточных вычислений
extern size_t BufSize;

// Избавляемся от лидирующих нулей
void ZeroTrim(char *str)
{
	size_t shift = 0;
	*(str + BufSize - 1) = 0; // Должна же строка нулем заканчиваться. А вдруг не заканчивается?
	size_t len = strlen(str) + 1;
	while (*(str + shift) == '0')
	{
		shift++;
	}
	if (shift == len - 1)
	{
		shift--;
	}
	if (shift == 0)
	{
		return;
	}

	memcpy(str, str + shift, len - shift);
	// Зануляем байты в конце строки
	memset(str + len, 0, shift);
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
	int res = 0;
	int sta = 0, stb = 0; // Стартовые индексы
	signed char sga = 1, sgb = 1; // Знаки чисел. По дефолту все положительно
	if (*a == '-')
	{
		sga = -1;
		sta++;
	}
	if (*b == '-')
	{
		sgb = -1;
		stb++;
	}
	if (sga != sgb)
	{
		return sga > sgb ? 1 : -1;
	}
	while (*(a + sta) == '0')
	{
		sta++;
	}
	while (*(b + stb) == '0')
	{
		stb++;
	}
	// Нашли, откуда числа начинаются
	int lna = strlen(a) - sta;
	int lnb = strlen(b) - stb;
	char *enda = a + strlen(a);
	char *endb = b + strlen(b);

	if (lna != lnb)
	{
		return sga * (lna > lnb ? 1 : -1);
	}

	char *posa = a + sta, *posb = b + stb;
	while (posa < enda && posb < endb)
	{
		if (*posa != *posb)
		{
			return sga * (*posa > *posb ? 1 : -1);
		}
		posa++;
		posb++;
	}
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
