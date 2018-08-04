#ifndef SCRIPTENGINE_H
#define SCRIPTENGINE_H

#define UNDEFINEDID (unsigned int)(-1)
#define SGERROR -1


void sgInit();
void sgAddSource(const char *text);
int sgCompile();
int *sggetCode(int *sz);
void sgEnd();
char *tokenStrValue(const char *str);
float *tokenFloatValue(const char *str);
int *tokenIntValue(const char *str);
void nextToken();
int findVarLoc(const char *name);
int mExpression(int level);
int mStatement();

#endif