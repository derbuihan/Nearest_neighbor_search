#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "priority_queue.h"
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

void print_pq(PgNode *node) {
  if (node) {
    printf("Node: %d, %f\n", node->id, node->priority);
    print_pq(node->left);
    print_pq(node->right);
  }
}

int main(void) {
  PriorityQueue *queue = new_priority_queue();

  int dimensions = 1536;
  int size = 100;

  VectorStore *store = create_test_vector_store(dimensions, size);
  // print_vector_store(store);

  Vector *query = new_vector(dimensions);
  set_random_vector(query);

  int k = 100;
  int result_ids[k];
  float result_dists[k];
  search_nearest_vector(store, query, k, result_ids, result_dists);

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

  return 0;
}