#include "vector_store.h"

#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

Node *new_node(int id, Vector *vector) {
  Node *n = malloc(sizeof(Node));
  n->id = id;
  n->vector = vector;
  n->next = NULL;
  return n;
}

void free_node(Node *node) {
  free_vector(node->vector);
  free(node);
}

VectorStore *new_vector_store() {
  VectorStore *store = malloc(sizeof(VectorStore));
  store->head = NULL;
  store->num_vectors = 0;
  return store;
}

void free_vector_store(VectorStore *store) {
  Node *node = store->head;
  while (node) {
    Node *next = node->next;
    free_node(node);
    node = next;
  }
  free(store);
}

void add_vector(VectorStore *store, Vector *v) {
  Node *node = new_node(++store->num_vectors, v);
  node->next = store->head;
  store->head = node;
}

void print_vector_store(VectorStore *store) {
  for (Node *node = store->head; node; node = node->next) {
    printf("Vector %d: ", node->id);
    print_vector(node->vector);
  }
}

void search_nearest_vector(VectorStore *store, Vector *query, int k,
                           int *result_ids, float *result_dists) {
  if (store->num_vectors < k) {
    printf("Error: not enough vectors in the store\n");
    exit(1);
  }

  int size = store->num_vectors;
  int *ids = malloc(size * sizeof(int));
  float *dists = malloc(size * sizeof(float));

  for (Node *node = store->head; node; node = node->next) {
    Vector *vector = node->vector;
    ids[node->id - 1] = node->id;
    dists[node->id - 1] = dot_product_vector(query, vector);
  }

  for (int i = 0; i < k; i++) {
    result_ids[i] = -1;
    result_dists[i] = -1;
  }

  for (int i = 0; i < size; i++) {
    for (int j = 0; j < k; j++) {
      if (dists[i] > result_dists[j]) {
        for (int l = k - 1; l > j; l--) {
          result_ids[l] = result_ids[l - 1];
          result_dists[l] = result_dists[l - 1];
        }
        result_ids[j] = ids[i];
        result_dists[j] = dists[i];
        break;
      }
    }
  }

  free(ids);
  free(dists);
}