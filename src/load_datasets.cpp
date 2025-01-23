#include "load_datasets.h"

#include <arrow/api.h>
#include <parquet/arrow/reader.h>

#include <iostream>

extern "C" {
void read_parquet(char *filename) {
  auto reader = parquet::ParquetFileReader::OpenFile(filename);

  std::shared_ptr<parquet::FileMetaData> file_metadata = reader->metadata();

  // 基本情報の出力
  printf("Number of row groups: %d\n", file_metadata->num_row_groups());
  printf("Number of columns: %d\n", file_metadata->num_columns());
  printf("Number of rows: %d\n", file_metadata->num_rows());
}
}
