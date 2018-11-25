#ifndef __COMPILER_H__
#define __COMPILER_H__

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define TMAX 10000000
#define SMAX 100000

enum { Id, Keyword, Type, Number, Literal, Char };

extern char *typeName[];

extern char code[];
extern char strTable[], *strTableEnd;
extern char *tokens[], tokenTop, tokenIdx;
// extern int types[];
extern int tokenLines[], tokenPos[], tokenTypes[];
extern char *tokenStarts[];
extern FILE *pFile;

#define isDigit(ch) ((ch) >= '0' && (ch) <='9')

#define isAlpha(ch) (((ch) >= 'a' && (ch) <='z') || ((ch) >= 'A' && (ch) <= 'Z'))

int lex(char *text);
void compile();

#endif