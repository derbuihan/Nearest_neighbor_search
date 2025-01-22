#include <stdio.h>

#include "linear_store.h"
#include "priority_queue.h"
#include "vector.h"

LinearStore *create_test_vector_store(int dimensions, int size) {
  LinearStore *store = new_linear_store();
  for (int i = 0; i < size; i++) {
    Vector *v = new_vector(dimensions);
    set_random_vector(v);
    add_vector_linear_store(store, v);
  }
  return store;
}

void print_pq(PgNode *node) {
  if (node) {
    printf("Node: %d, %f\n", node->id, node->priority);
    print_pq(node->left);
    print_pq(node->right);
  }
}

int main(void) {
  int dimensions = 1536;
  int size = 327003;

  LinearStore *store = create_test_vector_store(dimensions, size);
  // print_vector_store(store);

  Vector *query = new_vector(dimensions);
  set_random_vector(query);

  int k = 100;
  int result_ids[k];
  float result_dists[k];
  search_vector_linear_store(store, query, k, result_ids, result_dists);

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
  free_linear_store(store);

  return 0;
}
