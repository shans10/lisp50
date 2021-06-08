/* Builtins Header */
#include "builtins.h"

lval *builtin_lambda(lenv *e, lval *a) {
  /* Check Two arguments, each of which are Q-Expressions */
  LASSERT_NUM("\\", a, 2);
  LASSERT_TYPE("\\", a, 0, LVAL_QEXPR);
  LASSERT_TYPE("\\", a, 1, LVAL_QEXPR);

  /* Check first Q-Expressions contains only Symbols */
  for (int i = 0; i < a->cell[0]->count; i++) {
    LASSERT(a, (a->cell[0]->cell[i]->type == LVAL_SYM),
            "Cannot define non-symbol. Got %s, Expected %s.",
            ltype_name(a->cell[0]->cell[i]->type), ltype_name(LVAL_SYM));
  }

  /* Pop first two arguments and pass them to lval_lambda */
  lval *formals = lval_pop(a, 0);
  lval *body = lval_pop(a, 0);
  lval_del(a);

  return lval_lambda(formals, body);
}

lval *builtin_list(lenv *e, lval *a) {
  a->type = LVAL_QEXPR;
  return a;
}

lval *builtin_head(lenv *e, lval *a) {
  LASSERT_NUM("head", a, 1);
  LASSERT_TYPE("head", a, 0, LVAL_QEXPR);
  LASSERT_NOT_EMPTY("head", a, 0);

  lval *v = lval_take(a, 0);
  while (v->count > 1) {
    lval_del(lval_pop(v, 1));
  }
  return v;
}

lval *builtin_tail(lenv *e, lval *a) {
  LASSERT_NUM("tail", a, 1);
  LASSERT_TYPE("tail", a, 0, LVAL_QEXPR);
  LASSERT_NOT_EMPTY("tail", a, 0);

  lval *v = lval_take(a, 0);
  lval_del(lval_pop(v, 0));
  return v;
}

lval *builtin_eval(lenv *e, lval *a) {
  LASSERT_NUM("eval", a, 1);
  LASSERT_TYPE("eval", a, 0, LVAL_QEXPR);

  lval *x = lval_take(a, 0);
  x->type = LVAL_SEXPR;
  return lval_eval(e, x);
}

lval *builtin_join(lenv *e, lval *a) {

  for (int i = 0; i < a->count; i++) {
    LASSERT_TYPE("join", a, i, LVAL_QEXPR);
  }

  lval *x = lval_pop(a, 0);

  while (a->count) {
    lval *y = lval_pop(a, 0);
    x = lval_join(x, y);
  }

  lval_del(a);
  return x;
}

lval *builtin_op(lenv *e, lval *a, char *op) {

  for (int i = 0; i < a->count; i++) {
    LASSERT_TYPE(op, a, i, LVAL_NUM);
  }

  lval *x = lval_pop(a, 0);

  if ((strcmp(op, "-") == 0) && a->count == 0) {
    x->num = -x->num;
  }

  while (a->count > 0) {
    lval *y = lval_pop(a, 0);

    if (strcmp(op, "+") == 0) {
      x->num += y->num;
    }
    if (strcmp(op, "-") == 0) {
      x->num -= y->num;
    }
    if (strcmp(op, "*") == 0) {
      x->num *= y->num;
    }
    if (strcmp(op, "/") == 0) {
      if (y->num == 0) {
        lval_del(x);
        lval_del(y);
        x = lval_err("Division By Zero.");
        break;
      }
      x->num /= y->num;
    }

    lval_del(y);
  }

  lval_del(a);
  return x;
}

lval *builtin_add(lenv *e, lval *a) { return builtin_op(e, a, "+"); }
lval *builtin_sub(lenv *e, lval *a) { return builtin_op(e, a, "-"); }
lval *builtin_mul(lenv *e, lval *a) { return builtin_op(e, a, "*"); }
lval *builtin_div(lenv *e, lval *a) { return builtin_op(e, a, "/"); }

lval *builtin_var(lenv *e, lval *a, char *func) {
  LASSERT_TYPE(func, a, 0, LVAL_QEXPR);

  lval *syms = a->cell[0];
  for (int i = 0; i < syms->count; i++) {
    LASSERT(a, (syms->cell[i]->type == LVAL_SYM),
            "Function '%s' cannot define non-symbol. "
            "Got %s, Expected %s.",
            func, ltype_name(syms->cell[i]->type), ltype_name(LVAL_SYM));
  }

  LASSERT(a, (syms->count == a->count - 1),
          "Function '%s' passed too many arguments for symbols. "
          "Got %i, Expected %i.",
          func, syms->count, a->count - 1);

  for (int i = 0; i < syms->count; i++) {
    /* If 'def' define in globally. If 'put' define in locally */
    if (strcmp(func, "def") == 0) {
      lenv_def(e, syms->cell[i], a->cell[i + 1]);
    }

    if (strcmp(func, "=") == 0) {
      lenv_put(e, syms->cell[i], a->cell[i + 1]);
    }
  }

  lval_del(a);
  return lval_sexpr();
}

lval *builtin_def(lenv *e, lval *a) { return builtin_var(e, a, "def"); }

lval *builtin_put(lenv *e, lval *a) { return builtin_var(e, a, "="); }

lval *builtin_ord(lenv *e, lval *a, char *op) {
  LASSERT_NUM(op, a, 2);
  LASSERT_TYPE(op, a, 0, LVAL_NUM);
  LASSERT_TYPE(op, a, 1, LVAL_NUM);

  int r;
  if (strcmp(op, ">") == 0) {
    r = (a->cell[0]->num > a->cell[1]->num);
  }
  if (strcmp(op, "<") == 0) {
    r = (a->cell[0]->num < a->cell[1]->num);
  }
  if (strcmp(op, ">=") == 0) {
    r = (a->cell[0]->num >= a->cell[1]->num);
  }
  if (strcmp(op, "<=") == 0) {
    r = (a->cell[0]->num <= a->cell[1]->num);
  }
  lval_del(a);
  return lval_num(r);
}

