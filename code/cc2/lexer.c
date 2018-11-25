#include "compiler.h"

#define TMAX 10000000
#define LMAX 100

char *typeName[6] = {"Id", "Keyword", "Type", "Number", "Literal", "Char"};
char code[TMAX], *p;
char strTable[TMAX], *strTableEnd=strTable;
char *tokens[TMAX], tokenTop=0, tokenIdx=0, token[LMAX];
int tokenLines[TMAX], tokenPos[TMAX], tokenTypes[TMAX];
char *tokenStarts[TMAX];
char *lineBegin = code;

char *strAdd(char *str) {
  char *strPtr = strTableEnd;
  strcpy(strTableEnd, str);
  strTableEnd += (strlen(str)+1);
  return strPtr;
}

int lineIdx = 1;

char *scan() {
  while (isspace(*p)) {
    if (*p == '\n') {
      lineBegin = p+1;
      lineIdx ++;
    }
    p++;
  }
  char *start = p;
  int type;

  if (*p == '\0') return NULL;
  if (*p == '"') {
    p++;
    while (*p != '"') p++;
    p++;
    type = Literal;
  } else if (*p >='0' && *p <='9') { // 數字
    while (*p >='0' && *p <='9') p++;
    type = Number;
  } else if (isAlpha(*p) || *p == '_') { // 變數名稱或關鍵字
    while (isAlpha(*p) || isDigit(*p) || *p == '_') p++;
    type = Id;
  } else if (strchr("<>!=", *p) != NULL) { // < , <=, >, >=, != ==
    p++;
    if (*p == '=') p++;  
  } /* else if (*p == '\n') {
    lineBegin = p+1;
    lineIdx ++;
  } */ else {
    p++;
    type = Char;
  }
  int len = p-start;
  strncpy(token, start, len);
  token[len] = '\0';

  // strcpy(strTableEnd, token);
  // tokens[tokenTop] = strTableEnd;
  tokens[tokenTop] = strAdd(token);
  tokenTypes[tokenTop] = type;
  tokenLines[tokenTop] = lineIdx;
  tokenPos[tokenTop] = p - lineBegin;
  tokenStarts[tokenTop] = start;
  
  // strTableEnd += (strlen(token)+1);
  // printf("token=%s\n", token);
  tokenTop++;
  return token;
}

int lexDump() {
  printf("========== lexDump() ==============\n");
  for (int i=0; i<tokenTop; i++) {
    printf("%04d:%-10s line=%d pos=%d\n", i, tokens[i], tokenLines[i], tokenPos[i]);
  }
}

int lex(char *code) {
  p = code;
  tokenTop = 0;
  while (1) {
    char *tok = scan();
    if (tok == NULL) break;
  }
  lexDump();
}
