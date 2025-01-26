#include "load_datasets.h"

#include <arrow/api.h>
#include <arrow/io/file.h>
#include <parquet/arrow/reader.h>

extern "C" {

Dataset *load_dataset(char *filename) {
  try {
    auto maybe_infile = arrow::io::ReadableFile::Open(filename);
    if (!maybe_infile.ok()) return nullptr;
    std::shared_ptr<arrow::io::RandomAccessFile> infile = *maybe_infile;

    auto maybe_reader =
        parquet::arrow::OpenFile(infile, arrow::default_memory_pool());
    if (!maybe_reader.ok()) return nullptr;
    std::unique_ptr<parquet::arrow::FileReader> reader =
        std::move(*maybe_reader);

    std::shared_ptr<arrow::Table> table;
    auto status = reader->ReadTable(&table);
    if (!status.ok()) return nullptr;

    Dataset *dataset = (Dataset *)malloc(sizeof(Dataset));
    if (!dataset) return nullptr;
    dataset->num_records = table->num_rows();
    dataset->records = (Record *)malloc(sizeof(Record) * dataset->num_records);
    if (!dataset->records) {
      free(dataset);
      return nullptr;
    }

    auto url_array = std::static_pointer_cast<arrow::StringArray>(
        table->column(0)->chunk(0));
    auto content_array = std::static_pointer_cast<arrow::StringArray>(
        table->column(1)->chunk(0));

    // embedding カラムを ListArray として取得
    auto embedding_list_array =
        std::static_pointer_cast<arrow::ListArray>(table->column(2)->chunk(0));
    auto embedding_values = std::static_pointer_cast<arrow::FloatArray>(
        embedding_list_array->values());

    for (int i = 0; i < dataset->num_records; i++) {
      dataset->records[i].url = strdup(url_array->GetString(i).c_str());
      dataset->records[i].content = strdup(content_array->GetString(i).c_str());

      // 各行の embedding のサイズと値を取得
      dataset->records[i].embedding_size =
          embedding_list_array->value_length(i);
      dataset->records[i].embedding =
          (float *)malloc(sizeof(float) * dataset->records[i].embedding_size);

      // embedding データのコピー
      auto embedding_offset = embedding_list_array->value_offset(i);
      memcpy(dataset->records[i].embedding,
             embedding_values->raw_values() + embedding_offset,
             sizeof(float) * dataset->records[i].embedding_size);
    }

    return dataset;
  } catch (const std::exception &e) {
    return nullptr;
  }
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
