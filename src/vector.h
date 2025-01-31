#ifndef VECTOR_H
#define VECTOR_H
#include <stdio.h>

typedef struct Vector Vector;

struct Vector {
  int size;
  float *data;
};

Vector *new_vector(int size);
void free_vector(Vector *v);
float dot_product_vector(Vector *v1, Vector *v2);
float length_vector(Vector *v);
void set_data_vector(Vector *v, float *data);
void set_random_vector(Vector *v);
void print_vector(Vector *v);
void save_vector(Vector *v, FILE *fp);
Vector *load_vector(FILE *fp);

#endif  // VECTOR_H
