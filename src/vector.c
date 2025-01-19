#include "vector.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

vector *new_vector(int size) {
  vector *v = malloc(sizeof(vector));
  v->size = size;
  v->data = malloc(size * sizeof(float));
  return v;
}

void free_vector(vector *v) {
  free(v->data);
  free(v);
}

float dot_product_vector(vector *v1, vector *v2) {
  if (v1->size != v2->size) {
    printf("Error: vectors must have the same size\n");
    exit(1);
  }

  float dot_product = 0;
  for (int i = 0; i < v1->size; i++) {
    dot_product += v1->data[i] * v2->data[i];
  }
  return dot_product;
}
float length_vector(vector *v) { return sqrtf(dot_product_vector(v, v)); }

static float rand_normal(float mean, float std) {
  float u1 = (float)rand() / (float)RAND_MAX;
  float u2 = (float)rand() / (float)RAND_MAX;
  float M_PI_F = 3.14159265358979323846f;
  float z = sqrtf(-2 * logf(u1)) * cosf(2 * M_PI_F * u2);
  return mean + z * std;
}

void set_random_vector(vector *v) {
  for (int i = 0; i < v->size; i++) {
    v->data[i] = rand_normal(0, 1);
  }

  float length = length_vector(v);
  for (int i = 0; i < v->size; i++) {
    v->data[i] /= length;
  }
}

void print_vector(vector *v) {
  for (int i = 0; i < v->size; i++) {
    printf("%f ", v->data[i]);
  }
  printf("\n");
}
