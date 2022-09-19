/********************************************************************************************************************************************************
Copyright (C) CEVA(R) Inc. All rights reserved

This information embodies materials and concepts, which are proprietary and confidential to CEVA Inc., and is made available solely pursuant to the 
terms of a written license agreement, or NDA, or another written agreement, as applicable (“CEVA Agreement”), with CEVA Inc. or any of its subsidiaries 
(“CEVA”). 

This information can be used only with the written permission from CEVA, in accordance with the terms and conditions stipulated in the CEVA Agreement, 
under which the information has been supplied and solely as expressly permitted for the purpose specified in the CEVA Agreement.
This information is made available exclusively to licensees or parties that have received express written authorization from CEVA to download or receive 
the information and have agreed to the terms and conditions of the CEVA Agreement.
IF YOU HAVE NOT RECEIVED SUCH EXPRESS AUTHORIZATION AND AGREED TO THE CEVA AGREEMENT, YOU MAY NOT DOWNLOAD, INSTALL OR USE THIS INFORMATION.

The information contained in this document is subject to change without notice and does not represent a commitment on any part of CEVA. Unless 
specifically agreed otherwise in the CEVA Agreement, CEVA make no warranty of any kind with regard to this material, including, but not limited to 
implied warranties of merchantability and fitness for a particular purpose whether arising out of law, custom, conduct or otherwise.

While the information contained herein is assumed to be accurate, CEVA assumes no responsibility for any errors or omissions contained herein, 
and assumes no liability for special, direct, indirect or consequential damage, losses, costs, charges, claims, demands, fees or expenses, of any nature
or kind, which are incurred in connection with the furnishing, performance or use of this material.

This document contains proprietary information, which is protected by U.S. and international copyright laws. All rights reserved. 
No part of this document may be reproduced, photocopied, or translated into another language without the prior written consent of CEVA. 
********************************************************************************************************************************************************/


#ifndef __TYPEDEF__
#define __TYPEDEF__

typedef int		I32;	
typedef short	I16;	
typedef unsigned int	U32;	
typedef unsigned short	U16;	
#ifdef WIN32
typedef long long	acc_t;
typedef __int64	int64;
#else
typedef long long	int64;
#endif

I32 CEVA_TL4_pow2_int32(I32 arg_in, I16 arg_exp_in, I16 *arg_exp_out);
I32 CEVA_TL4_pow10_int32(I32 arg_in, I16 arg_exp_in, I16 *arg_exp_out);
I32 CEVA_TL4_pow_int32(I32 arg_in_X, I16 arg_exp_in_X, I32 arg_in_Y, I16 arg_exp_in_Y, I16 *arg_exp_out);
U16 CEVA_TL4_sqrt_int32_asm(U32 val,U16 rnd_flag);
I16 CEVA_TL4_sqrt_int16 (U16 inp_val, U16 rnd_flag);
I32 CEVA_TL4_isqrt32(I32 arg_in);
I16 CEVA_TL4_isqrt16(I16 arg_in);
I16 CEVA_TL4_div16_ss(I16 inp_val1,I16 inp_val2);
I32 CEVA_TL4_div32_ss_asm(I32 inp_val1,I32 inp_val2);
U16 CEVA_TL4_div16_uu(U16 inp_val1,U16 inp_val2);
U32 CEVA_TL4_div32_uu_asm(U32 inp_val1,U32 inp_val2);
I16 CEVA_TL4_cos_int16(I16 arg);
I32 CEVA_TL4_cos_int32(I32 arg);
I16 CEVA_TL4_sin_int16(I16 arg);
I32 CEVA_TL4_sin_int32(I32 arg);
I16 CEVA_TL4_tan_int16(I16 arg);
I32 CEVA_TL4_tan_int32(I32 arg);
I16 CEVA_TL4_atan_int16(I16 arg);
I32 CEVA_TL4_atan_int32(I32 arg);
I32 CEVA_TL4_log2_int32 (I32 arg_in, I16 arg_exp_in, I16 *arg_exp_out);
I32 CEVA_TL4_logn_int32 (I32 arg_in, I16 arg_exp_in, I16 *arg_exp_out);
I32 CEVA_TL4_log10_int32(I32 arg_in, I16 arg_exp_in, I16 *arg_exp_out);


