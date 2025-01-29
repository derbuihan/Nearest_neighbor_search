#include "nsw_store.h"

#include <stdlib.h>

/*
typedef struct NSWNode NSWNode;
typedef struct NSWEdge NSWEdge;

struct NSWEdge {
NSWNode *node;
NSWEdge *next;
};

struct NSWNode {
  int id;
  Vector *vector;
  NSWEdge *edges;
  int num_edges;
  NSWNode *next;
};

typedef struct NSWStore NSWStore;
struct NSWStore {
NSWNode *root;
int num_vectors;
};
*/

NSWStore *new_nsw_store() {
  NSWStore *store = malloc(sizeof(NSWStore));
  store->root = NULL;
  store->num_vectors = 0;
  return store;
}

static void free_nsw_edge(NSWEdge *edge) {
  while (edge) {
    NSWEdge *next = edge->next;
    free(edge);
    edge = next;
  }
}

static void free_nsw_node(NSWNode *node) {
  free_vector(node->vector);
  free_nsw_edge(node->edges);
  free(node);
}

void free_nsw_store(NSWStore *store) {
  NSWNode *node = store->root;
  while (node) {
    NSWNode *next = node->next;
    free_nsw_node(node);
    node = next;
  }
}

void add_vector_nsw_store(NSWStore *store, Vector *v);
void search_vectors_nsw_store(NSWStore *store, Vector *query, int top_k,
                              int *result_ids, float *result_dists);
void print_nsw_store(NSWStore *store);
