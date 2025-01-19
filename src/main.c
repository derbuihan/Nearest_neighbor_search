#include <stdio.h>

#include "vector.h"

struct vector_store {
  vector *v;
};

int main(void) {
  int dimensions = 1536;
  vector *v1 = new_vector(dimensions);
  set_random_vector(v1);
  print_vector(v1);

  float length = length_vector(v1);
  printf("Length of the vector: %f\n", length);

  return 0;
}