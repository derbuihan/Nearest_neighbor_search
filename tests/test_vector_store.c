#include <assert.h>
#include <stdio.h>

#include "vector_store.h"

void test_vector_store(StoreType store_type) {
  VectorStore *store = new_vector_store(store_type);
  assert(store != NULL);
  assert(store->type == store_type);

  Vector *vec1 = new_vector(3);
  set_data_vector(vec1, (float[]){1.0, 0.0, 0.0});
  add_vector(store, vec1);

  Vector *vec2 = new_vector(3);
  set_data_vector(vec2, (float[]){0.0, 1.0, 0.0});
  add_vector(store, vec2);

  Vector *vec3 = new_vector(3);
  set_data_vector(vec3, (float[]){0.0, 0.0, 1.0});
  add_vector(store, vec3);
  assert(store->num_vectors == 3);

  Vector *query = new_vector(3);
  set_data_vector(query, (float[]){0.0, 1.0, 2.0});

  int result_ids[3];
  float result_dists[3];
  search_vectors(store, query, 3, result_ids, result_dists);
  assert(result_ids[0] == 2);
  assert(result_ids[1] == 1);
  assert(result_ids[2] == 0);
  assert(result_dists[0] == 2.0);
  assert(result_dists[1] == 1.0);
  assert(result_dists[2] == 0.0);

  free_vector(query);
  free_vector_store(store);
}

int main() {
  test_vector_store(STORE_LINEAR);
  test_vector_store(STORE_NSW);

  printf("All vector store tests passed!\n");
}