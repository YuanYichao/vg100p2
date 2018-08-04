#include "scriptEngine.h"

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
static int code[10000];
static int *out;
static const char *src;
static unsigned int pos;
static char Tbuffer[256];
static char ErrorLog[256];
static int row;
static int col;
static int st;
static int bk;

static char varNames[256][17];
static unsigned int varLoc[256];
static int nmax;

typedef struct _token {
  unsigned int type;
  void *value;
} token;

typedef struct _func {
  char name[16];
  unsigned int argNum;
} func;

static func funcTable[7] = {{"sin", 1},  {"cos", 1},  {"tan", 1}, {"atan", 1},
                            {"asin", 1}, {"acos", 1}, {"time", 0}};
static token curToken;
enum {
  CIMM,
  CLF,
  CSF,
  CPUSH,
  CJMP,
  CJZ,
  COR,
  CAND,
  CEQ,
  CBIG,
  CSMA,
  CADD,
  CSUB,
  CDIV,
  CMUL,
  CPOW,
  CNOT,
  CNEG,
  CSIN,
  CCOS,
  CTAN,
  CATAN,
  CASIN,
  CACOS,
  CTIME,
  CSTART,
  CBACK,
  CBIND,
  CCLOCK
};

enum {
  END = -2,
  UNDEFINEDSTATE = -1,
  ST = 128,
  BK,
  VAR,
  ELSE,
  LEFTBRACE,
  RIGHTBRACE,
  LEFTPARE,
  RIGHTPARE,
  COMMA,
  SEMICOLON,
  IF,
  ID,
  NUM,
  NAME,
  FUNC,
  NOT,
  NEG,
  ASSIGN,
  OR,
  AND,
  BIGGER,
  SMALLER,
  EQUAL,
  PLUS,
  MINUS,
  DIVIDE,
  MOD,
  MULTIPLY,
  POWER,
  MMM
};

#define STERRORHELP(condition, err)            \
  if (condition) {                             \
    sprintf(ErrorLog, "%d:%d " err, row, col); \
    return -1;                                 \
  }

char *tokenStrValue(const char *str) {
  char *res = malloc((sizeof(str) + 1) * sizeof(char));
  strcpy(res, str);
  return res;
}

float *tokenFloatValue(const char *str) {
  float *res = malloc(sizeof(float));
  *res = atof(str);
  return res;
}

int *tokenIntValue(const char *str) {
  int *res = malloc(sizeof(int));
  *res = strtol(str, NULL, 10);
  return res;
}

// usable
void nextToken() {
  int state = UNDEFINEDSTATE;
  int bufferPos = 0, i = 0, mt = 0, d = 0;
  if (curToken.value != NULL) {
    free(curToken.value);
    curToken.value = NULL;
  }
  while (isspace(src[pos])) pos++;
  if (src[pos] == 0) {
    curToken.type = END;
    return;
  }
  while (1) {
    if (state == UNDEFINEDSTATE) {
      if (isalpha(src[pos])) {
        state = ID;
      }
      if (isdigit(src[pos])) {
        state = NUM;
        d = 0;
      }
      if (src[pos] == '+') {
        curToken.type = PLUS;
        pos++;
        return;
      }
      if (src[pos] == '-') {
        if (curToken.type != NUM && curToken.type != NAME) {
          curToken.type = NEG;
          pos++;
          return;
        } else {
          curToken.type = MINUS;
          pos++;
          return;
        }
      }
      if (src[pos] == '/') {
        curToken.type = DIVIDE;
        pos++;
        return;
      }
      if (src[pos] == '*') {
        curToken.type = MULTIPLY;
        pos++;
        return;
      }
      if (src[pos] == '^') {
        curToken.type = POWER;
        pos++;
        return;
      }
      if (src[pos] == '.') {
        state = NUM;
        d = 1;
      }
      if (src[pos] == '(') {
        curToken.type = LEFTPARE;
        pos++;
        return;
      }
      if (src[pos] == ')') {
        curToken.type = RIGHTPARE;
        pos++;
        return;
      }
      if (src[pos] == '{') {
        curToken.type = LEFTBRACE;
        pos++;
        return;
      }
      if (src[pos] == '}') {
        curToken.type = RIGHTBRACE;
        pos++;
        return;
      }
      if (src[pos] == ';') {
        curToken.type = SEMICOLON;
        pos++;
        return;
      }
      if (src[pos] == '<') {
        curToken.type = SMALLER;
        pos++;
        return;
      }
      if (src[pos] == '>') {
        curToken.type = BIGGER;
        pos++;
        return;
      }
      if (src[pos] == '=') {
        state = ASSIGN;
      }
      if (src[pos] == '&') {
        curToken.type = AND;
        pos++;
        return;
      }
      if (src[pos] == '|') {
        curToken.type = OR;
        pos++;
        return;
      }
      if (src[pos] == '%') {
        curToken.type = MOD;
        pos++;
        return;
      }
      if (src[pos] == ',') {
        curToken.type = COMMA;
        pos++;
        return;
      }
    }
    if (state == ID) {
      if (isalnum(src[pos])) {
        Tbuffer[bufferPos++] = src[pos++];
      } else {
        Tbuffer[bufferPos] = 0;
        if (!strcmp(Tbuffer, "if")) {
          curToken.type = IF;
          return;
        }
        if (!strcmp(Tbuffer, "var")) {
          curToken.type = VAR;
          return;
        }
        if (!strcmp(Tbuffer, "start")) {
          curToken.type = ST;
          return;
        }
        if (!strcmp(Tbuffer, "back")) {
          curToken.type = BK;
          return;
        }
        if (!strcmp(Tbuffer, "else")) {
          curToken.type = ELSE;
          return;
        }
        for (i = 0; i < 7; i++) {
          if (!strcmp(Tbuffer, funcTable[i].name)) {
            curToken.type = FUNC;
            curToken.value = tokenStrValue(Tbuffer);
            return;
          }
        }
        curToken.type = NAME;
        curToken.value = tokenStrValue(Tbuffer);
        return;
      }
    }
    if (state == NUM) {
      if (src[pos] == '-' && mt == 0) {
        mt++;
        Tbuffer[bufferPos++] = src[pos++];
      }
      if (isdigit(src[pos])) {
        mt++;
        Tbuffer[bufferPos++] = src[pos++];
      } else if (src[pos] == '.' && d == 0) {
        mt++;
        d++;
        Tbuffer[bufferPos++] = src[pos++];
      } else {
        curToken.type = NUM;
        Tbuffer[bufferPos] = 0;
        curToken.value = tokenFloatValue(Tbuffer);
        return;
      }
    }
    if (state == ASSIGN) {
      if (bufferPos == 0) {
        bufferPos++;
        pos++;
      } else if (src[pos] == '=') {
        curToken.type = EQUAL;
        pos++;
        return;
      } else {
        curToken.type = ASSIGN;
        return;
      }
    }
  }
}

