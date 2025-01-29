#ifndef VECTOR_STORE_H
#define VECTOR_STORE_H

#include "linear_store.h"
#include "vector.h"

typedef enum {
  STORE_LINEAR,
  STORE_NSW,
} StoreType;

typedef struct VectorStore VectorStore;
struct VectorStore {
  StoreType type;
  int num_vectors;

  union {
    LinearStore *linear_store;
  };
};

VectorStore *new_vector_store(StoreType type);
void free_vector_store(VectorStore *store);
void add_vector(VectorStore *store, Vector *v);
void search_vectors(VectorStore *store, Vector *query, int top_k,
                    int *result_ids, float *result_dists);

#endif  // VECTOR_STORE_H
