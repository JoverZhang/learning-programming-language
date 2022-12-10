#ifndef _CALC_TOKEN_H_
#define _CALC_TOKEN_H_

#include <stdio.h>

#include "types.h"

typedef enum {
  BAD_TOKEN,
  NUMBER_TOKEN,
  ADD_OPERATION_TOKEN,
  SUB_OPERATION_TOKEN,
  MUL_OPERATION_TOKEN,
  DIV_OPERATION_TOKEN,
  LEFT_PARENT_TOKEN,
  RIGHT_PARENT_TOKEN,
  EOF_TOKEN,
} TokenKind;

typedef struct {
  TokenKind kind;
  f64 value;
  char str[100];
} Token;

#endif
