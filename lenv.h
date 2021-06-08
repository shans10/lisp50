#ifndef LENV_H
#define LENV_H
#pragma once

/* System Headers */
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Project Header */
#include "lval.h"

/* Lisp Environment */
struct lenv {
  lenv *par;
  int count;
  char **syms;
  lval **vals;
};

/* Forward Declarations for functions defined in lenv.c */
lenv *lenv_new(void);
void lenv_del(lenv *e);
lenv *lenv_copy(lenv *e);
lval *lenv_get(lenv *e, lval *k);
void lenv_put(lenv *e, lval *k, lval *v);
void lenv_def(lenv *e, lval *k, lval *v);

#endif