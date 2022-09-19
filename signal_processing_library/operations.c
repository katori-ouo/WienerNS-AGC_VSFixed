/*
 * operations.c
 *
 *  Created on: 2021Äê5ÔÂ4ÈÕ
 *      Author: w
 */
#include "signal_processing_library.h"

static const uint16_t kResampleAllpass1[3] = { 3284, 24441, 49528 };
static const uint16_t kResampleAllpass2[3] = { 12199, 37471, 60255 };
#define MUL_ACCUM_1(a, b, c) INNOTALK_SPL_SCALEDIFF32(a, b, c)
#define MUL_ACCUM_2(a, b, c) INNOTALK_SPL_SCALEDIFF32(a, b, c)

int T_abs(int L_var1)
{
	int L_var_out;

	if (L_var1 == (int)0x80000000L)
	{
		L_var_out = (int)0x7fffffffL;
	}
	else
	{
		if (L_var1 < 0)
		{
			L_var_out = -L_var1;
		}
		else
		{
			L_var_out = L_var1;
		}
	}

	return (L_var_out);
}
void InnoTalkSpl_MemSetW16(int16_t *ptr, int16_t set_value, int length)
{
    int j;
    int16_t *arrptr = ptr;

    for (j = length; j > 0; j--)
    {
        *arrptr++ = set_value;
    }
}

void InnoTalkSpl_MemSetW32(int32_t *ptr, int32_t set_value, int length)
{
    int j;
    int32_t *arrptr = ptr;

    for (j = length; j > 0; j--)
    {
        *arrptr++ = set_value;
    }
}

void InnoTalkSpl_MemCpyReversedOrder(int16_t* dest, int16_t* source, int length)
{
    int j;
    int16_t* destPtr = dest;
    int16_t* sourcePtr = source;

    for (j = 0; j < length; j++)
    {
        *destPtr-- = *sourcePtr++;
    }
}

int16_t InnoTalkSpl_CopyFromEndW16(const int16_t *vector_in,
                                 int16_t length,
                                 int16_t samples,
                                 int16_t *vector_out)
{
    // Copy the last <samples> of the input vector to vector_out
    INNOTALK_SPL_MEMCPY_W16(vector_out, &vector_in[length - samples], samples);

    return samples;
}

int16_t InnoTalkSpl_ZerosArrayW16(int16_t *vector, int16_t length)
{
    InnoTalkSpl_MemSetW16(vector, 0, length);
    return length;
}

int16_t InnoTalkSpl_ZerosArrayW32(int32_t *vector, int16_t length)
{
    InnoTalkSpl_MemSetW32(vector, 0, length);
    return length;
}

int16_t InnoTalkSpl_OnesArrayW16(int16_t *vector, int16_t length)
{
    int16_t i;
    int16_t *tmpvec = vector;
    for (i = 0; i < length; i++)
    {
        *tmpvec++ = 1;
    }
    return length;
}

int16_t InnoTalkSpl_OnesArrayW32(int32_t *vector, int16_t length)
{
    int16_t i;
    int32_t *tmpvec = vector;
    for (i = 0; i < length; i++)
    {
        *tmpvec++ = 1;
    }
    return length;
}

uint32_t InnoTalkSpl_DivU32U16(uint32_t num, uint16_t den)
{
    // Guard against division with 0
    if (den != 0)
    {
        return (uint32_t)(num / den);
    } else
    {
        return (uint32_t)0xFFFFFFFF;
    }
}

int32_t InnoTalkSpl_DivW32W16(int32_t num, int16_t den)
{
    // Guard against division with 0
    if (den != 0)
    {
        return (int32_t)(num / den);
    } else
    {
        return (int32_t)0x7FFFFFFF;
    }
}

int16_t InnoTalkSpl_DivW32W16ResW16(int32_t num, int16_t den)
{
    // Guard against division with 0
    if (den != 0)
    {
        return (int16_t)(num / den);
    } else
    {
        return (int16_t)0x7FFF;
    }
}

