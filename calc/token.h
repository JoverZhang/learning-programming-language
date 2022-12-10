#ifndef _CALC_TOKEN_H_
#define _CALC_TOKEN_H_

#include <stdio.h>

typedef enum {
  BAD_TOKEN,
  NUMBER_TOKEN,
  ADD_OPERATION_TOKEN,
  SUB_OPERATION_TOKEN,
  EOF_TOKEN,
} TokenKind;

typedef struct {
  TokenKind kind;
  int value;
  char str[100];
} Token;

#endif