I16 CEVA_TL4_vec_max16_with_index(I16 *ind, I16 *buf, I16 size_buf, I16 flag);
I32 CEVA_TL4_vec_max32_with_index(I16 *ind, I32 *buf, I16 size_buf, I16 flag);
I16 CEVA_TL4_vec_min16_with_index(I16 *ind, I16 *buf, I16 size_buf, I16 flag);
I32 CEVA_TL4_vec_min32_with_index(I16 *ind, I32 *buf, I16 size_buf, I16 flag);
I16 CEVA_TL4_vec_max_abs16_with_index(I16 *ind, I16 *buf, I16 size_buf, I16 flag);
I32 CEVA_TL4_vec_max_abs32_with_index(I16 *ind, I32 *buf, I16 size_buf, I16 flag);
I16 CEVA_TL4_vec_max16_asm(I16 *buf, I16 size_buf);
I32 CEVA_TL4_vec_max32(I32 *buf, I16 size_buf);
I16 CEVA_TL4_vec_min16(I16 *buf, I16 size_buf);
I32 CEVA_TL4_vec_min32(I32 *buf, I16 size_buf);
I16 CEVA_TL4_vec_max_abs16_asm(I16 *buf, I16 size_buf);
I32 CEVA_TL4_vec_max_abs32(I32 *buf, I16 size_buf);
void CEVA_TL4_vec_add16(I16 *inp1, I16 *inp2, I16 size_buf, I16 *out);
void CEVA_TL4_vec_sub16(I16 *inp1, I16 *inp2, I16 size_buf, I16 *out);
void CEVA_TL4_vec_sub32(I32 *inp1, I32 *inp2, I16 size_buf, I32 *out);
void CEVA_TL4_vec_add32(I32 *inp1, I32 *inp2, I16 size_buf, I32 *out);
void CEVA_TL4_vec_shf16_asm(I16 *inp, I16 shift_val, I16 size_buf, I16 *out);
void CEVA_TL4_vec_shf32(I32 *inp, I16 shift_val, I16 size_buf, I32 *out);
void CEVA_TL4_vec_shf16_no_saturation(I16 *inp, I16 shift_val, I16 size_buf, I16 *out);
void CEVA_TL4_vec_shf32_no_saturation(I32 *inp, I16 shift_val, I16 size_buf, I32 *out);
void CEVA_TL4_vec_mul16_asm(I16 *inp1, I16 *inp2, I16 size_buf, I16 scale, I16 *out);
void CEVA_TL4_vec_mul32(I32 *inp1, I32 *inp2, I16 size_buf, I16 scale, I32 *out);
void CEVA_TL4_vec_mul32_rnd(I32 *inp1, I32 *inp2, I16 size_buf, I16 scale, I32 *out);
I32 CEVA_TL4_vec_dot16_single_mac(I16 *inp1, I16 *inp2, I16 size_buf);
acc_t CEVA_TL4_vec_dot32(I32 *inp1, I32 *inp2, I16 size_buf, I16 scale);
acc_t CEVA_TL4_vec_dot32rnd(I32 *inp1, I32 *inp2, I16 size_buf, I16 scale);
acc_t CEVA_TL4_vec_dot16(I16 *inp1, I16 *inp2, I16 size_buf, I16 scale);

