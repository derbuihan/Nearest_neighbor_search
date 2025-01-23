#include "load_datasets.h"

#include <arrow/api.h>
#include <parquet/arrow/reader.h>

extern "C" {
Dataset *load_dataset(char *filename) {
  Dataset *dataset = (Dataset *)malloc(sizeof(Dataset));

  auto reader = parquet::ParquetFileReader::OpenFile(filename);
  auto file_metadata = reader->metadata();

  dataset->num_records = file_metadata->num_rows();
  dataset->records = (Record *)malloc(sizeof(Record) * dataset->num_records);

  for (int i = 0; i < dataset->num_records; i++) {
    dataset->records[i].url = NULL;
    dataset->records[i].content = NULL;
    dataset->records[i].embedding = NULL;
    dataset->records[i].embedding_size = 0;
  }

  return dataset;
}

void free_dataset(Dataset *dataset) {
  if (!dataset) return;
  if (dataset->records) {
    for (int i = 0; i < dataset->num_records; i++) {
      free(dataset->records[i].url);
      free(dataset->records[i].content);
      free(dataset->records[i].embedding);
    }
    free(dataset->records);
  }
  free(dataset);
}
}
