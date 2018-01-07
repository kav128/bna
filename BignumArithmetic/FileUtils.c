#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include "Utils.h"
#include "FileUtils.h"

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

int FileInput(FILE *file, char *a, char *b, char *op)
{
	fscanf(file, "%s", a);
	if (!Validate(a))
	{
		printf("Invalid numeric input\n");
		return 0;
	}
	fscanf(file, "%s", b);
	if (!Validate(b))
	{
		printf("Invalid numeric input\n");
		return 0;
	}
	getc(file); // Символ конца строки
	*op = getc(file);
	return 1;
}
