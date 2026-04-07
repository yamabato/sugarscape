#include <math.h>

float agent_utility_function(int sugar, int spice, int sugar_metabolism, int spice_metabolism) {
  int total_metabolism = sugar_metabolism + spice_metabolism;
  return (float)pow((double)sugar, (double)sugar_metabolism/total_metabolism) * pow((double)spice, (double)spice_metabolism/total_metabolism);
}
