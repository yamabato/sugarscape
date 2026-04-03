#include <stdio.h>

#include "sugar.h"
#include "show.h"

void show_sugarscape(Simulation *sim) {
  int lvl, cap;
  float r;
  int c;
  for (int y=0; y<sim->height; y++) {
    for (int x=0; x<sim->width; x++) {
      lvl = sim->sugar_lvl[y][x];
      cap = sim->sugar_cap[y][x];

      /*
      if (cap == 0) { r = 0; }
      else { r = (float)lvl/cap; }
      */

      r = (float)lvl/MAX_SUGAR_LVL;
      if (r > 1) { r = 1; }
      c = (int)((1-r)*255);

      // printf("\x1b[38;2;%d;%d;%dm", c, c, c);
      printf("%02d/%02d ", lvl, cap);
      // printf("\x1b[0m");
    }
    printf("\n");
  }

}

void show_agents(Simulation *sim) {
  for (int y=0; y<sim->height; y++) {
    for (int x=0; x<sim->width; x++) {
      if (sim->agents_map[y][x] != NULL) {
        printf("%03d ", sim->agents_map[y][x]->sugar);
      } else {
        printf("--- ");
      }
    }
    printf("\n");
  }
}
