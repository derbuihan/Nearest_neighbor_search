#include <assert.h>
#include <stdio.h>

#include "vector_store.h"

void test_vector_store(StoreType store_type) {
  // Test new_vector_store
  VectorStore *store1 = new_vector_store(store_type);
  assert(store1 != NULL);
  assert(store1->type == store_type);

  // Test add_vector
  Vector *vec11 = new_vector(3);
  set_data_vector(vec11, (float[]){1.0, 0.0, 0.0});
  add_vector(store1, vec11);

  Vector *vec12 = new_vector(3);
  set_data_vector(vec12, (float[]){0.0, 1.0, 0.0});
  add_vector(store1, vec12);

  Vector *vec13 = new_vector(3);
  set_data_vector(vec13, (float[]){0.0, 0.0, 1.0});
  add_vector(store1, vec13);
  assert(store1->num_vectors == 3);

  // Test search_vectors
  Vector *query = new_vector(3);
  set_data_vector(query, (float[]){0.0, 1.0, 2.0});

  int result_ids[3];
  float result_dists[3];
  search_vectors(store1, query, 3, result_ids, result_dists);
  assert(result_ids[0] == 2 && result_ids[1] == 1 && result_ids[2] == 0);
  assert(result_dists[0] == 2.0 && result_dists[1] == 1.0 &&
         result_dists[2] == 0.0);
  free_vector(query);

  // Test is_equal_vector_store
  VectorStore *store2 = new_vector_store(store_type);
  Vector *vec21 = new_vector(3);
  set_data_vector(vec21, (float[]){1.0, 0.0, 0.0});
  add_vector(store2, vec21);

  Vector *vec22 = new_vector(3);
  set_data_vector(vec22, (float[]){0.0, 1.0, 0.0});
  add_vector(store2, vec22);

  Vector *vec23 = new_vector(3);
  set_data_vector(vec23, (float[]){0.0, 0.0, 1.0});
  add_vector(store2, vec23);

  assert(is_equal_vector_store(store1, store2));
  free_vector_store(store2);

  // Test save_vector_store and load_vector_store
  FILE *fp = fopen("test_vector_store.bin", "wb");
  save_vector_store(store1, fp);
  fclose(fp);

  fp = fopen("test_vector_store.bin", "rb");
  VectorStore *loaded_store = load_vector_store(fp);
  fclose(fp);

  assert(is_equal_vector_store(store1, loaded_store));
  free_vector_store(loaded_store);
  remove("test_vector_store.bin");

  // Test free_vector_store
  free_vector_store(store1);
}

int main() {
  test_vector_store(STORE_LINEAR);
  // test_vector_store(STORE_NSW);

  printf("All vector store tests passed!\n");
}