int32_t InnoTalkSpl_DivResultInQ31(int32_t num, int32_t den)
{
    int32_t L_num = num;
    int32_t L_den = den;
    int32_t div = 0;
    int k = 31;
    int change_sign = 0;

    if (num == 0)
        return 0;

    if (num < 0)
    {
        change_sign++;
        L_num = -num;
    }
    if (den < 0)
    {
        change_sign++;
        L_den = -den;
    }
    while (k--)
    {
        div <<= 1;
        L_num <<= 1;
        if (L_num >= L_den)
        {
            L_num -= L_den;
            div++;
        }
    }
    if (change_sign == 1)
    {
        div = -div;
    }
    return div;
}

int32_t InnoTalkSpl_DivW32HiLow(int32_t num, int16_t den_hi, int16_t den_low)
{
    int16_t approx, tmp_hi, tmp_low, num_hi, num_low;
    int32_t tmpW32;

    approx = (int16_t)InnoTalkSpl_DivW32W16((int32_t)0x1FFFFFFF, den_hi);
    // result in Q14 (Note: 3FFFFFFF = 0.5 in Q30)

    // tmpW32 = 1/den = approx * (2.0 - den * approx) (in Q30)
    tmpW32 = (INNOTALK_SPL_MUL_16_16(den_hi, approx) << 1)
            + ((INNOTALK_SPL_MUL_16_16(den_low, approx) >> 15) << 1);
    // tmpW32 = den * approx

    tmpW32 = (int32_t)0x7fffffffL - tmpW32; // result in Q30 (tmpW32 = 2.0-(den*approx))

    // Store tmpW32 in hi and low format
    tmp_hi = (int16_t)INNOTALK_SPL_RSHIFT_W32(tmpW32, 16);
    tmp_low = (int16_t)INNOTALK_SPL_RSHIFT_W32((tmpW32
            - INNOTALK_SPL_LSHIFT_W32((int32_t)tmp_hi, 16)), 1);

    // tmpW32 = 1/den in Q29
    tmpW32 = ((INNOTALK_SPL_MUL_16_16(tmp_hi, approx) + (INNOTALK_SPL_MUL_16_16(tmp_low, approx)
            >> 15)) << 1);

    // 1/den in hi and low format
    tmp_hi = (int16_t)INNOTALK_SPL_RSHIFT_W32(tmpW32, 16);
    tmp_low = (int16_t)INNOTALK_SPL_RSHIFT_W32((tmpW32
            - INNOTALK_SPL_LSHIFT_W32((int32_t)tmp_hi, 16)), 1);

    // Store num in hi and low format
    num_hi = (int16_t)INNOTALK_SPL_RSHIFT_W32(num, 16);
    num_low = (int16_t)INNOTALK_SPL_RSHIFT_W32((num
            - INNOTALK_SPL_LSHIFT_W32((int32_t)num_hi, 16)), 1);

    // num * (1/den) by 32 bit multiplication (result in Q28)

    tmpW32 = (INNOTALK_SPL_MUL_16_16(num_hi, tmp_hi) + (INNOTALK_SPL_MUL_16_16(num_hi, tmp_low)
            >> 15) + (INNOTALK_SPL_MUL_16_16(num_low, tmp_hi) >> 15));

    // Put result in Q31 (convert from Q28)
    tmpW32 = INNOTALK_SPL_LSHIFT_W32(tmpW32, 3);

    return tmpW32;
}

#if 1
int16_t InnoTalkSpl_MaxAbsValueW16C(const int16_t* vector, int length) {
	int i = 0, absolute = 0, maximum = 0;

	if (vector == NULL || length <= 0) {
		return -1;
	}

	for (i = 0; i < length; i++) {
		absolute = T_abs((int)vector[i]);

		if (absolute > maximum) {
			maximum = absolute;
		}
	}

	// Guard the case for abs(-32768).
	if (maximum > INNOTALK_SPL_WORD16_MAX) {
		maximum = INNOTALK_SPL_WORD16_MAX;
	}

	return (int16_t)maximum;
}