void sgInit() {
  src = NULL;
  out = code;
  st = 0;
  bk = 0;
  nmax = 0;
  curToken.type = UNDEFINEDSTATE;
  curToken.value = NULL;
  pos = 0;
  memset(varNames, 0, 256 * 17);
  memset(varLoc, 0, 256 * sizeof(float));
}

void sgAddSource(const char *text) { src = text; }
int findVarLoc(const char *name) {
  int i;
  for (i = 0; i < nmax; i++) {
    if (!strcmp(name, varNames[i])) {
      return varLoc[i];
    }
  }
  return -1;
}

int mExpression(int level) {
  int t;
  if (curToken.type == LEFTPARE) {
    nextToken();
    mExpression(NUM);
    nextToken();
    STERRORHELP(curToken.type != ')', "cannot match )");
  }
  if (curToken.type == NAME) {
    t = findVarLoc(curToken.value);
    if (t == -1) {
      STERRORHELP(1, "undefined variable");
      return -1;
    }
    *out++ = CIMM;
    *out++ = t;
    *out++ = CLF;
    nextToken();
  }

  if (curToken.type == FUNC && !strcmp(curToken.value, "sin")) {
    nextToken();
    STERRORHELP(curToken.type != LEFTPARE, "expect ( when call a function");
    nextToken();
    mExpression(NUM);
    *out++ = CSIN;
    STERRORHELP(curToken.type != RIGHTPARE, "expect )");
    nextToken();
  }
  if (curToken.type == FUNC && !strcmp(curToken.value, "cos")) {
    nextToken();
    STERRORHELP(curToken.type != LEFTPARE, "expect ( when call a function");
    nextToken();
    mExpression(NUM);
    *out++ = CCOS;
    STERRORHELP(curToken.type != RIGHTPARE, "expect )");
    nextToken();
  }
  if (curToken.type == FUNC && !strcmp(curToken.value, "tan")) {
    nextToken();
    STERRORHELP(curToken.type != LEFTPARE, "expect ( when call a function");
    nextToken();
    mExpression(NUM);
    *out++ = CTAN;
    STERRORHELP(curToken.type != RIGHTPARE, "expect )");
    nextToken();
  }
  if (curToken.type == FUNC && !strcmp(curToken.value, "acos")) {
    nextToken();
    STERRORHELP(curToken.type != LEFTPARE, "expect ( when call a function");
    nextToken();
    mExpression(NUM);
    *out++ = CACOS;
    STERRORHELP(curToken.type != RIGHTPARE, "expect )");
    nextToken();
  }
  if (curToken.type == FUNC && !strcmp(curToken.value, "asin")) {
    nextToken();
    STERRORHELP(curToken.type != LEFTPARE, "expect ( when call a function");
    nextToken();
    mExpression(NUM);
    *out++ = CASIN;
    STERRORHELP(curToken.type != RIGHTPARE, "expect )");
    nextToken();
  }
  if (curToken.type == FUNC && !strcmp(curToken.value, "atan")) {
    nextToken();
    STERRORHELP(curToken.type != LEFTPARE, "expect ( when call a function");
    nextToken();
    mExpression(NUM);
    *out++ = CATAN;
    STERRORHELP(curToken.type != RIGHTPARE, "expect )");
    nextToken();
  }
  if (curToken.type == FUNC && !strcmp(curToken.value, "time")) {
    nextToken();
    STERRORHELP(curToken.type != LEFTPARE, "expect ( when call a function");
    nextToken();
    *out++ = CCLOCK;
    STERRORHELP(curToken.type != RIGHTPARE, "expect )");
    nextToken();
  }

  if (curToken.type == NUM) {
    *out++ = CIMM;
    *out++ = *(int *)curToken.value;
    nextToken();
  }
  if (curToken.type == NOT) {
    nextToken();
    mExpression(MMM);
    *out++ = CNOT;
  }
  if (curToken.type == NEG) {
    nextToken();
    mExpression(MMM);
    *out++ = CNEG;
  }

  while (curToken.type >= level) {
    if (curToken.type == PLUS) {
      nextToken();
      *out++ = CPUSH;
      mExpression(PLUS);
      *out++ = CADD;
    }
    if (curToken.type == MINUS) {
      nextToken();
      *out++ = CPUSH;
      mExpression(MINUS);
      *out++ = CSUB;
    }
    if (curToken.type == DIVIDE) {
      nextToken();
      *out++ = CPUSH;
      mExpression(DIVIDE);
      *out++ = CDIV;
    }
    if (curToken.type == MULTIPLY) {
      nextToken();
      *out++ = CPUSH;
      mExpression(MULTIPLY);
      *out++ = CMUL;
    }
    if (curToken.type == POWER) {
      nextToken();
      *out++ = CPUSH;
      mExpression(POWER);
      *out++ = CPOW;
    }
    if (curToken.type == EQUAL) {
      nextToken();
      *out++ = CPUSH;
      mExpression(EQUAL);
      *out++ = CEQ;
    }
    if (curToken.type == BIGGER) {
      nextToken();
      *out++ = CPUSH;
      mExpression(BIGGER);
      *out++ = CBIG;
    }
    if (curToken.type == SMALLER) {
      nextToken();
      *out++ = CPUSH;
      mExpression(SMALLER);
      *out++ = CSMA;
    }
    if (curToken.type == ASSIGN) {
      nextToken();
      STERRORHELP(*(out - 1) != CLF, "can only assign to lvalue")
      *(out - 1) = CPUSH;
      mExpression(ASSIGN);
      *out++ = CSF;
    }
  }
    return 0;
}

