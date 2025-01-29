#include <assert.h>
#include <stdio.h>

#include "nsw_store.h"

/*
NSWStore *new_nsw_store();
void free_nsw_store(NSWStore *store);
void add_vector_nsw_store(NSWStore *store, Vector *v);
void search_vectors_nsw_store(NSWStore *store, Vector *query, int top_k,
                              int *result_ids, float *result_dists);
void print_nsw_store(NSWStore *store);
 */

void test_new_nsw_store() {
  NSWStore *store = new_nsw_store();
  assert(store != NULL);
  assert(store->num_vectors == 0);
  free_nsw_store(store);
}

int main() {
  test_new_nsw_store();

  printf("All linear store tests passed!\n");
  return 0;
}
