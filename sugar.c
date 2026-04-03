#include <stdio.h>
#include <stdlib.h>

#include "agent.h"
#include "sugar.h"
#include "util.h"
#include "setting.h"
#include "show.h"
#include "update.h"

void initialize(Simulation *sim) {
  int x_, y_;
  Agent *agent;

  sim->sugar_lvl = (int **)malloc(sizeof(int *)*sim->height);
  sim->sugar_cap = (int **)malloc(sizeof(int *)*sim->height);
  sim->agents_map = (Agent ***)malloc(sizeof(int *)*sim->height);
  for (int i=0; i<HEIGHT; i++) {
    sim->sugar_lvl[i] = (int *)malloc(sizeof(int)*sim->width);
    sim->sugar_cap[i] = (int *)malloc(sizeof(int)*sim->width);
    sim->agents_map[i] = (Agent **)calloc(sim->width, sizeof(Agent *));
  }

  for (int i=0; i<sim->height; i++) {
    for (int j=0; j<sim->width; j++) {
      sim->sugar_lvl[i][j] = SUGAR_CAP_SETTING1[i][j];
      sim->sugar_cap[i][j] = SUGAR_CAP_SETTING1[i][j];
    }
  }

  sim->agent_arr = (Agent *)calloc(MAX_AGENTS, sizeof(Agent));
  sim->agents = NULL;
  sim->unused_agents = &sim->agent_arr[0];
  sim->unused_agents->next = NULL;
  for (int i=1; i<MAX_AGENTS; i++) {
    sim->agent_arr[i].next = sim->unused_agents;
    sim->unused_agents = &sim->agent_arr[i];
  }

  for (int i=0; i<INIT_AGENTS; i++) {
    do {
      x_ = uniform_dist_rand(0, sim->width-1);
      y_ = uniform_dist_rand(0, sim->height-1);
    } while (sim->agents_map[y_][x_] != NULL);

    agent = sim->unused_agents;
    sim->agents_map[y_][x_] = agent;
    sim->unused_agents = agent->next;
    agent->next = sim->agents;
    if (sim->agents != NULL) { sim->agents->prev = agent; }
    agent->prev = NULL;
    sim->agents = agent;

    agent->x = x_;
    agent->y = y_;
    agent->metabolism = uniform_dist_rand(MIN_METABOLISM, MAX_METABOLISM);
    agent->vision = uniform_dist_rand(MIN_VISION, MAX_VISION);
    agent->sugar = uniform_dist_rand(MIN_INIT_SUGAR, MAX_INIT_SUGAR);
  }
}

int main(void) {
  Simulation sim;
  sim.height = HEIGHT;
  sim.width = WIDTH;
  sim.sugar_lvl = NULL;
  sim.sugar_cap = NULL;
  sim.agent_arr = NULL;
  sim.agents = NULL;
  sim.unused_agents = NULL;
  sim.agents_map = NULL;

  srand(123);

  initialize(&sim);

  for (int i=0; i<50; i++) {
    update(&sim);
  }

  show_sugarscape(&sim);
  show_agents(&sim);

  return 0;
}
