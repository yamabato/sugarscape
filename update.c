#include <stdio.h>
#include <limits.h>

#include "sugar.h"
#include "util.h"
#include "update.h"

void update(Simulation *sim) {
  Agent *agent;
  Agent *dead_agent;

  rule_M(sim);
  rule_G(sim, 1);

  for (agent=sim->agents; agent!=NULL;) {
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

      dead_agent = agent;
      agent = agent->next;

      dead_agent->next = sim->unused_agents;
      sim->unused_agents = dead_agent;

      sim->agents_map[dead_agent->y][dead_agent->x] = NULL;
    }

    agent=agent->next;
  }
}

void rule_G(Simulation *sim, int amount) {
  for (int y=0; y<sim->height; y++) {
    for (int x=0; x<sim->width; x++) {
      sim->sugar_lvl[y][x] = min(sim->sugar_lvl[y][x]+amount, sim->sugar_cap[y][x]);
    }
  }
}

void rule_M(Simulation *sim) {
  Agent *agent;
  int dist, s, s_max;
  int x, y;
  int y_, x_;
  int ny, nx;

  for (agent=sim->agents; agent!=NULL; agent=agent->next) {
    s_max = INT_MIN;
    dist = INT_MAX;
    x = agent->x;
    y = agent->y;

    for (int i=-agent->vision; i<=agent->vision; i++) {
      x_ = (x+i+sim->width)%sim->width;
      s = sim->sugar_lvl[y][x_];
      if (sim->agents_map[y][x_]==NULL && s>=s_max && dist>abs(i)) {
        s_max = s;
        dist = abs(i);
        ny = y;
        nx = x_;
      }
    }

    for (int i=-agent->vision; i<=agent->vision; i++) {
      y_ = (y+i+sim->height)%sim->height;
      s = sim->sugar_lvl[y_][x];
      if (sim->agents_map[y_][x]==NULL && s>=s_max && dist>abs(i)) {
        s_max = s;
        dist = abs(i);
        ny = y_;
        nx = x;
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
