#ifndef NSW_STORE_H
#define NSW_STORE_H

#include "vector.h"

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

NSWStore *new_nsw_store();
void free_nsw_store(NSWStore *store);
void add_vector_nsw_store(NSWStore *store, Vector *v);
void search_vectors_nsw_store(NSWStore *store, Vector *query, int top_k,
                              int *result_ids, float *result_dists);
void print_nsw_store(NSWStore *store);

#endif  // NSW_STORE_H
