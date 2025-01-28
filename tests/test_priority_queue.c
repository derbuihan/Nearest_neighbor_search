#include <assert.h>
#include <stdio.h>

#include "priority_queue.h"

/*
PriorityQueue *new_priority_queue();
void free_priority_queue(PriorityQueue *queue);
void push_priority_queue(PriorityQueue *queue, int id, float priority);
void pop_priority_queue(PriorityQueue *queue, int *result_id,
float *result_priority);
*/

void test_new_priority_queue() {
  PriorityQueue *queue = new_priority_queue();
  assert(queue != NULL);
  assert(queue->size == 0);
  free_priority_queue(queue);
}

void test_push_priority_queue() {
  PriorityQueue *queue = new_priority_queue();

  push_priority_queue(queue, 1, 3.0f);
  push_priority_queue(queue, 2, 1.0f);
  push_priority_queue(queue, 3, 2.0f);

  assert(queue->size == 3);
  free_priority_queue(queue);
}

void test_pop_priority_queue() {
  PriorityQueue *queue = new_priority_queue();

  push_priority_queue(queue, 1, 3.0f);
  push_priority_queue(queue, 2, 1.0f);
  push_priority_queue(queue, 3, 2.0f);

  int id;
  float priority;
  pop_priority_queue(queue, &id, &priority);
  assert(id == 1);
  assert(priority == 3.0f);

  pop_priority_queue(queue, &id, &priority);
  assert(id == 3);
  assert(priority == 2.0f);

  pop_priority_queue(queue, &id, &priority);
  assert(id == 2);
  assert(priority == 1.0f);

  free_priority_queue(queue);
}

int main() {
  test_new_priority_queue();
  test_push_priority_queue();
  test_pop_priority_queue();
  printf("All priority queue tests passed!\n");
  return 0;
}