void CEVA_TL4_fft_compl_16_A(I32 log2_buf_len, I16 *in_buf, I16 *out_buf, I16 const *twi_table, I16 *bitrev_buf, I32 bitrev_flag);
void CEVA_TL4_fft_compl_16_VC(I32 log2_buf_len, I16 *in_buf, I16 *out_buf, I16 const *twi_table, I16 *bitrev_buf, I32 bitrev_flag);
void CEVA_TL4_fft_compl_inv_16_A(I32 log2_buf_len, I16 *in_buf, I16 *out_buf, I16 const *twi_table, I16 *bitrev_buf, I32 bitrev_flag);
void CEVA_TL4_fft_compl_inv_16_VC(I32 log2_buf_len, I16 *in_buf, I16 *out_buf, I16 const *twi_table, I16 *bitrev_buf, I32 bitrev_flag);
void CEVA_TL4_fft_real_16_A(I32 log2_buf_len, I16 *in_buf, I16 *out_buf, I16 const *twi_table, I16 const *last_stage_twi_table, I16 *bitrev_buf, I32 bitrev_flag);
void CEVA_TL4_fft_real_16_VC(I32 log2_buf_len, I16 *in_buf, I16 *out_buf, I16 const *twi_table, I16 const *last_stage_twi_table, I16 *bitrev_buf, I32 bitrev_flag);
void CEVA_TL4_fft_real_inv_16_A(I32 log2_buf_len, I16 *in_buf, I16 *out_buf, I16 const *twi_table, I16 const *last_stage_twi_table, I16 *bitrev_buf, I32 bitrev_flag);
void CEVA_TL4_fft_real_inv_16_VC(I32 log2_buf_len, I16 *in_buf, I16 *out_buf, I16 const *twi_table, I16 const *last_stage_twi_table, I16 *bitrev_buf, I32 bitrev_flag);
void CEVA_TL4_fft_compl_32(I32 log2_buf_len, I32 *in_buf, I32 *out_buf, I32 const *twi_table, I32 *bitrev_buf, I32 bitrev_flag);
void CEVA_TL4_fft_compl_inv_32(I32 log2_buf_len, I32 *in_buf, I32 *out_buf, I32 const *twi_table, I32 *bitrev_buf, I32 bitrev_flag);
void CEVA_TL4_fft_real_32_asm(I32 log2_buf_len, I32 *in_buf, I32 *out_buf, I32 const *twi_table, I32 const *last_stage_twi_table, I32 *bitrev_buf, I32 bitrev_flag);
void CEVA_TL4_fft_real_32_411(I32 log2_buf_len, I32 *in_buf, I32 *out_buf, I32 const *twi_table, I32 const *last_stage_twi_table, I32 *bitrev_buf, I32 bitrev_flag);
void CEVA_TL4_fft_real_inv_32_asm(I32 log2_buf_len, I32 *in_buf, I32 *out_buf, I32 const *twi_table, I32 const *last_stage_twi_table, I32 *bitrev_buf, I32 bitrev_flag);
void CEVA_TL4_BitRev_int16(I16 data[], I16 data_out[], I32 nLog2np, I16 tmp[]);
void CEVA_TL4_BitRev_int32(I32 data[], I32 data_out[], I32 nLog2np, I32 tmp[]);

