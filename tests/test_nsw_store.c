#include <assert.h>
#include <stdio.h>

#include "nsw_store.h"

/*
typedef struct NSWNode NSWNode;
typedef struct NSWEdge NSWEdge;

struct NSWEdge {
  int id;
  NSWNode *node;
  NSWEdge *next;
};

struct NSWNode {
  int id;
  Vector *vector;
  NSWEdge *edges;
  int num_edges;
  int max_degree;
  NSWNode *next;
};

typedef struct NSWStore NSWStore;
struct NSWStore {
  NSWNode *root;
  int num_vectors;
  int max_degree;
  int ef_construction;
  int ef_search;
};


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
  assert(store->max_degree == 16);
  assert(store->ef_construction == 40);
  assert(store->ef_search == 40);
  free_nsw_store(store);
}

void test_add_vector_nsw_store() {
  NSWStore *store = new_nsw_store(2, 40, 40);

  Vector *vec1 = new_vector(3);
  set_data_vector(vec1, (float[]){1.0, 0.0, 0.0});
  add_vector_nsw_store(store, vec1);

  Vector *vec2 = new_vector(3);
  set_data_vector(vec2, (float[]){0.0, 1.0, 0.0});
  add_vector_nsw_store(store, vec2);

  Vector *vec3 = new_vector(3);
  set_data_vector(vec3, (float[]){0.0, 0.0, 1.0});
  add_vector_nsw_store(store, vec3);

  Vector *vec4 = new_vector(3);
  set_data_vector(vec4, (float[]){0.0, 1.0, 1.0});
  add_vector_nsw_store(store, vec4);

  assert(store->num_vectors == 4);
  assert(store->root->id == 3);
  assert(store->root->num_edges == 2);
  for (NSWEdge *edge = store->root->edges; edge; edge = edge->next) {
    assert(edge->id == 0 || edge->id == 1);
    assert(edge->node->id == 1 || edge->node->id == 2);
  }

  free_nsw_store(store);
}

int main() {
  test_new_nsw_store();
  test_add_vector_nsw_store();

  printf("All linear store tests passed!\n");
  return 0;
}
