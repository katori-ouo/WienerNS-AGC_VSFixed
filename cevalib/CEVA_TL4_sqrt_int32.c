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


#ifdef WIN32

#include <math.h>


U16 CEVA_TL4_sqrt_int32_asm (
		U32 operand_be1,		/* input:  number to be square rooted	*/
		U16 rnd_flag
		)		
{
	/* declare local variables */
	int out_fl;

	if (rnd_flag==1)
		out_fl = (int) (pow((double)operand_be1,0.5)+0.5);
	else
		out_fl = (int) (pow((double)operand_be1,0.5));

	return (out_fl);
}

#else

acc_t CEVA_TL4_mpy_ll (U32 var1, U32 var2)
{
	int64 out = (int64)var1 * (int64)var2;
	return (acc_t) out;
}

U16 CEVA_TL4_sqrt_int32 (
		U32 operand_be1,		/* input:  number to be square rooted	*/
		U16 rnd_flag
		)		
{
	/* declare local variables */
	I32		testbit_be;		/* 16-bit integer for bit-exact operations */
	I32		guess_be;		/* 16-bit integer for bit-exact operations */
	U32		square_be;		/* 16-bit integer for bit-exact operations */
	I32		 p_result_be;
	U32	bit;
	I32 operand_be;


	/* initialize local variables */
	if (rnd_flag==1)
	{
		acc_t square_be, operand_be;
		acc_t guess_be;

		p_result_be = 0;
		testbit_be = 0x10000;
#ifdef WIN32
		operand_be = ((acc_t)operand_be1 << 2) & 0x3ffffffff;
#else
		operand_be = (acc_t)operand_be1 << 2;
#endif
		for(bit = 0; bit < 16; bit++)
		{
			guess_be = p_result_be + testbit_be;
			square_be =  CEVA_TL4_mpy_ll(guess_be, guess_be);
			if(operand_be >= square_be)
				p_result_be += testbit_be;
			testbit_be >>= 1;
		}

		guess_be = p_result_be + testbit_be;
		square_be =  CEVA_TL4_mpy_ll(guess_be, guess_be);
		if(operand_be >= square_be)
			p_result_be += testbit_be;

		p_result_be ++;
		p_result_be = p_result_be >> 1;
	}
	else
	{
		p_result_be = 0;
		testbit_be = 0x8000;

		operand_be = (U32)operand_be1 ;

		for(bit = 0; bit < 16; bit++)
		{
			guess_be = p_result_be + testbit_be;
			square_be =  guess_be * guess_be;
			if(operand_be >= square_be)
				p_result_be += testbit_be;
			testbit_be >>= 1;
		}
	}

	return (p_result_be);
}
#endif

