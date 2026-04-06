#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "sugar.h"
#include "util.h"
#include "update.h"
#include "initialize.h"

void update(Simulation *sim) {
  Agent *agent;
  Agent *next_agent;
  int x, y;
  int consumed_sugar;

  int rule_g_alpha = 1; // 砂糖の復元量
  int rule_d_alpha = 1; // 汚染物質拡散速度

  float rule_m_alpha = 1; // 砂糖回収時の汚染度
  float rule_m_beta = 1;  // 砂糖消費時の汚染度

  sim->time++;

  /*
  if (sim->time <50) { sim->pollute = false; }
  else { sim->pollute = true; }
  */

  rule_M(sim, rule_m_alpha);
  rule_G(sim, rule_g_alpha);

  /*
  if (sim->time >= 100) {
    rule_D(sim, rule_d_alpha);
  }
  */
  // rule_S_abg(sim, 5, 25, 50);
  rule_S(sim);

  for (agent=sim->agents; agent!=NULL; agent=next_agent) {
    x = agent->x;
    y = agent->y;
    agent->age++;
    next_agent = agent->next;

    consumed_sugar = min(agent->metabolism, agent->sugar);
    if (sim->pollute) {
      sim->pollution_lvl[y][x] += consumed_sugar*rule_m_beta;
    }

    agent->sugar -= agent->metabolism;
    if (agent->sugar<0 || agent->age>=agent->lifespan) {
      if (agent->prev != NULL) {
        agent->prev->next = agent->next;
      }
      if (agent->next != NULL) {
        agent->next->prev = agent->prev;
      }

      if (agent == sim->agents) {
        sim->agents = agent->next;
      }

      sim->agents_map[y][x] = NULL;

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

void rule_D(Simulation *sim, int period) {
  if (sim->time%period != 0) { return; }

  int w, h;
  w = sim->width;
  h = sim->height;

  float **new_pollution_lvl;
  new_pollution_lvl = (float **)malloc(sizeof(float *)*h);
  for (int i=0; i<h; i++) {
    new_pollution_lvl[i] = (float *)calloc(w, sizeof(float));
  }

  for (int y=0; y<h; y++) {
    for (int x=0; x<w; x++) {
      new_pollution_lvl[y][x] = (
        sim->pollution_lvl[(y-1+h)%h][x]+
        sim->pollution_lvl[(y+1)%h][x]+
        sim->pollution_lvl[y][(x-1+w)%w]+
        sim->pollution_lvl[y][(x+1)%w]
      )/4;
    }
  }

  for (int y=0; y<h; y++) {
    for (int x=0; x<w; x++) {
      sim->pollution_lvl[y][x] = new_pollution_lvl[y][x];
    }
  }
}

void rule_S_abg(Simulation *sim, int amount, int interval, int period) {
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

void rule_M(Simulation *sim, float alpha) {
  Agent *agent;
  int dist;
  int sugar;
  float pollution;
  float sugar_ratio, sugar_ratio_max;
  int x, y;
  int y_, x_;
  int ny, nx;
  int collected_sugar;

  int directions_n_arr[4] = {0, 1, 2, 3};

  for (agent=sim->agents; agent!=NULL; agent=agent->next) {
    sugar_ratio_max = 0;
    dist = INT_MAX;
    x = agent->x;
    y = agent->y;
    nx = x;
    ny = y;

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

        sugar = sim->sugar_lvl[y_][x_];
        pollution = sim->pollution_lvl[y_][x_]+alpha/10;
        sugar_ratio = sugar/pollution;

        if (sim->agents_map[y_][x_]== NULL && (sugar_ratio>sugar_ratio_max || (sugar_ratio==sugar_ratio_max && dist>d))) {
          sugar_ratio_max = sugar_ratio;
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

    collected_sugar = sim->sugar_lvl[ny][nx];
    if (sim->pollute) {
      sim->pollution_lvl[ny][nx] += collected_sugar*alpha;
    }
    agent->sugar += collected_sugar;
    sim->sugar_lvl[ny][nx] = 0;
  }
}

void rule_S(Simulation *sim) {
  int dir_n_arr[4] = {0, 1, 2, 3};
  Agent *neighbor;
  Agent *child;
  int vision, metabolism;
  int empty_sites[6];
  int empty_site_n;
  int x_, y_;
  int child_x, child_y;

  int w, h;
  w = sim->width;
  h = sim->height;

  for (Agent *agent=sim->agents; agent!=NULL; agent=agent->next) {
    if (agent->age>=agent->start_fertile_age && agent->age<=agent->end_fertile_age) { agent->is_fertile = true; }
    else { agent->is_fertile = false; }
  }

  for (Agent *agent=sim->agents; agent!=NULL; agent=agent->next) {
    if (!agent->is_fertile) { continue; }
    if (agent->sugar < agent->endowment_sugar) { continue; }
    // 近傍のエージェントをランダムに選ぶ
    // 生殖可能なら，子エージェントを配置

    shuffle(dir_n_arr, 4);
    for (int i=0; i<4; i++) {
      x_ = (agent->x+SCAN_DIRECTIONS[dir_n_arr[i]][1]+w)%w;
      y_ = (agent->y+SCAN_DIRECTIONS[dir_n_arr[i]][0]+h)%h;
      neighbor = sim->agents_map[y_][x_];
      if (neighbor==NULL || !neighbor->is_fertile || neighbor->sugar<neighbor->endowment_sugar) { continue; }
      if (agent->sex == neighbor->sex) { continue; }

      if (sim->unused_agents == NULL) {
        printf("no unused agents\n");
        return;
      }

      // 空き地を探す
      empty_site_n = 0;
      for (int j=0; j<4; j++) {
        x_ = (agent->x+SCAN_DIRECTIONS[j][1]+w)%w;
        y_ = (agent->y+SCAN_DIRECTIONS[j][0]+h)%h;
        if (sim->agents_map[y_][x_] == NULL) {
          empty_sites[empty_site_n++] = x_+y_*w;
        }

        x_ = (neighbor->x+SCAN_DIRECTIONS[j][1]+w)%w;
        y_ = (neighbor->y+SCAN_DIRECTIONS[j][0]+h)%h;
        if (sim->agents_map[y_][x_] == NULL) {
          empty_sites[empty_site_n++] = x_+y_*w;
        }
      }

      if (empty_site_n == 0) { continue; }

      child = sim->unused_agents;
      sim->unused_agents = child->next;
      if (sim->unused_agents != NULL) {
        sim->unused_agents->prev = NULL;
      }

      agent->sugar -= agent->endowment_sugar/2;
      neighbor->sugar -= neighbor->endowment_sugar/2;
      vision = rand()<0.5 ? agent->vision : neighbor->vision;
      metabolism = rand()<0.5 ? agent->metabolism : neighbor->metabolism;

      shuffle(empty_sites, empty_site_n);
      child_x = empty_sites[0]%w;
      child_y = empty_sites[0]/w;
      initialize_agent(sim, child, child_x, child_y, vision, metabolism, agent->endowment_sugar/2+neighbor->endowment_sugar/2);
    }
  }
}
