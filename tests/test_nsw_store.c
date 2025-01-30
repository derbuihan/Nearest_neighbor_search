#include <assert.h>
#include <stdio.h>

#include "nsw_store.h"

/*
NSWStore *new_nsw_store(int max_degree, int ef_construction, int ef_search);
void free_nsw_store(NSWStore *store);
void add_vector_nsw_store(NSWStore *store, Vector *v);
void search_vectors_nsw_store(NSWStore *store, Vector *query, int top_k,
                              int *result_ids, float *result_dists);
void print_nsw_store(NSWStore *store);
 */

void test_new_nsw_store() {
  NSWStore *store = new_nsw_store(16, 40, 40);
  assert(store != NULL);
  assert(store->num_vectors == 0);
  free_nsw_store(store);
}

void test_add_vector_nsw_store() {
  NSWStore *store = new_nsw_store(16, 40, 40);

  Vector *vec1 = new_vector(3);
  set_data_vector(vec1, (float[]){1.0, 0.0, 0.0});
  add_vector_nsw_store(store, vec1);

  Vector *vec2 = new_vector(3);
  set_data_vector(vec2, (float[]){0.0, 1.0, 0.0});
  add_vector_nsw_store(store, vec2);

  Vector *vec3 = new_vector(3);
  set_data_vector(vec3, (float[]){0.0, 0.0, 1.0});
  add_vector_nsw_store(store, vec3);

  assert(store->num_vectors == 3);
  free_nsw_store(store);
}

int main() {
  test_new_nsw_store();
  test_add_vector_nsw_store();

  printf("All linear store tests passed!\n");
  return 0;
}