void CEVA_TL4_fir_xcorr16(I32 size_buf_coef, I16 *h, I16 *y, I32 size_buf, I16 *x);
void CEVA_TL4_fir_xcorr16_mac3(I32 size_buf_coef, I16 *h, I16 *y, I32 size_buf, I16 *x);
void CEVA_TL4_fir_xcorr16_single_mac(I32 size_buf_coef, I16 *h, I16 *y, I32 size_buf, I16 *x);
void CEVA_TL4_fir_xcorr16_mac3_tl411vc(I32 size_buf_coef, I16 *h, I16 *y, I32 size_buf, I16 *x );
void CEVA_TL4_fir_xcorr32(I32 *x, I32 *h, I32 *y, I32 size_buf, I32 size_buf_coef, I32 shift_val);
void CEVA_TL4_fir_xcorr32_mac3(I32 *x, I32 *h, I32 *y, I32 size_buf, I32 size_buf_coef, I32 shift_val);
void CEVA_TL4_fir_convol32(I32 *in_buffer, I32 *out_buffer, I32 *State_buf, I32 *filter_coeffs_table, I32 Vec_size, I32 N_Coeffs, I32 shift_val);
void CEVA_TL4_fir_convol32_mac3(I32 *in_buffer, I32 *out_buffer, I32 *State_buf, I32 *filter_coeffs_table, I32 Vec_size, I32 N_Coeffs, I32 shift_val);
void CEVA_TL4_fir_convol16(I32 Vec_size, I32 N_Coeffs, I16 *State_buf, I16 *filter_coeffs_table, I16 *out_buffer, I16 *in_buffer);
void CEVA_TL4_fir_convol16_mac3(I32 Vec_size, I32 N_Coeffs, I16 *State_buf, I16 *filter_coeffs_table, I16 *out_buffer, I16 *in_buffer);
void CEVA_TL4_fir_convol16_tl411a(I32 Vec_size, I32 N_Coeffs, I16 *State_buf, I16 *filter_coeffs_table, I16 *out_buffer, I16 *in_buffer);
void CEVA_TL4_fir_convol16_mac3_tl411vc(I32 Vec_size, I32 N_Coeffs, I16 *State_buf, I16 *filter_coeffs_table, I16 *out_buffer, I16 *in_buffer);
void CEVA_TL4_fir_convol16_single_mac(I32 Vec_size, I32 N_Coeffs, I16 *State_buf, I16 *filter_coeffs_table, I16 *out_buffer, I16 *in_buffer);
void CEVA_TL4_fir_dec32(I32 *in_buffer, I32 *out_buffer, I32 *State_buf, I32 *filter_coeffs_table, I32 dec_val, I32 Vec_size, I32 N_Coeffs, I32 shift_val);
void CEVA_TL4_fir_dec32_mac3(I32 *in_buffer, I32 *out_buffer, I32 *State_buf, I32 *filter_coeffs_table, I32 dec_val, I32 Vec_size, I32 N_Coeffs, I32 shift_val);
void CEVA_TL4_fir_dec16(I16 *in_buffer, I16 *out_buffer, I16 *State_buf, I16 *filter_coeffs_table, I32 dec_val, I32 Vec_size, I32 N_Coeffs);
void CEVA_TL4_fir_dec16_mac3(I16 *in_buffer, I16 *out_buffer, I16 *State_buf, I16 *filter_coeffs_table, I32 dec_val, I32 Vec_size, I32 N_Coeffs);
void CEVA_TL4_fir_dec16_mac3_tl411(I16 *in_buffer, I16 *out_buffer, I16 *State_buf, I16 *filter_coeffs_table, I32 dec_val, I32 Vec_size, I32 N_Coeffs);
void CEVA_TL4_fir_interp32(I16 interp_val, I16 Vec_size, I16 N_Coeffs, I16 T_D_ratio, I16 shift_val, I32 *in_buffer, I32 *out_buffer, I32 *State_buf, I32 *filter_coeffs_table);
void CEVA_TL4_fir_interp16(I16 interp_val, I16 Vec_size, I16 N_Coeffs, I16 T_D_ratio, I16 *in_buffer, I16 *out_buffer, I16 *State_buf, I16 *filter_coeffs_table);
void CEVA_TL4_fir_blms16(I16 nInputs_len, I32 adaptRate, I16 T_len, I16 *input,I16 *desired,I16 *StateAddr,I16 *CoefAddr, I16 *output,I16 *error);
void CEVA_TL4_fir_blms16_mac3(I16 nInputs_len, I32 adaptRate, I16 T_len, I16 *input,I16 *desired,I16 *StateAddr,I16 *CoefAddr, I16 *output,I16 *error);
void CEVA_TL4_fir_blms16_mac3_tl411(I16 nInputs_len, I32 adaptRate, I16 T_len, I16 *input,I16 *desired,I16 *StateAddr,I16 *CoefAddr, I16 *output,I16 *error);
void CEVA_TL4_fir_blms32(I16 nInputs_len, I32 adaptRate, I16 T_len, I16 sh_val, I32 *input,I32 *desired,I32 *StateAddr,I32 *CoefAddr, I32 *output,I32 *error);
void CEVA_TL4_fir_blms32_mac3(I16 nInputs_len, I32 adaptRate, I16 T_len, I16 sh_val, I32 *input,I32 *desired,I32 *StateAddr,I32 *CoefAddr, I32 *output,I32 *error);
void CEVA_TL4_fir_dlms16(I32 adaptRate, I16 T_len, I16 input,I16 desired,I16 *StateAddr,I16 *CoefAddr, I16 *output,I16 *error);
void CEVA_TL4_fir_dlms16_mac3(I32 adaptRate, I16 T_len, I16 input,I16 desired,I16 *StateAddr,I16 *CoefAddr, I16 *output,I16 *error);
void CEVA_TL4_fir_dlms16_mac3_tl411(I32 adaptRate, I16 T_len, I16 input,I16 desired,I16 *StateAddr,I16 *CoefAddr, I16 *output,I16 *error);
void CEVA_TL4_fir_dlms16_tl411a(I32 adaptRate, I16 T_len, I16 input,I16 desired,I16 *StateAddr,I16 *CoefAddr, I16 *output,I16 *error);
void CEVA_TL4_fir_dlms32(I32 adaptRate, I16 T_len, I16 sh_val, I32 input,I32 *StateAddr,I32 *CoefAddr, I32 *output,I32 *error, I32 desired);
void CEVA_TL4_fir_dlms32_mac3(I32 adaptRate, I16 T_len, I16 sh_val, I32 input,I32 *StateAddr,I32 *CoefAddr, I32 *output,I32 *error,I32 desired);
void CEVA_TL4_fir_sr16_410A(I32 size_table, I32 buf_len, I16 const *in_buf, I16 *out_buf, I16 const *coeff);
void CEVA_TL4_fir_sr16_411A(I32 size_table, I32 buf_len, I16 const *in_buf, I16 *out_buf, I16 const *coeff);
void CEVA_TL4_fir_sr16_VC(I32 size_table, I32 buf_len, I16 const *in_buf, I16 *out_buf, I16 const *coeff);
I16 CEVA_TL4_fir_ss16_410A(I32 size_buf, I16 sample, I16 *state, I16 const *coeff);
I16 CEVA_TL4_fir_ss16_410VC(I32 size_buf, I16 sample, I16 *state, I16 const *coeff);
I16 CEVA_TL4_fir_ss16_411A(I32 size_buf, I16 sample, I16 *state, I16 const *coeff);
I16 CEVA_TL4_fir_ss16_411VC(I32 size_buf, I16 sample, I16 *state, I16 const *coeff);
void CEVA_TL4_fir_sr32_410(I32 size_table, I32 buf_len, I32 const *in_buf, I32 *out_buf, I32 const *coeff, I32 shift_val);
void CEVA_TL4_fir_sr32_411(I32 size_table, I32 buf_len, I32 const *in_buf, I32 *out_buf, I32 const *coeff, I32 shift_val);
I32 CEVA_TL4_fir_ss32_410(I32 size_table, I32 sample, I32 *state, I32 const *coeff, I32 shift_val);
I32 CEVA_TL4_fir_ss32_411(I32 size_table, I32 sample, I32 *state, I32 const *coeff, I32 shift_val);

