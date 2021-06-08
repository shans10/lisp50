#ifndef READING_H
#define READING_H
#pragma once

/* Project Header */
#include "lenv.h"

/* Forward Declarations for functions defined in reading.c */
lval *lval_read_sym(char *s, int *i);
lval *lval_read_str(char *s, int *i);
lval *lval_read(char *s, int *i);
lval *lval_read_expr(char *s, int *i, char end);

#endif