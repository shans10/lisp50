#include "builtins.h"

#ifdef _WIN32

static char buffer[2048];

char *readline(char *prompt) {
  fputs(prompt, stdout);
  fgets(buffer, 2048, stdin);
  char *cpy = malloc(strlen(buffer) + 1);
  strcpy(cpy, buffer);
  cpy[strlen(cpy) - 1] = '\0';
  return cpy;
}

void add_history(char *unused) {}

#else
#include <editline/readline.h>
#endif

int main(int argc, char **argv) {

  /* Initializing the new environment and adding builtins */
  lenv *e = lenv_new();
  lenv_add_builtins(e);

  /* Loading Standard Library */
  lval *s = builtin_load(e, lval_add(lval_sexpr(), lval_str("stdlib.lspy")));
  if (s->type == LVAL_ERR) {
    lval_println(s);
  }
  lval_del(s);

  /* Interactive Prompt */
  if (argc == 1) {

    puts("Lisp50 Version 1.0");
    puts("Press Ctrl+c or type 'exit' to Exit\n");

    while (1) {

      char *input = readline("lisp50> ");
      add_history(input);

      /* Read from input to create an S-Expr */
      int pos = 0;
      lval *expr = lval_read_expr(input, &pos, '\0');

      /* Evaluate and print input */
      lval *x = lval_eval(e, expr);
      lval_println(x);
      lval_del(x);

      free(input);
    }
  }

  /*  Supplied with lsit of files */
  if (argc >= 2) {
    for (int i = 1; i < argc; i++) {
      lval *args = lval_add(lval_sexpr(), lval_str(argv[i]));
      lval *x = builtin_load(e, args);
      if (x->type == LVAL_ERR) {
        lval_println(x);
      }
      lval_del(x);
    }
  }

  lenv_del(e);

  return 0;
}
