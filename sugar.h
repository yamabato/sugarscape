#pragma once

#include "agent.h"

static int WIDTH = 50;
static int HEIGHT = 50;

static int MAX_AGENTS = 1000;
static int INIT_AGENTS = 400;

typedef struct {
  int height;
  int width;
  int **sugar_lvl;
  int **sugar_cap;
  Agent *agent_arr;
  Agent *agents;
  Agent *unused_agents;
  Agent ***agents_map;
} Simulation;
