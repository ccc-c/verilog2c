#include <assert.h>
#include "compiler.h"

int E();
void STMT();
void VAR();

int tempIdx = 0, labelIdx = 0;
FILE *pFile;

#define nextTemp() (tempIdx++)
#define nextLabel() (labelIdx++)

#define emit(...) do{ printf( __VA_ARGS__ ); fprintf(pFile, __VA_ARGS__); } while( 0 )
// #define emit printf

int isNext(char *set) {
  char eset[SMAX], etoken[SMAX];
  sprintf(eset, " %s ", set);
  sprintf(etoken, " %s ", tokens[tokenIdx]);
  return (tokenIdx < tokenTop && strstr(eset, etoken) != NULL);
}

char *next() {
  return tokens[tokenIdx++];
}

void back() {
  tokenIdx --;
}

char *skip(char *set) {
  if (isNext(set)) {
    return next();
  } else {
    printf("skip(%s) got %s fail!\n", set, tokens[tokenIdx]);
    printf("-- line: %d pos: %d\n", tokenLines[tokenIdx], tokenPos[tokenIdx]);
    char errorCode[SMAX];
    strncpy(errorCode, tokenStarts[tokenIdx], SMAX-1);
    printf("-- error code: %s\n", errorCode);
    assert(0);
  }
}

// F = (E) | Number | Id
int F() {
  int f;
  if (isNext("(")) { // '(' E ')'
    next(); // (
    f = E();
    next(); // )
  } else { // Number | Id
    f = nextTemp();
    char *item = next();
    emit("t%d = %s\n", f, item);
  }
  return f;
}

// E = F (op E)*
int E() {
  int i1 = F();
  while (isNext("+ - * / & | < > <= >= != ==")) {
    char *op = next();
    int i2 = E();
    int i = nextTemp();
    emit("t%d = t%d %s t%d\n", i, i1, op, i2);
    i1 = i;
  }
  return i1;
}

// while (E) STMT
void WHILE() {
  int whileBegin = nextLabel();
  int whileEnd = nextLabel();
  emit("(L%d)\n", whileBegin);
  skip("while");
  skip("(");
  int e = E();
  emit("goto L%d if T%d\n", whileEnd, e);
  skip(")");
  STMT();
  emit("goto L%d\n", whileBegin);
  emit("(L%d)\n", whileEnd);
}

// if (E) STMT (else STMT)?
void IF() {
  int elseBegin = nextLabel();
  int ifEnd = nextLabel();
  skip("if");
  skip("(");
  int e = E();
  emit("goto L%d if T%d=%d\n", elseBegin, e, 0);
  skip(")");
  STMT();
  emit("goto L%d\n", ifEnd);
  if (isNext("else")) {
    skip("else");
    STMT();
    emit("(L%d)\n", ifEnd);
  }
}

// BLOCK = { STMT* }
void BLOCK(char type) { // type: b:body, c:code
  skip("{");
  if (type == 'b') {
    while (isNext("var")) VAR();
  }
  while (!isNext("}")) {
    STMT();
  }
  skip("}");
}

void RETURN() {
  skip("return");
  int e = E();
  emit("return t%d", e);
}


// CALL = id ( ELIST ) ; 
void CALL() {
  char *id = next();
  skip("(");
  if (!isNext(")")) {
    int e = E();
    emit("push t%d\n", e);
    while (isNext(",")) {
      e = E();
      emit("push t%d\n", e);
    }
  }
  skip(")");
  skip(";");
  emit("call %s\n", id);
}

// STMT = WHILE | IF | BLOCK | RETURN | ASSIGN | CALL
void STMT() {
  if (isNext("while"))
    WHILE();
  else if (isNext("if"))
    IF();
  else if (isNext("{"))
    BLOCK('c'); // code block
  else if (isNext("return"))
    RETURN();
  else {
    char *id = next();
    if (isNext("(")) { // CALL: id (...)
      back();
      CALL();
    } else if (isNext("=")) { // ASSIGN: id = E;
      skip("=");
      int e = E();
      skip(";");
      emit("%s = t%d\n", id, e);
    }
  }
}

// VAR = var idList;
void VAR() {
  skip("var");
  char *id = next(); // 宣告變數
  emit("var %s\n", id);
  while (isNext(",")) {
    skip(",");
    id = next();
    emit("var %s\n", id);
  }
  skip(";");
}

// FUNC = def id (idLIST) BODY   ; BODY = BLOCK('b')
void FUNC() {
  skip("def");
  char *id = next();
  emit("function %s\n", id);
  skip("(");
  if (!isNext(")")) {
    char *name = next(); // 宣告變數
    emit("param %s\n", name);
    while (isNext(",")) {
      skip(",");
      name = next();
      emit("param %s\n", name);
    }
  }
  skip(")");
  BLOCK('b');
}

// PROG = STMT
void PROG() {
  while (tokenIdx < tokenTop) {
    if (isNext("def"))
      FUNC();
    else if (isNext("var"))
      VAR();
    else
      STMT();
  }
  // STMT();
}

void compile() {
  printf("============ compile =============\n");
  tokenIdx = 0;
  PROG();
}