#include <assert.h>
#include <stdio.h>

#include "linear_store.h"

void test_new_linear_store() {
  LinearStore *store = new_linear_store();
  assert(store != NULL);
  assert(store->num_vectors == 0);
  free_linear_store(store);
}

void test_add_vector_linear_store() {
  LinearStore *store = new_linear_store();
  Vector *vec = new_vector(3);
  set_random_vector(vec);
  add_vector_linear_store(store, vec);
  assert(store->num_vectors == 1);
  free_linear_store(store);
}

void test_search_vectors_linear_store() {
  LinearStore *store = new_linear_store();

  Vector *vec1 = new_vector(3);
  set_data_vector(vec1, (float[]){1.0, 0.0, 0.0});
  add_vector_linear_store(store, vec1);

  Vector *vec2 = new_vector(3);
  set_data_vector(vec2, (float[]){0.0, 1.0, 0.0});
  add_vector_linear_store(store, vec2);

  Vector *vec3 = new_vector(3);
  set_data_vector(vec3, (float[]){0.0, 0.0, 1.0});
  add_vector_linear_store(store, vec3);

  Vector *query = new_vector(3);
  set_data_vector(query, (float[]){0.0, 1.0, 2.0});

  int top_k = 3;
  int result_ids[top_k];
  float result_dists[top_k];

  search_vectors_linear_store(store, query, top_k, result_ids, result_dists);
  assert(result_ids[0] == 2);
  assert(result_ids[1] == 1);
  assert(result_ids[2] == 0);

  assert(result_dists[0] == 2.0);
  assert(result_dists[1] == 1.0);
  assert(result_dists[2] == 0.0);

  free_vector(query);
  free_linear_store(store);
}

void test_save_load_linear_store() {
  LinearStore *store = new_linear_store();
  Vector *vec1 = new_vector(3);
  set_data_vector(vec1, (float[]){1.0, 0.0, 0.0});
  add_vector_linear_store(store, vec1);

  Vector *vec2 = new_vector(3);
  set_data_vector(vec2, (float[]){0.0, 1.0, 0.0});
  add_vector_linear_store(store, vec2);

  Vector *vec3 = new_vector(3);
  set_data_vector(vec3, (float[]){0.0, 0.0, 1.0});
  add_vector_linear_store(store, vec3);

  FILE *fp = fopen("test_linear_store.bin", "wb");
  save_linear_store(store, fp);
  fclose(fp);

  fp = fopen("test_linear_store.bin", "rb");
  LinearStore *loaded_store = load_linear_store(fp);
  fclose(fp);

  assert(loaded_store->num_vectors == store->num_vectors);
  assert(loaded_store->head->id == store->head->id);
  assert(loaded_store->head->next->id == store->head->next->id);
  assert(loaded_store->head->next->next->id == store->head->next->next->id);
  for (int i = 0; i < 3; i++) {
    assert(loaded_store->head->vector->data[i] == store->head->vector->data[i]);
    assert(loaded_store->head->next->vector->data[i] ==
           store->head->next->vector->data[i]);
    assert(loaded_store->head->next->next->vector->data[i] ==
           store->head->next->next->vector->data[i]);
  }

  free_linear_store(store);
  free_linear_store(loaded_store);
  remove("test_linear_store.bin");
}

int main() {
  test_new_linear_store();
  test_add_vector_linear_store();
  test_search_vectors_linear_store();
  test_save_load_linear_store();

  printf("All linear store tests passed!\n");
  return 0;
}
