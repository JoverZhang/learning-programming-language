#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"
#include "types.h"

typedef enum {
  INIT_STATUS,
  IN_INT_PART_STATUS,
} LexerStatus;

static const char *st_line;
static int st_line_ops;

void get_token(Token *token) {
  LexerStatus status = INIT_STATUS;
  char cur_char;
  int ops = 0;

  token->kind = BAD_TOKEN;
  while (st_line[st_line_ops] != '\0') {
    cur_char = st_line[st_line_ops];

    // termination condition
    if (status == IN_INT_PART_STATUS &&
        !isdigit(cur_char)) {
      token->kind = NUMBER_TOKEN;
      sscanf(token->str, "%lf", &token->value);
      return;
    }

    if (isspace(cur_char)) {
      // termination in EOF
      if (cur_char == '\n') {
        token->kind = EOF_TOKEN;
        return;
      }
      st_line_ops++;
      continue;
    }

    // eat current char
    token->str[ops++] = st_line[st_line_ops++];
    token->str[ops] = '\0';

    if (cur_char == '+') {
      token->kind = ADD_OPERATION_TOKEN;
      return;
    } else if (cur_char == '-') {
      token->kind = SUB_OPERATION_TOKEN;
      return;
    } else if (cur_char == '*') {
      token->kind = MUL_OPERATION_TOKEN;
      return;
    } else if (cur_char == '/') {
      token->kind = DIV_OPERATION_TOKEN;
      return;
    } else if (cur_char == '(') {
      token->kind = LEFT_PARENT_TOKEN;
      return;
    } else if (cur_char == ')') {
      token->kind = RIGHT_PARENT_TOKEN;
      return;
    }

    if (isdigit(cur_char)) {
      status = IN_INT_PART_STATUS;
    }
    // bad token
    else {
      fprintf(stderr, "bad character(%c)\n", cur_char);
      exit(1);
    }
  }
}

void set_line(const char *line) {
  st_line = line;
  st_line_ops = 0;
}

// parser

static Token st_look_ahead_token;
static int st_look_ahead_token_exists;

static void m_get_token(Token *token) {
  if (st_look_ahead_token_exists) {
    *token = st_look_ahead_token;
    st_look_ahead_token_exists = 0;
  } else {
    get_token(token);
  }
}

static void unget_token(Token *token) {
  st_look_ahead_token = *token;
  st_look_ahead_token_exists = 1;
}

static f64 parse_expression(void);

static f64 parse_primary_expression(void) {
  Token token;
  f64 value = 0;
  int minus_flag = 0;

  // handle negative
  m_get_token(&token);
  if (token.kind == SUB_OPERATION_TOKEN) {
    minus_flag = 1;
    m_get_token(&token);
  }

  if (token.kind == NUMBER_TOKEN) {
    value = token.value;
  }
  // handle parent
  else if (token.kind == LEFT_PARENT_TOKEN) {
    value = parse_expression();
    m_get_token(&token);
    if (token.kind != RIGHT_PARENT_TOKEN) {
      fprintf(stderr, "missing ')' error.\n");
      exit(1);
    }
  }
  // error
  else {
    fprintf(stderr, "syntax error.\n");
    exit(1);
  }

  if (minus_flag) {
    value = -value;
  }
  return value;
}

static f64 parse_term(void) {
  Token token;

  f64 v = parse_primary_expression();
  for (;;) {
    m_get_token(&token);

    if (token.kind == MUL_OPERATION_TOKEN) {
      v *= parse_primary_expression();
    } else if (token.kind == DIV_OPERATION_TOKEN) {
      v /= parse_primary_expression();
    } else {
      unget_token(&token);
      break;
    }
  }
  return v;
}

static f64 parse_expression(void) {
  f64 v1, v2;
  Token token;

  v1 = parse_term();
  for (;;) {
    m_get_token(&token);
    if (token.kind != ADD_OPERATION_TOKEN &&
        token.kind != SUB_OPERATION_TOKEN) {
      unget_token(&token);
      break;
    }

    v2 = parse_term();
    if (token.kind == ADD_OPERATION_TOKEN) {
      v1 += v2;
    } else if (token.kind == SUB_OPERATION_TOKEN) {
      v1 -= v2;
    }
  }

  return v1;
}

static f64 parse_line(void) {
  st_look_ahead_token_exists = 0;
  return parse_expression();
}

int main(int argc, char **argv) {
  set_line("-(3 - 2) * 3\n");

  f64 value = parse_line();
  printf("%f", value);

  return 0;
}

#if 0
void parse_line(const char *line) {
  set_line(line);

  Token token;
  for (;;) {
    get_token(&token);
    if (token.kind == EOF_TOKEN) {
      break;
    }
    printf("kind=%d, str=%s\n", token.kind, token.str);
  }
}

int main(int argc, char **argv) {
  parse_line("1+2\n");
  return 0;
}
#endif
