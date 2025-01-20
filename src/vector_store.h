#ifndef VECTOR_STORE_H
#define VECTOR_STORE_H

#include "vector.h"

typedef struct Node Node;
struct Node {
  int id;
  Vector *vector;
  Node *next;
};

typedef struct VectorStore VectorStore;
struct VectorStore {
  Node *head;
  int num_vectors;
};

Node *new_node(int id, Vector *v);
void free_node(Node *n);

VectorStore *new_vector_store();
void free_vector_store(VectorStore *store);
void add_vector(VectorStore *store, Vector *v);
void print_vector_store(VectorStore *store);

void search_nearest_vector(VectorStore *store, Vector *query, int k,
                           int *result_ids, float *result_dists);

#endif  // VECTOR_STORE_H
