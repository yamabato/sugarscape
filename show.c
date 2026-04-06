#include <stdio.h>

#include "sugar.h"
#include "show.h"

void show_sugarscape(Simulation *sim) {
  int lvl, cap;
  for (int y=0; y<sim->height; y++) {
    for (int x=0; x<sim->width; x++) {
      lvl = sim->sugar_lvl[y][x];
      cap = sim->sugar_cap[y][x];

      if (cap == 0) {
        printf("----- ");
      } else {
        printf("%02d/%02d ", lvl, cap);
      }
    }
    printf("\n");
  }

}

void show_pollution(Simulation *sim) {
  float lvl;
  for (int y=0; y<sim->height; y++) {
    for (int x=0; x<sim->width; x++) {
      lvl = sim->pollution_lvl[y][x];
      printf("%.2f ", lvl);
    }
    printf("\n");
  }
}

void show_agents(Simulation *sim) {
  for (int y=0; y<sim->height; y++) {
    for (int x=0; x<sim->width; x++) {
      if (sim->agents_map[y][x] != NULL) {
        printf("%04d ", sim->agents_map[y][x]->sugar);
        // printf("%01d ", sim->agents_map[y][x]->vision);
      } else {
        printf("---- ");
        // printf("- ");
      }
    }
    printf("\n");
  }
}