// Maximum absolute value of word32 vector. C version for generic platforms.
int32_t InnoTalkSpl_MaxAbsValueW32C(const int32_t* vector, int length) {
	// Use uint32_t for the local variables, to accommodate the return value
	// of abs(0x80000000), which is 0x80000000.

	uint32_t absolute = 0, maximum = 0;
	int i = 0;

	if (vector == NULL || length <= 0) {
		return -1;
	}

	for (i = 0; i < length; i++) {
		absolute = T_abs((int)vector[i]);
		if (absolute > maximum) {
			maximum = absolute;
		}
	}

	maximum = INNOTALK_SPL_MIN(maximum, INNOTALK_SPL_WORD32_MAX);

	return (int32_t)maximum;
}

// Maximum value of word16 vector. C version for generic platforms.
int16_t InnoTalkSpl_MaxValueW16C(const int16_t* vector, int length) {
	int16_t maximum = INNOTALK_SPL_WORD16_MIN;
	int i = 0;

	if (vector == NULL || length <= 0) {
		return maximum;
	}

	for (i = 0; i < length; i++) {
		if (vector[i] > maximum)
			maximum = vector[i];
	}
	return maximum;
}

// Maximum value of word32 vector. C version for generic platforms.
int32_t InnoTalkSpl_MaxValueW32C(const int32_t* vector, int length) {
	int32_t maximum = INNOTALK_SPL_WORD32_MIN;
	int i = 0;

	if (vector == NULL || length <= 0) {
		return maximum;
	}

	for (i = 0; i < length; i++) {
		if (vector[i] > maximum)
			maximum = vector[i];
	}
	return maximum;
}

// Minimum value of word16 vector. C version for generic platforms.
int16_t InnoTalkSpl_MinValueW16C(const int16_t* vector, int length) {
	int16_t minimum = INNOTALK_SPL_WORD16_MAX;
	int i = 0;

	if (vector == NULL || length <= 0) {
		return minimum;
	}

	for (i = 0; i < length; i++) {
		if (vector[i] < minimum)
			minimum = vector[i];
	}
	return minimum;
}

// Minimum value of word32 vector. C version for generic platforms.
int32_t InnoTalkSpl_MinValueW32C(const int32_t* vector, int length) {
	int32_t minimum = INNOTALK_SPL_WORD32_MAX;
	int i = 0;

	if (vector == NULL || length <= 0) {
		return minimum;
	}

	for (i = 0; i < length; i++) {
		if (vector[i] < minimum)
			minimum = vector[i];
	}
	return minimum;
}

// Index of maximum absolute value in a word16 vector.
int InnoTalkSpl_MaxAbsIndexW16(const int16_t* vector, int length) {
	// Use type int for local variables, to accomodate the value of T_abs(-32768).

	int i = 0, absolute = 0, maximum = 0, index = 0;

	if (vector == NULL || length <= 0) {
		return -1;
	}

	for (i = 0; i < length; i++) {
		absolute = T_abs((int)vector[i]);

		if (absolute > maximum) {
			maximum = absolute;
			index = i;
		}
	}

	return index;
}

// Index of maximum value in a word16 vector.
int InnoTalkSpl_MaxIndexW16(const int16_t* vector, int length) {
	int i = 0, index = 0;
	int16_t maximum = INNOTALK_SPL_WORD16_MIN;

	if (vector == NULL || length <= 0) {
		return -1;
	}

	for (i = 0; i < length; i++) {
		if (vector[i] > maximum) {
			maximum = vector[i];
			index = i;
		}
	}

	return index;
}

