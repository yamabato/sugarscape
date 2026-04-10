/*
option.o
2026/04/10
Haruta Kutsukawa
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "option.h"
#include "sugar.h"

void parse_options(
  int argc, const char **argv,
  Simulation *sim,
  int *N, int *step
) {
  const char *opt;

  for (int i=1; i<argc; i++) {
    opt = argv[i];

    // 最大反復数と情報を表示するステップ
    if (strcmp(opt, "--N") == 0) { *N = get_int_opt(argc, argv, &i); }
    else if (strcmp(opt, "--step") == 0) { *step = get_int_opt(argc, argv, &i); }

    else if (strcmp(opt, "--seed") == 0) { sim->seed = get_int_opt(argc, argv, &i); }

    // 取引価格を全て出力
    else if (strcmp(opt, "--show-price") == 0) { sim->show_price = true; }

    else if (strcmp(opt, "--inf-lifespan") == 0) { sim->inf_lifespan = true; }
  }
}

int get_int_opt(int argc, const char **argv, int *i) {
  if (*i+1 < argc) {
    return atoi(argv[++*i]);
  } else {
    printf("error: option(%s)\n", argv[*i]);
    exit(1);
  }
}
