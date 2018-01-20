#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include "Utils.h"
#include "FileUtils.h"

extern size_t BufSize;

int InitFiles(FILE **in, FILE **out)
{
	char *inputFileName = malloc(255);
	char *outputFileName = malloc(255);

	printf("Input file name>\t");
	scanf("%s", inputFileName);
	*in = fopen(inputFileName, "r");
	if (*in == NULL)
	{
		printf("Unable to read file \"%s\"\n", inputFileName);
		return 0;
	}

	printf("Output file name>\t");
	scanf("%s", outputFileName);
	*out = fopen(outputFileName, "w");
	if (*out == NULL)
	{
		printf("Unable to write to file \"%s\"\n", outputFileName);
		return 0;
	}

	free(outputFileName);
	free(inputFileName);
	return 1;
}

int FileInput(FILE *file, bignum *a, bignum *b, char *op)
{
	char *inpbuf = calloc(BufSize, sizeof(char));
	fscanf(file, "%s", inpbuf);
	if (!Validate(inpbuf))
	{
		printf("Invalid numeric input\n");
		return 0;
	}
	a->sign = Abs(inpbuf);
	memcpy(a->absnum, inpbuf, BufSize);

	fscanf(file, "%s", inpbuf);
	if (!Validate(inpbuf))
	{
		printf("Invalid numeric input\n");
		return 0;
	}
	b->sign = Abs(inpbuf);
	memcpy(b->absnum, inpbuf, BufSize);

	getc(file); // Символ конца строки
	*op = getc(file);
	free(inpbuf);
	return 1;
}

void FileOutput(FILE *file, bignum r)
{
	if (r.sign && strcmp(r.absnum, "0"))
	{
		fprintf(file, "-");
	}
	fprintf(file, "%s\n", r.absnum);
}
