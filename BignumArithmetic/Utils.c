#include <malloc.h>
#include <string.h>
#include "Utils.h"

// Размер всех строк для хранения чисел и промежуточных вычислений
const size_t BufSize = 64;

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
	if (shift == len - 1)
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
