#include <assert.h>
#include <stdio.h>

#include "priority_queue.h"

void test_new_priority_queue() {
  PriorityQueue *queue = new_priority_queue();
  assert(queue != NULL);
  assert(queue->size == 0);
  free_priority_queue(queue);
}

void test_push_priority_queue() {
  PriorityQueue *queue = new_priority_queue();
  int x = 1, y = 2, z = 3;
  push_priority_queue(queue, &x, 3.0f);
  push_priority_queue(queue, &y, 1.0f);
  push_priority_queue(queue, &z, 2.0f);

  assert(queue->size == 3);
  free_priority_queue(queue);
}

void test_pop_priority_queue() {
  PriorityQueue *queue = new_priority_queue();

  int x = 1, y = 2, z = 3;
  push_priority_queue(queue, &x, 3.0f);
  push_priority_queue(queue, &y, 1.0f);
  push_priority_queue(queue, &z, 2.0f);

  int *id;
  float priority;
  pop_priority_queue(queue, (void **)&id, &priority);
  assert(*id == 1);
  assert(priority == 3.0f);

  pop_priority_queue(queue, (void **)&id, &priority);
  assert(*id == 3);
  assert(priority == 2.0f);

  pop_priority_queue(queue, (void **)&id, &priority);
  assert(*id == 2);
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
