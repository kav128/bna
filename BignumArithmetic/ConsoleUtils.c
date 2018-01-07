#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "Utils.h"
#include "ConsoleUtils.h"

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

int ConsoleInput(char *a, char *b, char *op, char clrmode)
{
	while (1)
	{
		if (!clrmode)
		{
			printf("a> ");
		}
		scanf("%s", a);
		if (!strncmp(a, "exit", 4))
		{
			return 1; // Код возврата, если прилетело "exit"
		}
		if (!strncmp(a, "help", 4))
		{
			return 2; // Код возврата, если прилетело "help"
		}
		if (!Validate(a))
		{
			printf("Invalid number a\n");
			Erase(a);
		}
		else
		{
			break;
		}
	}

	while (1)
	{
		if (!clrmode)
		{
			printf("b> ");
		}
		scanf("%s", b);
		if (!Validate(b))
		{
			printf("Invalid number b\n");
			Erase(b);
		}
		else
		{
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
	return 0; // Введено все. Можно выполнять вычисления
}

void ConsoleOutput(char *r, char clrmode)
{
	if (!clrmode)
	{
		printf("Ans = ");
	}
	printf("%s\n", r);
}
