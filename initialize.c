#include <stdlib.h>

#include "initialize.h"
#include "agent.h"
#include "util.h"
#include "sugar.h"

void initialize_agent(Simulation *sim, Agent *agent, int x, int y, int vision, int metabolism, int endowment_sugar) {
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

  if (metabolism == -1) {
    agent->metabolism = uniform_dist_rand(MIN_METABOLISM, MAX_METABOLISM);
  } else {
    agent->metabolism = metabolism;
  }

  if (vision == -1) {
    agent->vision = uniform_dist_rand(MIN_VISION, MAX_VISION);
  } else {
    agent->vision = vision;
  }

  if (endowment_sugar == -1) {
    agent->sugar = uniform_dist_rand(MIN_INIT_SUGAR, MAX_INIT_SUGAR);
  } else {
    agent->sugar = endowment_sugar;
  }

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