int mStatement() {
  int *p;

  if (curToken.type == VAR) {
    nextToken();
    STERRORHELP(curToken.type != NAME, "must have name to declare");
    strcpy(varNames[nmax], curToken.value);
    varLoc[nmax] = nmax;
    nmax++;
    if (!st) {
      *out++ = CBIND;
    }
    nextToken();
    STERRORHELP(curToken.type != SEMICOLON, "must have ; after statement");
    nextToken();
  } else if (curToken.type == IF) {
    nextToken();
    STERRORHELP(curToken.type != LEFTPARE, "must have ( after if");
    nextToken();
    mExpression(NUM);
    STERRORHELP(curToken.type != RIGHTPARE, "must have ) after if");
    nextToken();
    *out++ = CJZ;
    p = out++;
    mStatement();
    if (curToken.type == ELSE) {
      nextToken();
      *out++ = CJMP;
      *p = out - code + 1;
      p = out++;
      mStatement();
    }
    *p = out - code;
  } else if (curToken.type == ST) {
    STERRORHELP(bk, "cannot start after back");
    nextToken();
    st = 1;
    *out++ = CSTART;
    STERRORHELP(curToken.type != SEMICOLON, "must have ; after statement");
    nextToken();
  } else if (curToken.type == BK) {
    STERRORHELP(!st, "cannot back before start");
    nextToken();
    bk = 1;
    *out++ = CBACK;
    STERRORHELP(curToken.type != SEMICOLON, "must have ; after statement");
    nextToken();
  } else if (curToken.type == LEFTBRACE) {
    nextToken();
    while (curToken.type != RIGHTBRACE) {
      mStatement();
    }
  } else if (curToken.type == SEMICOLON) {
    nextToken();
  } else {
    mExpression(NUM);
    STERRORHELP(curToken.type != SEMICOLON, "must have ; after statement");
    nextToken();
  }
  return 0;
}

int sgCompile() {
  nextToken();
  while (curToken.type != END) {
    if( -1 == mStatement() ){
		return -1;
	}
  }
}

int *sggetCode(int *sz) {
  *sz = (out - code) * 4;
  return code;
}

void sgEnd() {
  if (curToken.value != NULL) {
    free(curToken.value);
  }
}

int connectPython_sz(char* source, int* code_return){
    int sz;
    int* code;
    sgInit();
    sgAddSource(source);
    if( -1 == sgCompile()){
		strcpy((char *)code_return, ErrorLog);
		return -1;
	}
    code=sggetCode(&sz);
    for(int i=0;i< sz / 4;i++){code_return[i]=code[i];}
    return sz;
}

int test(){
  return 1;
}

