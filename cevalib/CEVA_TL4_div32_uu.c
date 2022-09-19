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

#define DIV_BY_ZERO 0x7fffffff

U32 CEVA_TL4_div32_uu_asm(U32 var1, U32 var2)
{
#ifdef WIN32
	U32 L_div;

	if (var2!=0)
		L_div = var1/var2;
	else
		L_div = DIV_BY_ZERO;

	return (L_div);
#else
    int i;
    U32 L_div;
    I16 exp_num, exp_den, exp;
	U32 num, den;
	acc_t num_t, den_t;

    if (var1 < var2)
    {
        /* undefined output for invalid input into divide_s */
        return (0);
    }

	if (var2==0)
		return DIV_BY_ZERO;

    num = var1;
    den = var2;
    
	if (num & 0x80000000)
		exp_num = -1;
	else
		exp_num = norm_l(num);

	if (den & 0x80000000)
		exp_den = -1;
	else
		exp_den = norm_l(den);

    exp = exp_den - exp_num;


    den_t = (acc_t)den << exp;
    num_t = (acc_t)num;

    L_div = 0;
    if (num_t >= den_t)
    {
        num_t -= den_t;
        L_div = 1;
    }

    for(i=0; i<exp; i++)
    {
        L_div = L_div<<1;
        num_t <<= 1;
        if (num_t >= den_t)
        {
            num_t -= den_t;
            L_div += 1;
        }
    }

    return (L_div);
#endif
}