void CEVA_TL4_iir_bqc32(I32 buf_len, I32 shift_val, I32 *x,	I32 *y,	I32 *a_b, I32 *x_scr, I32 *y_scr);
void CEVA_TL4_iir_bqd32_410(I32 buf_len, I32 shift_val, I32 *x,	I32 *y,	I32 *a_b, I32 *x_scr, I32 *y_scr);
void CEVA_TL4_iir_bqd32_411(I32 buf_len, I32 shift_val, I32 *x,	I32 *y,	I32 *a_b, I32 *x_scr, I32 *y_scr);


void CEVA_TL4_fir_acorr32(I32 *input, I32 *output, I32 input_length, I32 output_length, I16 shift_m);
void CEVA_TL4_fir_acorr16(I16 *input, I16 *output, I16 input_length, I16 output_length);
void CEVA_TL4_fir_acorr16_tl411(I16 *input, I16 *output, I16 input_length, I16 output_length);
void CEVA_TL4_fir_acorr16_mac3(I16 *input, I16 *output, I16 input_length, I16 output_length, I16 *temp);
void CEVA_TL4_fir_acorr16_mac3_tl411(I16 *input, I16 *output, I16 input_length, I16 output_length, I16 *temp_buf);
void CEVA_TL4_fir_acorr16_single_mac(I16 *input, I16 *output, I16 input_length, I16 output_length);

