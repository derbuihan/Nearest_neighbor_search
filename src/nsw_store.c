#include "nsw_store.h"

#include <stdio.h>
#include <stdlib.h>

#include "priority_queue.h"

/*
typedef struct NSWNode NSWNode;
typedef struct NSWEdge NSWEdge;

struct NSWEdge {
  int id;
  NSWNode *node;
  NSWEdge *next;
};

struct NSWNode {
  int id;
  Vector *vector;
  NSWEdge *edges;
  int num_edges;
  int max_degree;
  NSWNode *next;
};

typedef struct NSWStore NSWStore;
struct NSWStore {
  NSWNode *root;
  int num_vectors;
  int max_degree;
  int ef_construction;
  int ef_search;
};
*/

NSWStore *new_nsw_store(int max_degree, int ef_construction, int ef_search) {
  NSWStore *store = malloc(sizeof(NSWStore));
  store->root = NULL;
  store->num_vectors = 0;
  store->max_degree = max_degree;
  store->ef_construction = ef_construction;
  store->ef_search = ef_search;
  return store;
}

static void free_nsw_edge(NSWEdge *edge) {
  while (edge) {
    NSWEdge *next = edge->next;
    free(edge);
    edge = next;
  }
}

static void free_nsw_node(NSWNode *node) {
  free_vector(node->vector);
  free_nsw_edge(node->edges);
  free(node);
}

void free_nsw_store(NSWStore *store) {
  NSWNode *node = store->root;
  while (node) {
    NSWNode *next = node->next;
    free_nsw_node(node);
    node = next;
  }
}

static NSWEdge *new_nsw_edge(int id, NSWNode *node) {
  NSWEdge *edge = malloc(sizeof(NSWEdge));
  edge->id = id;
  edge->node = node;
  edge->next = NULL;
  return edge;
}

static void connect_nsw_nodes(NSWNode *src, NSWNode *dst) {
  NSWEdge *edge = new_nsw_edge(src->num_edges++, dst);
  edge->next = src->edges;
  src->edges = edge;
}

static NSWNode *new_nsw_node(int id, Vector *v, int max_degree) {
  NSWNode *node = malloc(sizeof(NSWNode));
  node->id = id;
  node->vector = v;
  node->edges = NULL;
  node->num_edges = 0;
  node->max_degree = max_degree;
  node->next = NULL;
  return node;
}

static NSWNode *get_nsw_node(NSWStore *store, int id) {
  NSWNode *node = store->root;
  while (node) {
    if (node->id == id) return node;
    node = node->next;
  }
  return NULL;
}

void add_vector_nsw_store(NSWStore *store, Vector *v) {
  NSWNode *root = new_nsw_node(store->num_vectors++, v, store->max_degree);
  root->next = store->root;
  store->root = root;
  if (store->num_vectors == 1) return;

  // Add root node to priority queue
  PriorityQueue *candidates = new_priority_queue();
  NSWNode *node = store->root->next;
  push_priority_queue(candidates, node->id,
                      dot_product_vector(v, node->vector));

  PriorityQueue *closest = new_priority_queue();
  int visited[store->num_vectors];
  for (int i = 0; i < store->num_vectors; i++) visited[i] = 0;

  // Search for the nearest neighbors
  while (candidates->size > 0 && closest->size < store->max_degree) {
    int current_id;
    float current_priority;
    pop_priority_queue(candidates, &current_id, &current_priority);
    push_priority_queue(closest, current_id, current_priority);
    visited[current_id] = 1;

    NSWNode *current_node = get_nsw_node(store, current_id);
    for (NSWEdge *edge = current_node->edges; edge; edge = edge->next) {
      NSWNode *neighbor = edge->node;
      int neighbor_id = neighbor->id;
      float neighbor_priority = dot_product_vector(v, neighbor->vector);
      if (visited[neighbor_id]) continue;
      push_priority_queue(candidates, neighbor_id, neighbor_priority);
    }
  }

  // Add edges to the graph
  while (closest->size > 0 && root->num_edges < root->max_degree) {
    int id;
    float priority;
    pop_priority_queue(closest, &id, &priority);
    NSWNode *neighbor = get_nsw_node(store, id);

    connect_nsw_nodes(root, neighbor);
    if (neighbor->num_edges < neighbor->max_degree) {
      connect_nsw_nodes(neighbor, root);
    }
  }
}

void search_vectors_nsw_store(NSWStore *store, Vector *query, int top_k,
                              int *result_ids, float *result_dists);

void print_nsw_store(NSWStore *store) {
  for (NSWNode *node = store->root; node; node = node->next) {
    printf("Node: %d, Edge:", node->id);
    for (NSWEdge *edge = node->edges; edge; edge = edge->next) {
      printf(" %d", edge->node->id);
    }
    printf("\n");
  }
}
