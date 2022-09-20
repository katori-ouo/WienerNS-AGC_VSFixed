/*
 *  Copyright (c) 2012 The InnoTALK project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */


/*
 * This header file includes all of the fix point signal processing library (SPL) function
 * descriptions and declarations.
 * For specific function calls, see bottom of file.
 */

#ifndef INNOTALK_SPL_SIGNAL_PROCESSING_LIBRARY_H_
#define INNOTALK_SPL_SIGNAL_PROCESSING_LIBRARY_H_

#include <string.h>
#include "audio_config.h"
typedef struct {
	int32_t F1A[4];
	int32_t F1B[4];
	int32_t F2A[4];
	int32_t F2B[4];
} CEVA_FILTER;
// Macros specific for the fixed point implementation
#define INNOTALK_SPL_WORD16_MAX       32767
#define INNOTALK_SPL_WORD16_MIN       -32768
#define INNOTALK_SPL_WORD32_MAX       (int32_t)0x7fffffff
#define INNOTALK_SPL_WORD32_MIN       (int32_t)0x80000000
#define INNOTALK_SPL_MAX_LPC_ORDER    14
#define INNOTALK_SPL_MAX_SEED_USED    0x80000000L
#define INNOTALK_SPL_MIN(A, B)        (A < B ? A : B)  // Get min value
#define INNOTALK_SPL_MAX(A, B)        (A > B ? A : B)  // Get max value
// TODO(kma/bjorn): For the next two macros, investigate how to correct the code
// for inputs of a = INNOTALK_SPL_WORD16_MIN or INNOTALK_SPL_WORD32_MIN.
#define INNOTALK_SPL_ABS_W16(a) \
    (((int16_t)a >= 0) ? ((int16_t)a) : -((int16_t)a))
#define INNOTALK_SPL_ABS_W32(a) \
    (((int32_t)a >= 0) ? ((int32_t)a) : -((int32_t)a))

#ifdef INNOTALK_ARCH_LITTLE_ENDIAN
#define INNOTALK_SPL_GET_BYTE(a, nr)  (((int8_t *)a)[nr])
#define INNOTALK_SPL_SET_BYTE(d_ptr, val, index) \
    (((int8_t *)d_ptr)[index] = (val))
#else
#define INNOTALK_SPL_GET_BYTE(a, nr) \
    ((((int16_t *)a)[nr >> 1]) >> (((nr + 1) & 0x1) * 8) & 0x00ff)
#define INNOTALK_SPL_SET_BYTE(d_ptr, val, index) \
    ((int16_t *)d_ptr)[index >> 1] = \
    ((((int16_t *)d_ptr)[index >> 1]) \
    & (0x00ff << (8 * ((index) & 0x1)))) | (val << (8 * ((index + 1) & 0x1)))
#endif

#define INNOTALK_SPL_MUL(a, b) \
    ((int32_t) ((int32_t)(a) * (int32_t)(b)))
#define INNOTALK_SPL_UMUL(a, b) \
    ((uint32_t) ((uint32_t)(a) * (uint32_t)(b)))
#define INNOTALK_SPL_UMUL_RSFT16(a, b) \
    ((uint32_t) ((uint32_t)(a) * (uint32_t)(b)) >> 16)
#define INNOTALK_SPL_UMUL_16_16(a, b) \
    ((uint32_t) (uint16_t)(a) * (uint16_t)(b))
#define INNOTALK_SPL_UMUL_16_16_RSFT16(a, b) \
    (((uint32_t) (uint16_t)(a) * (uint16_t)(b)) >> 16)
#define INNOTALK_SPL_UMUL_32_16(a, b) \
    ((uint32_t) ((uint32_t)(a) * (uint16_t)(b)))
#define INNOTALK_SPL_UMUL_32_16_RSFT16(a, b) \
    ((uint32_t) ((uint32_t)(a) * (uint16_t)(b)) >> 16)
#define INNOTALK_SPL_MUL_16_U16(a, b) \
    ((int32_t)(int16_t)(a) * (uint16_t)(b))
#define INNOTALK_SPL_DIV(a, b) \
    ((int32_t) ((int32_t)(a) / (int32_t)(b)))
#define INNOTALK_SPL_UDIV(a, b) \
    ((uint32_t) ((uint32_t)(a) / (uint32_t)(b)))

#ifndef INNOTALK_ARCH_ARM_V7
// For ARMv7 platforms, these are inline functions in spl_inl_armv7.h
#ifndef MIPS32_LE
// For MIPS platforms, these are inline functions in spl_inl_mips.h
#define INNOTALK_SPL_MUL_16_16(a, b) \
    ((int32_t) (((int16_t)(a)) * ((int16_t)(b))))
