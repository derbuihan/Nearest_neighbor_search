#include "vector_store.h"

#include <stdlib.h>

#include "linear_store.h"

VectorStore *new_vector_store(StoreType type) {
  VectorStore *store = malloc(sizeof(VectorStore));

  switch (type) {
    case STORE_LINEAR:
      store->store = new_linear_store();
      store->type = type;
      store->add_vector = (void (*)(void *, Vector *))add_vector_linear_store;
      store->search_vector = (void (*)(void *, Vector *, int, int *,
                                       float *))search_vector_linear_store;
      break;
  }

  return store;
}

void free_vector_store(VectorStore *store) {
  switch (store->type) {
    case STORE_LINEAR:
      free_linear_store(store->store);
      break;
  }

  free(store);
}

void add_vector(VectorStore *store, Vector *v) { store->add_vector(store, v); }
void search_vector(VectorStore *store, Vector *query, int k, int *result_ids,
                   float *result_dists) {
  store->search_vector(store, query, k, result_ids, result_dists);
}
