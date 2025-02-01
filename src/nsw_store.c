#include "nsw_store.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "priority_queue.h"

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
  push_priority_queue(candidates, (void *)node,
                      dot_product_vector(v, node->vector));

  PriorityQueue *closest = new_priority_queue();
  int visited[store->num_vectors];
  for (int i = 0; i < store->num_vectors; i++) visited[i] = 0;
  visited[node->id] = 1;

  // Search for the nearest neighbors
  while (candidates->size > 0 && closest->size < store->ef_construction) {
    NSWNode *current_node;
    float current_priority;
    pop_priority_queue(candidates, (void **)&current_node, &current_priority);
    push_priority_queue(closest, current_node, current_priority);

    for (NSWEdge *edge = current_node->edges; edge; edge = edge->next) {
      NSWNode *neighbor_node = edge->node;
      if (visited[neighbor_node->id]) continue;
      float neighbor_priority = dot_product_vector(v, neighbor_node->vector);
      push_priority_queue(candidates, neighbor_node, neighbor_priority);
      visited[neighbor_node->id] = 1;
    }
  }

  // Add edges to the graph
  while (closest->size > 0 && root->num_edges < root->max_degree) {
    NSWNode *node;
    float priority;
    pop_priority_queue(closest, (void **)&node, &priority);

    connect_nsw_nodes(root, node);
    if (node->num_edges < node->max_degree) {
      connect_nsw_nodes(node, root);
    }
  }

  // Free memory
  free_priority_queue(candidates);
  free_priority_queue(closest);
}

void search_vectors_nsw_store(NSWStore *store, Vector *query, int top_k,
                              int *result_ids, float *result_dists) {
  // Add root node to priority queue
  PriorityQueue *candidates = new_priority_queue();
  NSWNode *root = store->root;
  push_priority_queue(candidates, (void *)root,
                      dot_product_vector(query, root->vector));

  PriorityQueue *closest = new_priority_queue();
  int visited[store->num_vectors];
  for (int i = 0; i < store->num_vectors; i++) visited[i] = 0;
  visited[root->id] = 1;

  // Search for the nearest neighbors
  while (candidates->size > 0 && closest->size < store->ef_search) {
    NSWNode *current_node;
    float current_priority;
    pop_priority_queue(candidates, (void **)&current_node, &current_priority);
    push_priority_queue(closest, current_node, current_priority);

    for (NSWEdge *edge = current_node->edges; edge; edge = edge->next) {
      NSWNode *neighbor_node = edge->node;
      if (visited[neighbor_node->id]) continue;
      float neighbor_priority =
          dot_product_vector(query, neighbor_node->vector);
      push_priority_queue(candidates, neighbor_node, neighbor_priority);
      visited[neighbor_node->id] = 1;
    }
  }

  // Add the closest nodes to the result
  for (int i = 0; i < top_k; i++) {
    NSWNode *node;
    float dist;
    pop_priority_queue(closest, (void **)&node, &dist);
    result_ids[i] = node->id;
    result_dists[i] = dist;
  }

  // Free memory
  free_priority_queue(candidates);
  free_priority_queue(closest);
}

void print_nsw_store(NSWStore *store) {
  for (NSWNode *node = store->root; node; node = node->next) {
    printf("Node: %d, Edge:", node->id);
    for (NSWEdge *edge = node->edges; edge; edge = edge->next) {
      printf(" %d", edge->node->id);
    }
    printf("\n");
  }
}

static void save_nsw_edge(NSWEdge *edge, FILE *fp) {
  fwrite(&edge->id, sizeof(int), 1, fp);
  fwrite(&edge->node->id, sizeof(int), 1, fp);
}

static void save_nsw_node(NSWNode *node, FILE *fp) {
  fwrite(&node->id, sizeof(int), 1, fp);
  save_vector(node->vector, fp);
  fwrite(&node->max_degree, sizeof(int), 1, fp);
  fwrite(&node->num_edges, sizeof(int), 1, fp);

  for (NSWEdge *edge = node->edges; edge; edge = edge->next) {
    save_nsw_edge(edge, fp);
  }
}

void save_nsw_store(NSWStore *store, FILE *fp) {
  fwrite(&store->max_degree, sizeof(int), 1, fp);
  fwrite(&store->ef_construction, sizeof(int), 1, fp);
  fwrite(&store->ef_search, sizeof(int), 1, fp);
  fwrite(&store->num_vectors, sizeof(int), 1, fp);

  for (NSWNode *node = store->root; node; node = node->next) {
    save_nsw_node(node, fp);
  }
}

static NSWEdge *load_nsw_edge(FILE *fp) {
  NSWEdge *edge = malloc(sizeof(NSWEdge));
  fread(&edge->id, sizeof(int), 1, fp);
  int node_id;
  fread(&node_id, sizeof(int), 1, fp);
  edge->node = (NSWNode *)(intptr_t)node_id;
  edge->next = NULL;
  return edge;
}

static NSWNode *load_nsw_node(FILE *fp) {
  NSWNode *node = malloc(sizeof(NSWNode));
  fread(&node->id, sizeof(int), 1, fp);
  node->vector = load_vector(fp);
  fread(&node->max_degree, sizeof(int), 1, fp);
  fread(&node->num_edges, sizeof(int), 1, fp);

  NSWEdge *prev = NULL;
  for (int i = 0; i < node->num_edges; i++) {
    NSWEdge *edge = load_nsw_edge(fp);
    if (prev == NULL) {
      node->edges = edge;
    } else {
      prev->next = edge;
    }
    prev = edge;
  }

  return node;
}

NSWStore *load_nsw_store(FILE *fp) {
  NSWStore *store = malloc(sizeof(NSWStore));
  fread(&store->max_degree, sizeof(int), 1, fp);
  fread(&store->ef_construction, sizeof(int), 1, fp);
  fread(&store->ef_search, sizeof(int), 1, fp);
  fread(&store->num_vectors, sizeof(int), 1, fp);

  NSWNode *node_ptrs[store->num_vectors];
  NSWNode *prev = NULL;
  for (int i = 0; i < store->num_vectors; i++) {
    NSWNode *node = load_nsw_node(fp);

    if (prev == NULL) {
      store->root = node;
    } else {
      prev->next = node;
    }
    prev = node;

    node_ptrs[node->id] = node;
  }

  for (NSWNode *node = store->root; node; node = node->next) {
    for (NSWEdge *edge = node->edges; edge; edge = edge->next) {
      edge->node = node_ptrs[(intptr_t)edge->node];
    }
  }

  return store;
}
