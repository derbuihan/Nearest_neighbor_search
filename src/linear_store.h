#ifndef LINEAR_STORE_H
#define LINEAR_STORE_H

#include "vector.h"

typedef struct Node Node;
struct Node {
  int id;
  Vector *vector;
  Node *next;
};

typedef struct LinearStore LinearStore;
struct LinearStore {
  Node *head;
  int num_vectors;
};

LinearStore *new_linear_store();
void free_linear_store(LinearStore *store);
void add_vector_linear_store(LinearStore *store, Vector *v);
void search_vector_linear_store(LinearStore *store, Vector *query, int k,
                                int *result_ids, float *result_dists);
void print_linear_store(LinearStore *store);

#endif  // LINEAR_STORE_H
