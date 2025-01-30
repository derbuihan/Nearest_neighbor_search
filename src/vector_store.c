#include "vector_store.h"

#include <stdlib.h>

VectorStore *new_vector_store(StoreType type) {
  VectorStore *store = malloc(sizeof(VectorStore));
  store->type = type;

  switch (type) {
    case STORE_LINEAR:
      store->linear_store = new_linear_store();
      break;
    case STORE_NSW:
      store->nsw_store = new_nsw_store(8, 20, 100);
      break;
    default:
      free(store);
      return NULL;
  }

  return store;
}

void free_vector_store(VectorStore *store) {
  switch (store->type) {
    case STORE_LINEAR:
      free_linear_store(store->linear_store);
      break;
    case STORE_NSW:
      free_nsw_store(store->nsw_store);
      break;
    default:
      break;
  }
  free(store);
}

void add_vector(VectorStore *store, Vector *v) {
  switch (store->type) {
    case STORE_LINEAR:
      add_vector_linear_store(store->linear_store, v);
      store->num_vectors++;
      break;
    case STORE_NSW:
      add_vector_nsw_store(store->nsw_store, v);
      store->num_vectors++;
      break;
    default:
      break;
  }
}
void search_vectors(VectorStore *store, Vector *query, int top_k,
                    int *result_ids, float *result_dists) {
  switch (store->type) {
    case STORE_LINEAR:
      search_vectors_linear_store(store->linear_store, query, top_k, result_ids,
                                  result_dists);
      break;
    case STORE_NSW:
      search_vectors_nsw_store(store->nsw_store, query, top_k, result_ids,
                               result_dists);
      break;
    default:
      break;
  }
}
