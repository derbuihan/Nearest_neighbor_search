#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

typedef struct PgNode PgNode;
struct PgNode {
  int id;
  float priority;
  PgNode *left;
  PgNode *right;
};

typedef struct PriorityQueue PriorityQueue;
struct PriorityQueue {
  PgNode *root;
  int size;
};

PriorityQueue *new_priority_queue();
void free_priority_queue(PriorityQueue *queue);
void push_priority_queue(PriorityQueue *queue, int id, float priority);
void pop_priority_queue(PriorityQueue *queue, int *result_id,
                        float *result_priority);

#endif  // PRIORITY_QUEUE_H
