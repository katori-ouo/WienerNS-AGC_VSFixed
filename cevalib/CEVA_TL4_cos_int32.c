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

#define FIXED_ONE_32		0x20000000
#define FIXED_ONE_EXP_32	29

// =============================================================================================
// =                              cosine approximation [-pi...pi]                              =
// =============================================================================================
I32 CEVA_TL4_cos_int32(I32 arg)
{
	I32 cos_res;
	I32 arg_sqr;

	I32 pi;
	I32 flag;
	I32 a2, a4, a6, a8, a10;

	a2 = 0xc0000008;	//(I32)((double)-0.4999999963*(0x80000000));	//<<2;
	a4 = 0x2aaaa8ff;	//(I32)((double) 0.0416666418*(0x400000000));	//<<5;
	a6 = 0xf49f645e;	//(I32)((double)-0.0013888397*(0x2000000000));	//<<8;
	a8 = 0x019f6b41;	//(I32)((double) 0.0000247609*(0x10000000000));	//<<11;
	a10= 0xfffba12a;	//(I32)((double)-0.0000002605*(0x10000000000));	//<<11;
	pi = 0x6487ed51;	//(I32)(((double)3.1415926535897932384626433832795)*FIXED_ONE_32);

	flag = 0;
	if (arg < 0)
		arg = -arg;

	if (arg > pi>>1)
	{
		arg = pi-arg;
		flag = 1;
	}

	arg_sqr = ((long long)arg * arg)>>(FIXED_ONE_EXP_32);
	cos_res = ((long long)arg_sqr * a10)>>(FIXED_ONE_EXP_32);

	cos_res += a8;
	cos_res = ((long long)cos_res*arg_sqr)>>(FIXED_ONE_EXP_32+3);

	cos_res += a6;
	cos_res = ((long long)cos_res*arg_sqr)>>(FIXED_ONE_EXP_32+3);

	cos_res += a4;
	cos_res = ((long long)cos_res*arg_sqr)>>(FIXED_ONE_EXP_32+3);

	cos_res += a2;
	cos_res = ((long long)cos_res*arg_sqr)>>(FIXED_ONE_EXP_32+2);

	cos_res += FIXED_ONE_32;

	if (flag)
		cos_res = - cos_res;

	return((int)cos_res);
}
