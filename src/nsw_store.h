#ifndef NSW_STORE_H
#define NSW_STORE_H

#include "vector.h"

typedef struct Node Node;
typedef struct Edge Edge;

struct Node {
  int id;
  Vector *vector;
  Edge *edges;
  int num_edges;
};

struct Edge {
  Node *target;
  float distance;
};

typedef struct NSWStore NSWStore;
struct NSWStore {
  Node *root;
  int num_vectors;
};

NSWStore *new_nsw_store();
void free_nsw_store(NSWStore *store);
void add_vector_nsw_store(NSWStore *store, Vector *v);
void search_vectors_nsw_store(NSWStore *store, Vector *query, int top_k,
                              int *result_ids, float *result_dists);
void print_nsw_store(NSWStore *store);

#endif  // NSW_STORE_H