#ifndef WIN32
I32 CEVA_TL4_pow2_int32_asm(I32 arg_in, I16 arg_exp_in, I16 *arg_exp_out);
I32 CEVA_TL4_pow10_int32_asm(I32 arg_in, I16 arg_exp_in, I16 *arg_exp_out);
I32 CEVA_TL4_pow_int32_asm(I32 arg_in_X, I16 arg_exp_in_X, I32 arg_in_Y, I16 arg_exp_in_Y, I16 *arg_exp_out);
I32 CEVA_TL4_isqrt32_asm(I32 arg_in);
I16 CEVA_TL4_isqrt16_asm(I16 arg_in);
U16 CEVA_TL4_sqrt_int32_asm(U32 val, U16 rnd_flag);
I16 CEVA_TL4_sqrt_int16_asm(U16 inp_val, U16 rnd_flag);
I16 CEVA_TL4_div16_ss_asm(I16 inp_val1,I16 inp_val2);
I32 CEVA_TL4_div32_ss_asm(I32 inp_val1,I32 inp_val2);
U16 CEVA_TL4_div16_uu_asm(U16 inp_val1,U16 inp_val2);
U32 CEVA_TL4_div32_uu_asm(U32 inp_val1,U32 inp_val2);
I32 CEVA_TL4_log10_int32_asm(I32 arg_in, I16 arg_exp_in, I16 *arg_exp_out);
I32 CEVA_TL4_log2_int32_asm(I32 arg_in, I16 arg_exp_in, I16 *arg_exp_out);
I32 CEVA_TL4_logn_int32_asm(I32 arg_in, I16 arg_exp_in, I16 *arg_exp_out);
I16 CEVA_TL4_cos_int16_asm(I16 arg);
I32 CEVA_TL4_cos_int32_asm(I32 arg);
I16 CEVA_TL4_sin_int16_asm(I16 arg);
I32 CEVA_TL4_sin_int32_asm(I32 arg);
I16 CEVA_TL4_tan_int16_asm(I16 arg);
I32 CEVA_TL4_tan_int32_asm(I32 arg);
I16 CEVA_TL4_atan_int16_asm(I16 arg);
I32 CEVA_TL4_atan_int32_asm(I32 arg);

I16 CEVA_TL4_vec_max16_with_index_asm(I16 *ind, I16 *buf, I16 size_buf, I16 flag);
I32 CEVA_TL4_vec_max32_with_index_asm(I16 *ind, I32 *buf, I16 size_buf, I16 flag);
I16 CEVA_TL4_vec_min16_with_index_asm(I16 *ind, I16 *buf, I16 size_buf, I16 flag);
I32 CEVA_TL4_vec_min32_with_index_asm(I16 *ind, I32 *buf, I16 size_buf, I16 flag);
I16 CEVA_TL4_vec_max_abs16_with_index_asm(I16 *ind, I16 *buf, I16 size_buf, I16 flag);
I32 CEVA_TL4_vec_max_abs32_with_index_asm(I16 *ind, I32 *buf, I16 size_buf, I16 flag);
I16 CEVA_TL4_vec_max16_asm(I16 *buf, I16 size_buf);
I32 CEVA_TL4_vec_max32_asm(I32 *buf, I16 size_buf);
I16 CEVA_TL4_vec_min16_asm(I16 *buf, I16 size_buf);
I32 CEVA_TL4_vec_min32_asm(I32 *buf, I16 size_buf);
I16 CEVA_TL4_vec_max_abs16_asm(I16 *buf, I16 size_buf);
I32 CEVA_TL4_vec_max_abs32_asm(I32 *buf, I16 size_buf);
void CEVA_TL4_vec_add16_asm(I16 *inp1, I16 *inp2, I16 size_buf, I16 *out);
void CEVA_TL4_vec_sub16_asm(I16 *inp1, I16 *inp2, I16 size_buf, I16 *out);
void CEVA_TL4_vec_sub32_asm(I32 *inp1, I32 *inp2, I16 size_buf, I32 *out);
void CEVA_TL4_vec_add32_asm(I32 *inp1, I32 *inp2, I16 size_buf, I32 *out);
void CEVA_TL4_vec_shf16_asm(I16 *inp, I16 shift_val, I16 size_buf, I16 *out);
void CEVA_TL4_vec_shf32_asm(I32 *inp, I16 shift_val, I16 size_buf, I32 *out);
void CEVA_TL4_vec_shf16_no_saturation_asm(I16 *inp, I16 shift_val, I16 size_buf, I16 *out);
void CEVA_TL4_vec_shf32_no_saturation_asm(I32 *inp, I16 shift_val, I16 size_buf, I32 *out);
void CEVA_TL4_vec_mul16_asm(I16 *inp1, I16 *inp2, I16 size_buf, I16 scale, I16 *out);
void CEVA_TL4_vec_mul32_asm(I32 *inp1, I32 *inp2, I16 size_buf, I16 scale, I32 *out);
void CEVA_TL4_vec_mul32_rnd_asm(I32 *inp1, I32 *inp2, I16 size_buf, I16 scale, I32 *out);
acc_t CEVA_TL4_vec_dot32_asm(I32 *inp1, I32 *inp2, I16 size_buf, I16 scale);
acc_t CEVA_TL4_vec_dot32rnd_asm(I32 *inp1, I32 *inp2, I16 size_buf, I16 scale);
acc_t CEVA_TL4_vec_dot16_asm(I16 *inp1, I16 *inp2, I16 size_buf, I16 scale);

