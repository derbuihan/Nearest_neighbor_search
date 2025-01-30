#include "linear_store.h"

#include <stdio.h>
#include <stdlib.h>

#include "priority_queue.h"

static Node *new_node(int id, Vector *v) {
  Node *n = malloc(sizeof(Node));
  n->id = id;
  n->vector = v;
  n->next = NULL;
  return n;
}

static void free_node(Node *node) {
  free_vector(node->vector);
  free(node);
}

LinearStore *new_linear_store() {
  LinearStore *store = malloc(sizeof(LinearStore));
  store->head = NULL;
  store->num_vectors = 0;
  return store;
}

void free_linear_store(LinearStore *store) {
  Node *node = store->head;
  while (node) {
    Node *next = node->next;
    free_node(node);
    node = next;
  }
  free(store);
}

void add_vector_linear_store(LinearStore *store, Vector *v) {
  Node *node = new_node(store->num_vectors++, v);
  node->next = store->head;
  store->head = node;
}

void search_vectors_linear_store(LinearStore *store, Vector *query, int top_k,
                                 int *result_ids, float *result_dists) {
  if (store->num_vectors < top_k) {
    printf("Error: not enough vectors in the store\n");
    exit(1);
  }

  PriorityQueue *queue = new_priority_queue();

  for (Node *node = store->head; node; node = node->next) {
    Vector *vector = node->vector;
    float dist = dot_product_vector(query, vector);
    push_priority_queue(queue, (void *)node, dist);
  }

  for (int i = 0; i < top_k; i++) {
    Node *node;
    float dist;
    pop_priority_queue(queue, (void *)&node, &dist);
    result_ids[i] = node->id;
    result_dists[i] = dist;
  }

  free_priority_queue(queue);
}

void print_linear_store(LinearStore *store) {
  for (Node *node = store->head; node; node = node->next) {
    printf("Vector %d: ", node->id);
    print_vector(node->vector);
  }
}
