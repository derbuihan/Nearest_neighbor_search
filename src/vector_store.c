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
      store->nsw_store = new_nsw_store(16, 128, 128);
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

bool is_equal_vector_store(VectorStore *store1, VectorStore *store2) {
  if (store1->type != store2->type) return false;
  if (store1->num_vectors != store2->num_vectors) return false;

  switch (store1->type) {
    case STORE_LINEAR:
      if (!is_equal_linear_store(store1->linear_store, store2->linear_store))
        return false;
      break;
    case STORE_NSW:
      if (!is_equal_nsw_store(store1->nsw_store, store2->nsw_store))
        return false;
      break;
    default:
      break;
  }

  return true;
}

void save_vector_store(VectorStore *store, FILE *fp) {
  fwrite(&store->type, sizeof(StoreType), 1, fp);
  fwrite(&store->num_vectors, sizeof(int), 1, fp);
  switch (store->type) {
    case STORE_LINEAR:
      save_linear_store(store->linear_store, fp);
      break;
    case STORE_NSW:
      save_nsw_store(store->nsw_store, fp);
      break;
    default:
      break;
  }
}

VectorStore *load_vector_store(FILE *fp) {
  VectorStore *store = malloc(sizeof(VectorStore));
  fread(&store->type, sizeof(StoreType), 1, fp);
  fread(&store->num_vectors, sizeof(int), 1, fp);
  switch (store->type) {
    case STORE_LINEAR:
      store->linear_store = load_linear_store(fp);
      break;
    case STORE_NSW:
      store->nsw_store = load_nsw_store(fp);
      break;
    default:
      free(store);
      return NULL;
  }
  return store;
}