#define INNOTALK_SPL_MUL_16_32_RSFT16(a, b) \
    (INNOTALK_SPL_MUL_16_16(a, b >> 16) \
     + ((INNOTALK_SPL_MUL_16_16(a, (b & 0xffff) >> 1) + 0x4000) >> 15))
#define INNOTALK_SPL_MUL_32_32_RSFT32(a32a, a32b, b32) \
    ((int32_t)(INNOTALK_SPL_MUL_16_32_RSFT16(a32a, b32) \
    + (INNOTALK_SPL_MUL_16_32_RSFT16(a32b, b32) >> 16)))
#define INNOTALK_SPL_MUL_32_32_RSFT32BI(a32, b32) \
    ((int32_t)(INNOTALK_SPL_MUL_16_32_RSFT16(( \
    (int16_t)(a32 >> 16)), b32) + \
    (INNOTALK_SPL_MUL_16_32_RSFT16(( \
    (int16_t)((a32 & 0x0000FFFF) >> 1)), b32) >> 15)))
#endif
#endif

#define INNOTALK_SPL_MUL_16_32_RSFT11(a, b) \
    ((INNOTALK_SPL_MUL_16_16(a, (b) >> 16) << 5) \
    + (((INNOTALK_SPL_MUL_16_U16(a, (uint16_t)(b)) >> 1) + 0x0200) >> 10))
#define INNOTALK_SPL_MUL_16_32_RSFT14(a, b) \
    ((INNOTALK_SPL_MUL_16_16(a, (b) >> 16) << 2) \
    + (((INNOTALK_SPL_MUL_16_U16(a, (uint16_t)(b)) >> 1) + 0x1000) >> 13))
#define INNOTALK_SPL_MUL_16_32_RSFT15(a, b) \
    ((INNOTALK_SPL_MUL_16_16(a, (b) >> 16) << 1) \
    + (((INNOTALK_SPL_MUL_16_U16(a, (uint16_t)(b)) >> 1) + 0x2000) >> 14))

#define INNOTALK_SPL_MUL_16_16_RSFT(a, b, c) \
    (INNOTALK_SPL_MUL_16_16(a, b) >> (c))

#define INNOTALK_SPL_MUL_16_16_RSFT_WITH_ROUND(a, b, c) \
    ((INNOTALK_SPL_MUL_16_16(a, b) + ((int32_t) \
                                  (((int32_t)1) << ((c) - 1)))) >> (c))
#define INNOTALK_SPL_MUL_16_16_RSFT_WITH_FIXROUND(a, b) \
    ((INNOTALK_SPL_MUL_16_16(a, b) + ((int32_t) (1 << 14))) >> 15)

// C + the 32 most significant bits of A * B
#define INNOTALK_SPL_SCALEDIFF32(A, B, C) \
    (C + (B >> 16) * A + (((uint32_t)(0x0000FFFF & B) * A) >> 16))

#define INNOTALK_SPL_ADD_SAT_W32(a, b)    InnoTalkSpl_AddSatW32(a, b)
#define INNOTALK_SPL_SAT(a, b, c)         (b > a ? a : b < c ? c : b)
#define INNOTALK_SPL_MUL_32_16(a, b)      ((a) * (b))

#define INNOTALK_SPL_SUB_SAT_W32(a, b)    InnoTalkSpl_SubSatW32(a, b)
#define INNOTALK_SPL_ADD_SAT_W16(a, b)    InnoTalkSpl_AddSatW16(a, b)
#define INNOTALK_SPL_SUB_SAT_W16(a, b)    InnoTalkSpl_SubSatW16(a, b)

// We cannot do casting here due to signed/unsigned problem
#define INNOTALK_SPL_IS_NEG(a)            ((a) & 0x80000000)
// Shifting with negative numbers allowed
// Positive means left shift
#define INNOTALK_SPL_SHIFT_W16(x, c) \
    (((c) >= 0) ? ((x) << (c)) : ((x) >> (-(c))))
#define INNOTALK_SPL_SHIFT_W32(x, c) \
    (((c) >= 0) ? ((x) << (c)) : ((x) >> (-(c))))

// Shifting with negative numbers not allowed
// We cannot do casting here due to signed/unsigned problem
#define INNOTALK_SPL_RSHIFT_W16(x, c)     ((x) >> (c))
#define INNOTALK_SPL_LSHIFT_W16(x, c)     ((x) << (c))
#define INNOTALK_SPL_RSHIFT_W32(x, c)     ((x) >> (c))
#define INNOTALK_SPL_LSHIFT_W32(x, c)     ((x) << (c))

