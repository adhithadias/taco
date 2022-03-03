#ifndef TACO_C_HEADERS
#define TACO_C_HEADERS
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <complex.h>
#include <string.h>
#include <omp.h>
#if _OPENMP
#include <omp.h>
#endif
#define TACO_MIN(_a,_b) ((_a) < (_b) ? (_a) : (_b))
#define TACO_MAX(_a,_b) ((_a) > (_b) ? (_a) : (_b))
#define TACO_DEREF(_a) (((___context___*)(*__ctx__))->_a)
#ifndef TACO_TENSOR_T_DEFINED
#define TACO_TENSOR_T_DEFINED
typedef enum { taco_mode_dense, taco_mode_sparse } taco_mode_t;
typedef struct {
  int32_t      order;         // tensor order (number of modes)
  int32_t*     dimensions;    // tensor dimensions
  int32_t      csize;         // component size
  int32_t*     mode_ordering; // mode storage ordering
  taco_mode_t* mode_types;    // mode storage types
  uint8_t***   indices;       // tensor index data (per mode)
  uint8_t*     vals;          // tensor values
  int32_t      vals_size;     // values array size
} taco_tensor_t;
#endif
#if !_OPENMP
int omp_get_thread_num() { return 0; }
int omp_get_max_threads() { return 1; }
#endif
int cmp(const void *a, const void *b) {
  return *((const int*)a) - *((const int*)b);
}
int taco_binarySearchAfter(int *array, int arrayStart, int arrayEnd, int target) {
  if (array[arrayStart] >= target) {
    return arrayStart;
  }
  int lowerBound = arrayStart; // always < target
  int upperBound = arrayEnd; // always >= target
  while (upperBound - lowerBound > 1) {
    int mid = (upperBound + lowerBound) / 2;
    int midValue = array[mid];
    if (midValue < target) {
      lowerBound = mid;
    }
    else if (midValue > target) {
      upperBound = mid;
    }
    else {
      return mid;
    }
  }
  return upperBound;
}
int taco_binarySearchBefore(int *array, int arrayStart, int arrayEnd, int target) {
  if (array[arrayEnd] <= target) {
    return arrayEnd;
  }
  int lowerBound = arrayStart; // always <= target
  int upperBound = arrayEnd; // always > target
  while (upperBound - lowerBound > 1) {
    int mid = (upperBound + lowerBound) / 2;
    int midValue = array[mid];
    if (midValue < target) {
      lowerBound = mid;
    }
    else if (midValue > target) {
      upperBound = mid;
    }
    else {
      return mid;
    }
  }
  return lowerBound;
}
taco_tensor_t* init_taco_tensor_t(int32_t order, int32_t csize,
                                  int32_t* dimensions, int32_t* mode_ordering,
                                  taco_mode_t* mode_types) {
  taco_tensor_t* t = (taco_tensor_t *) malloc(sizeof(taco_tensor_t));
  t->order         = order;
  t->dimensions    = (int32_t *) malloc(order * sizeof(int32_t));
  t->mode_ordering = (int32_t *) malloc(order * sizeof(int32_t));
  t->mode_types    = (taco_mode_t *) malloc(order * sizeof(taco_mode_t));
  t->indices       = (uint8_t ***) malloc(order * sizeof(uint8_t***));
  t->csize         = csize;
  for (int32_t i = 0; i < order; i++) {
    t->dimensions[i]    = dimensions[i];
    t->mode_ordering[i] = mode_ordering[i];
    t->mode_types[i]    = mode_types[i];
    switch (t->mode_types[i]) {
      case taco_mode_dense:
        t->indices[i] = (uint8_t **) malloc(1 * sizeof(uint8_t **));
        break;
      case taco_mode_sparse:
        t->indices[i] = (uint8_t **) malloc(2 * sizeof(uint8_t **));
        break;
    }
  }
  return t;
}
void deinit_taco_tensor_t(taco_tensor_t* t) {
  for (int i = 0; i < t->order; i++) {
    free(t->indices[i]);
  }
  free(t->indices);
  free(t->dimensions);
  free(t->mode_ordering);
  free(t->mode_types);
  free(t);
}
#endif

int assemble(taco_tensor_t *A, taco_tensor_t *C, taco_tensor_t *v, taco_tensor_t *B) {
  double* restrict A_vals = (double*)(A->vals);

  A_vals = (double*)malloc(sizeof(double) * 5);

  A->vals = (uint8_t*)A_vals;
  return 0;
}

int compute(taco_tensor_t *A, taco_tensor_t *C, taco_tensor_t *v, taco_tensor_t *B) {
  printf("Adhitha1\n");

  double* restrict A_vals = (double*)(A->vals);
  int* restrict C2_pos = (int*)(C->indices[1][0]);
  int* restrict C2_crd = (int*)(C->indices[1][1]);
  double* restrict C_vals = (double*)(C->vals);
  double* restrict v_vals = (double*)(v->vals);
  printf("Adhitha2\n");
  int B1_dimension = (int)(B->dimensions[0]);
  int C1_dimension = (int)(B->dimensions[0]);
  printf("Adhitha3 %d, %d\n", B1_dimension, C1_dimension);
  int* restrict B2_pos = (int*)(B->indices[1][0]);
  printf("Adhitha4\n");
  int* restrict B2_crd = (int*)(B->indices[1][1]);
  printf("Adhitha2\n");
  double* restrict B_vals = (double*)(B->vals);

  printf("Adhitha3\n");

  double* restrict tA = 0;
  tA = (double*)malloc(sizeof(double) * C1_dimension);
  for (int32_t ptA = 0; ptA < C1_dimension; ptA++) {
    tA[ptA] = 0.0;
  }
  for (int32_t i1439 = 0; i1439 < C1_dimension; i1439++) {
    double ti1440tA_val = 0.0;
    for (int32_t i1440C = C2_pos[i1439]; i1440C < C2_pos[(i1439 + 1)]; i1440C++) {
      int32_t i1440 = C2_crd[i1440C];
      ti1440tA_val += C_vals[i1440C] * v_vals[i1440];
    }
    tA[i1439] = ti1440tA_val;
  }
  for (int32_t i1438 = 0; i1438 < B1_dimension; i1438++) {
    double ti1439A_val = 0.0;
    for (int32_t i1439B = B2_pos[i1438]; i1439B < B2_pos[(i1438 + 1)]; i1439B++) {
      int32_t i1439 = B2_crd[i1439B];
      ti1439A_val += B_vals[i1439B] * tA[i1439];
    }
    A_vals[i1438] = ti1439A_val;
  }
  free(tA);

  A->vals = (uint8_t*)A_vals;
  return 0;
}
#include "/home/min/a/kadhitha/workspace/my_taco/taco/test/kernels/spmv_spmv/spmv_fused.h"
int _shim_assemble(void** parameterPack) {
  return assemble((taco_tensor_t*)(parameterPack[0]), (taco_tensor_t*)(parameterPack[1]), (taco_tensor_t*)(parameterPack[2]), (taco_tensor_t*)(parameterPack[3]));
}
int _shim_compute(void** parameterPack) {
  return compute((taco_tensor_t*)(parameterPack[0]), (taco_tensor_t*)(parameterPack[1]), (taco_tensor_t*)(parameterPack[2]), (taco_tensor_t*)(parameterPack[3]));
}
