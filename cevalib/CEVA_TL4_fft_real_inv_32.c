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


void CEVA_TL4_fft_compl_inv_32(I32 log2_buf_len, I32 *in_buf, I32 *out_buf, I32 const *twi_table, I32 *bitrev_buf, I32 bitrev_flag);


void CEVA_TL4_fft_real_inv_32_asm(I32 log2_buf_len, 
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
	I32 rtemp, itemp;
	I32 const *ptr_cos_sin;
	I32 *ptr_in_0, *ptr_in_1;
	I32 *ptr_out_0, *ptr_out_1;

	// Do the first part of the first stage
	ptr_in_0 = in_buf;
	ptr_in_1 = in_buf + fftn;
	ptr_out_0 = out_buf;
	ptr_out_1 = out_buf + fftn - 1;

	ar = *ptr_in_0++;
	ptr_in_0++;
	br = *ptr_in_1--;

	*ptr_out_0++ = ar + br;
	*ptr_out_0++ = ar - br;

 	for(i = 1; i < fftn / 4; i++)
	{
		ar = *ptr_in_0++;
		ai = *ptr_in_0++;
		bi = *ptr_in_1--;
		br = *ptr_in_1--;

		*ptr_out_0++ = ar + br;
		*ptr_out_0++ = ai - bi;
		*ptr_out_1-- = ai + bi;
		*ptr_out_1-- = ar - br;
	}

	bi = *ptr_in_1--;
	br = *ptr_in_1--;
	ptr_out_0[0] =  br + br;
	ptr_out_0[1] = -bi - bi;

	// Do the second part of the first stage
	ptr_out_0 = out_buf + 2;
	ptr_out_1 = out_buf + fftn - 1;
	ptr_cos_sin = last_stage_twi_table + 2;

 	for(i = 1; i < fftn / 4; i++)
	{
		ar = ptr_out_0[0];
		ai = ptr_out_0[1];
		br = ptr_out_1[-1];
		bi = ptr_out_1[0];

		cr = *ptr_cos_sin++;
		ci = *ptr_cos_sin++;

		rtemp = (I32)L_mult_sh(br, cr, 30) - (I32)L_mult_sh(bi, ci, 30);
		itemp = (I32)L_mult_sh(bi, cr, 30) + (I32)L_mult_sh(br, ci, 30);

		*ptr_out_0++ =  ar - itemp;
		*ptr_out_0++ =  ai + rtemp;
		*ptr_out_1-- = -ai + rtemp;
		*ptr_out_1-- =  ar + itemp;
	}

	// Do (log2_buf_len - 1) complex inverse fft
	CEVA_TL4_fft_compl_inv_32(log2_buf_len - 1, out_buf, out_buf, twi_table, bitrev_buf, bitrev_flag);
}
