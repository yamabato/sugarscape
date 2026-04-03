#pragma once

static int MIN_METABOLISM = 1;
static int MAX_METABOLISM = 4;

static int MIN_VISION = 1;
static int MAX_VISION = 6;

static int MIN_INIT_SUGAR = 5;
static int MAX_INIT_SUGAR = 25;

struct Agent;
typedef struct Agent {
  int vision;
  int metabolism;
  int sugar;
  int x;
  int y;
  struct Agent *prev;
  struct Agent *next;
} Agent;
