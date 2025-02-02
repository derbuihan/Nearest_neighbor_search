#ifndef VECTOR_STORE_H
#define VECTOR_STORE_H

#include <stdbool.h>

#include "linear_store.h"
#include "nsw_store.h"

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
    NSWStore *nsw_store;
  };
};

VectorStore *new_vector_store(StoreType type);
void free_vector_store(VectorStore *store);
void add_vector(VectorStore *store, Vector *v);
void search_vectors(VectorStore *store, Vector *query, int top_k,
                    int *result_ids, float *result_dists);
bool is_equal_vector_store(VectorStore *store1, VectorStore *store2);
void save_vector_store(VectorStore *store, FILE *fp);
VectorStore *load_vector_store(FILE *fp);

#endif  // VECTOR_STORE_H
