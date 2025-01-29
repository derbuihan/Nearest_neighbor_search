#include <assert.h>
#include <stdio.h>

#include "load_datasets.h"

void test_load_dataset() {
  char *filename = "../datasets/query.parquet";
  Dataset *dataset = load_dataset(filename);
  assert(dataset != NULL);
  assert(dataset->num_records > 0);

  for (int i = 0; i < dataset->num_records; i++) {
    Record *record = &dataset->records[i];
    assert(record->url != NULL);
    assert(record->content != NULL);
    assert(record->embedding != NULL);
    assert(record->embedding_size > 0);
  }

  free_dataset(dataset);
}

int main() {
  test_load_dataset();

  printf("All load dataset tests passed!\n");
  return 0;
}
