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



I32 CEVA_TL4_div32_ss_asm(I32 var1, I32 var2)
{
    int i;
    I32 L_div;
    I16 exp_num, exp_den;
	U32 num, den, exp;
	I32 sign = 0;
	U32 inp1,inp2;
	

	if (var1 < 0)
	{
		var1 = L_abs(var1);
		sign++;
	}
    if (var2 < 0)
	{
		var2 = L_abs(var2);
		sign++;
	}
	inp1 = (U32)var1;
	inp2 = (U32)var2;

    if (inp1 > inp2)
    {
        /* undefined output for invalid input into divide_s */
        return (0);
    }
    if (var1 == var2)
	{
		L_div = MAX_32;
		if (sign==1)
			L_div = MIN_32;
       return (L_div);
	}


	
    num = inp1;
    den = inp2;
    
	exp_num = norm_l(num);

	exp_den = norm_l(den);

    exp = exp_num - exp_den;

    num = num << exp;
 
    L_div = 0;
    if (num >= den)
    {
        num -= den;
        L_div = 1;
    }

    for(i=exp; i<31; i++)
    {
        L_div = L_div<<1;
        num <<= 1;
        if (num >= den)
        {
            num -= den;
            L_div += 1;
        }
    }

	if (sign==1)
		L_div = - L_div;


    return (L_div);
}



