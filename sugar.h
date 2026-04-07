#pragma once

#include <stdbool.h>

#include "agent.h"

static int WIDTH = 50;
static int HEIGHT = 50;

static int MAX_AGENTS = 2000;
static int INIT_AGENTS = 400;

typedef struct {
  int time;
  int height;
  int width;
  int **sugar_lvl;
  int **sugar_cap;
  int **spice_lvl;
  int **spice_cap;
  float **pollution_lvl;
  Agent *agent_arr;
  Agent *agents;
  Agent *unused_agents;
  Agent ***agents_map;

  bool pollute;
} Simulation;