// Index of maximum value in a word32 vector.
int InnoTalkSpl_MaxIndexW32(const int32_t* vector, int length) {
	int i = 0, index = 0;
	int32_t maximum = INNOTALK_SPL_WORD32_MIN;

	if (vector == NULL || length <= 0) {
		return -1;
	}

	for (i = 0; i < length; i++) {
		if (vector[i] > maximum) {
			maximum = vector[i];
			index = i;
		}
	}

	return index;
}

// Index of minimum value in a word16 vector.
int InnoTalkSpl_MinIndexW16(const int16_t* vector, int length) {
	int i = 0, index = 0;
	int16_t minimum = INNOTALK_SPL_WORD16_MAX;

	if (vector == NULL || length <= 0) {
		return -1;
	}

	for (i = 0; i < length; i++) {
		if (vector[i] < minimum) {
			minimum = vector[i];
			index = i;
		}
	}

	return index;
}

// Index of minimum value in a word32 vector.
int InnoTalkSpl_MinIndexW32(const int32_t* vector, int length) {
	int i = 0, index = 0;
	int32_t minimum = INNOTALK_SPL_WORD32_MAX;

	if (vector == NULL || length <= 0) {
		return -1;
	}

	for (i = 0; i < length; i++) {
		if (vector[i] < minimum) {
			minimum = vector[i];
			index = i;
		}
	}

	return index;
}

#define INNOTALK_SPL_SQRT_ITER(N)                 \
  try1 = root + (1 << (N));                     \
  if (value >= try1 << (N))                     \
  {                                             \
    value -= try1 << (N);                       \
    root |= 2 << (N);                           \
  }

int32_t InnoTalkSpl_SqrtFloor(int32_t value)
{
	int32_t root = 0, try1;

	INNOTALK_SPL_SQRT_ITER(15);
	INNOTALK_SPL_SQRT_ITER(14);
	INNOTALK_SPL_SQRT_ITER(13);
	INNOTALK_SPL_SQRT_ITER(12);
	INNOTALK_SPL_SQRT_ITER(11);
	INNOTALK_SPL_SQRT_ITER(10);
	INNOTALK_SPL_SQRT_ITER(9);
	INNOTALK_SPL_SQRT_ITER(8);
	INNOTALK_SPL_SQRT_ITER(7);
	INNOTALK_SPL_SQRT_ITER(6);
	INNOTALK_SPL_SQRT_ITER(5);
	INNOTALK_SPL_SQRT_ITER(4);
	INNOTALK_SPL_SQRT_ITER(3);
	INNOTALK_SPL_SQRT_ITER(2);
	INNOTALK_SPL_SQRT_ITER(1);
	INNOTALK_SPL_SQRT_ITER(0);

	return root >> 1;
}

void InnoTalkSpl_DownsampleBy2(const int16_t* in, int16_t len,
	int16_t* out, int32_t* filtState) {
	int32_t tmp1, tmp2, diff, in32, out32;
	int16_t i;

	register int32_t state0 = filtState[0];
	register int32_t state1 = filtState[1];
	register int32_t state2 = filtState[2];
	register int32_t state3 = filtState[3];
	register int32_t state4 = filtState[4];
	register int32_t state5 = filtState[5];
	register int32_t state6 = filtState[6];
	register int32_t state7 = filtState[7];

	for (i = (len >> 1); i > 0; i--) {
		// lower allpass filter
		in32 = (int32_t)(*in++) << 10;
		diff = in32 - state1;
		tmp1 = MUL_ACCUM_1(kResampleAllpass2[0], diff, state0);
		state0 = in32;
		diff = tmp1 - state2;
		tmp2 = MUL_ACCUM_2(kResampleAllpass2[1], diff, state1);
		state1 = tmp1;
		diff = tmp2 - state3;
		state3 = MUL_ACCUM_2(kResampleAllpass2[2], diff, state2);
		state2 = tmp2;

		// upper allpass filter
		in32 = (int32_t)(*in++) << 10;
		diff = in32 - state5;
		tmp1 = MUL_ACCUM_1(kResampleAllpass1[0], diff, state4);
		state4 = in32;
		diff = tmp1 - state6;
		tmp2 = MUL_ACCUM_1(kResampleAllpass1[1], diff, state5);
		state5 = tmp1;
		diff = tmp2 - state7;
		state7 = MUL_ACCUM_2(kResampleAllpass1[2], diff, state6);
		state6 = tmp2;

		// add two allpass outputs, divide by two and round
		out32 = (state3 + state7 + 1024) >> 11;

		// limit amplitude to prevent wrap-around, and write to output array
		*out++ = InnoTalkSpl_SatW32ToW16(out32);
	}

	filtState[0] = state0;
	filtState[1] = state1;
	filtState[2] = state2;
	filtState[3] = state3;
	filtState[4] = state4;
	filtState[5] = state5;
	filtState[6] = state6;
	filtState[7] = state7;
}

