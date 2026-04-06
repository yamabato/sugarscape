#include <stdio.h>
#include <limits.h>

#include "sugar.h"
#include "util.h"
#include "update.h"

void update(Simulation *sim) {
  Agent *agent;
  Agent *next_agent;

  sim->time++;

  rule_M(sim);
  // rule_G(sim, 1);
  rule_S(sim, 5, 25, 50);

  for (agent=sim->agents; agent!=NULL; agent=next_agent) {
    next_agent = agent->next;

    agent->sugar -= agent->metabolism;
    if (agent->sugar < 0) {
      if (agent->prev != NULL) {
        agent->prev->next = agent->next;
      }
      if (agent->next != NULL) {
        agent->next->prev = agent->prev;
      }

      if (agent == sim->agents) {
        sim->agents = agent->next;
      }

      sim->agents_map[agent->y][agent->x] = NULL;

      agent->next = sim->unused_agents;
      sim->unused_agents = agent;
    }
  }
}

void rule_G(Simulation *sim, int amount) {
  for (int y=0; y<sim->height; y++) {
    for (int x=0; x<sim->width; x++) {
      sim->sugar_lvl[y][x] = min(sim->sugar_lvl[y][x]+amount, sim->sugar_cap[y][x]);
    }
  }
}

void rule_S(Simulation *sim, int amount, int interval, int period) {
  // season: 0->上がsummer
  int season = ((sim->time-1)/period)%2;
  for (int y=0; y<sim->height; y++) {
    for (int x=0; x<sim->width; x++) {
      if ((y<sim->height/2 && season==0) || (y>=sim->height/2 && season==1)) { // summer
        sim->sugar_lvl[y][x] = min(sim->sugar_lvl[y][x]+amount, sim->sugar_cap[y][x]);
      } else if ((sim->time-1)%interval == 0) { // winter
        sim->sugar_lvl[y][x] = min(sim->sugar_lvl[y][x]+amount, sim->sugar_cap[y][x]);
      }
    }
  }
}

void rule_M(Simulation *sim) {
  Agent *agent;
  int dist, s, s_max;
  int x, y;
  int y_, x_;
  int ny, nx;

  int directions_n_arr[4] = {0, 1, 2, 3};

  for (agent=sim->agents; agent!=NULL; agent=agent->next) {
    s_max = INT_MIN;
    dist = INT_MAX;
    x = agent->x;
    y = agent->y;

    shuffle(directions_n_arr, 4);

    for (int i=0; i<4; i++) {
      for (int d=0; d<=agent->vision; d++) {
        /*
        x_ = (x+d*SCAN_DIRECTIONS[directions_n_arr[i]][0]+sim->width)%sim->width;
        y_ = (y+d*SCAN_DIRECTIONS[directions_n_arr[i]][1]+sim->height)%sim->height;
        */
        x_ = x+d*SCAN_DIRECTIONS[directions_n_arr[i]][0];
        y_ = y+d*SCAN_DIRECTIONS[directions_n_arr[i]][1];
        if (x_ < 0) { x_ = 0; }
        if (x_ >= sim->width) { x_ = sim->width-1; }
        if (y_ < 0) { y_ = 0; }
        if (y_ >= sim->height) { y_ = sim->height-1; }

        s = sim->sugar_lvl[y_][x_];

        if (sim->agents_map[y_][x_]== NULL && (s>s_max || (s==s_max && dist>d))) {
          s_max = s;
          dist = d;
          ny = y_;
          nx = x_;
        }
      }
    }

    sim->agents_map[y][x] = NULL;
    sim->agents_map[ny][nx] = agent;
    agent->x = nx;
    agent->y = ny;

    agent->sugar += sim->sugar_lvl[ny][nx];
    sim->sugar_lvl[ny][nx] = 0;
  }
}
