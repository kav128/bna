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
extern size_t ISize;
inline void pHeader();
inline void pHelp(char clrmode, enum ioMode iomode);
inline int Calculate(bignum a, bignum b, bignum *r, char op);
inline int ReadParameters(int argc, char* argv[], char *clrmode, enum ioMode *iomode, char **inputfilename, char **outputfilename);


int main(int argc, char* argv[])
{
	// По умолчанию читаем с консоли
	enum ioMode curmode = ioConsole;
	char clrmode = 0;

	char *infn, *outfn; // Имена входного и выходного файлов
	int pcorrect = ReadParameters(argc, argv, &clrmode, &curmode, &infn, &outfn);

	if (!pcorrect)
	{
		printf("\n");
		pHelp(clrmode, curmode);
		return 0;
	}

	if (!clrmode)
	{
		printf("Buffer size: %d bytes\n", BufSize);
	}

	bignum a, b, r;
	a.absnum = calloc(ISize, sizeof(char));
	b.absnum = calloc(ISize, sizeof(char));
	char op;

	if (curmode == ioFile)
	{
		FILE *ifile = NULL;
		FILE *ofile = NULL;
		if (!InitFiles(&ifile, &ofile, infn, outfn))
		{
			return 0;
		}
		FileInput(ifile, &a, &b, &op);
		BufSize = CalcBufSize(a.absnum, b.absnum, op);
		r.absnum = calloc(BufSize, sizeof(char));
		switch (Calculate(a, b, &r, op))
		{
			case 1:
				FileOutput(ofile, r);
				printf("OK\n");
				break;
			case 2:
				printf("Buffer size is too small\n");
				break;
			default:
				printf("Error\n");
		}
		fclose(ofile);
		fclose(ifile);
		free(r.absnum);
	}
	else
	{
		while (1)
		{
			int inp = ConsoleInput(&a, &b, &op, clrmode);
			if (inp == 1)
			{
				break;
			}
			if (inp == 2)
			{
				pHelp(clrmode, curmode);
				continue;
			}

			BufSize = CalcBufSize(a.absnum, b.absnum, op);
			r.absnum = calloc(BufSize, sizeof(char));
			switch (Calculate(a, b, &r, op))
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

			free(r.absnum);
			Erase(a.absnum);
			Erase(b.absnum);
		}
	}

	free(b.absnum);
	free(a.absnum);
	return 0;
}

inline void pHeader()
{
	printf("Bignum Arithmetic\n");
	printf("Artem Krylov, M3105; ITMO University, St. Petersburg, 2017-2018\n\n");
}

inline void pHelp(char clrmode, enum ioMode iomode)
{
	pHeader();
	printf("Syntax: bna [options]\n\n");
	printf("Options:\n");
	printf("-bsize <n>\n\tSets size for string buffers\n");
	printf("-clear\n\tTurns on \"clear mode\" - no prompts are displaying\n");
	printf("-iomode <console|file>\n\tSets input and output modes: console or file\n");
	printf("-input <filename>\n\tSets input file name. Works only in file mode. Required argument if file mode is enabled\n");
	printf("-output <filename>\n\tSets output file name. Works only in file mode. Required argument if file mode is enabled\n\n");
	if (iomode == ioConsole)
	{
		pConsoleHelp(clrmode);
	}
}

inline int ReadIntParameter(int argc, char *argv[], int *curnum, char *parametername, char *description, int *val)
{
	if (argc - 1> *curnum)
	{
		if (Validate(argv[*curnum + 1]))
		{
			*val = atoi(argv[*curnum + 1]);
			// Перескакиваем через один аргумент - мы его уже учли
			(*curnum)++;
		}
		else
		{
			// Ввели некорректное число
			printf("Invalid numberic input \"%s\"\n", argv[*curnum + 1]);
			return 0;
		}
	}
	else
	{
		// Пропустили задание размера после аргумента -bsize
		printf("%s is missing after \"%s\" argument\n", description, parametername);
		return 0;
	}
	return 1;
}

inline int ReadIoModeParameter(int argc, char *argv[], int *curnum, char *parametername, char *description, enum ioMode *val)
{
	if (argc - 1 > *curnum)
	{
		// Пока ввод считаем некорректным. Дальше разберемся
		char ioModeCorrect = 0;
		if (!strcmp(argv[*curnum + 1], "console"))
		{
			ioModeCorrect = 1;
			*val = ioConsole;
		}
		if (!strcmp(argv[*curnum + 1], "file"))
		{
			ioModeCorrect = 1;
			*val = ioFile;
		}

		if (!ioModeCorrect)
		{
			// Ввели некорректное значение
			printf("Invalid mode \"%s\"\n", argv[*curnum + 1]);
			return 0;
		}
		// Перескакиваем через один аргумент - мы его уже учли
		(*curnum)++;
	}
	else
	{
		// Пропустили задание размера после аргумента -bsize
		printf("%s is missing after \"%s\" argument\n", description, parametername);
		return 0;
	}
	return 1;
}

inline int ReadFilenameParameter(int argc, char *argv[], int *curnum, char *parametername, char *description, enum ioMode iomode, char **val)
{
	if (iomode != ioFile)
	{
		printf("File mode is not enabled. Cannot define %s\n", description);
		return 0;
	}

	if (argc - 1 > *curnum)
	{
		*val = argv[*curnum + 1];
		// Перескакиваем через один аргумент - мы его уже учли
		(*curnum)++;
	}
	else
	{
		printf("%s is missing after \"%s\" argument\n", description, parametername);
		return 0;
	}
	return 1;
}

inline int ReadParameters(int argc, char* argv[], char *clrmode, enum ioMode *iomode, char **inputfilename, char **outputfilename)
{
	*inputfilename = NULL;
	*outputfilename = NULL;
	
	char paramsCorrect = 1; // Пока все хорошо
	// Читаем аргументы командной строки
	for (int i = 1; i < argc; i++)
	{
		char argCorrect = 0; // Пока аргумент не подошел ни под один обработчик 
		// Задание размера буферов для чисел
		if (!strcmp(argv[i], "-bsize"))
		{
			argCorrect = 1;
			paramsCorrect = ReadIntParameter(argc, argv, &i, "-bsize", "Buffer size", &BufSize);
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
			paramsCorrect = ReadIoModeParameter(argc, argv, &i, "-iomode", "I/O mode name", iomode, inputfilename);
		}
		// Задание имени входного файла
		if (!strcmp(argv[i], "-input"))
		{
			argCorrect = 1;
			paramsCorrect = ReadFilenameParameter(argc, argv, &i, "-input", "input filename", *iomode, inputfilename);
		}
		// Задание имени выходного файла
		if (!strcmp(argv[i], "-output"))
		{
			argCorrect = 1;
			paramsCorrect = ReadFilenameParameter(argc, argv, &i, "-output", "output filename", *iomode, outputfilename);
		}
		// А правильный ли вообще аргумент нам подкинули?
		if (!argCorrect)
		{
			printf("Invalid argument \"%s\"\n", argv[i]);
			return 0;
		}
		if (!paramsCorrect)
		{
			return 0;
		}
	}

	if (*iomode == ioFile && (*inputfilename == NULL || *outputfilename == NULL))
	{
		printf("Input file name or output file name is not defined. Cannot work in file mode\n");
		return 0;
	}

	return paramsCorrect;
}

inline int Calculate(bignum a, bignum b, bignum *r, char op)
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
