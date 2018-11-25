#include "compiler.h"

#define TMAX 10000000
#define LMAX 100

char *typeName[5] = {"Id", "Int", "Keyword", "Literal", "Char"};
char code[TMAX], *p;
char strTable[TMAX], *strTableEnd=strTable;
char *tokens[TMAX], tokenTop=0, tokenIdx=0, token[LMAX];
int tokenLines[TMAX], tokenPos[TMAX];
char *tokenStarts[TMAX];

char *scan() {
  while (isspace(*p)) p++;

  char *start = p;
  int type, lineIdx = 1;
  char *lineBegin;
  if (*p == '\0') return NULL;
  if (*p == '"') {
    p++;
    while (*p != '"') p++;
    p++;
    type = Literal;
  } else if (*p >='0' && *p <='9') { // 數字
    while (*p >='0' && *p <='9') p++;
    type = Int;
  } else if (isAlpha(*p) || *p == '_') { // 變數名稱或關鍵字
    while (isAlpha(*p) || isDigit(*p) || *p == '_') p++;
    type = Id;
  } else if (strchr("<>!=", *p) != NULL) { // < , <=, >, >=, != ==
    p++;
    if (*p == '=') p++;  
  } else if (*p == '\n') {
    lineBegin = p;
    lineIdx ++;
  } else {
    p++;
    type = Char;
  }
  int len = p-start;
  strncpy(token, start, len);
  token[len] = '\0';

  strcpy(strTableEnd, token);
  tokens[tokenTop] = strTableEnd;
  tokenLines[tokenTop] = lineIdx;
  tokenPos[tokenTop] = p - lineBegin;
  tokenStarts[tokenTop] = start;
  strTableEnd += (strlen(token)+1);
  // printf("token=%s\n", token);
  tokenTop++;
  return token;
}

int lex(char *code) {
  p = code;
  tokenTop = 0;
  while (1) {
    char *tok = scan();
    if (tok == NULL) break;
  }
}