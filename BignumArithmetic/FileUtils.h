#ifndef BIGNUM_FILEUTILS_H
#define BIGNUM_FILEUTILS_H

int InitFiles(FILE **in, FILE **out, char *infn, char *outfn);
int FileInput(FILE *file, bignum *a, bignum *b, char *op);
void FileOutput(FILE *file, bignum r);

#endif //BIGNUM_FILEUTILS_H
