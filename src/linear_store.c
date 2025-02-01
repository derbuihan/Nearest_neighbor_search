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

bool is_equal_linear_store(LinearStore *store1, LinearStore *store2) {
  if (store1->num_vectors != store2->num_vectors) return false;
  for (Node *node1 = store1->head, *node2 = store2->head; node1 || node2;
       node1 = node1->next, node2 = node2->next) {
    if (node1->id != node2->id) return false;
    if (!is_equal_vector(node1->vector, node2->vector)) return false;
  }
  return true;
}

void print_linear_store(LinearStore *store) {
  for (Node *node = store->head; node; node = node->next) {
    printf("Vector %d: ", node->id);
    print_vector(node->vector);
  }
}

static void save_node(Node *node, FILE *fp) {
  fwrite(&node->id, sizeof(int), 1, fp);
  save_vector(node->vector, fp);
}

void save_linear_store(LinearStore *store, FILE *fp) {
  fwrite(&store->num_vectors, sizeof(int), 1, fp);
  for (Node *node = store->head; node; node = node->next) {
    save_node(node, fp);
  }
}

static Node *load_node(FILE *fp) {
  Node *node = malloc(sizeof(Node));
  fread(&node->id, sizeof(int), 1, fp);
  node->vector = load_vector(fp);
  node->next = NULL;
  return node;
}

LinearStore *load_linear_store(FILE *fp) {
  LinearStore *store = new_linear_store();
  fread(&store->num_vectors, sizeof(int), 1, fp);

  Node *prev = NULL;
  for (int i = 0; i < store->num_vectors; i++) {
    Node *node = load_node(fp);

    if (prev == NULL) {
      store->head = node;
    } else {
      prev->next = node;
    }
    prev = node;
  }

  return store;
}
