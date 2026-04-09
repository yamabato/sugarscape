#pragma once

#include "sugar.h"

static int SCAN_DIRECTIONS[4][2] = {
  { 1,  0},
  {-1,  0},
  { 0,  1},
  { 0, -1},
};

void update(Simulation *);

void rule_G(Simulation *, int);
void rule_D(Simulation *, int);
void rule_S_abg(Simulation *, int, int, int);

void rule_M(Simulation *, float);
void rule_S(Simulation *);
void rule_T(Simulation *);
