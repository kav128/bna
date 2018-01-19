#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h> 
#include "Operations.h"
#include "Utils.h"
#include "ConsoleUtils.h"
#include "FileUtils.h"

enum ioMode
{
	ioConsole, ioFile
};
extern size_t BufSize;
inline void pHeader();
inline void pHelp(char clrmode, enum ioMode iomode);
inline int Calculate(char *a, char *b, char *r, char op);
inline int ReadParameters(int argc, char* argv[], char *clrmode, enum ioMode *iomode);


int main(int argc, char* argv[])
{
	// По умолчанию читаем с консоли
	enum ioMode curmode = ioConsole;
	char clrmode = 0;
	int pcorrect = ReadParameters(argc, argv, &clrmode, &curmode);

	if (!pcorrect)
	{
		pHelp(clrmode, curmode);
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

	char op;

	if (curmode == ioFile)
	{
		FILE *ifile = NULL;
		FILE *ofile = NULL;
		while (!InitFiles(&ifile, &ofile)); // Повторяем, пока не получим валидные указатели на файлы
		FileInput(ifile, a, b, &op);
		switch (Calculate(a, b, r, op))
		{
			case 1:
				fprintf(ofile, "%s\n", r);
				break;
			case 2:
				printf("Buffer size is too small\n");
				break;
			default:
				printf("Error\n");
		}
		fclose(ofile);
		fclose(ifile);
	}
	else
	{
		while (1)
		{
			int inp = ConsoleInput(a, b, &op, clrmode);
			if (inp == 1)
			{
				break;
			}
			if (inp == 2)
			{
				pHelp(clrmode, curmode);
				continue;
			}

			switch (Calculate(a, b, r, op))
			{
				case 1:
					ConsoleOutput(r, clrmode);
					break;
				case 2:
					printf("Buffer size is too small\n");
					break;
				default:
					printf("Error\n");
			}

			Erase(a);
			Erase(b);
			Erase(r);
		}
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

inline void pHelp(char clrmode, enum ioMode iomode)
{
	pHeader();
	printf("Syntax: bna [options]\n\n");
	printf("Options:\n");
	printf("-bsize <n>\n\tSets size for string buffers\n");
	printf("-clear\n\tTurns on \"clear mode\" - no prompts are displaying\n");
	printf("-iomode <console|file>\n\tSets input and output modes: console or file\n\n");
	if (iomode == ioConsole)
	{
		pConsoleHelp(clrmode);
	}
}

inline int ReadParameters(int argc, char* argv[], char *clrmode, enum ioMode *iomode)
{
	char paramsCorrect = 1; // Пока все хорошо
	// Читаем аргументы командной строки
	for (int i = 1; i < argc; i++)
	{
		char argCorrect = 0; // Пока аргумент не подошел ни под один обработчик 
		// Задание размера буферов для чисел
		if (!strcmp(argv[i], "-bsize"))
		{
			argCorrect = 1;
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
					paramsCorrect = 0;
					break;
				}
			}
			else
			{
				// Пропустили задание размера после аргумента -bsize
				printf("Buffer size is missing after \"-bsize\" argument\n");
				paramsCorrect = 0;
				break;
			}
		}
		// "Чистый вывод" - только результаты вычислений или сообщения об ошибках. Справка работает, если запрошена
		if (!strcmp(argv[i], "-clear"))
		{
			argCorrect = 1;
			*clrmode = 1;
		}
		// Откуда читаем и куда пишем: файл или консоль
		if (!strcmp(argv[i], "-iomode"))
		{
			argCorrect = 1;
			if (argc - 1 > i)
			{
				// Пока ввод считаем некорректным. Дальше разберемся
				char ioModeCorrect = 0;
				if (!strcmp(argv[i + 1], "console"))
				{
					ioModeCorrect = 1;
					*iomode = ioConsole;
				}
				if (!strcmp(argv[i + 1], "file"))
				{
					ioModeCorrect = 1;
					*iomode = ioFile;
				}

				if (!ioModeCorrect)
				{
					// Ввели некорректное значение
					printf("Invalid mode \"%s\"\n", argv[i + 1]);
					paramsCorrect = 0;
					break;
				}
				// Перескакиваем через один аргумент - мы его уже учли
				i++;
			}
			else
			{
				// Пропустили задание размера после аргумента -bsize
				printf("I/O mode name is missing after \"-iomode\" argument\n");
				paramsCorrect = 0;
				break;
			}
		}
		// А правильный ли вообще аргумент нам подкинули?
		if (!argCorrect)
		{
			printf("Invalid argument \"%s\"\n", argv[i]);
			paramsCorrect = 0;
		}
	}

	return paramsCorrect;
}

inline int Calculate(char *a, char *b, char *r, char op)
{
	switch (op)
	{
		case '+':
			Add(a, b, r);
			break;
		case '-':
			Sub(a, b, r);
			break;
		case '*':
			if (!Mul(a, b, r))
			{
				return 2;
			}
			break;
		case '/':
			Div(a, b, r);
			break;
		default:
			return 0; // Оператор неверный
	}
	return 1; // Все ок
}
