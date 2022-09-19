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

#include "CEVA_typedef.h"
#include "CEVA_TL4_basic_op.h"


void CEVA_TL4_BitRev_int32(I32 data[], I32 data_out[], I32 nLog2np, I32 tmp[]);
void CEVA_TL4_fft_compl_32(I32 log2_buf_len, I32 *in_buf, I32 *out_buf, I32 const *twi_table, I32 *bitrev_buf, I32 bitrev_flag);


//--------------------------------------------------------------------
void CEVA_TL4_fft_real_32_asm(I32 log2_buf_len,
							  I32 *in_buf,
							  I32 *out_buf,
							  I32 const *twi_table,
							  I32 const *last_stage_twi_table,
							  I32 *bitrev_buf,
							  I32 bitrev_flag)
{
	I32 i;
	I32 fftn = 1 << log2_buf_len;
	I32 ar, ai, br, bi, cr, ci;
	I32 re1, im1, re2, im2, rtemp, itemp;
	I32 const *ptr_cos_sin;
	I32 *ptr_out_0, *ptr_out_1;


	// Do (log2_buf_len - 1) complex fft without bitrev
	CEVA_TL4_fft_compl_32(log2_buf_len - 1, in_buf, out_buf, twi_table, bitrev_buf, 1);

	// Do the first part of the last stage
	ptr_out_0 = out_buf + 2;
	ptr_out_1 = out_buf + fftn - 1;

 	for(i = 1; i < fftn / 4; i++)
	{
		ar = ptr_out_0[0];
		ai = ptr_out_0[1];
		br = ptr_out_1[-1];
		bi = ptr_out_1[0];

		re1 = (I32)L_mult_sh(ar, 0x40000000, 31) + (I32)L_mult_sh(br, 0x40000000, 31);
		im1 = (I32)L_mult_sh(ai, 0x40000000, 31) - (I32)L_mult_sh(bi, 0x40000000, 31);

		re2 = bi + ai;
		im2 = br - ar;

		*ptr_out_0++ = im1;
		*ptr_out_0++ = re1;
		*ptr_out_1-- = im2;
		*ptr_out_1-- = re2;
	}

	// Do the second part of the last stage
	ptr_out_0 = out_buf;
	ptr_out_1 = out_buf + fftn + 1;
	ptr_cos_sin = last_stage_twi_table + 2;

	ar = ptr_out_0[0];
	ai = ptr_out_0[1];

	*ptr_out_0++ = ar + ai;
	*ptr_out_0++ = 0;
	*ptr_out_1-- = 0;
	*ptr_out_1-- = ar - ai;

 	for(i = 1; i < fftn / 4; i++)
	{
		ai = ptr_out_0[0];
		ar = ptr_out_0[1];
		br = ptr_out_1[-1];
		bi = ptr_out_1[0];

		cr = *ptr_cos_sin++;
		ci = *ptr_cos_sin++;

		rtemp = (I32)L_mult_sh(br, cr, 31) + (I32)L_mult_sh(bi, ci, 31);
		itemp = (I32)L_mult_sh(bi, cr, 31) - (I32)L_mult_sh(br, ci, 31);

		*ptr_out_0++ =  ar + rtemp;
		*ptr_out_0++ =  ai + itemp;

		*ptr_out_1-- = -ai + itemp;
		*ptr_out_1-- =  ar - rtemp;
	}

	ptr_out_0[1] = -ptr_out_0[1];

	// Completion of the second half of the array
	ptr_out_0 = out_buf + 2;
	ptr_out_1 = out_buf - 1 + 2 * fftn;
 	for(i = 1; i < fftn / 2; i++)
	{
		ar = *ptr_out_0++;
		ai = *ptr_out_0++;

		*ptr_out_1-- = -ai;
		*ptr_out_1-- =  ar;
	}

	//if(bitrev_flag)
	//	CEVA_TL4_BitRev_int32(out_buf, out_buf, log2_buf_len, bitrev_buf);
}

