#include <assert.h>
#include <stdio.h>

#include "nsw_store.h"

void test_new_nsw_store() {
  NSWStore *store = new_nsw_store(16, 40, 40);
  assert(store != NULL);
  assert(store->num_vectors == 0);
  assert(store->max_degree == 16);
  assert(store->ef_construction == 40);
  assert(store->ef_search == 40);
  free_nsw_store(store);
}

void test_add_vector_nsw_store() {
  NSWStore *store = new_nsw_store(2, 40, 40);

  Vector *vec1 = new_vector(3);
  set_data_vector(vec1, (float[]){1.0, 0.0, 0.0});
  add_vector_nsw_store(store, vec1);

  Vector *vec2 = new_vector(3);
  set_data_vector(vec2, (float[]){0.0, 1.0, 0.0});
  add_vector_nsw_store(store, vec2);

  Vector *vec3 = new_vector(3);
  set_data_vector(vec3, (float[]){0.0, 0.0, 1.0});
  add_vector_nsw_store(store, vec3);

  Vector *vec4 = new_vector(3);
  set_data_vector(vec4, (float[]){0.0, 1.0, 1.0});
  add_vector_nsw_store(store, vec4);

  assert(store->num_vectors == 4);
  assert(store->root->id == 3);
  assert(store->root->num_edges == 2);
  for (NSWEdge *edge = store->root->edges; edge; edge = edge->next) {
    assert(edge->id == 0 || edge->id == 1);
    assert(edge->node->id == 1 || edge->node->id == 2);
  }

  free_nsw_store(store);
}

void test_search_vectors_nsw_store() {
  NSWStore *store = new_nsw_store(2, 40, 40);

  Vector *vec1 = new_vector(3);
  set_data_vector(vec1, (float[]){1.0, 0.0, 0.0});
  add_vector_nsw_store(store, vec1);

  Vector *vec2 = new_vector(3);
  set_data_vector(vec2, (float[]){0.0, 1.0, 0.0});
  add_vector_nsw_store(store, vec2);

  Vector *vec3 = new_vector(3);
  set_data_vector(vec3, (float[]){0.0, 0.0, 1.0});
  add_vector_nsw_store(store, vec3);

  Vector *query = new_vector(3);
  set_data_vector(query, (float[]){0.0, 1.0, 2.0});

  int top_k = 3;
  int result_ids[top_k];
  float result_dists[top_k];

  search_vectors_nsw_store(store, query, top_k, result_ids, result_dists);
  assert(result_ids[0] == 2);
  assert(result_ids[1] == 1);
  assert(result_ids[2] == 0);

  assert(result_dists[0] == 2.0);
  assert(result_dists[1] == 1.0);
  assert(result_dists[2] == 0.0);

  free_vector(query);
  free_nsw_store(store);
}

void test_save_load_linear_store() {
  NSWStore *store = new_nsw_store(16, 40, 40);
  Vector *vec1 = new_vector(3);
  set_data_vector(vec1, (float[]){1.0, 0.0, 0.0});
  add_vector_nsw_store(store, vec1);

  Vector *vec2 = new_vector(3);
  set_data_vector(vec2, (float[]){0.0, 1.0, 0.0});
  add_vector_nsw_store(store, vec2);

  Vector *vec3 = new_vector(3);
  set_data_vector(vec3, (float[]){0.0, 0.0, 1.0});
  add_vector_nsw_store(store, vec3);

  FILE *fp = fopen("test_nsw_store.bin", "wb");
  save_nsw_store(store, fp);
  fclose(fp);

  fp = fopen("test_nsw_store.bin", "rb");
  NSWStore *loaded_store = load_nsw_store(fp);
  fclose(fp);

  // Check if the loaded store is the same as the original store
  assert(loaded_store->max_degree == store->max_degree);
  assert(loaded_store->ef_construction == store->ef_construction);
  assert(loaded_store->ef_search == store->ef_search);
  assert(loaded_store->num_vectors == store->num_vectors);
  for (NSWNode *node = store->root, *loaded_node = loaded_store->root;
       node || loaded_node;
       node = node->next, loaded_node = loaded_node->next) {
    assert(loaded_node->id == node->id);
    assert(loaded_node->max_degree == node->max_degree);
    assert(loaded_node->num_edges == node->num_edges);
    assert(loaded_node->vector->size == node->vector->size);
    for (int i = 0; i < node->vector->size; i++) {
      assert(loaded_node->vector->data[i] == node->vector->data[i]);
    }
    for (NSWEdge *edge = node->edges, *loaded_edge = loaded_node->edges;
         edge || loaded_edge;
         edge = edge->next, loaded_edge = loaded_edge->next) {
      assert(loaded_edge->id == edge->id);
      assert(loaded_edge->node->id == edge->node->id);
    }
  }

  free_nsw_store(store);
  free_nsw_store(loaded_store);
  remove("test_nsw_store.bin");
}

int main() {
  test_new_nsw_store();
  test_add_vector_nsw_store();
  test_search_vectors_nsw_store();
  test_save_load_linear_store();

  printf("All linear store tests passed!\n");
  return 0;
}
