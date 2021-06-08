#ifndef BUILTINS_H
#define BUILTINS_H
#pragma once

/* Project Headers */
#include "lenv.h"
#include "evaluation.h"
#include "reading.h"

#define LASSERT(args, cond, fmt, ...)                                          \
  if (!(cond)) {                                                               \
    lval *err = lval_err(fmt, ##__VA_ARGS__);                                  \
    lval_del(args);                                                            \
    return err;                                                                \
  }

#define LASSERT_TYPE(func, args, index, expect)                                \
  LASSERT(args, args->cell[index]->type == expect,                             \
          "Function '%s' passed incorrect type for argument %i. "              \
          "Got %s, Expected %s.",                                              \
          func, index, ltype_name(args->cell[index]->type),                    \
          ltype_name(expect))

#define LASSERT_NUM(func, args, num)                                           \
  LASSERT(args, args->count == num,                                            \
          "Functions '%s' passed incorrect number of arguments. "              \
          "Got %i, Expected %i.",                                              \
          func, args->count, num)

#define LASSERT_NOT_EMPTY(func, args, index)                                   \
  LASSERT(args, args->cell[index]->count != 0,                                 \
          "Function '%s' passed {} for argument %i.", func, index);


/* Forward Declarations for functions defined in builtins.c */

lval *builtin_lambda(lenv *e, lval *a);
lval *builtin_list(lenv *e, lval *a);
lval *builtin_head(lenv *e, lval *a);
lval *builtin_tail(lenv *e, lval *a);
lval *builtin_eval(lenv *e, lval *a);
lval *builtin_join(lenv *e, lval *a);
lval *builtin_op(lenv *e, lval *a, char *op);
lval *builtin_add(lenv *e, lval *a);
lval *builtin_sub(lenv *e, lval *a);
lval *builtin_mul(lenv *e, lval *a);
lval *builtin_div(lenv *e, lval *a);
lval *builtin_var(lenv *e, lval *a, char *func);
lval *builtin_def(lenv *e, lval *a);
lval *builtin_put(lenv *e, lval *a);
lval *builtin_ord(lenv *e, lval *a, char *op);
lval *builtin_gt(lenv *e, lval *a);
lval *builtin_lt(lenv *e, lval *a);
lval *builtin_ge(lenv *e, lval *a);
lval *builtin_le(lenv *e, lval *a);
lval *builtin_cmp(lenv *e, lval *a, char *op);
lval *builtin_eq(lenv *e, lval *a);
lval *builtin_ne(lenv *e, lval *a);
lval *builtin_if(lenv *e, lval *a);
lval *builtin_load(lenv *e, lval *a);
lval *builtin_print(lenv *e, lval *a);
lval *builtin_error(lenv *e, lval *a);
void lenv_add_builtin(lenv *e, char *name, lbuiltin func);
void lenv_add_builtins(lenv *e);

#endif
