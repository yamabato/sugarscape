#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "agent.h"
#include "sugar.h"
#include "util.h"
#include "setting.h"
#include "show.h"
#include "update.h"
#include "initialize.h"

void initialize(Simulation *sim) {
  Agent *agent;

  sim->time = 0;

  sim->sugar_lvl = (int **)malloc(sizeof(int *)*sim->height);
  sim->sugar_cap = (int **)malloc(sizeof(int *)*sim->height);
  sim->pollution_lvl = (float **)malloc(sizeof(float *)*sim->height);
  sim->agents_map = (Agent ***)malloc(sizeof(int *)*sim->height);
  for (int i=0; i<HEIGHT; i++) {
    sim->sugar_lvl[i] = (int *)malloc(sizeof(int)*sim->width);
    sim->sugar_cap[i] = (int *)malloc(sizeof(int)*sim->width);
    sim->pollution_lvl[i] = (float *)calloc(sim->width, sizeof(float));
    sim->agents_map[i] = (Agent **)calloc(sim->width, sizeof(Agent *));
  }

  for (int i=0; i<sim->height; i++) {
    for (int j=0; j<sim->width; j++) {
      sim->sugar_lvl[i][j] = SUGAR_CAP_SETTING4[i][j];
      sim->sugar_cap[i][j] = SUGAR_CAP_SETTING4[i][j];
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
    agent = sim->unused_agents;
    sim->unused_agents = agent->next;
    initialize_agent(sim, agent, -1, -1, -1, -1, -1);
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

  sim.pollute = false;

  srand(123);

  initialize(&sim);

  show_sugarscape(&sim);

  int n;
  show_agents(&sim);
  for (int i=0; i<1000; i++) {
    update(&sim);

    if (i%100== 0) {
      n = 0;
      for (Agent *agent=sim.agents; agent!=NULL; agent=agent->next) {
        n++;
      }

      printf("\n");
      printf("step: %d\n", i+1);
      printf("agents: %d\n", n);
      show_agents(&sim);
      // show_pollution(&sim);
    }
  }

  return 0;
}