void CEVA_TL4_fft_compl_16_asm(I32 log2_buf_len, I16 *in_buf, I16 *out_buf, I16 const *twi_table, I16 *bitrev_buf, I32 bitrev_flag);
void CEVA_TL4_fft_compl_inv_16_asm(I32 log2_buf_len, I16 *in_buf, I16 *out_buf, I16 const *twi_table, I16 *bitrev_buf, I32 bitrev_flag);
void CEVA_TL4_fft_compl_32_asm(I32 log2_buf_len, I32 *in_buf, I32 *out_buf, I32 const *twi_table, I32 *bitrev_buf, I32 bitrev_flag);
void CEVA_TL4_fft_compl_inv_32_asm(I32 log2_buf_len, I32 *in_buf, I32 *out_buf, I32 const *twi_table, I32 *bitrev_buf, I32 bitrev_flag);
void CEVA_TL4_fft_real_16_asm(I32 log2_buf_len, I16 *in_buf, I16 *out_buf, I16 const *twi_table, I16 const *last_stage_twi_table, I16 *bitrev_buf, I32 bitrev_flag);
void CEVA_TL4_fft_real_inv_16_asm(I32 log2_buf_len, I16 *in_buf, I16 *out_buf, I16 const *twi_table, I16 const *last_stage_twi_table, I16 *bitrev_buf, I32 bitrev_flag);
void CEVA_TL4_fft_real_32_asm(I32 log2_buf_len, I32 *in_buf, I32 *out_buf, I32 const *twi_table, I32 const *last_stage_twi_table, I32 *bitrev_buf, I32 bitrev_flag);
void CEVA_TL4_fft_real_inv_32_asm(I32 log2_buf_len, I32 *in_buf, I32 *out_buf, I32 const *twi_table, I32 const *last_stage_twi_table, I32 *bitrev_buf, I32 bitrev_flag);
void CEVA_TL4_BitRev_int16_asm(I16 data[], I16 data_out[], I32 nLog2np, I16 tmp[]);
void CEVA_TL4_BitRev_int32_asm(I32 data[], I32 data_out[], I32 nLog2np, I32 tmp[]);

void CEVA_TL4_fir_xcorr16_asm(I32 size_buf_coef, I16 *h, I16 *y, I32 size_buf, I16 *x);
void CEVA_TL4_fir_xcorr32_asm(I32 *x, I32 *h, I32 *y, I32 size_buf, I32 size_buf_coef, I32 shift_val);
void CEVA_TL4_fir_xcorr16_mac3_asm(I16 *x, I16 *h, I16 *y, I32 size_buf, I32 size_buf_coef);
void CEVA_TL4_fir_xcorr16_single_mac_asm(I32 size_buf_coef, I16 *h, I16 *y, I32 size_buf, I16 *x);
void CEVA_TL4_fir_xcorr32_mac3_asm(I32 *x, I32 *h, I32 *y, I32 size_buf, I32 size_buf_coef, I32 shift_val);
void CEVA_TL4_fir_convol32_asm(I32 *in_buffer, I32 *out_buffer, I32 *State_buf, I32 *filter_coeffs_table, I32 Vec_size, I32 N_Coeffs, I32 shift_val);
void CEVA_TL4_fir_convol16_single_mac_asm(I32 Vec_size, I32 N_Coeffs, I16 *State_buf, I16 *filter_coeffs_table, I16 *out_buffer, I16 *in_buffer);
void CEVA_TL4_fir_convol16_asm(I32 Vec_size, I32 N_Coeffs, I16 *State_buf, I16 *filter_coeffs_table, I16 *out_buffer, I16 *in_buffer);
void CEVA_TL4_fir_dec32_asm(I32 *in_buffer, I32 *out_buffer, I32 *State_buf, I32 *filter_coeffs_table, I32 dec_val, I32 Vec_size, I32 N_Coeffs, I32 shift_val);
void CEVA_TL4_fir_dec16_asm(I16 *in_buffer, I16 *out_buffer, I16 *State_buf, I16 *filter_coeffs_table, I32 dec_val, I32 Vec_size, I32 N_Coeffs);
void CEVA_TL4_fir_interp32_asm(I16 interp_val, I16 Vec_size, I16 N_Coeffs, I16 T_D_ratio, I16 shift_val, I32 *in_buffer, I32 *out_buffer, I32 *State_buf, I32 *filter_coeffs_table);
#if (_TL_410V | _TL_410C | _TL_411A | _TL_411V | _TL_411C)
		void CEVA_TL4_fir_interp16_asm(I16 interp_val, I16 Vec_size, I16 N_Coeffs, I16 T_D_ratio, I16 *in_buffer_asm, I16 *out_buffer, I16 *State_buf_asm, I16 *filter_coeffs_table, I16 *temp_buffer);
