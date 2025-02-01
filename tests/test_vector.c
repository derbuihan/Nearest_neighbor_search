#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "vector.h"

void test_new_vector() {
  Vector *vec = new_vector(3);
  assert(vec != NULL);
  assert(vec->size == 3);
  free_vector(vec);
}

void test_dot_product_vector() {
  Vector *vec1 = new_vector(3);
  vec1->data[0] = 1.0;
  vec1->data[1] = 2.0;
  vec1->data[2] = 3.0;

  Vector *vec2 = new_vector(3);
  vec2->data[0] = 4.0;
  vec2->data[1] = 5.0;
  vec2->data[2] = 6.0;

  float dot_product = dot_product_vector(vec1, vec2);
  assert(dot_product == 32.0);

  free_vector(vec1);
  free_vector(vec2);
}

void test_length_vector() {
  Vector *vec = new_vector(2);
  vec->data[0] = 3.0;
  vec->data[1] = 4.0;

  float length = length_vector(vec);
  assert(length == 5.0);

  free_vector(vec);
}

void test_set_data_vector() {
  Vector *vec = new_vector(3);
  set_data_vector(vec, (float[]){1.0, 2.0, 3.0});
  assert(vec->data[0] == 1.0);
  assert(vec->data[1] == 2.0);
  assert(vec->data[2] == 3.0);
  assert(vec->size == 3);
}

void test_set_random_vector() {
  Vector *vec = new_vector(2);
  set_random_vector(vec);

  float length = length_vector(vec);
  assert(fabs(length - 1.0) < 1e-6);

  Vector *vec2 = new_vector(2);
  set_random_vector(vec2);
  assert(vec->data[0] != vec2->data[0] || vec->data[1] != vec2->data[1]);

  free_vector(vec);
  free_vector(vec2);
}

void test_is_equal_vector() {
  Vector *vec1 = new_vector(3);
  set_data_vector(vec1, (float[]){1.0, 2.0, 3.0});
  Vector *vec2 = new_vector(3);
  set_data_vector(vec2, (float[]){1.0, 2.0, 3.0});

  assert(is_equal_vector(vec1, vec2));

  free_vector(vec1);
  free_vector(vec2);
}

void test_save_load_vector() {
  Vector *vec = new_vector(3);
  set_data_vector(vec, (float[]){1.0, 2.0, 3.0});

  FILE *fp = fopen("test_vector.bin", "wb");
  save_vector(vec, fp);
  fclose(fp);

  fp = fopen("test_vector.bin", "rb");
  Vector *loaded_vec = load_vector(fp);
  fclose(fp);

  assert(is_equal_vector(vec, loaded_vec));

  free_vector(vec);
  free_vector(loaded_vec);
  remove("test_vector.bin");
}

int main() {
  test_new_vector();
  test_dot_product_vector();
  test_length_vector();
  test_set_random_vector();
  test_is_equal_vector();
  test_save_load_vector();
  printf("All vector tests passed!\n");
  return 0;
}
