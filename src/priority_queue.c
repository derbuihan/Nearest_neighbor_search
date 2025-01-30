#include "priority_queue.h"

#include <stdlib.h>

static PgNode *new_pg_node(void *value, float priority) {
  PgNode *node = malloc(sizeof(PgNode));
  node->value = value;
  node->priority = priority;
  node->left = NULL;
  node->right = NULL;
  return node;
}

static void free_pg_node(PgNode *node) {
  if (node) {
    free_pg_node(node->left);
    free_pg_node(node->right);
    free(node);
  }
}

PriorityQueue *new_priority_queue() {
  PriorityQueue *queue = malloc(sizeof(PriorityQueue));
  queue->root = NULL;
  queue->size = 0;
  return queue;
}

void free_priority_queue(PriorityQueue *queue) {
  free_pg_node(queue->root);
  free(queue);
}

static void push_node(PgNode **head, PgNode *node) {
  if (*head == NULL) {
    *head = node;
    return;
  }

  if ((*head)->priority < node->priority) {
    push_node(&(*head)->right, node);
  } else {
    push_node(&(*head)->left, node);
  }
}

void push_priority_queue(PriorityQueue *queue, void *value, float priority) {
  PgNode *node = new_pg_node(value, priority);
  push_node(&queue->root, node);
  queue->size++;
}

void pop_priority_queue(PriorityQueue *queue, void **result_value,
                        float *result_priority) {
  // If the queue is empty, return.
  if (queue->root == NULL) {
    return;
  }

  // Find the maximum priority node.
  PgNode *parent = NULL;
  PgNode *node = queue->root;
  while (node->right) {
    parent = node;
    node = node->right;
  }

  *result_value = node->value;
  *result_priority = node->priority;

  // Remove the maximum priority node.
  if (parent == NULL) {
    queue->root = node->left;
  } else {
    parent->right = node->left;
  }

  free(node);
  queue->size--;
}