#else
		void CEVA_TL4_fir_interp16_asm(I16 interp_val, I16 Vec_size, I16 N_Coeffs, I16 T_D_ratio, I16 *in_buffer_asm, I16 *out_buffer, I16 *State_buf_asm, I16 *filter_coeffs_table);
#endif


void CEVA_TL4_fir_blms16_asm(I16 nInputs_len, I32 adaptRate, I16 T_len, I16 *input,I16 *desired,I16 *StateAddr,I16 *CoefAddr, I16 *output,I16 *error);
void CEVA_TL4_fir_blms32_asm(I16 nInputs_len, I32 adaptRate, I16 T_len, I16 sh_val, I32 *input,I32 *desired,I32 *StateAddr,I32 *CoefAddr, I32 *output,I32 *error);
void CEVA_TL4_fir_dlms16_asm(I32 adaptRate, I16 T_len, I16 input,I16 desired,I16 *StateAddr,I16 *CoefAddr, I16 *output,I16 *error);
void CEVA_TL4_fir_dlms32_asm(I32 adaptRate, I16 T_len, I16 sh_val, I32 input,I32 *StateAddr,I32 *CoefAddr, I32 *output,I32 *error,I32 desired);
void CEVA_TL4_fir_sr16_asm(I32 size_table, I32 buf_len, I16 const *in_buf, I16 *out_buf, I16 const *coeff);
I16 CEVA_TL4_fir_ss16_asm(I32 size_buf, I16 sample, I16 *state, I16 const *coeff);
void CEVA_TL4_fir_sr32_asm(I32 size_table, I32 buf_len, I32 const *in_buf, I32 *out_buf, I32 const *coeff, I32 shift_val);
I32 CEVA_TL4_fir_ss32_asm(I32 size_table, I32 sample, I32 *state, I32 const *coeff, I32 shift_val);
void CEVA_TL4_iir_bqc32_asm(I32 buf_len, I32 shift_val, I32 *x,	I32 *y,	I32 *a_b, I32 *x_scr, I32 *y_scr);
void CEVA_TL4_iir_bqd32_asm(I32 buf_len, I32 shift_val, I32 *x,	I32 *y,	I32 *a_b, I32 *x_scr, I32 *y_scr);
void CEVA_TL4_fir_acorr32_asm(I32 *input, I32 *output, I32 input_length, I32 output_length, I32 shift_m);

#if (_TL_411V | _TL_411C | _TL_410V | _TL_410C)
void CEVA_TL4_fir_acorr16_asm(I16 *input, I16 *output, I16 input_length, I16 output_length, I16 *temp_buf);
#else
void CEVA_TL4_fir_acorr16_asm(I16 *input, I16 *output, I16 input_length, I16 output_length);
#endif
void CEVA_TL4_fir_acorr16_single_mac_asm(I16 *input, I16 *output, I16 input_length, I16 output_length);

#endif

#endif	
