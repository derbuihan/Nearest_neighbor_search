#ifndef LINEAR_STORE_H
#define LINEAR_STORE_H

#include <stdbool.h>

#include "vector.h"

typedef struct Node Node;
struct Node {
  int id;
  Vector *vector;
  Node *next;
};

typedef struct LinearStore LinearStore;
struct LinearStore {
  int num_vectors;
  Node *head;
};

LinearStore *new_linear_store();
void free_linear_store(LinearStore *store);
void add_vector_linear_store(LinearStore *store, Vector *v);
void search_vectors_linear_store(LinearStore *store, Vector *query, int top_k,
                                 int *result_ids, float *result_dists);
bool is_equal_linear_store(LinearStore *store1, LinearStore *store2);
void print_linear_store(LinearStore *store);
void save_linear_store(LinearStore *store, FILE *fp);
LinearStore *load_linear_store(FILE *fp);

#endif  // LINEAR_STORE_H
