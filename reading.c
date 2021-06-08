/* Reading Header File */
#include "reading.h"

lval *lval_read_sym(char *s, int *i) {

  /* Allocate Empty String */
  char *part = calloc(1, 1);

  /* While valid identifier characters */
  while (strchr("abcdefghijklmnopqrstuvwxyz"
                "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                "0123456789_+-*\\/=<>!&",
                s[*i]) &&
         s[*i] != '\0') {

    /* Append character to end of string */
    part = realloc(part, strlen(part) + 2);
    part[strlen(part) + 1] = '\0';
    part[strlen(part) + 0] = s[*i];
    (*i)++;
  }

  /* Check if Identifier looks like number */
  int is_num = strchr("-0123456789", part[0]) != NULL;
  for (int j = 1; j < strlen(part); j++) {
    if (strchr("0123456789", part[j]) == NULL) {
      is_num = 0;
      break;
    }
  }
  if (strlen(part) == 1 && part[0] == '-') {
    is_num = 0;
  }

  /* Add symbol or number as lval */
  lval *x = NULL;
  if (is_num) {
    errno = 0;
    long v = strtol(part, NULL, 10);
    x = (errno != ERANGE) ? lval_num(v) : lval_err("Invalid Number %s", part);
  } else {
    x = lval_sym(part);
  }

  /* Free temp string */
  free(part);

  /* Return lval */
  return x;
}

lval *lval_read_str(char *s, int *i) {

  /* Allocate empty string */
  char *part = calloc(1, 1);

  /* Move forward one step past initial " character */
  (*i)++;
  while (s[*i] != '"') {

    char c = s[*i];

    /* If end of input then there is an unterminated string literal */
    if (c == '\0') {
      free(part);
      return lval_err("Unexpected end of input");
    }

    /* If backslash then unescape character after it */
    if (c == '\\') {
      (*i)++;
      /* Check next character is escapable */
      if (strchr(lval_str_unescapable, s[*i])) {
        c = lval_str_unescape(s[*i]);
      } else {
        free(part);
        return lval_err("Invalid escape sequence \\%c", s[*i]);
      }
    }

    /* Append character to string */
    part = realloc(part, strlen(part) + 2);
    part[strlen(part) + 1] = '\0';
    part[strlen(part) + 0] = c;
    (*i)++;
  }
  /* Move forward past the final " character */
  (*i)++;

  lval *x = lval_str(part);

  /* free temp string */
  free(part);

  return x;
}

lval *lval_read_expr(char *s, int *i, char end) {

  /* Either create a new qexpr or sexpr */
  lval *x = (end == '}') ? lval_qexpr() : lval_sexpr();

  /* While not at end character keep reading lvals */
  while (s[*i] != end) {
    lval *y = lval_read(s, i);
    /* If an error then return this and stop */
    if (y->type == LVAL_ERR) {
      lval_del(x);
      return y;
    } else {
      lval_add(x, y);
    }
  }

  /* Move past end character */
  (*i)++;

  return x;
}

lval *lval_read(char *s, int *i) {

  /* Skip all trailing whitespace and comments */
  while (strchr(" \t\v\r\n;", s[*i]) && s[*i] != '\0') {
    if (s[*i] == ';') {
      while (s[*i] != '\n' && s[*i] != '\0') {
        (*i)++;
      }
    }
    (*i)++;
  }

  lval *x = NULL;

  /* If we reach end of input then we're missing something */
  if (s[*i] == '\0') {
    return lval_err("Unexpexted end of input");
  }

  /* If next character is ( then read S-Expr */
  else if (s[*i] == '(') {
    (*i)++;
    x = lval_read_expr(s, i, ')');
  }

  /* If next character is { then read Q-Expr */
  else if (s[*i] == '{') {
    (*i)++;
    x = lval_read_expr(s, i, '}');
  }

  /* If next character is part of a symbol then read symbol */
  else if (strchr("abcdefghijklmnopqrstuvwxyz"
                  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                  "0123456789_+-*\\/=<>!&",
                  s[*i])) {
    x = lval_read_sym(s, i);
  }

  /* If next character is " then read string */
  else if (strchr("\"", s[*i])) {
    x = lval_read_str(s, i);
  }

  /* Encountred some unexpected character */
  else {
    x = lval_err("Unexpected character %c", s[*i]);
  }

  /* Skip all trailing whitespace and comments */
  while (strchr(" \t\v\r\n;", s[*i]) && s[*i] != '\0') {
    if (s[*i] == ';') {
      while (s[*i] != '\n' && s[*i] != '\0') {
        (*i)++;
      }
    }
    (*i)++;
  }

  return x;
}
