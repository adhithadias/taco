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

int assemble(taco_tensor_t *A2531, taco_tensor_t *cage3, taco_tensor_t *A1392, taco_tensor_t *A1451) {
  int* restrict A25312_pos = (int*)(A2531->indices[1][0]);
  int* restrict A25312_crd = (int*)(A2531->indices[1][1]);
  double* restrict A2531_vals = (double*)(A2531->vals);
  int* restrict cage32_pos = (int*)(cage3->indices[1][0]);
  int* restrict cage32_crd = (int*)(cage3->indices[1][1]);
  int A13921_dimension = (int)(A1392->dimensions[0]);

  A25312_pos = (int32_t*)malloc(sizeof(int32_t) * 6);
  A25312_pos[0] = 0;
  for (int32_t pA25312 = 1; pA25312 < 6; pA25312++) {
    A25312_pos[pA25312] = 0;
  }
  int32_t A25312_crd_size = 1048576;
  A25312_crd = (int32_t*)malloc(sizeof(int32_t) * A25312_crd_size);
  int32_t i1468A2531 = 0;

  for (int32_t i1467 = 0; i1467 < A13921_dimension; i1467++) {
    int32_t pA25312_begin = i1468A2531;

    for (int32_t i1468cage3 = cage32_pos[i1467]; i1468cage3 < cage32_pos[(i1467 + 1)]; i1468cage3++) {
      int32_t i1468 = cage32_crd[i1468cage3];
      if (A25312_crd_size <= i1468A2531) {
        A25312_crd = (int32_t*)realloc(A25312_crd, sizeof(int32_t) * (A25312_crd_size * 2));
        A25312_crd_size *= 2;
      }
      A25312_crd[i1468A2531] = i1468;
      i1468A2531++;
    }

    A25312_pos[i1467 + 1] = i1468A2531 - pA25312_begin;
  }

  int32_t csA25312 = 0;
  for (int32_t pA253120 = 1; pA253120 < 6; pA253120++) {
    csA25312 += A25312_pos[pA253120];
    A25312_pos[pA253120] = csA25312;
  }

  A2531_vals = (double*)malloc(sizeof(double) * i1468A2531);

  A2531->indices[1][0] = (uint8_t*)(A25312_pos);
  A2531->indices[1][1] = (uint8_t*)(A25312_crd);
  A2531->vals = (uint8_t*)A2531_vals;
  return 0;
}

int compute(taco_tensor_t *A, taco_tensor_t *B, taco_tensor_t *C, taco_tensor_t *D) {

  int A1_dimension = (int)(A->dimensions[0]);
  double* restrict A_vals = (double*)(A->vals);
  int B1_dimension = (int)(B->dimensions[0]);
  int* restrict B2_pos = (int*)(B->indices[1][0]);
  int* restrict B2_crd = (int*)(B->indices[1][1]);
  double* restrict B_vals = (double*)(B->vals);
  int C1_dimension = (int)(C->dimensions[0]);
  int C2_dimension = (int)(C->dimensions[1]);
  double* restrict C_vals = (double*)(C->vals);
  int D1_dimension = (int)(D->dimensions[0]);
  int D2_dimension = (int)(D->dimensions[1]);
  double* restrict D_vals = (double*)(D->vals);

  int32_t jA = 0;

  #pragma omp parallel for schedule(runtime)
  for (int32_t i0 = 0; i0 < ((C1_dimension + 15) / 16); i0++) {
    for (int32_t i1 = 0; i1 < 16; i1++) {
      int32_t i = i0 * 16 + i1;
      if (i >= C1_dimension)
        continue;

      for (int32_t jB = B2_pos[i]; jB < B2_pos[(i + 1)]; jB++) {
        int32_t j = B2_crd[jB];
        double tkA_val = 0.0;
        for (int32_t k = 0; k < D2_dimension; k++) {
          int32_t kC = i * C2_dimension + k;
          int32_t kD = j * D2_dimension + k;
          tkA_val += (B_vals[jB] * C_vals[kC]) * D_vals[kD];
        }
        A_vals[jB] = tkA_val;
        // jA++;
      }
    }
  }
  return 0;

}
#include "/home/min/a/kadhitha/workspace/my_taco/taco/test/kernels/sddmm_spmm/sddmm_ryan.h"
int _shim_assemble(void** parameterPack) {
  return assemble((taco_tensor_t*)(parameterPack[0]), (taco_tensor_t*)(parameterPack[1]), (taco_tensor_t*)(parameterPack[2]), (taco_tensor_t*)(parameterPack[3]));
}
int _shim_compute(void** parameterPack) {
  return compute((taco_tensor_t*)(parameterPack[0]), (taco_tensor_t*)(parameterPack[1]), (taco_tensor_t*)(parameterPack[2]), (taco_tensor_t*)(parameterPack[3]));
}
