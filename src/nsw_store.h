#ifndef NSW_STORE_H
#define NSW_STORE_H

#include <stdbool.h>

#include "vector.h"

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
  int max_degree;
  int num_edges;
  NSWEdge *edges;
  NSWNode *next;
};

typedef struct NSWStore NSWStore;
struct NSWStore {
  int max_degree;
  int ef_construction;
  int ef_search;
  int num_vectors;
  NSWNode *root;
};

NSWStore *new_nsw_store(int max_degree, int ef_construction, int ef_search);
void free_nsw_store(NSWStore *store);
void add_vector_nsw_store(NSWStore *store, Vector *v);
void search_vectors_nsw_store(NSWStore *store, Vector *query, int top_k,
                              int *result_ids, float *result_dists);
bool is_equal_nsw_store(NSWStore *store1, NSWStore *store2);
void print_nsw_store(NSWStore *store);
void save_nsw_store(NSWStore *store, FILE *fp);
NSWStore *load_nsw_store(FILE *fp);

#endif  // NSW_STORE_H