int32_t InnoTalkSpl_SqrtLocal(int32_t in)
{

    int16_t x_half, t16;
    int32_t A, B, x2;

    /* The following block performs:
     y=in/2
     x=y-2^30
     x_half=x/2^31
     t = 1 + (x_half) - 0.5*((x_half)^2) + 0.5*((x_half)^3) - 0.625*((x_half)^4)
         + 0.875*((x_half)^5)
     */

    B = in;

    B = INNOTALK_SPL_RSHIFT_W32(B, 1); // B = in/2
    B = B - ((int32_t)0x40000000); // B = in/2 - 1/2
    x_half = (int16_t)INNOTALK_SPL_RSHIFT_W32(B, 16);// x_half = x/2 = (in-1)/2
    B = B + ((int32_t)0x40000000); // B = 1 + x/2
    B = B + ((int32_t)0x40000000); // Add 0.5 twice (since 1.0 does not exist in Q31)

    x2 = ((int32_t)x_half) * ((int32_t)x_half) * 2; // A = (x/2)^2
    A = -x2; // A = -(x/2)^2
    B = B + (A >> 1); // B = 1 + x/2 - 0.5*(x/2)^2

    A = INNOTALK_SPL_RSHIFT_W32(A, 16);
    A = A * A * 2; // A = (x/2)^4
    t16 = (int16_t)INNOTALK_SPL_RSHIFT_W32(A, 16);
    B = B + INNOTALK_SPL_MUL_16_16(-20480, t16) * 2; // B = B - 0.625*A
    // After this, B = 1 + x/2 - 0.5*(x/2)^2 - 0.625*(x/2)^4

    t16 = (int16_t)INNOTALK_SPL_RSHIFT_W32(A, 16);
    A = INNOTALK_SPL_MUL_16_16(x_half, t16) * 2; // A = (x/2)^5
    t16 = (int16_t)INNOTALK_SPL_RSHIFT_W32(A, 16);
    B = B + INNOTALK_SPL_MUL_16_16(28672, t16) * 2; // B = B + 0.875*A
    // After this, B = 1 + x/2 - 0.5*(x/2)^2 - 0.625*(x/2)^4 + 0.875*(x/2)^5

    t16 = (int16_t)INNOTALK_SPL_RSHIFT_W32(x2, 16);
    A = INNOTALK_SPL_MUL_16_16(x_half, t16) * 2; // A = x/2^3

    B = B + (A >> 1); // B = B + 0.5*A
    // After this, B = 1 + x/2 - 0.5*(x/2)^2 + 0.5*(x/2)^3 - 0.625*(x/2)^4 + 0.875*(x/2)^5

    B = B + ((int32_t)32768); // Round off bit

    return B;
}

