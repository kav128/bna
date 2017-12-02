#include <malloc.h>
#include "Operations.h"
#include "Utils.h"

// Размер всех строк для хранения чисел и промежуточных вычислений
const size_t BufSize = 64;
// Строки для локального хранения аргументов (чтобы не подпортить внешние данные)
char *_a, *_b;

void Initialize()
{
	_a = calloc(BufSize, 1);
	_b = calloc(BufSize, 1);
}

void Finalize()
{
	free(_b);
	free(_a);
}
