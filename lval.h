#ifndef LVAL_H
#define LVAL_H
#pragma once

/* Forward Declarations */
struct lval;
typedef struct lval lval;
typedef struct lenv lenv;

/* Lisp Value */

enum {
  LVAL_ERR,
  LVAL_NUM,
  LVAL_SYM,
  LVAL_STR,
  LVAL_FUN,
  LVAL_SEXPR,
  LVAL_QEXPR
};

typedef lval *(*lbuiltin)(lenv *, lval *);

struct lval {
  int type;

  /* Basic */
  long num;
  char *err;
  char *sym;
  char *str;

  /* Function */
  lbuiltin builtin;
  lenv *env;
  lval *formals;
  lval *body;

  /* Expression */
  int count;
  lval **cell;
};

/* Forward Declarations for variables defined in lval.c */
extern char *lval_str_unescapable;
extern char *lval_str_escapable;

/* Forward Declarations for functions defined in lval.c */
lval *lval_num(long x);
lval *lval_err(char *fmt, ...);
lval *lval_sym(char *s);
lval *lval_str(char *s);
lval *lval_builtin(lbuiltin func);
lval *lval_lambda(lval *formals, lval *body);
lval *lval_sexpr(void);
lval *lval_qexpr(void);
void lval_del(lval *v);
lval *lval_copy(lval *v);
lval *lval_add(lval *v, lval *x);
lval *lval_join(lval *x, lval *y);
lval *lval_pop(lval *v, int i);
lval *lval_take(lval *v, int i);
void lval_print(lval *v);
void lval_print_expr(lval *v, char open, char close);
char lval_str_unescape(char x);
char *lval_str_escape(char x);
void lval_print_str(lval *v);
void lval_println(lval *v);
int lval_eq(lval *x, lval *y);
char *ltype_name(int t);

#endif