#define INNOTALK_SPL_RSHIFT_U16(x, c)     ((uint16_t)(x) >> (c))
#define INNOTALK_SPL_LSHIFT_U16(x, c)     ((uint16_t)(x) << (c))
#define INNOTALK_SPL_RSHIFT_U32(x, c)     ((uint32_t)(x) >> (c))
#define INNOTALK_SPL_LSHIFT_U32(x, c)     ((uint32_t)(x) << (c))

#define INNOTALK_SPL_VNEW(t, n)           (t *) malloc (sizeof (t) * (n))
#define INNOTALK_SPL_FREE                 free

#define INNOTALK_SPL_RAND(a) \
    ((int16_t)(INNOTALK_SPL_MUL_16_16_RSFT((a), 18816, 7) & 0x00007fff))

#ifdef __cplusplus
extern "C" {
#endif

#define INNOTALK_SPL_MEMCPY_W8(v1, v2, length) \
  memcpy(v1, v2, (length) * sizeof(char))
#define INNOTALK_SPL_MEMCPY_W16(v1, v2, length) \
  memcpy(v1, v2, (length) * sizeof(int16_t))

#define INNOTALK_SPL_MEMMOVE_W16(v1, v2, length) \
  memmove(v1, v2, (length) * sizeof(int16_t))

// inline functions:
#include "spl_inl.h"

// Initialize SPL. Currently it contains only function pointer initialization.
// If the underlying platform is known to be ARM-Neon (INNOTALK_ARCH_ARM_NEON
// defined), the pointers will be assigned to code optimized for Neon; otherwise
// if run-time Neon detection (INNOTALK_DETECT_ARM_NEON) is enabled, the pointers
// will be assigned to either Neon code or generic C code; otherwise, generic C
// code will be assigned.
// Note that this function MUST be called in any application that uses SPL
// functions.
//void InnoTalkSpl_Init();

// Get SPL Version
//int16_t InnoTalkSpl_get_version(char* version, int16_t length_in_bytes);

int InnoTalkSpl_GetScalingSquare(int16_t* in_vector,
                               int in_vector_length,
                               int times);

// Copy and set operations. Implementation in copy_set_operations.c.
// Descriptions at bottom of file.
void InnoTalkSpl_MemSetW16(int16_t* vector,
                         int16_t set_value,
                         int vector_length);
void InnoTalkSpl_MemSetW32(int32_t* vector,
                         int32_t set_value,
                         int vector_length);
void InnoTalkSpl_MemCpyReversedOrder(int16_t* out_vector,
                                   int16_t* in_vector,
                                   int vector_length);
int16_t InnoTalkSpl_CopyFromEndW16(const int16_t* in_vector,
                                 int16_t in_vector_length,
                                 int16_t samples,
                                 int16_t* out_vector);
int16_t InnoTalkSpl_ZerosArrayW16(int16_t* vector,
                                int16_t vector_length);
int16_t InnoTalkSpl_ZerosArrayW32(int32_t* vector,
                                int16_t vector_length);
int16_t InnoTalkSpl_OnesArrayW16(int16_t* vector,
                               int16_t vector_length);
int16_t InnoTalkSpl_OnesArrayW32(int32_t* vector,
                               int16_t vector_length);
// End: Copy and set operations.


// Minimum and maximum operation functions and their pointers.
// Implementation in min_max_operations.c.

// Returns the largest absolute value in a signed 16-bit vector.
//
// Input:
//      - vector : 16-bit input vector.
//      - length : Number of samples in vector.
//
// Return value  : Maximum absolute value in vector;
//                 or -1, if (vector == NULL || length <= 0).
typedef int16_t (*MaxAbsValueW16)(const int16_t* vector, int length);
extern MaxAbsValueW16 InnoTalkSpl_MaxAbsValueW16;
int16_t InnoTalkSpl_MaxAbsValueW16C(const int16_t* vector, int length);

// Returns the largest absolute value in a signed 32-bit vector.
//
// Input:
//      - vector : 32-bit input vector.
//      - length : Number of samples in vector.
//
// Return value  : Maximum absolute value in vector;
//                 or -1, if (vector == NULL || length <= 0).
typedef int32_t (*MaxAbsValueW32)(const int32_t* vector, int length);
extern MaxAbsValueW32 InnoTalkSpl_MaxAbsValueW32;
int32_t InnoTalkSpl_MaxAbsValueW32C(const int32_t* vector, int length);

// Returns the maximum value of a 16-bit vector.
//
// Input:
//      - vector : 16-bit input vector.
//      - length : Number of samples in vector.
//
// Return value  : Maximum sample value in |vector|.
//                 If (vector == NULL || length <= 0) INNOTALK_SPL_WORD16_MIN
//                 is returned. Note that INNOTALK_SPL_WORD16_MIN is a feasible
//                 value and we can't catch errors purely based on it.
typedef int16_t (*MaxValueW16)(const int16_t* vector, int length);
extern MaxValueW16 InnoTalkSpl_MaxValueW16;
int16_t InnoTalkSpl_MaxValueW16C(const int16_t* vector, int length);

// Returns the maximum value of a 32-bit vector.
//
// Input:
//      - vector : 32-bit input vector.
//      - length : Number of samples in vector.
//
// Return value  : Maximum sample value in |vector|.
//                 If (vector == NULL || length <= 0) INNOTALK_SPL_WORD32_MIN
//                 is returned. Note that INNOTALK_SPL_WORD32_MIN is a feasible
//                 value and we can't catch errors purely based on it.
typedef int32_t (*MaxValueW32)(const int32_t* vector, int length);
extern MaxValueW32 InnoTalkSpl_MaxValueW32;
int32_t InnoTalkSpl_MaxValueW32C(const int32_t* vector, int length);

// Returns the minimum value of a 16-bit vector.
//
// Input:
//      - vector : 16-bit input vector.
//      - length : Number of samples in vector.
//
// Return value  : Minimum sample value in |vector|.
//                 If (vector == NULL || length <= 0) INNOTALK_SPL_WORD16_MAX
//                 is returned. Note that INNOTALK_SPL_WORD16_MAX is a feasible
//                 value and we can't catch errors purely based on it.
typedef int16_t (*MinValueW16)(const int16_t* vector, int length);
extern MinValueW16 InnoTalkSpl_MinValueW16;
int16_t InnoTalkSpl_MinValueW16C(const int16_t* vector, int length);

// Returns the minimum value of a 32-bit vector.
//
// Input:
//      - vector : 32-bit input vector.
//      - length : Number of samples in vector.
//
// Return value  : Minimum sample value in |vector|.
//                 If (vector == NULL || length <= 0) INNOTALK_SPL_WORD32_MAX
//                 is returned. Note that INNOTALK_SPL_WORD32_MAX is a feasible
//                 value and we can't catch errors purely based on it.
typedef int32_t (*MinValueW32)(const int32_t* vector, int length);
extern MinValueW32 InnoTalkSpl_MinValueW32;
int32_t InnoTalkSpl_MinValueW32C(const int32_t* vector, int length);

// Returns the vector index to the largest absolute value of a 16-bit vector.
//
// Input:
//      - vector : 16-bit input vector.
//      - length : Number of samples in vector.
//
// Return value  : Index to the maximum absolute value in vector, or -1,
//                 if (vector == NULL || length <= 0).
//                 If there are multiple equal maxima, return the index of the
//                 first. -32768 will always have precedence over 32767 (despite
//                 -32768 presenting an int16 absolute value of 32767);
int InnoTalkSpl_MaxAbsIndexW16(const int16_t* vector, int length);

// Returns the vector index to the maximum sample value of a 16-bit vector.
//
// Input:
//      - vector : 16-bit input vector.
//      - length : Number of samples in vector.
//
// Return value  : Index to the maximum value in vector (if multiple
//                 indexes have the maximum, return the first);
//                 or -1, if (vector == NULL || length <= 0).
int InnoTalkSpl_MaxIndexW16(const int16_t* vector, int length);

// Returns the vector index to the maximum sample value of a 32-bit vector.
//
// Input:
//      - vector : 32-bit input vector.
//      - length : Number of samples in vector.
//
// Return value  : Index to the maximum value in vector (if multiple
//                 indexes have the maximum, return the first);
//                 or -1, if (vector == NULL || length <= 0).
int InnoTalkSpl_MaxIndexW32(const int32_t* vector, int length);

// Returns the vector index to the minimum sample value of a 16-bit vector.
//
// Input:
//      - vector : 16-bit input vector.
//      - length : Number of samples in vector.
//
// Return value  : Index to the mimimum value in vector  (if multiple
//                 indexes have the minimum, return the first);
//                 or -1, if (vector == NULL || length <= 0).
int InnoTalkSpl_MinIndexW16(const int16_t* vector, int length);

// Returns the vector index to the minimum sample value of a 32-bit vector.
//
// Input:
//      - vector : 32-bit input vector.
//      - length : Number of samples in vector.
//
// Return value  : Index to the mimimum value in vector  (if multiple
//                 indexes have the minimum, return the first);
//                 or -1, if (vector == NULL || length <= 0).
int InnoTalkSpl_MinIndexW32(const int32_t* vector, int length);

// End: Minimum and maximum operations.


// Vector scaling operations. Implementation in vector_scaling_operations.c.
// Description at bottom of file.
void InnoTalkSpl_VectorBitShiftW16(int16_t* out_vector,
                                 int16_t vector_length,
                                 const int16_t* in_vector,
                                 int16_t right_shifts);
void InnoTalkSpl_VectorBitShiftW32(int32_t* out_vector,
                                 int16_t vector_length,
                                 const int32_t* in_vector,
                                 int16_t right_shifts);
void InnoTalkSpl_VectorBitShiftW32ToW16(int16_t* out_vector,
                                      int vector_length,
                                      const int32_t* in_vector,
                                      int right_shifts);
void InnoTalkSpl_ScaleVector(const int16_t* in_vector,
                           int16_t* out_vector,
                           int16_t gain,
                           int16_t vector_length,
                           int16_t right_shifts);
void InnoTalkSpl_ScaleVectorWithSat(const int16_t* in_vector,
                                  int16_t* out_vector,
                                  int16_t gain,
                                  int16_t vector_length,
                                  int16_t right_shifts);
void InnoTalkSpl_ScaleAndAddVectors(const int16_t* in_vector1,
                                  int16_t gain1, int right_shifts1,
                                  const int16_t* in_vector2,
                                  int16_t gain2, int right_shifts2,
                                  int16_t* out_vector,
                                  int vector_length);

// The functions (with related pointer) perform the vector operation:
//   out_vector[k] = ((scale1 * in_vector1[k]) + (scale2 * in_vector2[k])
//        + round_value) >> right_shifts,
//   where  round_value = (1 << right_shifts) >> 1.
//
// Input:
//      - in_vector1       : Input vector 1
//      - in_vector1_scale : Gain to be used for vector 1
//      - in_vector2       : Input vector 2
//      - in_vector2_scale : Gain to be used for vector 2
//      - right_shifts     : Number of right bit shifts to be applied
//      - length           : Number of elements in the input vectors
//
// Output:
//      - out_vector       : Output vector
// Return value            : 0 if OK, -1 if (in_vector1 == NULL
//                           || in_vector2 == NULL || out_vector == NULL
//                           || length <= 0 || right_shift < 0).
typedef int (*ScaleAndAddVectorsWithRound)(const int16_t* in_vector1,
                                           int16_t in_vector1_scale,
                                           const int16_t* in_vector2,
                                           int16_t in_vector2_scale,
                                           int right_shifts,
                                           int16_t* out_vector,
                                           int length);
extern ScaleAndAddVectorsWithRound InnoTalkSpl_ScaleAndAddVectorsWithRound;
int InnoTalkSpl_ScaleAndAddVectorsWithRoundC(const int16_t* in_vector1,
                                           int16_t in_vector1_scale,
                                           const int16_t* in_vector2,
                                           int16_t in_vector2_scale,
                                           int right_shifts,
                                           int16_t* out_vector,
                                           int length);

// End: Vector scaling operations.

// iLBC specific functions. Implementations in ilbc_specific_functions.c.
// Description at bottom of file.
void InnoTalkSpl_ReverseOrderMultArrayElements(int16_t* out_vector,
                                             const int16_t* in_vector,
                                             const int16_t* window,
                                             int16_t vector_length,
                                             int16_t right_shifts);
void InnoTalkSpl_ElementwiseVectorMult(int16_t* out_vector,
                                     const int16_t* in_vector,
                                     const int16_t* window,
                                     int16_t vector_length,
                                     int16_t right_shifts);
void InnoTalkSpl_AddVectorsAndShift(int16_t* out_vector,
                                  const int16_t* in_vector1,
                                  const int16_t* in_vector2,
                                  int16_t vector_length,
                                  int16_t right_shifts);
void InnoTalkSpl_AddAffineVectorToVector(int16_t* out_vector,
                                       int16_t* in_vector,
                                       int16_t gain,
                                       int32_t add_constant,
                                       int16_t right_shifts,
                                       int vector_length);
void InnoTalkSpl_AffineTransformVector(int16_t* out_vector,
                                     int16_t* in_vector,
                                     int16_t gain,
                                     int32_t add_constant,
                                     int16_t right_shifts,
                                     int vector_length);
// End: iLBC specific functions.

// Signal processing operations.

// A 32-bit fix-point implementation of auto-correlation computation
//
// Input:
//      - in_vector        : Vector to calculate autocorrelation upon
//      - in_vector_length : Length (in samples) of |vector|
//      - order            : The order up to which the autocorrelation should be
//                           calculated
//
// Output:
//      - result           : auto-correlation values (values should be seen
//                           relative to each other since the absolute values
//                           might have been down shifted to avoid overflow)
//
//      - scale            : The number of left shifts required to obtain the
//                           auto-correlation in Q0
//
// Return value            :
//      - -1, if |order| > |in_vector_length|;
//      - Number of samples in |result|, i.e. (order+1), otherwise.
int InnoTalkSpl_AutoCorrelation(const int16_t* in_vector,
                              int in_vector_length,
                              int order,
                              int32_t* result,
                              int* scale);

// A 32-bit fix-point implementation of the Levinson-Durbin algorithm that
// does NOT use the 64 bit class
//
// Input:
//      - auto_corr : Vector with autocorrelation values of length >=
//                    |use_order|+1
//      - use_order : The LPC filter order (support up to order 20)
//
// Output:
//      - lpc_coef  : lpc_coef[0..use_order] LPC coefficients in Q12
//      - refl_coef : refl_coef[0...use_order-1]| Reflection coefficients in
//                    Q15
//
// Return value     : 1 for stable 0 for unstable
int16_t InnoTalkSpl_LevinsonDurbin(int32_t* auto_corr,
                                 int16_t* lpc_coef,
                                 int16_t* refl_coef,
                                 int16_t order);

// Converts reflection coefficients |refl_coef| to LPC coefficients |lpc_coef|.
// This version is a 16 bit operation.
//
// NOTE: The 16 bit refl_coef -> lpc_coef conversion might result in a
// "slightly unstable" filter (i.e., a pole just outside the unit circle) in
// "rare" cases even if the reflection coefficients are stable.
//
// Input:
//      - refl_coef : Reflection coefficients in Q15 that should be converted
//                    to LPC coefficients
//      - use_order : Number of coefficients in |refl_coef|
//
// Output:
//      - lpc_coef  : LPC coefficients in Q12
void InnoTalkSpl_ReflCoefToLpc(const int16_t* refl_coef,
                             int use_order,
                             int16_t* lpc_coef);

// Converts LPC coefficients |lpc_coef| to reflection coefficients |refl_coef|.
// This version is a 16 bit operation.
// The conversion is implemented by the step-down algorithm.
//
// Input:
//      - lpc_coef  : LPC coefficients in Q12, that should be converted to
//                    reflection coefficients
//      - use_order : Number of coefficients in |lpc_coef|
//
// Output:
//      - refl_coef : Reflection coefficients in Q15.
void InnoTalkSpl_LpcToReflCoef(int16_t* lpc_coef,
                             int use_order,
                             int16_t* refl_coef);

// Calculates reflection coefficients (16 bit) from auto-correlation values
//
// Input:
//      - auto_corr : Auto-correlation values
//      - use_order : Number of coefficients wanted be calculated
//
// Output:
//      - refl_coef : Reflection coefficients in Q15.
void InnoTalkSpl_AutoCorrToReflCoef(const int32_t* auto_corr,
                                  int use_order,
                                  int16_t* refl_coef);

// The functions (with related pointer) calculate the cross-correlation between
// two sequences |seq1| and |seq2|.
// |seq1| is fixed and |seq2| slides as the pointer is increased with the
// amount |step_seq2|. Note the arguments should obey the relationship:
// |dim_seq| - 1 + |step_seq2| * (|dim_cross_correlation| - 1) <
//      buffer size of |seq2|
//
// Input:
//      - seq1           : First sequence (fixed throughout the correlation)
//      - seq2           : Second sequence (slides |step_vector2| for each
//                            new correlation)
//      - dim_seq        : Number of samples to use in the cross-correlation
//      - dim_cross_correlation : Number of cross-correlations to calculate (the
//                            start position for |vector2| is updated for each
//                            new one)
//      - right_shifts   : Number of right bit shifts to use. This will
//                            become the output Q-domain.
//      - step_seq2      : How many (positive or negative) steps the
//                            |vector2| pointer should be updated for each new
//                            cross-correlation value.
//
// Output:
//      - cross_correlation : The cross-correlation in Q(-right_shifts)
typedef void (*CrossCorrelation)(int32_t* cross_correlation,
                                 const int16_t* seq1,
                                 const int16_t* seq2,
                                 int16_t dim_seq,
                                 int16_t dim_cross_correlation,
                                 int16_t right_shifts,
                                 int16_t step_seq2);
extern CrossCorrelation InnoTalkSpl_CrossCorrelation;
void InnoTalkSpl_CrossCorrelationC(int32_t* cross_correlation,
                                 const int16_t* seq1,
                                 const int16_t* seq2,
                                 int16_t dim_seq,
                                 int16_t dim_cross_correlation,
                                 int16_t right_shifts,
                                 int16_t step_seq2);
#if (defined INNOTALK_DETECT_ARM_NEON) || (defined INNOTALK_ARCH_ARM_NEON)
void InnoTalkSpl_CrossCorrelationNeon(int32_t* cross_correlation,
                                    const int16_t* seq1,
                                    const int16_t* seq2,
                                    int16_t dim_seq,
                                    int16_t dim_cross_correlation,
                                    int16_t right_shifts,
                                    int16_t step_seq2);
#endif
#if defined(MIPS32_LE)
void InnoTalkSpl_CrossCorrelation_mips(int32_t* cross_correlation,
                                     const int16_t* seq1,
                                     const int16_t* seq2,
                                     int16_t dim_seq,
                                     int16_t dim_cross_correlation,
                                     int16_t right_shifts,
                                     int16_t step_seq2);
#endif

// Creates (the first half of) a Hanning window. Size must be at least 1 and
// at most 512.
//
// Input:
//      - size      : Length of the requested Hanning window (1 to 512)
//
// Output:
//      - window    : Hanning vector in Q14.
void InnoTalkSpl_GetHanningWindow(int16_t* window, int16_t size);

// Calculates y[k] = sqrt(1 - x[k]^2) for each element of the input vector
// |in_vector|. Input and output values are in Q15.
//
// Inputs:
//      - in_vector     : Values to calculate sqrt(1 - x^2) of
//      - vector_length : Length of vector |in_vector|
//
// Output:
//      - out_vector    : Output values in Q15
void InnoTalkSpl_SqrtOfOneMinusXSquared(int16_t* in_vector,
                                      int vector_length,
                                      int16_t* out_vector);
// End: Signal processing operations.

// Randomization functions. Implementations collected in
// randomization_functions.c and descriptions at bottom of this file.
uint32_t InnoTalkSpl_IncreaseSeed(uint32_t* seed);
int16_t InnoTalkSpl_RandU(uint32_t* seed);
int16_t InnoTalkSpl_RandN(uint32_t* seed);
int16_t InnoTalkSpl_RandUArray(int16_t* vector,
                             int16_t vector_length,
                             uint32_t* seed);
// End: Randomization functions.

// Math functions
int32_t InnoTalkSpl_Sqrt(int32_t value);
int32_t InnoTalkSpl_SqrtFloor(int32_t value);

// Divisions. Implementations collected in division_operations.c and
// descriptions at bottom of this file.
uint32_t InnoTalkSpl_DivU32U16(uint32_t num, uint16_t den);
int32_t InnoTalkSpl_DivW32W16(int32_t num, int16_t den);
int16_t InnoTalkSpl_DivW32W16ResW16(int32_t num, int16_t den);
int32_t InnoTalkSpl_DivResultInQ31(int32_t num, int32_t den);
int32_t InnoTalkSpl_DivW32HiLow(int32_t num, int16_t den_hi, int16_t den_low);
// End: Divisions.

int32_t InnoTalkSpl_Energy(int16_t* vector, int vector_length, int* scale_factor);

// Calculates the dot product between two (int16_t) vectors.
//
// Input:
//      - vector1       : Vector 1
//      - vector2       : Vector 2
//      - vector_length : Number of samples used in the dot product
//      - scaling       : The number of right bit shifts to apply on each term
//                        during calculation to avoid overflow, i.e., the
//                        output will be in Q(-|scaling|)
//
// Return value         : The dot product in Q(-scaling)
int32_t InnoTalkSpl_DotProductWithScale(const int16_t* vector1,
                                      const int16_t* vector2,
                                      int length,
                                      int scaling);

// Filter operations.
int InnoTalkSpl_FilterAR(const int16_t* ar_coef,
                       int ar_coef_length,
                       const int16_t* in_vector,
                       int in_vector_length,
                       int16_t* filter_state,
                       int filter_state_length,
                       int16_t* filter_state_low,
                       int filter_state_low_length,
                       int16_t* out_vector,
                       int16_t* out_vector_low,
                       int out_vector_low_length);

void InnoTalkSpl_FilterMAFastQ12(int16_t* in_vector,
                               int16_t* out_vector,
                               int16_t* ma_coef,
                               int16_t ma_coef_length,
                               int16_t vector_length);

// Performs a AR filtering on a vector in Q12
// Input:
//      - data_in            : Input samples
//      - data_out           : State information in positions
//                               data_out[-order] .. data_out[-1]
//      - coefficients       : Filter coefficients (in Q12)
//      - coefficients_length: Number of coefficients (order+1)
//      - data_length        : Number of samples to be filtered
// Output:
//      - data_out           : Filtered samples
void InnoTalkSpl_FilterARFastQ12(const int16_t* data_in,
                               int16_t* data_out,
                               const int16_t* __restrict coefficients,
                               int coefficients_length,
                               int data_length);

// The functions (with related pointer) perform a MA down sampling filter
// on a vector.
// Input:
//      - data_in            : Input samples (state in positions
//                               data_in[-order] .. data_in[-1])
//      - data_in_length     : Number of samples in |data_in| to be filtered.
//                               This must be at least
//                               |delay| + |factor|*(|out_vector_length|-1) + 1)
//      - data_out_length    : Number of down sampled samples desired
//      - coefficients       : Filter coefficients (in Q12)
//      - coefficients_length: Number of coefficients (order+1)
//      - factor             : Decimation factor
//      - delay              : Delay of filter (compensated for in out_vector)
// Output:
//      - data_out           : Filtered samples
// Return value              : 0 if OK, -1 if |in_vector| is too short
typedef int (*DownsampleFast)(const int16_t* data_in,
                              int data_in_length,
                              int16_t* data_out,
                              int data_out_length,
                              const int16_t* __restrict coefficients,
                              int coefficients_length,
                              int factor,
                              int delay);
extern DownsampleFast InnoTalkSpl_DownsampleFast;
int InnoTalkSpl_DownsampleFastC(const int16_t* data_in,
                              int data_in_length,
                              int16_t* data_out,
                              int data_out_length,
                              const int16_t* __restrict coefficients,
                              int coefficients_length,
                              int factor,
                              int delay);
#if (defined INNOTALK_DETECT_ARM_NEON) || (defined INNOTALK_ARCH_ARM_NEON)
int InnoTalkSpl_DownsampleFastNeon(const int16_t* data_in,
                                 int data_in_length,
                                 int16_t* data_out,
                                 int data_out_length,
                                 const int16_t* __restrict coefficients,
                                 int coefficients_length,
                                 int factor,
                                 int delay);
#endif
#if defined(MIPS32_LE)
int InnoTalkSpl_DownsampleFast_mips(const int16_t* data_in,
                                  int data_in_length,
                                  int16_t* data_out,
                                  int data_out_length,
                                  const int16_t* __restrict coefficients,
                                  int coefficients_length,
                                  int factor,
                                  int delay);
#endif

void InnoTalkSpl_DownsampleBy2(const int16_t* in, int16_t len,
                             int16_t* out, int32_t* filtState);

void InnoTalkSpl_UpsampleBy2(const int16_t* in, int16_t len,
                           int16_t* out, int32_t* filtState);

// End: Filter operations.

void InnoTalkSpl_AnalysisQMF(const int16_t* in_data,
                           int16_t* low_band,
                           int16_t* high_band,
                           int32_t* filter_state1,
                           int32_t* filter_state2);
void InnoTalkSpl_SynthesisQMF(const int16_t* low_band,
                            const int16_t* high_band,
                            int16_t* out_data,
                            int32_t* filter_state1,
                            int32_t* filter_state2);

// FFT operations

int InnoTalkSpl_ComplexFFT(int16_t vector[], int stages, int mode);
int InnoTalkSpl_ComplexIFFT(int16_t vector[], int stages, int mode);
void InnoTalk_rdft(int, int, float *, int *, float *);
void InnoTalk_cdft(int, int, float *, int *, float *);

// Treat a 16-bit complex data buffer |complex_data| as an array of 32-bit
// values, and swap elements whose indexes are bit-reverses of each other.
//
// Input:
//      - complex_data  : Complex data buffer containing 2^|stages| real
//                        elements interleaved with 2^|stages| imaginary
//                        elements: [Re Im Re Im Re Im....]
//      - stages        : Number of FFT stages. Must be at least 3 and at most
//                        10, since the table InnoTalkSpl_kSinTable1024[] is 1024
//                        elements long.
//
// Output:
//      - complex_data  : The complex data buffer.

void InnoTalkSpl_ComplexBitReverse(int16_t* __restrict complex_data, int stages);

// End: FFT operations

#ifdef __cplusplus
}
#endif  // __cplusplus
#endif  // INNOTALK_SPL_SIGNAL_PROCESSING_LIBRARY_H_

