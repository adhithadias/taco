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

int assemble(taco_tensor_t *A1459, taco_tensor_t *cage3, taco_tensor_t *A1392, taco_tensor_t *A1451, taco_tensor_t *A1455) {
  int A14592_dimension = (int)(A1459->dimensions[1]);
  double* restrict A1459_vals = (double*)(A1459->vals);

  A1459_vals = (double*)malloc(sizeof(double) * (5 * A14592_dimension));

  A1459->vals = (uint8_t*)A1459_vals;
  return 0;
}

int compute(taco_tensor_t *A1459, taco_tensor_t *B, taco_tensor_t *A1392, taco_tensor_t *A1451, taco_tensor_t *A1455) {
  int A14591_dimension = (int)(A1459->dimensions[0]);
  int A14592_dimension = (int)(A1459->dimensions[1]);
  double* restrict A1459_vals = (double*)(A1459->vals);
  int B1_dimension = (int)(B->dimensions[0]);
  int* restrict B2_pos = (int*)(B->indices[1][0]);
  int* restrict B2_crd = (int*)(B->indices[1][1]);
  double* restrict B_vals = (double*)(B->vals);
  int A13921_dimension = (int)(A1392->dimensions[0]);
  int A13922_dimension = (int)(A1392->dimensions[1]);
  double* restrict A1392_vals = (double*)(A1392->vals);
  int A14511_dimension = (int)(A1451->dimensions[0]);
  int A14512_dimension = (int)(A1451->dimensions[1]);
  double* restrict A1451_vals = (double*)(A1451->vals);
  int A14551_dimension = (int)(A1455->dimensions[0]);
  int A14552_dimension = (int)(A1455->dimensions[1]);
  double* restrict A1455_vals = (double*)(A1455->vals);

  #pragma omp parallel for schedule(static)
  for (int32_t pA1459 = 0; pA1459 < (A14591_dimension * A14592_dimension); pA1459++) {
    A1459_vals[pA1459] = 0.0;
  }
  

  #pragma omp parallel for schedule(runtime)
  for (int32_t i0 = 0; i0 < ((A13921_dimension + 15) / 16); i0++) {

    for (int32_t i1 = 0; i1 < 16; i1++) {
      int32_t i1467 = i0 * 16 + i1;
      if (i1467 >= A13921_dimension)
        continue;

      for (int32_t i1468B = B2_pos[i1467]; i1468B < B2_pos[(i1467 + 1)]; i1468B++) {
        int32_t i1468 = B2_crd[i1468B];
        double tA1459_val = 0.0;
        for (int32_t i1469 = 0; i1469 < A14512_dimension; i1469++) {
          int32_t i1469A1392 = i1467 * A13922_dimension + i1469;
          int32_t i1469A1451 = i1468 * A14512_dimension + i1469;
          tA1459_val += (B_vals[i1468B] * A1392_vals[i1469A1392]) * A1451_vals[i1469A1451];
        }
        for (int32_t i1470 = 0; i1470 < A14552_dimension; i1470++) {
          int32_t i1470A1459 = i1467 * A14592_dimension + i1470;
          int32_t i1470A1455 = i1468 * A14552_dimension + i1470;
          A1459_vals[i1470A1459] = A1459_vals[i1470A1459] + tA1459_val * A1455_vals[i1470A1455];
        }
      }
    }
  }
  return 0;
}
#include "/home/min/a/kadhitha/workspace/my_taco/taco/test/kernels/sddmm_spmm/fused_kernel.h"
int _shim_assemble(void** parameterPack) {
  return assemble((taco_tensor_t*)(parameterPack[0]), (taco_tensor_t*)(parameterPack[1]), (taco_tensor_t*)(parameterPack[2]), (taco_tensor_t*)(parameterPack[3]), (taco_tensor_t*)(parameterPack[4]));
}
int _shim_compute(void** parameterPack) {
  return compute((taco_tensor_t*)(parameterPack[0]), (taco_tensor_t*)(parameterPack[1]), (taco_tensor_t*)(parameterPack[2]), (taco_tensor_t*)(parameterPack[3]), (taco_tensor_t*)(parameterPack[4]));
}
