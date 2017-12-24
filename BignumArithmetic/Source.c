#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h> 
#include "Operations.h"
#include "Utils.h"

extern size_t BufSize;
inline void pHeader();
inline void pHelp(char clrmode);
inline void pConsoleHelp(char clrmode);

int main(int argc, char* argv[])
{
	
	char flg = 1; // Пока все хорошо
	char clrmode = 0;
	// Читаем аргументы командной строки
	for (int i = 1; i < argc; i++)
	{
		char correct = 0; // Пока аргумент не подошел ни под один обработчик 
		// Задание размера буферов для чисел
		if (!strcmp(argv[i], "-bsize"))
		{
			correct = 1;
			if (argc - 1> i)
			{
				if (Validate(argv[i + 1]))
				{
					BufSize = atoi(argv[i + 1]);
					// Перескакиваем через один аргумент - мы его уже учли
					i++;
				}
				else
				{
					// Ввели некорректное число
					printf("Invalid numberic input \"%s\"\n", argv[i + 1]);
					flg = 0;
					break;
				}
			}
			else
			{
				// Пропустили задание размера после аргумента -bsize
				printf("Buffer size is missing after \"-bsize\" argument\n");
				flg = 0;
				break;
			}
		}
		// "Чистый вывод" - только результаты вычислений или сообщения об ошибках. Справка работает, если запрошена
		if (!strcmp(argv[i], "-clear"))
		{
			correct = 1;
			clrmode = 1;
		}
		// А правильный ли вообще аргумент нам подкинули?
		if (!correct)
		{
			printf("Invalid argument \"%s\"\n", argv[i]);
			flg = 0;
		}
	}

	if (!flg)
	{
		pHelp(clrmode);
		return 0;
	}

	if (!clrmode)
	{
		printf("Buffer size: %d bytes\n", BufSize);
	}
	Initialize();

	char *a = calloc(BufSize, 1);
	char *b = calloc(BufSize, 1);
	char *r = calloc(BufSize, 1);

	while (1)
	{
		if (!clrmode)
		{
			printf("a> ");
		}
		scanf("%s", a);
		if (!strncmp(a, "exit", 4))
		{
			break;
		}
		if (!strncmp(a, "help", 4))
		{
			pHelp(clrmode);
			Erase(a);
			continue;
		}
		if (!Validate(a))
		{
			printf("Invalid number a\n");
			Erase(a);
			continue;
		}
		if (!clrmode)
		{
			printf("b> ");
		}
		scanf("%s", b);
		if (!Validate(b))
		{
			printf("Invalid number b\n");
			Erase(a);
			Erase(b);
			continue;
		}
		if (!clrmode)
		{
			printf("o> ");
		}
		char op;
		getchar(); // Символ конца строки
		scanf("%c", &op);
		switch (op)
		{
			case '+':
				Add(a, b, r);
				break;
			case '-':
				Sub(a, b, r);
				break;
			case '*':
				Mul(a, b, r);
				break;
			case '/':
				Div(a, b, r);
				break;
			default:
				printf("Invalid operation\n");
				Erase(a);
				Erase(b);
				continue;
				break;
		}

		if (!clrmode)
		{
			printf("Ans = ");
		}
		printf("%s\n", r);
		Erase(a);
		Erase(b);
		Erase(r);
	}

	free(r);
	free(b);
	free(a);
	Finalize();
	return 0;
}

inline void pHeader()
{
	printf("Bignum Arithmetic\n");
	printf("Artem Krylov, M3105; ITMO University, St. Petersburg, 2017\n\n");
}

inline void pHelp(char clrmode)
{
	pHeader();
	printf("Syntax: bna [options]\n\n");
	printf("Options:\n");
	printf("-bsize <n>\tSets size for string buffers\n");
	printf("-clear\t\tTurns on \"clear mode\" - no prompts are displaying\n\n");
	pConsoleHelp(clrmode);
}

inline void pConsoleHelp(char clrmode)
{
	printf("Console interface usage:\n");
	if (!clrmode)
	{
		printf("a> (Enter operand 'a' here)\n");
		printf("b> (Enter operand 'b' here)\n");
		printf("o> (Enter operator here. Supported operators: '+', '-', '*', '/')\n\n");
		printf("Example:\n");
		printf("a> 25\n");
		printf("b> 89\n");
		printf("o> +\n\n");
	}
	else
	{
		printf("Enter operand 'a'\n");
		printf("Enter operand 'b'\n");
		printf("Enter operator. Supported operators: '+', '-', '*', '/'\n\n");
		printf("Example:\n");
		printf("25\n");
		printf("89\n");
		printf("+\n\n");
	}
}
