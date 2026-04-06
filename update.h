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

void rule_M(Simulation *);
