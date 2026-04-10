/*
option.h
2026/04/10
Haruta Kutsukawa
*/

#pragma once

#include "sugar.h"

void parse_options(
  int, const char **,
  Simulation *sim,
  int *, int * // 反復回数N, 情報表示ステップ数
);
int get_int_opt(int, const char **, int *);
