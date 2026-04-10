/*
sugar.h
2026/04/09
2026/04/10 更新
Haruta Kutsukawa
*/

#pragma once

#include <stdbool.h>

#include "agent.h"

static int WIDTH = 50;
static int HEIGHT = 50;

static int MAX_AGENTS = 2000;
static int INIT_AGENTS = 400;

typedef struct {
  int seed;
  bool pollute;
  bool show_price;
  bool inf_lifespan;

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
} Simulation;
