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


void CEVA_TL4_fft_compl_inv_32(I32 log2_buf_len,
                               I32 *in_buf,
                               I32 *out_buf,
                               I32 *twi_table,
                               I32 *bitrev_buf,
                               I32 bitrev_flag)
{
	I32 i, j, k, bg, gp;
	I32 fftn = 1 << (log2_buf_len + 1);
	I32 ar, ai, br, bi, cr, ci;
	I32 rtemp, itemp;
	I32 *ptr_cos_sin;
	I32 *ptr_in_0,  *ptr_in_1;
	I32 *ptr_out_0, *ptr_out_1;

	// Do the first radix-2 pass
	ptr_in_0 = in_buf + (0 * fftn / 2);
	ptr_in_1 = in_buf + (1 * fftn / 2);

	ptr_out_0 = out_buf + (0 * fftn / 2);
	ptr_out_1 = out_buf + (1 * fftn / 2);

	for(i = 0; i < fftn / 4; i++)
	{
		ar = *ptr_in_0++;
		ai = *ptr_in_0++;
		br = *ptr_in_1++;
		bi = *ptr_in_1++;

		*ptr_out_0++ = ar + br;
		*ptr_out_0++ = ai + bi;
		*ptr_out_1++ = ar - br;
		*ptr_out_1++ = ai - bi;
	}

	// Do the second radix-2 pass
	ptr_out_0 = out_buf + (0 * fftn / 4);
	ptr_out_1 = out_buf + (1 * fftn / 4);

	for(i = 0; i < fftn / 8; i++)
	{
		ar = ptr_out_0[0];
		ai = ptr_out_0[1];
		br = ptr_out_1[0];
		bi = ptr_out_1[1];

		*ptr_out_0++ = ar + br;
		*ptr_out_0++ = ai + bi;
		*ptr_out_1++ = ar - br;
		*ptr_out_1++ = ai - bi;
	}

	ptr_out_0 = out_buf + (2 * fftn / 4);
	ptr_out_1 = out_buf + (3 * fftn / 4);

	for(i = 0; i < fftn / 8; i++)
	{
		ar = ptr_out_0[0];
		ai = ptr_out_0[1];
		br = ptr_out_1[0];
		bi = ptr_out_1[1];

		*ptr_out_0++ = ar - bi;
		*ptr_out_0++ = ai + br;
		*ptr_out_1++ = ar + bi;
		*ptr_out_1++ = ai - br;
	}

	// Do all radix-2 passes except the first two and last
	bg = fftn / 8;					// butterflies per group
	gp = 4;							// groups per pass

	for(k = 0; k < log2_buf_len - 3; k++)
	{
		ptr_out_0 = out_buf;
		ptr_out_1 = out_buf + bg;
		ptr_cos_sin = twi_table;

		for(j = 0; j < gp; j++)
		{
			cr = *ptr_cos_sin++;
			ci = *ptr_cos_sin++;

			for(i = 0; i < bg / 2; i++)
			{
				ar = ptr_out_0[0];
				ai = ptr_out_0[1];
				br = ptr_out_1[0];
				bi = ptr_out_1[1];

				rtemp = (I32)L_mult_sh(br, cr, 31) + (I32)L_mult_sh(bi, ci, 31);
				itemp = (I32)L_mult_sh(bi, cr, 31) - (I32)L_mult_sh(br, ci, 31);

				*ptr_out_0++ = ar + rtemp;
				*ptr_out_0++ = ai + itemp;
				*ptr_out_1++ = ar - rtemp;
				*ptr_out_1++ = ai - itemp;
			}

			ptr_out_0 += bg;
			ptr_out_1 += bg;
		}
		bg >>= 1;
		gp <<= 1;
	}

	// Do the last radix-2 pass
	ptr_cos_sin = twi_table;
	ptr_out_0 = out_buf;
	ptr_out_1 = out_buf;

	for(i = 0; i < fftn / 4; i++)
	{
		ar = *ptr_out_0++;
		ai = *ptr_out_0++;
		br = *ptr_out_0++;
		bi = *ptr_out_0++;

		cr = *ptr_cos_sin++;
		ci = *ptr_cos_sin++;

		rtemp = (I32)L_mult_sh(br, cr, 31) + (I32)L_mult_sh(bi, ci, 31);
		itemp = (I32)L_mult_sh(bi, cr, 31) - (I32)L_mult_sh(br, ci, 31);

		*ptr_out_1++ = ar + rtemp;
		*ptr_out_1++ = ai + itemp;
		*ptr_out_1++ = ar - rtemp;
		*ptr_out_1++ = ai - itemp;
	}

	// Do bitrev if it is needed
	if(bitrev_flag)
		CEVA_TL4_BitRev_int32(out_buf, out_buf, log2_buf_len, bitrev_buf);
}
