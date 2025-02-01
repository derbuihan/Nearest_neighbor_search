#include <stdio.h>
#include <sys/time.h>
#include <time.h>

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

void save_nsw_index(Dataset *dataset, char *filename, StoreType store_type) {
  remove(filename);
  VectorStore *store = new_vector_store(store_type);
  set_vector_store_from_dataset(store, dataset);

  FILE *fp = fopen(filename, "wb");
  save_vector_store(store, fp);
  fclose(fp);
}

VectorStore *load_nsw_index(char *filename) {
  FILE *fp = fopen(filename, "rb");
  VectorStore *store = load_vector_store(fp);
  fclose(fp);
  return store;
}

int main(void) {
  char *awsdocs = "../datasets/awsdocs.parquet";
  char *querydocs = "../datasets/query.parquet";

  StoreType types[] = {STORE_LINEAR, STORE_NSW};
  char *index_files[] = {"awsdocs.linear.index", "awsdocs.nsw.index"};

  // Save Indexes
  Dataset *dataset = load_dataset(awsdocs);
  for (int i = 0; i < 2; i++) {
    save_nsw_index(dataset, index_files[i], types[i]);
  }

  // Load Indexes
  VectorStore *stores[2];
  for (int i = 0; i < 2; i++) {
    stores[i] = load_nsw_index(index_files[i]);
  }

  // Setup Query
  Dataset *query_dataset = load_dataset(querydocs);
  Vector *query[query_dataset->num_records];
  int num_queries = query_dataset->num_records;
  for (int i = 0; i < num_queries; i++) {
    Record *record = &query_dataset->records[i];
    query[i] = new_vector(record->embedding_size);
    set_data_vector(query[i], record->embedding);
  }

  // Search
  int top_k = 10;
  int result_ids[top_k];
  float result_dists[top_k];

  for (int i = 0; i < 2; i++) {
    VectorStore *store = stores[i];

    struct timeval start, end;
    clock_t start_t, end_t;
    double wall_time, cpu_time;

    gettimeofday(&start, NULL);
    start_t = clock();

    for (int j = 0; j < num_queries; j++) {
      search_vectors(store, query[j], top_k, result_ids, result_dists);
      printf("Reuslt: %d %d %d\n", result_ids[0], result_ids[1], result_ids[2]);
    }

    end_t = clock();
    gettimeofday(&end, NULL);

    wall_time = (double)(end.tv_sec - start.tv_sec) +
                (double)(end.tv_usec - start.tv_usec) / 1000000;
    cpu_time = (double)(end_t - start_t) / CLOCKS_PER_SEC;

    printf("Index: %s\n", index_files[i]);
    printf("Total queries: %d\n", query_dataset->num_records);
    printf("Wall time: %.3f seconds (%.3f ms per query)\n", wall_time,
           wall_time / query_dataset->num_records * 1000);
    printf("CPU time: %.3f seconds (%.3f ms per query)\n", cpu_time,
           cpu_time / query_dataset->num_records * 1000);
  }

  // Free Memory
  free_dataset(dataset);
  for (int i = 0; i < 2; i++) {
    free_vector_store(stores[i]);
  }
  free_dataset(query_dataset);
  for (int i = 0; i < num_queries; i++) {
    free_vector(query[i]);
  }

  return 0;
}
