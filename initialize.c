#include <stdlib.h>

#include "initialize.h"
#include "agent.h"
#include "util.h"
#include "sugar.h"

void initialize_agent(Simulation *sim, Agent *agent, int x, int y, int vision, int sugar_metabolism, int spice_metabolism, float endowment_sugar, float endowment_spice) {
  int x_, y_;

  if (x==-1 || y==-1) {
    do {
      x_ = uniform_dist_rand(0, sim->width-1);
      y_ = uniform_dist_rand(0, sim->height-1);
    } while (sim->agents_map[y_][x_] != NULL);

    // 左下に固めて配置
    /*
    x_ = i % 20;
    y_ = i/20+30;
    */
  } else {
    x_ = x;
    y_ = y;
  }

  sim->agents_map[y_][x_] = agent;
  agent->next = sim->agents;
  if (sim->agents != NULL) { sim->agents->prev = agent; }
  agent->prev = NULL;
  sim->agents = agent;

  agent->x = x_;
  agent->y = y_;

  if (sugar_metabolism == -1) {
    agent->sugar_metabolism = uniform_dist_rand(MIN_SUGAR_METABOLISM, MAX_SUGAR_METABOLISM);
  } else {
    agent->sugar_metabolism = sugar_metabolism;
  }

  if (spice_metabolism == -1) {
    agent->spice_metabolism = uniform_dist_rand(MIN_SPICE_METABOLISM, MAX_SPICE_METABOLISM);
  } else {
    agent->spice_metabolism = spice_metabolism;
  }

  if (vision == -1) {
    agent->vision = uniform_dist_rand(MIN_VISION, MAX_VISION);
  } else {
    agent->vision = vision;
  }

  if (endowment_sugar == -1) {
    agent->sugar = (float)uniform_dist_rand(MIN_INIT_SUGAR, MAX_INIT_SUGAR);
  } else {
    agent->sugar = endowment_sugar;
  }
  agent->endowment_sugar = agent->sugar;

  if (endowment_spice == -1) {
    agent->spice = (float)uniform_dist_rand(MIN_INIT_SPICE, MAX_INIT_SPICE);
  } else {
    agent->spice = endowment_spice;
  }
  agent->endowment_spice = agent->spice;

  agent->age = 0;
  agent->is_fertile = false;

  agent->start_fertile_age = uniform_dist_rand(MIN_FERTILE_AGE, MAX_FERTILE_AGE);
  agent->sex = uniform_dist_rand(0, 1);
  if (agent->sex == 0) {
    agent->end_fertile_age = uniform_dist_rand(MIN_WOMEN_CHILDBEARING_FIN, MAX_WOMEN_CHILDBEARING_FIN);
  } else {
    agent->end_fertile_age = uniform_dist_rand(MIN_MEN_CHILDBEARING_FIN, MAX_MEN_CHILDBEARING_FIN);
  }
  agent->lifespan = uniform_dist_rand(MIN_LIFESPAN, MAX_LIFESPAN);
}
