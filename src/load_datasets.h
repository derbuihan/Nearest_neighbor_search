#ifndef LOAD_DATASETS_H
#define LOAD_DATASETS_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Record Record;
struct Record {
  char *url;
  char *content;
  float *embedding;
  int embedding_size;
};

typedef struct Dataset Dataset;
struct Dataset {
  Record *records;
  int num_records;
};

Dataset *load_dataset(char *filename);
void free_dataset(Dataset *dataset);

#ifdef __cplusplus
}
#endif

#endif  // LOAD_DATASETS_H
