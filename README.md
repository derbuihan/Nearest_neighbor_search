# Nearest Neighbor Search

A C implementation of nearest neighbor search algorithms for multidimensional point data.

## Algorithms

- [x] Linear Search
- [x] Navigable Small World Graph (NSW)
- [ ] Hierarchical Navigable Small World Graph (HNSW)

## Installation

### Prerequisites

- C compiler (GCC, Clang, etc.)
- CMake (version 3.0 or higher)

### Build Steps

```bash
mkdir build
cd build
cmake ..
make
```

## Usage

```C
#include <stdio.h>

#include "vector.h"
#include "vector_store.h"

int main(void) {
  int dimensions = 1536;
  int size = 1000;

  // Create a vector store
  VectorStore *store = new_vector_store(STORE_LINEAR);

  // Add random vectors to the store
  for (int i = 0; i < size; i++) {
    Vector *v = new_vector(dimensions);
    set_random_vector(v);
    add_vector(store, v);
  }

  // Create a random query vector
  Vector *query = new_vector(dimensions);
  set_random_vector(query);

  // Search for the nearest neighbors
  int top_k = 10;
  int result_ids[top_k];
  int result_distances[top_k];
  search_vectors(store, query, top_k, result_ids, result_distances);

  // Print the results
  for (int i = 0; i < top_k; i++) {
    printf("ID: %d, Distance: %d\n", result_ids[i], result_distances[i]);
  }

  // Free the memory
  free_vector(query);
  free_vector_store(store);

  return 0;
}
```

## Performance


## License

This project is released under the MIT License.
