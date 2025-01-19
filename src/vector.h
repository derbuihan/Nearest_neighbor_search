#ifndef VECTOR_H
#define VECTOR_H

typedef struct vector vector;

struct vector {
  int size;
  float *data;
};

vector *new_vector(int size);
void free_vector(vector *v);
float dot_product_vector(vector *v1, vector *v2);
float length_vector(vector *v);
void set_random_vector(vector *v);
void print_vector(vector *v);

#endif  // VECTOR_H
