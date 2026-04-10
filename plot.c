/*
plot.c
2026/04/09
Haruta Kutsukawa
*/

#include <stdio.h>

#include "plot.h"

void plot_sugar_amount(Simulation *sim) {
  int amount;

  for (int x=0; x<sim->width; x++) {
    for (int y=0; y<sim->height; y++) {
      amount = sim->sugar_lvl[y][x];
      printf("%d %d %d\n", x, y, amount);
    }
    printf("\n");
  }
}
