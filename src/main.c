#include <stdio.h>

#include "vector.h"
#include "vector_store.h"

VectorStore *create_test_vector_store(int dimensions, int size) {
  VectorStore *store = new_vector_store();
  for (int i = 0; i < size; i++) {
    Vector *v = new_vector(dimensions);
    set_random_vector(v);
    add_vector(store, v);
  }
  return store;
}

int main(void) {
  int dimensions = 1536;
  int size = 100000;

  VectorStore *store = create_test_vector_store(dimensions, size);
  // print_vector_store(store);

  Vector *query = new_vector(dimensions);
  set_random_vector(query);

  int k = 3;
  int result_ids[k];
  float result_dists[k];
  search_nearest_vector(store, query, k, result_ids, result_dists);
  printf("Results: %d, %d, %d\n", result_ids[0], result_ids[1], result_ids[2]);
  printf("Distances: %f, %f, %f\n", result_dists[0], result_dists[1],
         result_dists[2]);

  return 0;
}