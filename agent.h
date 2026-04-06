#pragma once

#include <stdbool.h>

static int MIN_METABOLISM = 1;
static int MAX_METABOLISM = 4;

static int MIN_VISION = 1;
static int MAX_VISION = 6;

static int MIN_INIT_SUGAR = 50;
static int MAX_INIT_SUGAR = 100;

static int MIN_FERTILE_AGE = 12;
static int MAX_FERTILE_AGE = 15;

static int MIN_WOMEN_CHILDBEARING_FIN = 40;
static int MAX_WOMEN_CHILDBEARING_FIN = 50;
static int MIN_MEN_CHILDBEARING_FIN = 50;
static int MAX_MEN_CHILDBEARING_FIN = 60;

static int MIN_LIFESPAN = 60;
static int MAX_LIFESPAN = 100;

struct Agent;
typedef struct Agent {
  int age;
  int vision;
  int metabolism;
  int sugar;
  int x;
  int y;
  int sex;
  int endowment_sugar;
  int start_fertile_age;
  int end_fertile_age;
  bool is_fertile;
  int lifespan;
  struct Agent *prev;
  struct Agent *next;
} Agent;