//--------------------------------------------------------------------
void CEVA_TL4_fft_real_32_411(I32 log2_buf_len,
							  I32 *in_buf,
							  I32 *out_buf,
							  I32 const *twi_table,
							  I32 const *last_stage_twi_table,
							  I32 *bitrev_buf,
							  I32 bitrev_flag)
{
	I32 i;
	I32 fftn = 1 << log2_buf_len;
	I32 ar, ai, br, bi, cr, ci;
	I32 re1, im1, re2, im2, rtemp, itemp;
	I32 const *ptr_cos_sin;
	I32 *ptr_out_0, *ptr_out_1;
	I32 *ptr_out_2, *ptr_out_3;

	// Do (log2_buf_len - 1) complex fft without bitrev
	CEVA_TL4_fft_compl_32(log2_buf_len - 1, in_buf, out_buf, twi_table, bitrev_buf, 1);

	// Do the first part of the last stage
	ptr_out_0 = out_buf + 2;
	ptr_out_1 = out_buf + fftn - 1;

 	for(i = 1; i < fftn / 4; i++)
	{
		ar = ptr_out_0[0];
		ai = ptr_out_0[1];
		br = ptr_out_1[-1];
		bi = ptr_out_1[0];

		re1 = (I32)L_mult_sh(ar, 0x40000000, 31) + (I32)L_mult_sh(br, 0x40000000, 31);
		im1 = (I32)L_mult_sh(ai, 0x40000000, 31) - (I32)L_mult_sh(bi, 0x40000000, 31);

		re2 = bi + ai;
		im2 = br - ar;

		*ptr_out_0++ = im1;
		*ptr_out_0++ = re1;
		*ptr_out_1-- = im2;
		*ptr_out_1-- = re2;
	}

	// Do the second part of the last stage
	ptr_out_0 = out_buf;
	ptr_out_1 = out_buf + fftn + 1;
	ptr_cos_sin = last_stage_twi_table + 2;

	ar = ptr_out_0[0];
	ai = ptr_out_0[1];

	*ptr_out_0++ = ar + ai;
	*ptr_out_0++ = 0;
	*ptr_out_1-- = 0;
	*ptr_out_1-- = ar - ai;
	ptr_out_2 = ptr_out_1 + 3;
	
 	for(i = 1; i < fftn / 4; i++)
	{
		ai = ptr_out_0[0];
		ar = ptr_out_0[1];
		br = ptr_out_1[-1];
		bi = ptr_out_1[0];

		cr = *ptr_cos_sin++;
		ci = *ptr_cos_sin++;

		rtemp = (I32)L_mult_sh(br, cr, 31) + (I32)L_mult_sh(bi, ci, 31);
		itemp = (I32)L_mult_sh(bi, cr, 31) - (I32)L_mult_sh(br, ci, 31);

		*ptr_out_0++ =  ar + rtemp;
		*ptr_out_0++ =  ai + itemp;

		ptr_out_1 -= 2;
		*ptr_out_2++ =  ar - rtemp;
		*ptr_out_2++ =  ai - itemp;
	}

	ptr_out_0[1] = -ptr_out_0[1];

	// Completion of the second half of the array
	ptr_out_0 = out_buf + 2;
	ptr_out_1 = out_buf - 1 + fftn;
	ptr_out_2 = out_buf + 2 + fftn;
	ptr_out_3 = out_buf - 1 + 2 * fftn;
 	for(i = 1; i < fftn / 4; i++)
	{
		ar = *ptr_out_0++;
		ai = *ptr_out_0++;

		*ptr_out_3-- = -ai;
		*ptr_out_3-- =  ar;

		ar = *ptr_out_2++;
		ai = *ptr_out_2++;

		*ptr_out_1-- = -ai;
		*ptr_out_1-- =  ar;
	}

	ar = *ptr_out_0++;
	ai = *ptr_out_0++;

	*ptr_out_3-- = -ai;
	*ptr_out_3-- =  ar;

	//if(bitrev_flag)
	//	CEVA_TL4_BitRev_int32(out_buf, out_buf, log2_buf_len, bitrev_buf);
}
