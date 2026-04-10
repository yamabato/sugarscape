/*
update.h
2026/04/09
2026/04/10 更新
Haruta Kutsukawa
*/

#pragma once

#include "sugar.h"

static int SCAN_DIRECTIONS[4][2] = {
  { 1,  0},
  {-1,  0},
  { 0,  1},
  { 0, -1},
};

// 取引価格の標準偏差を求めるため，一時的に保持する配列
static float trade_price_arr[10000];

void update(Simulation *);

void rule_G(Simulation *, int);
void rule_D(Simulation *, int);
void rule_S_abg(Simulation *, int, int, int);

void rule_M(Simulation *, float);
void rule_S(Simulation *);
void rule_T(Simulation *);
