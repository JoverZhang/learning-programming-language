#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"

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
      sscanf(token->str, "%d", &token->value);
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
    }

    if (isdigit(cur_char)) {
      status = IN_INT_PART_STATUS;
    }
    // bad token
    else {
      fprintf(stderr, "bad character(%c)\n", cur_char);
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

static int parse_primary_expression(void) {
  Token token;

  m_get_token(&token);
  if (token.kind == NUMBER_TOKEN) {
    return token.value;
  }

  fprintf(stderr, "syntax error.\n");
  exit(1);
  return 0;
}

static int parse_expression(void) {
  int v1, v2;
  Token token;

  v1 = parse_primary_expression();
  for (;;) {
    m_get_token(&token);
    if (token.kind != ADD_OPERATION_TOKEN &&
        token.kind != SUB_OPERATION_TOKEN) {
      unget_token(&token);
      break;
    }

    v2 = parse_primary_expression();
    if (token.kind == ADD_OPERATION_TOKEN) {
      v1 += v2;
    } else if (token.kind == SUB_OPERATION_TOKEN) {
      v1 -= v2;
    }
  }

  return v1;
}

static int parse_line(void) {
  st_look_ahead_token_exists = 0;
  return parse_expression();
}

int main(int argc, char **argv) {
  int value;

  set_line("1 + 2 - 3 + 4\n");

  value = parse_line();
  printf("%d", value);

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
