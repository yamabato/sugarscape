#include <stdlib.h>

#include "util.h"

int uniform_dist_rand(int a, int b) {
  return rand()%(b-a) + a;
}

int max(int a, int b) {
  return a<b?b:a;
}

int min(int a, int b) {
  return a<b?a:b;
}

int abs(int x) {
  return max(x, -x);
}
