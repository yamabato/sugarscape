/*
util.c
2026/04/09
Haruta Kutsukawa
*/

#include <stdlib.h>

#include "util.h"

int uniform_dist_rand(int a, int b) {
  return rand()%(b-a+1) + a;
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

void shuffle(int *arr, int n) {
  int pos;
  int tmp;
  for (int i=0; i<n; i++) {
    pos = uniform_dist_rand(i, n-1);
    tmp = arr[i];
    arr[i] = arr[pos];
    arr[pos] = tmp;
  }
}
