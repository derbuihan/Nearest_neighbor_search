#include "vector_store.h"

#include <stdio.h>
#include <stdlib.h>

#include "priority_queue.h"

Node *new_node(int id, Vector *v) {
  Node *n = malloc(sizeof(Node));
  n->id = id;
  n->vector = v;
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

  PriorityQueue *queue = new_priority_queue();

  for (Node *node = store->head; node; node = node->next) {
    Vector *vector = node->vector;
    int id = node->id;
    float dist = dot_product_vector(query, vector);
    push_priority_queue(queue, id, dist);
  }

  for (int i = 0; i < k; i++) {
    int id;
    float dist;
    pop_priority_queue(queue, &id, &dist);
    result_ids[i] = id;
    result_dists[i] = dist;
  }

  free_priority_queue(queue);
}
