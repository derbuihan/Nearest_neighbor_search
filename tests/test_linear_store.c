#include <assert.h>
#include <stdio.h>

#include "linear_store.h"

void test_new_linear_store() {
  LinearStore *store = new_linear_store();
  assert(store != NULL);
  assert(store->num_vectors == 0);
  free_linear_store(store);
}

void test_add_vector_linear_store() {
  LinearStore *store = new_linear_store();
  Vector *vec = new_vector(3);
  set_random_vector(vec);
  add_vector_linear_store(store, vec);
  assert(store->num_vectors == 1);
  free_linear_store(store);
}

void test_search_vectors_linear_store() {
  LinearStore *store = new_linear_store();

  Vector *vec1 = new_vector(3);
  set_data_vector(vec1, (float[]){1.0, 0.0, 0.0});
  add_vector_linear_store(store, vec1);

  Vector *vec2 = new_vector(3);
  set_data_vector(vec2, (float[]){0.0, 1.0, 0.0});
  add_vector_linear_store(store, vec2);

  Vector *vec3 = new_vector(3);
  set_data_vector(vec3, (float[]){0.0, 0.0, 1.0});
  add_vector_linear_store(store, vec3);

  Vector *query = new_vector(3);
  set_data_vector(query, (float[]){0.0, 1.0, 2.0});

  int top_k = 3;
  int result_ids[top_k];
  float result_dists[top_k];

  search_vectors_linear_store(store, query, top_k, result_ids, result_dists);
  assert(result_ids[0] == 2);
  assert(result_ids[1] == 1);
  assert(result_ids[2] == 0);

  assert(result_dists[0] == 2.0);
  assert(result_dists[1] == 1.0);
  assert(result_dists[2] == 0.0);

  free_vector(query);
  free_linear_store(store);
}

void test_is_equal_linear_store() {
  LinearStore *store1 = new_linear_store();
  Vector *vec11 = new_vector(3);
  set_data_vector(vec11, (float[]){1.0, 0.0, 0.0});
  add_vector_linear_store(store1, vec11);

  Vector *vec12 = new_vector(3);
  set_data_vector(vec12, (float[]){0.0, 1.0, 0.0});
  add_vector_linear_store(store1, vec12);

  Vector *vec13 = new_vector(3);
  set_data_vector(vec13, (float[]){0.0, 0.0, 1.0});
  add_vector_linear_store(store1, vec13);

  LinearStore *store2 = new_linear_store();
  Vector *vec21 = new_vector(3);
  set_data_vector(vec21, (float[]){1.0, 0.0, 0.0});
  add_vector_linear_store(store2, vec21);

  Vector *vec22 = new_vector(3);
  set_data_vector(vec22, (float[]){0.0, 1.0, 0.0});
  add_vector_linear_store(store2, vec22);

  Vector *vec23 = new_vector(3);
  set_data_vector(vec23, (float[]){0.0, 0.0, 1.0});
  add_vector_linear_store(store2, vec23);

  assert(is_equal_linear_store(store1, store2));

  free_linear_store(store1);
  free_linear_store(store2);
}

void test_save_load_linear_store() {
  LinearStore *store = new_linear_store();
  Vector *vec1 = new_vector(3);
  set_data_vector(vec1, (float[]){1.0, 0.0, 0.0});
  add_vector_linear_store(store, vec1);

  Vector *vec2 = new_vector(3);
  set_data_vector(vec2, (float[]){0.0, 1.0, 0.0});
  add_vector_linear_store(store, vec2);

  Vector *vec3 = new_vector(3);
  set_data_vector(vec3, (float[]){0.0, 0.0, 1.0});
  add_vector_linear_store(store, vec3);

  FILE *fp = fopen("test_linear_store.bin", "wb");
  save_linear_store(store, fp);
  fclose(fp);

  fp = fopen("test_linear_store.bin", "rb");
  LinearStore *loaded_store = load_linear_store(fp);
  fclose(fp);

  assert(is_equal_linear_store(store, loaded_store));

  free_linear_store(store);
  free_linear_store(loaded_store);
  remove("test_linear_store.bin");
}

int main() {
  test_new_linear_store();
  test_add_vector_linear_store();
  test_search_vectors_linear_store();
  test_is_equal_linear_store();
  test_save_load_linear_store();

  printf("All linear store tests passed!\n");
  return 0;
}
