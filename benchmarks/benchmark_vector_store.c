#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#include "vector_store.h"

Vector **create_vectors(int num_vectors, int num_dimensions) {
  Vector **vectors = malloc(num_vectors * sizeof(Vector *));
  for (int i = 0; i < num_vectors; i++) {
    vectors[i] = new_vector(num_dimensions);
    set_random_vector(vectors[i]);
  }
  return vectors;
}

VectorStore *create_vector_store(StoreType type, int num_vectors,
                                 Vector **vectors) {
  VectorStore *store = new_vector_store(type);
  for (int i = 0; i < num_vectors; i++) add_vector(store, vectors[i]);
  return store;
}

void benchmark_vector_store(VectorStore *store, Vector **queries,
                            int num_queries, int top_k) {
  struct timeval start, end;
  clock_t start_t, end_t;
  double wall_time, cpu_time;

  gettimeofday(&start, NULL);
  start_t = clock();

  for (int i = 0; i < num_queries; i++) {
    int result_ids[top_k];
    float result_dists[top_k];
    search_vectors(store, queries[i], top_k, result_ids, result_dists);
    printf("Result: (%d, %.3f) (%d, %.3f) (%d, %.3f)\n", result_ids[0],
           result_dists[0], result_ids[1], result_dists[1], result_ids[2],
           result_dists[2]);
  }

  end_t = clock();
  gettimeofday(&end, NULL);

  wall_time = (double)(end.tv_sec - start.tv_sec) +
              (double)(end.tv_usec - start.tv_usec) / 1000000;
  cpu_time = (double)(end_t - start_t) / CLOCKS_PER_SEC;
  printf("Total queries: %d\n", num_queries);
  printf("Wall time: %.3f seconds (%.3f ms per query)\n", wall_time,
         wall_time / num_queries * 1000);
  printf("CPU time: %.3f seconds (%.3f ms per query)\n", cpu_time,
         cpu_time / num_queries * 1000);
}

int main() {
  int num_vectors = 16384;
  int num_dimensions = 1536;
  int num_queries = 3;
  int top_k = 3;

  Vector **queries = create_vectors(num_queries, num_dimensions);
  Vector **vectors = create_vectors(num_vectors, num_dimensions);

  VectorStore *linear_store =
      create_vector_store(STORE_LINEAR, num_vectors, vectors);
  benchmark_vector_store(linear_store, queries, num_queries, top_k);

  VectorStore *nsw_store = create_vector_store(STORE_NSW, num_vectors, vectors);
  benchmark_vector_store(nsw_store, queries, num_queries, top_k);

  return 0;
}
