#ifndef VECTOR_STORE_H
#define VECTOR_STORE_H

#include "vector.h"

typedef enum {
  STORE_LINEAR,
} StoreType;

typedef struct VectorStore VectorStore;
struct VectorStore {
  StoreType type;
  void *store;
  void (*add_vector)(void *store, Vector *v);
  void (*search_vectors)(void *store, Vector *query, int top_k, int *result_ids,
                         float *result_dists);
};

VectorStore *new_vector_store(StoreType type);
void free_vector_store(VectorStore *store);
void add_vector(VectorStore *store, Vector *v);
void search_vectors(VectorStore *store, Vector *query, int top_k,
                    int *result_ids, float *result_dists);

#endif  // VECTOR_STORE_H
