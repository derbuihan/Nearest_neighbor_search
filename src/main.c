#include <stdio.h>
#include <stdlib.h>

#include "load_datasets.h"
#include "vector.h"
#include "vector_store.h"

void set_vector_store_from_dataset(VectorStore *store, Dataset *dataset,
                                   int size) {
  if (dataset->num_records < size) {
    printf("Error: not enough vectors in the dataset\n");
    exit(1);
  }

  for (int i = 0; i < size; i++) {
    Record *record = &dataset->records[i];
    Vector *v = new_vector(record->embedding_size);
    for (int j = 0; j < record->embedding_size; j++) {
      v->data[j] = record->embedding[j];
    }
    add_vector(store, v);
  }
}

VectorStore *create_test_vector_store(int dimensions, int size) {
  VectorStore *store = new_vector_store(STORE_LINEAR);
  for (int i = 0; i < size; i++) {
    Vector *v = new_vector(dimensions);
    set_random_vector(v);
    add_vector(store, v);
  }
  return store;
}

int main(void) {
  char *filename = "../datasets/awsdocs.parquet";

  Dataset *dataset = load_dataset(filename);
  int dimensions = 1536;
  int size = 327003;
  VectorStore *store = create_test_vector_store(dimensions, size);
  set_vector_store_from_dataset(store, dataset, size);

  Vector *query = new_vector(dimensions);
  set_random_vector(query);

  int top_k = 10;
  int result_ids[top_k];
  float result_dists[top_k];
  search_vectors(store, query, top_k, result_ids, result_dists);

  printf("Results: ");
  for (int i = 0; i < top_k; i++) {
    printf("%d ", result_ids[i]);
  }
  printf("\n");

  printf("Distances: ");
  for (int i = 0; i < top_k; i++) {
    printf("%f ", result_dists[i]);
  }
  printf("\n");

  free_dataset(dataset);
  free_vector(query);
  free_vector_store(store);

  return 0;
}
