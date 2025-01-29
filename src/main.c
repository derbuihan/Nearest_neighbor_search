#include <stdio.h>

#include "load_datasets.h"
#include "vector.h"
#include "vector_store.h"

void set_vector_store_from_dataset(VectorStore *store, Dataset *dataset) {
  for (int i = 0; i < dataset->num_records; i++) {
    Record *record = &dataset->records[i];
    Vector *v = new_vector(record->embedding_size);
    for (int j = 0; j < record->embedding_size; j++) {
      v->data[j] = record->embedding[j];
    }
    add_vector(store, v);
  }
}

int main(void) {
  char *awsdocs = "../datasets/awsdocs.parquet";
  char *querydocs = "../datasets/query.parquet";

  // Setup VectorStore
  Dataset *dataset = load_dataset(awsdocs);
  VectorStore *store = new_vector_store(STORE_LINEAR);
  set_vector_store_from_dataset(store, dataset);

  // Setup Query
  Dataset *query_dataset = load_dataset(querydocs);
  Record *query_record = &query_dataset->records[0];
  Vector *query = new_vector(query_record->embedding_size);
  set_data_vector(query, query_record->embedding);
  printf("Query: %s\n", query_record->url);

  // Search
  int top_k = 10;
  int result_ids[top_k];
  float result_dists[top_k];
  search_vectors(store, query, top_k, result_ids, result_dists);

  // Print Results
  for (int i = 0; i < top_k; i++) {
    Record *record = &dataset->records[result_ids[i]];
    printf("Results: id: %d, distance: %f, url: %s\n", result_ids[i],
           result_dists[i], record->url);
  }

  // Free Memory
  free_dataset(dataset);
  free_vector_store(store);
  free_vector(query);
  free_dataset(query_dataset);

  return 0;
}