int32_t InnoTalkSpl_Sqrt(int32_t value)
{
    /*
     Algorithm:

     Six term Taylor Series is used here to compute the square root of a number
     y^0.5 = (1+x)^0.5 where x = y-1
     = 1+(x/2)-0.5*((x/2)^2+0.5*((x/2)^3-0.625*((x/2)^4+0.875*((x/2)^5)
     0.5 <= x < 1

     Example of how the algorithm works, with ut=sqrt(in), and
     with in=73632 and ut=271 (even shift value case):

     in=73632
     y= in/131072
     x=y-1
     t = 1 + (x/2) - 0.5*((x/2)^2) + 0.5*((x/2)^3) - 0.625*((x/2)^4) + 0.875*((x/2)^5)
     ut=t*(1/sqrt(2))*512

     or:

     in=73632
     in2=73632*2^14
     y= in2/2^31
     x=y-1
     t = 1 + (x/2) - 0.5*((x/2)^2) + 0.5*((x/2)^3) - 0.625*((x/2)^4) + 0.875*((x/2)^5)
     ut=t*(1/sqrt(2))
     ut2=ut*2^9

     which gives:

     in  = 73632
     in2 = 1206386688
     y   = 0.56176757812500
     x   = -0.43823242187500
     t   = 0.74973506527313
     ut  = 0.53014274874797
     ut2 = 2.714330873589594e+002

     or:

     in=73632
     in2=73632*2^14
     y=in2/2
     x=y-2^30
     x_half=x/2^31
     t = 1 + (x_half) - 0.5*((x_half)^2) + 0.5*((x_half)^3) - 0.625*((x_half)^4)
         + 0.875*((x_half)^5)
     ut=t*(1/sqrt(2))
     ut2=ut*2^9

     which gives:

     in  = 73632
     in2 = 1206386688
     y   = 603193344
     x   = -470548480
     x_half =  -0.21911621093750
     t   = 0.74973506527313
     ut  = 0.53014274874797
     ut2 = 2.714330873589594e+002

     */

    int16_t x_norm, nshift, t16, sh;
    int32_t A;

    int16_t k_sqrt_2 = 23170; // 1/sqrt2 (==5a82)

    A = value;

    if (A == 0)
        return (int32_t)0; // sqrt(0) = 0

    sh = InnoTalkSpl_NormW32(A); // # shifts to normalize A
    A = INNOTALK_SPL_LSHIFT_W32(A, sh); // Normalize A
    if (A < (INNOTALK_SPL_WORD32_MAX - 32767))
    {
        A = A + ((int32_t)32768); // Round off bit
    }
    else
    {
        A = INNOTALK_SPL_WORD32_MAX;
    }

    x_norm = (int16_t)INNOTALK_SPL_RSHIFT_W32(A, 16); // x_norm = AH

    nshift = INNOTALK_SPL_RSHIFT_W16(sh, 1); // nshift = sh>>1
    nshift = -nshift; // Negate the power for later de-normalization

    A = (int32_t)INNOTALK_SPL_LSHIFT_W32((int32_t)x_norm, 16);
    A = INNOTALK_SPL_ABS_W32(A); // A = abs(x_norm<<16)
    A = InnoTalkSpl_SqrtLocal(A); // A = sqrt(A)

    if ((-2 * nshift) == sh)
    { // Even shift value case

        t16 = (int16_t)INNOTALK_SPL_RSHIFT_W32(A, 16); // t16 = AH

        A = INNOTALK_SPL_MUL_16_16(k_sqrt_2, t16) * 2; // A = 1/sqrt(2)*t16
        A = A + ((int32_t)32768); // Round off
        A = A & ((int32_t)0x7fff0000); // Round off

        A = INNOTALK_SPL_RSHIFT_W32(A, 15); // A = A>>16

    }
    else
    {
        A = INNOTALK_SPL_RSHIFT_W32(A, 16); // A = A>>16
    }

    A = A & ((int32_t)0x0000ffff);
    A = (int32_t)INNOTALK_SPL_SHIFT_W32(A, nshift); // De-normalize the result

    return A;
}

#endif