lval *builtin_gt(lenv *e, lval *a) { return builtin_ord(e, a, ">"); }

lval *builtin_lt(lenv *e, lval *a) { return builtin_ord(e, a, "<"); }

lval *builtin_ge(lenv *e, lval *a) { return builtin_ord(e, a, ">="); }

lval *builtin_le(lenv *e, lval *a) { return builtin_ord(e, a, "<="); }

lval *builtin_cmp(lenv *e, lval *a, char *op) {
  LASSERT_NUM(op, a, 2);
  int r;
  if (strcmp(op, "==") == 0) {
    r = lval_eq(a->cell[0], a->cell[1]);
  }
  if (strcmp(op, "!=") == 0) {
    r = !lval_eq(a->cell[0], a->cell[1]);
  }
  lval_del(a);
  return lval_num(r);
}

lval *builtin_eq(lenv *e, lval *a) { return builtin_cmp(e, a, "=="); }

lval *builtin_ne(lenv *e, lval *a) { return builtin_cmp(e, a, "!="); }

lval *builtin_if(lenv *e, lval *a) {
  LASSERT_NUM("if", a, 3);
  LASSERT_TYPE("if", a, 0, LVAL_NUM);
  LASSERT_TYPE("if", a, 1, LVAL_QEXPR);
  LASSERT_TYPE("if", a, 2, LVAL_QEXPR);

  /* Mark Both Expressions as evaluable */
  lval *x;
  a->cell[1]->type = LVAL_SEXPR;
  a->cell[2]->type = LVAL_SEXPR;

  if (a->cell[0]->num) {
    /* If condition is true evaluate first expression */
    x = lval_eval(e, lval_pop(a, 1));
  } else {
    /* Otherwise evaluate second expression */
    x = lval_eval(e, lval_pop(a, 2));
  }

  /* Delete argument list and return */
  lval_del(a);
  return x;
}

lval *builtin_load(lenv *e, lval *a) {
  LASSERT_NUM("load", a, 1);
  LASSERT_TYPE("load", a, 0, LVAL_STR);

  /* Open file and check it exists */
  FILE *f = fopen(a->cell[0]->str, "rb");
  if (f == NULL) {
    lval *err = lval_err("Could not load Library %s", a->cell[0]->str);
    lval_del(a);
    return err;
  }

  /* Read file contents */
  fseek(f, 0, SEEK_END);
  long length = ftell(f);
  fseek(f, 0, SEEK_SET);
  char *input = calloc(length + 1, 1);
  fread(input, 1, length, f);
  fclose(f);

  /* Read from input to create an S-Expr */
  int pos = 0;
  lval *expr = lval_read_expr(input, &pos, '\0');
  free(input);

  /* Evaluate all expressions contained in S-Expr */
  if (expr->type != LVAL_ERR) {
    while (expr->count) {
      lval *x = lval_eval(e, lval_pop(expr, 0));
      if (x->type == LVAL_ERR) {
        lval_println(x);
      }
      lval_del(x);
    }
  } else {
    lval_println(expr);
  }

  lval_del(expr);
  lval_del(a);

  return lval_sexpr();
}

lval *builtin_print(lenv *e, lval *a) {

  /* Print each argument followed by a space */
  for (int i = 0; i < a->count; i++) {
    lval_print(a->cell[i]);
    putchar(' ');
  }

  /* Print a newline and delete arguments */
  putchar('\n');
  lval_del(a);

  return lval_sexpr();
}

lval *builtin_error(lenv *e, lval *a) {
  LASSERT_NUM("error", a, 1);
  LASSERT_TYPE("error", a, 0, LVAL_STR);

  /* Construct error from first argument */
  lval *err = lval_err(a->cell[0]->str);

  /* Delete arguments and return */
  lval_del(a);
  return err;
}

void lenv_add_builtin(lenv *e, char *name, lbuiltin func) {
  lval *k = lval_sym(name);
  lval *v = lval_builtin(func);
  lenv_put(e, k, v);
  lval_del(k);
  lval_del(v);
}

void lenv_add_builtins(lenv *e) {
  /* Variable Functions */
  lenv_add_builtin(e, "\\", builtin_lambda);
  lenv_add_builtin(e, "def", builtin_def);
  lenv_add_builtin(e, "=", builtin_put);

  /* List Functions */
  lenv_add_builtin(e, "list", builtin_list);
  lenv_add_builtin(e, "head", builtin_head);
  lenv_add_builtin(e, "tail", builtin_tail);
  lenv_add_builtin(e, "eval", builtin_eval);
  lenv_add_builtin(e, "join", builtin_join);

  /* Mathematical Functions */
  lenv_add_builtin(e, "+", builtin_add);
  lenv_add_builtin(e, "-", builtin_sub);
  lenv_add_builtin(e, "*", builtin_mul);
  lenv_add_builtin(e, "/", builtin_div);

  /* Comparison Functions */
  lenv_add_builtin(e, "if", builtin_if);
  lenv_add_builtin(e, "==", builtin_eq);
  lenv_add_builtin(e, "!=", builtin_ne);
  lenv_add_builtin(e, ">", builtin_gt);
  lenv_add_builtin(e, "<", builtin_lt);
  lenv_add_builtin(e, ">=", builtin_ge);
  lenv_add_builtin(e, "<=", builtin_le);

  /* String Functions */
  lenv_add_builtin(e, "load", builtin_load);
  lenv_add_builtin(e, "print", builtin_print);
  lenv_add_builtin(e, "error", builtin_error);
}