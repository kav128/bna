#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "Utils.h"
#include "ConsoleUtils.h"

extern size_t BufSize;

void pConsoleHelp(char clrmode)
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

int ConsoleInput(bignum *a, bignum *b, char *op, char clrmode)
{
	char *inpbuf = calloc(BufSize, sizeof(char));
	while (1)
	{
		if (!clrmode)
		{
			printf("a> ");
		}
		scanf("%s", inpbuf);
		if (!strcmp(inpbuf, "exit"))
		{
			return 1; // Код возврата, если прилетело "exit"
		}
		if (!strcmp(inpbuf, "help"))
		{
			return 2; // Код возврата, если прилетело "help"
		}
		if (!Validate(inpbuf))
		{
			printf("Invalid number a\n");
			Erase(inpbuf);
		}
		else
		{
			a->sign = Abs(inpbuf);
			memcpy(a->absnum, inpbuf, BufSize);
			break;
		}
	}

	while (1)
	{
		if (!clrmode)
		{
			printf("b> ");
		}
		scanf("%s", inpbuf);
		if (!Validate(inpbuf))
		{
			printf("Invalid number b\n");
			Erase(inpbuf);
		}
		else
		{
			b->sign = Abs(inpbuf);
			memcpy(b->absnum, inpbuf, BufSize);
			break;
		}
	}

	while (1)
	{
		if (!clrmode)
		{
			printf("o> ");
		}
		getchar(); // Символ конца строки
		scanf("%c", op);

		if (*op == '+' || *op == '-' || *op == '*' || *op == '/')
		{
			break;
		}
		else
		{
			printf("Invalid operator\n");
		}
	}
	free(inpbuf);
	return 0; // Введено все. Можно выполнять вычисления
}

void ConsoleOutput(bignum r, char clrmode)
{
	if (!clrmode)
	{
		printf("Ans = ");
	}
	if (r.sign && strcmp(r.absnum, "0"))
	{
		printf("-");
	}
	printf("%s\n", r.absnum);
}
