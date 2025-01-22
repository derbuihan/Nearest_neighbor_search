#include <stdio.h>

#include "vector.h"
#include "vector_store.h"

VectorStore *create_test_vector_store(int dimensions, int size) {
  VectorStore *store = new_vector_store(STORE_LINEAR);
  for (int i = 0; i < size; i++) {
    Vector *v = new_vector(dimensions);
    set_random_vector(v);
    add_vector(store, v);
  }
  return store;
}

int main(void) {
  int dimensions = 1536;
  int size = 327003;

  VectorStore *store = create_test_vector_store(dimensions, size);
  // print_vector_store(store);

  Vector *query = new_vector(dimensions);
  set_random_vector(query);

  int k = 10;
  int result_ids[k];
  float result_dists[k];
  search_vector(store, query, k, result_ids, result_dists);

  printf("Results: ");
  for (int i = 0; i < k; i++) {
    printf("%d ", result_ids[i]);
  }
  printf("\n");

  printf("Distances: ");
  for (int i = 0; i < k; i++) {
    printf("%f ", result_dists[i]);
  }
  printf("\n");

  free_vector(query);
  free_vector_store(store);

  return 0;
}
