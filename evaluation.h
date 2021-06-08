#ifndef EVALUATION_H
#define EVALUATION_H
#pragma once

/* Project Header */
#include "builtins.h"

/* Forward Declarations for functions defined in evaluation.c */
lval *lval_call(lenv *e, lval *f, lval *a);
lval *lval_eval_sexpr(lenv *e, lval *v);
lval *lval_eval(lenv *e, lval *v);

#endif
