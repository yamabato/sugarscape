/*
agent.c
2026/04/09
Haruta Kutsukawa
*/

#include <math.h>

#include "agent.h"

// エージェントの効用関数
// 2財w1, w2とその消費量m1, m2に対して
// u = (w2/m2) / (w1/m1)
float agent_utility_function(float sugar, float spice, int sugar_metabolism, int spice_metabolism) {
  int total_metabolism = sugar_metabolism + spice_metabolism;
  return (float)pow((double)sugar, (double)sugar_metabolism/total_metabolism) * pow((double)spice, (double)spice_metabolism/total_metabolism);
}
