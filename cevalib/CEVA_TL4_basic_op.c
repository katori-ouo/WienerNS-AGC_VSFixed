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

/*                                                      v.2.1 - March 2006
  =============================================================================

                          U    U   GGG    SSSS  TTTTT
                          U    U  G       S       T
                          U    U  G  GG   SSSS    T
                          U    U  G   G       S   T
                           UUU     GG     SSS     T

                   ========================================
                    ITU-T - USER'S GROUP ON SOFTWARE TOOLS
                   ========================================

       =============================================================
       COPYRIGHT NOTE: This source code, and all of its derivations,
       is subject to the "ITU-T General Public License". Please have
       it  read  in    the  distribution  disk,   or  in  the  ITU-T
       Recommendation G.191 on "SOFTWARE TOOLS FOR SPEECH AND  AUDIO
       CODING STANDARDS".
       =============================================================

MODULE:         BASOP32, BASIC OPERATORS

ORIGINAL BY:
                Incorporated from anonymous contributions for 
                ETSI Standards as well as G.723.1, G.729, and G.722.1

DESCRIPTION:
        This file contains the definition of 16- and 32-bit basic
        operators to be used in the implementation of signal
        processing algorithms. The basic operators try to resemble
        assembly language instructions that are commonly found in
        digital signal processor (DSP) CPUs, thus allowing algorithm
        C-code implementations more directly mapeable to DSP assembly
        code.

        *********************************************************
         NOTE: so far, this module does not have a demo program!
        *********************************************************

FUNCTIONS:
  Defined in basop32.h. Self-documentation within each function.

HISTORY:
  26.Jan.00    v1.0     Incorporated to the STL from updated G.723.1/G.729 
                        basic operator library (based on basicop2.c) and 
                        G.723.1's basop.c [L_mls(), div_l(), i_mult()]

  05.Jul.00    v1.1     Added 32-bit shiftless accumulation basic 
                        operators (L_msu0, L_mac0, L_mult0). Improved
                        documentation for i_mult().

   03 Nov 04   v2.0     Incorporation of new 32-bit / 40-bit / control
                        operators for the ITU-T Standard Tool Library as 
                        described in Geneva, 20-30 January 2004 WP 3/16 Q10/16
                        TD 11 document and subsequent discussions on the
                        wp3audio@yahoogroups.com email reflector.
                        norm_s()      weight reduced from 15 to 1.
                        norm_l()      weight reduced from 30 to 1.
                        L_abs()       weight reduced from  2 to 1.
                        L_add()       weight reduced from  2 to 1.
                        L_negate()    weight reduced from  2 to 1.
                        L_shl()       weight reduced from  2 to 1.
                        L_shr()       weight reduced from  2 to 1.
                        L_sub()       weight reduced from  2 to 1.
                        mac_r()       weight reduced from  2 to 1.
                        msu_r()       weight reduced from  2 to 1.
                        mult_r()      weight reduced from  2 to 1.
                        L_deposit_h() weight reduced from  2 to 1.
                        L_deposit_l() weight reduced from  2 to 1.
   15 Nov 04   v2.0     L_mls() weight of 5.
						div_l() weight of 32.
						i_mult() weight of 3.

  =============================================================================
*/


/*___________________________________________________________________________
 |                                                                           |
 | Basic arithmetic operators.                                               |
 |                                                                           |
 | $Id $                                                                     |
 |                                                                           |
 |       saturate()                                                          |
 |       add()                                                               |
 |       sub()                                                               |
 |       abs_s()                                                             |
 |       divide_s()                                                          |
 |       extract_h()                                                         |
 |       extract_l()                                                         |
 |       L_abs()                                                             |
 |       L_add()                                                             |
 |       L_deposit_h()                                                       |
 |       L_deposit_l()                                                       |
 |       L_mac()                                                             |
 |       L_msu()                                                             |
 |       L_mult()                                                            |
 |       L_negate()                                                          |
 |       L_shl()                                                             |
 |       L_shr()                                                             |
 |       L_sub()                                                             |
 |       mac_r()                                                             |
 |       msu_r()                                                             |
 |       mult()                                                              |
 |       mult_r()                                                            |
 |       negate()                                                            |
 |       norm_l()                                                            |
 |       norm_s()                                                            |
 |       round()                                                             |
 |       shl()                                                               |
 |       shr()                                                               |
 |___________________________________________________________________________|
*/


/*___________________________________________________________________________
 |                                                                           |
 |   Include-Files                                                           |
 |___________________________________________________________________________|
*/
#include <stdio.h>
#include <stdlib.h>



#include "CEVA_typedef.h"
#include "CEVA_TL4_basic_op.h"

//int Carry;
//int Overflow;
I16 saturate (I32 L_var1);

I32 L_msu_ll_sh_sat(I32 out, I32 inp1, I32 inp2, I16 sh_val)
{

	long long out_t = (long long)out;
	sh_val = -sh_val;

	out_t -= (((long long) inp1 * inp2) >> sh_val);



	if (out_t>=0)
	{
		if (out_t>0x7fffffff)
			out_t=0x7fffffff;
	}
	else
	{

		if (out_t<0xffffffff80000000)
			out_t=0xffffffff80000000;
	}

	out = (I32)out_t;
	return (out);
}


acc_t L_mult_sh(I32 inp1, I32 inp2, I16 scale)
{

	acc_t out;
	out = (acc_t) (((long long) inp1 * inp2) >> scale);

	return (out);
}

long long L_mult_ll(I32 inp1, I32 inp2)
{

	long long out;
	out = (((long long) inp1 * inp2));

	return (out);
}

I32 L_mpy_ll_sat(I32 inp1, I32 inp2)
{

	I32 out;

	out = (I32)(((long long) inp1 * inp2) >> 31);

	if ((inp1 == 0x80000000) & (inp2 == 0x80000000))
		out = 0x7fffffff;
	
	return (out);
}
I32 L_mac_ll_sat(I32 out, I32 inp1, I32 inp2)
{

	long long out_t = (long long)out;

	out_t += (((long long) inp1 * inp2) >> 31);

	if (out_t>=0)
	{
		if (out_t>0x7fffffff)
			out_t=0x7fffffff;
	}
	else
	{

		if (out_t<0xffffffff80000000)
			out_t=0xffffffff80000000;
	}

	out = (I32)out_t;
	return (out);
}

I32 L_mac_ll_sh_sat(I32 out, I32 inp1, I32 inp2, I16 sh_val)
{

	long long out_t = (long long)out;
	sh_val = -sh_val;


	out_t += (((long long) inp1 * inp2) >> sh_val);



	if (out_t>=0)
	{
		if (out_t>0x7fffffff)
			out_t=0x7fffffff;
	}
	else
	{

		if (out_t<0xffffffff80000000)
			out_t=0xffffffff80000000;
	}

	out = (I32)out_t;
	return (out);
}


I32 L_mult_ll_sh_sat(I32 inp1, I32 inp2, I16 sh_val)
{

	long long out_t;
	sh_val = -sh_val;

	out_t = (((long long) inp1 * inp2) >> sh_val);



	if (out_t>=0)
	{
		if (out_t>0x7fffffff)
			out_t=0x7fffffff;
	}
	else
	{

		if (out_t<0xffffffff80000000)
			out_t=0xffffffff80000000;
	}

	return (I32)out_t;
}

I32 L_mac3_ll_sh_sat(I32 out, I32 inp1, I32 inp2, I32 inp3, I32 inp4, I16 sh_val)
{


	long long out_t = (long long)out;
	sh_val = -sh_val;


	out_t += (((long long) inp1 * inp2) >> sh_val);
	out_t += (((long long) inp3 * inp4) >> sh_val);

	if (out_t>=0)
	{
		if (out_t>0x7fffffff)
			out_t=0x7fffffff;
	}
	else
	{

		if (out_t<0xffffffff80000000)
			out_t=0xffffffff80000000;
	}

	out = (I32)out_t;
	return (out);
}

I32 L_mac_nsp (I32 out, I16 inp1, I16 inp2) // not sat p
{
	long long out_t = (long long)out;
	out_t += (((long long) inp1 * inp2) << 1);

	if (out_t>=0)
	{
		if (out_t>0x7fffffff)
			out_t=0x7fffffff;
	}
	else
	{

		if (out_t<0xffffffff80000000)
			out_t=0xffffffff80000000;
	}

	out = (I32)out_t;
	return (out);
}

I32 L_mac3(I32 out, I16 inp1, I16 inp2, I16 inp3, I16 inp4)
{


	long long out_t = (long long)out;
	long long res1;
	long long res2;


	res1 = (((long long) inp1 * inp2) << 1);
	if (res1>=0)
	{
		if (res1>0x7fffffff)
			res1=0x7fffffff;
	}
	else
	{

		if (res1<0xffffffff80000000)
			res1=0xffffffff80000000;
	}
	res2 = (((long long) inp3 * inp4) << 1);
	if (res2>=0)
	{
		if (res2>0x7fffffff)
			res2=0x7fffffff;
	}
	else
	{

		if (res2<0xffffffff80000000)
			res2=0xffffffff80000000;
	}
	out_t=out_t+res1+res2;

	if (out_t>=0)
	{
		if (out_t>0x7fffffff)
			out_t=0x7fffffff;
	}
	else
	{

		if (out_t<0xffffffff80000000)
			out_t=0xffffffff80000000;
	}

	out = (I32)out_t;
	return (out);
}


I32 L_mac3_nsp(I32 out, I16 inp1, I16 inp2, I16 inp3, I16 inp4)
{


	long long out_t = (long long)out;
	long long res1;
	long long res2;


	res1 = (((long long) inp1 * inp2) << 1);
	res2 = (((long long) inp3 * inp4) << 1);
	out_t=out_t+res1+res2;

	if (out_t>=0)
	{
		if (out_t>0x7fffffff)
			out_t=0x7fffffff;
	}
	else
	{

		if (out_t<0xffffffff80000000)
			out_t=0xffffffff80000000;
	}

	out = (I32)out_t;
	return (out);
}


void L_mac3_double(I32 *out1, I32 *out2, I16 inp1, I16 inp2, I16 inp3, I16 inp4, I16 inp1a, I16 inp2a, I16 inp3a, I16 inp4a)
{


	long long out_t = (long long)*out1;


	out_t += (((long long) inp1 * inp2) << 1);
	out_t += (((long long) inp3 * inp4) << 1);
	if (out_t>=0)
	{
		if (out_t>0x7fffffff)
			out_t=0x7fffffff;
	}
	else
	{
		if (out_t<0xffffffff80000000)
			out_t=0xffffffff80000000;
	}
	*out1 = (I32)out_t;

	out_t = (long long)*out2;
	out_t += (((long long) inp1a * inp2a) << 1);
	out_t += (((long long) inp3a * inp4a) << 1);
	if (out_t>=0)
	{
		if (out_t>0x7fffffff)
			out_t=0x7fffffff;
	}
	else
	{
		if (out_t<0xffffffff80000000)
			out_t=0xffffffff80000000;
	}
	*out2 = (I32)out_t;

	
}









I32 L_mult_sh_r(I32 inp1, I32 inp2, I16 scale)
{

	long long rnd_val = 0;
	int out;

	if (scale>0)
		rnd_val = (long long)1 << (scale - 1);


	out = (I32) (  (((long long) inp1 * inp2) + rnd_val) >> scale);

	return (out);
}



/*___________________________________________________________________________
 |                                                                           |
 |   Functions                                                               |
 |___________________________________________________________________________|
*/

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : saturate                                                |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |    Limit the 32 bit input to the range of a 16 bit word.                  |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var1                                                                 |
 |             32 bit long signed integer (I32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var1 <= 0x7fff ffff.                 |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (I16) whose value falls in the |
 |             range : 0xffff 8000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
*/
I16 saturate (I32 L_var1)
{
	 I16 var_out;

	 if (L_var1 > 0X00007fffL)
	 {
//		  Overflow = 1;
		  var_out = MAX_16;
	 }
	 else if (L_var1 < (I32) 0xffff8000L)
	 {
//		  Overflow = 1;
		  var_out = MIN_16;
	 }
	 else
	 {
		  var_out = extract_l (L_var1);

	 }

	 return (var_out);
}


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : add                                                     |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |    Performs the addition (var1+var2) with overflow control and saturation;|
 |    the 16 bit result is set at +32767 when overflow occurs or at -32768   |
 |    when underflow occurs.                                                 |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (I16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |    var2                                                                   |
 |             16 bit short signed integer (I16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (I16) whose value falls in the |
 |             range : 0xffff 8000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
*/
I16 add (I16 var1, I16 var2)
{
	 I16 var_out;
	 I32 L_sum;

	 L_sum = (I32) var1 + var2;
	 var_out = saturate (L_sum);

	 return (var_out);
}


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : sub                                                     |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |    Performs the subtraction (var1+var2) with overflow control and satu-   |
 |    ration; the 16 bit result is set at +32767 when overflow occurs or at  |
 |    -32768 when underflow occurs.                                          |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (I16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |    var2                                                                   |
 |             16 bit short signed integer (I16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (I16) whose value falls in the |
 |             range : 0xffff 8000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
*/
I16 sub (I16 var1, I16 var2)
{
    I16 var_out;
    I32 L_diff;

    L_diff = (I32) var1 - var2;
    var_out = saturate (L_diff);

    return (var_out);
}


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : abs_s                                                   |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |    Absolute value of var1; abs_s(-32768) = 32767.                         |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (I16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (I16) whose value falls in the |
 |             range : 0x0000 0000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
*/
I16 abs_s (I16 var1)
{
    I16 var_out;

    if (var1 == (I16) MIN_16)	
    {
        var_out = MAX_16;
    }
    else
    {
        if (var1 < 0)
        {
            var_out = -var1;
        }
        else
        {
            var_out = var1;
        }
    }

    return (var_out);
}


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : shl                                                     |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Arithmetically shift the 16 bit input var1 left var2 positions.Zero fill|
 |   the var2 LSB of the result. If var2 is negative, arithmetically shift   |
 |   var1 right by -var2 with sign extension. Saturate the result in case of |
 |   underflows or overflows.                                                |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (I16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |    var2                                                                   |
 |             16 bit short signed integer (I16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (I16) whose value falls in the |
 |             range : 0xffff 8000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
*/
I16 shl (I16 var1, I16 var2)
{
    I16 var_out;
    I32 result;

    if (var2 < 0)
    {
        if (var2 < -16)
            var2 = -16;
        var2 = -var2;
        var_out = shr (var1, var2);


    }
    else
    {
        result = (I32) var1 *((I32) 1 << var2);

        if ((var2 > 15 && var1 != 0) || (result != (I32) ((I16) result)))
        {
//            Overflow = 1;
            var_out = (var1 > 0) ? MAX_16 : MIN_16;
        }
        else
        {
            var_out = extract_l (result);


        }
    }


    return (var_out);
}


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : shr                                                     |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Arithmetically shift the 16 bit input var1 right var2 positions with    |
 |   sign extension. If var2 is negative, arithmetically shift var1 left by  |
 |   -var2 with sign extension. Saturate the result in case of underflows or |
 |   overflows.                                                              |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (I16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |    var2                                                                   |
 |             16 bit short signed integer (I16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (I16) whose value falls in the |
 |             range : 0xffff 8000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
*/
I16 shr (I16 var1, I16 var2)
{
    I16 var_out;

    if (var2 < 0)
    {
        if (var2 < -16)
            var2 = -16;
        var2 = -var2;
        var_out = shl (var1, var2);


    }
    else
    {
        if (var2 >= 15)
        {
            var_out = (var1 < 0) ? -1 : 0;
        }
        else
        {
            if (var1 < 0)
            {
                var_out = ~((~var1) >> var2);
            }
            else
            {
                var_out = var1 >> var2;
            }
        }
    }


    return (var_out);
}


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : mult                                                    |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |    Performs the multiplication of var1 by var2 and gives a 16 bit result  |
 |    which is scaled i.e.:                                                  |
 |             mult(var1,var2) = extract_l(L_shr((var1 times var2),15)) and  |
 |             mult(-32768,-32768) = 32767.                                  |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (I16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |    var2                                                                   |
 |             16 bit short signed integer (I16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (I16) whose value falls in the |
 |             range : 0xffff 8000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
*/
I16 mult (I16 var1, I16 var2)
{
    I16 var_out;
    I32 L_product;

    L_product = (I32) var1 *(I32) var2;

    L_product = (L_product & (I32) 0xffff8000L) >> 15;

    if (L_product & (I32) 0x00010000L)
        L_product = L_product | (I32) 0xffff0000L;

    var_out = saturate (L_product);


    return (var_out);
}


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : L_mult                                                  |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   L_mult is the 32 bit result of the multiplication of var1 times var2    |
 |   with one shift left i.e.:                                               |
 |        L_mult(var1,var2) = L_shl((var1 times var2),1) and                 |
 |        L_mult(-32768,-32768) = 2147483647.                                |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (I16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |    var2                                                                   |
 |             16 bit short signed integer (I16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    L_var_out                                                              |
 |             32 bit long signed integer (I32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var_out <= 0x7fff ffff.              |
 |___________________________________________________________________________|
*/
I32 L_mult (I16 var1, I16 var2)
{
    I32 L_var_out;

    L_var_out = (I32) var1 *(I32) var2;

    if (L_var_out != (I32) 0x40000000L)
    {
        L_var_out *= 2;
    }
    else
    {
//        Overflow = 1;
        L_var_out = MAX_32;
    }


    return (L_var_out);
}


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : negate                                                  |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Negate var1 with saturation, saturate in the case where input is -32768:|
 |                negate(var1) = sub(0,var1).                                |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (I16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (I16) whose value falls in the |
 |             range : 0xffff 8000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
*/
I16 negate (I16 var1)
{
    I16 var_out;

    var_out = (var1 == MIN_16) ? MAX_16 : -var1;


    return (var_out);
}


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : extract_h                                               |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Return the 16 MSB of L_var1.                                            |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var1                                                                 |
 |             32 bit long signed integer (I32 ) whose value falls in the |
 |             range : 0x8000 0000 <= L_var1 <= 0x7fff ffff.                 |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (I16) whose value falls in the |
 |             range : 0xffff 8000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
*/
I16 extract_h (I32 L_var1)
{
    I16 var_out;

    var_out = (I16) (L_var1 >> 16);


    return (var_out);
}


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : extract_l                                               |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Return the 16 LSB of L_var1.                                            |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var1                                                                 |
 |             32 bit long signed integer (I32 ) whose value falls in the |
 |             range : 0x8000 0000 <= L_var1 <= 0x7fff ffff.                 |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (I16) whose value falls in the |
 |             range : 0xffff 8000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
*/
I16 extract_l (I32 L_var1)
{
    I16 var_out;

    var_out = (I16) L_var1;


    return (var_out);
}


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : round                                                   |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Round the lower 16 bits of the 32 bit input number into the MS 16 bits  |
 |   with saturation. Shift the resulting bits right by 16 and return the 16 |
 |   bit number:                                                             |
 |               round(L_var1) = extract_h(L_add(L_var1,32768))              |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var1                                                                 |
 |             32 bit long signed integer (I32 ) whose value falls in the |
 |             range : 0x8000 0000 <= L_var1 <= 0x7fff ffff.                 |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (I16) whose value falls in the |
 |             range : 0xffff 8000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
*/
I16 round (I32 L_var1)
{
    I16 var_out;
    I32 L_rounded;

    L_rounded = L_add (L_var1, (I32) 0x00008000L);
    var_out = extract_h (L_rounded);

    return (var_out);
}


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : L_mac                                                   |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Multiply var1 by var2 and shift the result left by 1. Add the 32 bit    |
 |   result to L_var3 with saturation, return a 32 bit result:               |
 |        L_mac(L_var3,var1,var2) = L_add(L_var3,L_mult(var1,var2)).         |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var3   32 bit long signed integer (I32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var3 <= 0x7fff ffff.                 |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (I16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |    var2                                                                   |
 |             16 bit short signed integer (I16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    L_var_out                                                              |
 |             32 bit long signed integer (I32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var_out <= 0x7fff ffff.              |
 |___________________________________________________________________________|
*/
I32 L_mac (I32 L_var3, I16 var1, I16 var2)
{
    I32 L_var_out;
    I32 L_product;

    L_product = L_mult (var1, var2);
    L_var_out = L_add (L_var3, L_product);


    return (L_var_out);
}


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : L_msu                                                   |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Multiply var1 by var2 and shift the result left by 1. Subtract the 32   |
 |   bit result from L_var3 with saturation, return a 32 bit result:         |
 |        L_msu(L_var3,var1,var2) = L_sub(L_var3,L_mult(var1,var2)).         |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var3   32 bit long signed integer (I32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var3 <= 0x7fff ffff.                 |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (I16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |    var2                                                                   |
 |             16 bit short signed integer (I16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    L_var_out                                                              |
 |             32 bit long signed integer (I32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var_out <= 0x7fff ffff.              |
 |___________________________________________________________________________|
*/
I32 L_msu (I32 L_var3, I16 var1, I16 var2)
{
    I32 L_var_out;
    I32 L_product;

    L_product = L_mult (var1, var2);
    L_var_out = L_sub (L_var3, L_product);


    return (L_var_out);
}


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : L_add                                                   |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   32 bits addition of the two 32 bits variables (L_var1+L_var2) with      |
 |   overflow control and saturation; the result is set at +2147483647 when  |
 |   overflow occurs or at -2147483648 when underflow occurs.                |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var1   32 bit long signed integer (I32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var3 <= 0x7fff ffff.                 |
 |                                                                           |
 |    L_var2   32 bit long signed integer (I32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var3 <= 0x7fff ffff.                 |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    L_var_out                                                              |
 |             32 bit long signed integer (I32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var_out <= 0x7fff ffff.              |
 |___________________________________________________________________________|
*/
I32 L_add (I32 L_var1, I32 L_var2)
{
    I32 L_var_out;

    L_var_out = L_var1 + L_var2;

    if (((L_var1 ^ L_var2) & MIN_32) == 0)
    {
        if ((L_var_out ^ L_var1) & MIN_32)
        {
            L_var_out = (L_var1 < 0) ? MIN_32 : MAX_32;
//            Overflow = 1;
        }
    }

    return (L_var_out);
}


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : L_sub                                                   |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   32 bits subtraction of the two 32 bits variables (L_var1-L_var2) with   |
 |   overflow control and saturation; the result is set at +2147483647 when  |
 |   overflow occurs or at -2147483648 when underflow occurs.                |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var1   32 bit long signed integer (I32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var3 <= 0x7fff ffff.                 |
 |                                                                           |
 |    L_var2   32 bit long signed integer (I32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var3 <= 0x7fff ffff.                 |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    L_var_out                                                              |
 |             32 bit long signed integer (I32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var_out <= 0x7fff ffff.              |
 |___________________________________________________________________________|
*/
I32 L_sub (I32 L_var1, I32 L_var2)
{
    I32 L_var_out;

    L_var_out = L_var1 - L_var2;

    if (((L_var1 ^ L_var2) & MIN_32) != 0)
    {
        if ((L_var_out ^ L_var1) & MIN_32)
        {
            L_var_out = (L_var1 < 0L) ? MIN_32 : MAX_32;
//            Overflow = 1;
        }
    }

    return (L_var_out);
}

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : L_negate                                                |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Negate the 32 bit variable L_var1 with saturation; saturate in the case |
 |   where input is -2147483648 (0x8000 0000).                               |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var1   32 bit long signed integer (I32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var3 <= 0x7fff ffff.                 |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    L_var_out                                                              |
 |             32 bit long signed integer (I32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var_out <= 0x7fff ffff.              |
 |___________________________________________________________________________|
*/
I32 L_negate (I32 L_var1)
{
    I32 L_var_out;

    L_var_out = (L_var1 == MIN_32) ? MAX_32 : -L_var1;

    return (L_var_out);
}


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : mult_r                                                  |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Same as mult with rounding, i.e.:                                       |
 |     mult_r(var1,var2) = extract_l(L_shr(((var1 * var2) + 16384),15)) and  |
 |     mult_r(-32768,-32768) = 32767.                                        |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (I16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |    var2                                                                   |
 |             16 bit short signed integer (I16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (I16) whose value falls in the |
 |             range : 0x8000 <= var_out <= 0x7fff.                          |
 |___________________________________________________________________________|
*/
I16 mult_r (I16 var1, I16 var2)
{
    I16 var_out;
    I32 L_product_arr;

    L_product_arr = (I32) var1 *(I32) var2;       /* product */
    L_product_arr += (I32) 0x00004000L;      /* round */
    L_product_arr &= (I32) 0xffff8000L;
    L_product_arr >>= 15;       /* shift */

    if (L_product_arr & (I32) 0x00010000L)   /* sign extend when necessary */
    {
        L_product_arr |= (I32) 0xffff0000L;
    }
    var_out = saturate (L_product_arr);

    return (var_out);
}


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : L_shl                                                   |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Arithmetically shift the 32 bit input L_var1 left var2 positions. Zero  |
 |   fill the var2 LSB of the result. If var2 is negative, arithmetically    |
 |   shift L_var1 right by -var2 with sign extension. Saturate the result in |
 |   case of underflows or overflows.                                        |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var1   32 bit long signed integer (I32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var3 <= 0x7fff ffff.                 |
 |                                                                           |
 |    var2                                                                   |
 |             16 bit short signed integer (I16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    L_var_out                                                              |
 |             32 bit long signed integer (I32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var_out <= 0x7fff ffff.              |
 |___________________________________________________________________________|
*/
I32 L_shl (I32 L_var1, I16 var2)
{

    I32 L_var_out = 0L;

    if (var2 <= 0)
    {
        if (var2 < -32)
            var2 = -32;
        var2 = -var2;
        L_var_out = L_shr (L_var1, var2);
    }
    else
    {
        for (; var2 > 0; var2--)
        {
            if (L_var1 > (I32) 0X3fffffffL)
            {
//                Overflow = 1;
                L_var_out = MAX_32;
                break;
            }
            else
            {
                if (L_var1 < (I32) 0xc0000000L)
                {
//                    Overflow = 1;
                    L_var_out = MIN_32;
                    break;
                }
            }
            L_var1 *= 2;
            L_var_out = L_var1;
        }
    }
    return (L_var_out);
}


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : L_shr                                                   |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Arithmetically shift the 32 bit input L_var1 right var2 positions with  |
 |   sign extension. If var2 is negative, arithmetically shift L_var1 left   |
 |   by -var2 and zero fill the -var2 LSB of the result. Saturate the result |
 |   in case of underflows or overflows.                                     |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var1   32 bit long signed integer (I32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var3 <= 0x7fff ffff.                 |
 |                                                                           |
 |    var2                                                                   |
 |             16 bit short signed integer (I16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    L_var_out                                                              |
 |             32 bit long signed integer (I32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var_out <= 0x7fff ffff.              |
 |___________________________________________________________________________|
*/
I32 L_shr (I32 L_var1, I16 var2)
{
    I32 L_var_out;

    if (var2 < 0)
    {
        if (var2 < -32)
            var2 = -32;
        var2 = -var2;
        L_var_out = L_shl (L_var1, var2);
    }
    else
    {
        if (var2 >= 31)
        {
            L_var_out = (L_var1 < 0L) ? -1 : 0;
        }
        else
        {
            if (L_var1 < 0)
            {
                L_var_out = ~((~L_var1) >> var2);
            }
            else
            {
                L_var_out = L_var1 >> var2;
            }
        }
    }
    return (L_var_out);
}


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : shr_r                                                   |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Same as shr(var1,var2) but with rounding. Saturate the result in case of|
 |   underflows or overflows :                                               |
 |    - If var2 is greater than zero :                                       |
 |          if (sub(shl(shr(var1,var2),1),shr(var1,sub(var2,1))))            |
 |          is equal to zero                                                 |
 |                     then                                                  |
 |                     shr_r(var1,var2) = shr(var1,var2)                     |
 |                     else                                                  |
 |                     shr_r(var1,var2) = add(shr(var1,var2),1)              |
 |    - If var2 is less than or equal to zero :                              |
 |                     shr_r(var1,var2) = shr(var1,var2).                    |
 |                                                                           |
 |   Complexity weight : 3                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (I16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |    var2                                                                   |
 |             16 bit short signed integer (I16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (I16) whose value falls in the |
 |             range : 0xffff 8000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
*/
I16 shr_r (I16 var1, I16 var2)
{
    I16 var_out;

    if (var2 > 15)
    {
        var_out = 0;
    }
    else
    {
        var_out = shr (var1, var2);


        if (var2 > 0)
        {
            if ((var1 & ((I16) 1 << (var2 - 1))) != 0)
            {
                var_out++;
            }
        }
    }

    return (var_out);
}


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : mac_r                                                   |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Multiply var1 by var2 and shift the result left by 1. Add the 32 bit    |
 |   result to L_var3 with saturation. Round the LS 16 bits of the result    |
 |   into the MS 16 bits with saturation and shift the result right by 16.   |
 |   Return a 16 bit result.                                                 |
 |            mac_r(L_var3,var1,var2) = round(L_mac(L_var3,var1,var2))       |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var3   32 bit long signed integer (I32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var3 <= 0x7fff ffff.                 |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (I16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |    var2                                                                   |
 |             16 bit short signed integer (I16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (I16) whose value falls in the |
 |             range : 0x0000 8000 <= L_var_out <= 0x0000 7fff.              |
 |___________________________________________________________________________|
*/
I16 mac_r (I32 L_var3, I16 var1, I16 var2)
{
    I16 var_out;

    L_var3 = L_mac (L_var3, var1, var2);
    L_var3 = L_add (L_var3, (I32) 0x00008000L);
    var_out = extract_h (L_var3);

    return (var_out);
}


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : msu_r                                                   |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Multiply var1 by var2 and shift the result left by 1. Subtract the 32   |
 |   bit result from L_var3 with saturation. Round the LS 16 bits of the res-|
 |   ult into the MS 16 bits with saturation and shift the result right by   |
 |   16. Return a 16 bit result.                                             |
 |            msu_r(L_var3,var1,var2) = round(L_msu(L_var3,var1,var2))       |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var3   32 bit long signed integer (I32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var3 <= 0x7fff ffff.                 |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (I16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |    var2                                                                   |
 |             16 bit short signed integer (I16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (I16) whose value falls in the |
 |             range : 0x0000 8000 <= L_var_out <= 0x0000 7fff.              |
 |___________________________________________________________________________|
*/
I16 msu_r (I32 L_var3, I16 var1, I16 var2)
{
    I16 var_out;

    L_var3 = L_msu (L_var3, var1, var2);
    L_var3 = L_add (L_var3, (I32) 0x00008000L);
    var_out = extract_h (L_var3);

    return (var_out);
}


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : L_deposit_h                                             |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Deposit the 16 bit var1 into the 16 MS bits of the 32 bit output. The   |
 |   16 LS bits of the output are zeroed.                                    |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (I16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    L_var_out                                                              |
 |             32 bit long signed integer (I32) whose value falls in the  |
 |             range : 0x8000 0000 <= var_out <= 0x7fff 0000.                |
 |___________________________________________________________________________|
*/
I32 L_deposit_h (I16 var1)
{
    I32 L_var_out;

    L_var_out = (I32) var1 << 16;

#
    return (L_var_out);
}


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : L_deposit_l                                             |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Deposit the 16 bit var1 into the 16 LS bits of the 32 bit output. The   |
 |   16 MS bits of the output are sign extended.                             |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (I16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    L_var_out                                                              |
 |             32 bit long signed integer (I32) whose value falls in the  |
 |             range : 0xFFFF 8000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
*/
I32 L_deposit_l (I16 var1)
{
	I32 L_var_out;

    L_var_out = (I32) var1;

    return (L_var_out);
}


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : L_shr_r                                                 |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Same as L_shr(L_var1,var2) but with rounding. Saturate the result in    |
 |   case of underflows or overflows :                                       |
 |    - If var2 is greater than zero :                                       |
 |          if (L_sub(L_shl(L_shr(L_var1,var2),1),L_shr(L_var1,sub(var2,1))))|
 |          is equal to zero                                                 |
 |                     then                                                  |
 |                     L_shr_r(L_var1,var2) = L_shr(L_var1,var2)             |
 |                     else                                                  |
 |                     L_shr_r(L_var1,var2) = L_add(L_shr(L_var1,var2),1)    |
 |    - If var2 is less than or equal to zero :                              |
 |                     L_shr_r(L_var1,var2) = L_shr(L_var1,var2).            |
 |                                                                           |
 |   Complexity weight : 3                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var1                                                                 |
 |             32 bit long signed integer (I32) whose value falls in the  |
 |             range : 0x8000 0000 <= var1 <= 0x7fff ffff.                   |
 |                                                                           |
 |    var2                                                                   |
 |             16 bit short signed integer (I16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    L_var_out                                                              |
 |             32 bit long signed integer (I32) whose value falls in the  |
 |             range : 0x8000 0000 <= var_out <= 0x7fff ffff.                |
 |___________________________________________________________________________|
*/
I32 L_shr_r (I32 L_var1, I16 var2)
{
    I32 L_var_out;

    if (var2 > 31)
    {
        L_var_out = 0;
    }
    else
    {
        L_var_out = L_shr (L_var1, var2);

        if (var2 > 0)
        {
            if ((L_var1 & ((I32) 1 << (var2 - 1))) != 0)
            {
                L_var_out++;
            }
        }
    }

    return (L_var_out);
}


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : L_abs                                                   |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |    Absolute value of L_var1; Saturate in case where the input is          |
 |                                                               -214783648  |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var1                                                                 |
 |             32 bit long signed integer (I32) whose value falls in the  |
 |             range : 0x8000 0000 <= var1 <= 0x7fff ffff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    L_var_out                                                              |
 |             32 bit long signed integer (I32) whose value falls in the  |
 |             range : 0x0000 0000 <= var_out <= 0x7fff ffff.                |
 |___________________________________________________________________________|
*/
I32 L_abs (I32 L_var1)
{
    I32 L_var_out;

    if (L_var1 == MIN_32)
    {
        L_var_out = MAX_32;
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


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : norm_s                                                  |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Produces the number of left shift needed to normalize the 16 bit varia- |
 |   ble var1 for positive values on the interval with minimum of 16384 and  |
 |   maximum of 32767, and for negative values on the interval with minimum  |
 |   of -32768 and maximum of -16384; in order to normalize the result, the  |
 |   following operation must be done :                                      |
 |                    norm_var1 = shl(var1,norm_s(var1)).                    |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (I16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (I16) whose value falls in the |
 |             range : 0x0000 0000 <= var_out <= 0x0000 000f.                |
 |___________________________________________________________________________|
*/
I16 norm_s (I16 var1)
{
    I16 var_out;

    if (var1 == 0)
    {
        var_out = 0;
    }
    else
    {
        if (var1 == (I16) 0xffff)
        {
            var_out = 15;
        }
        else
        {
            if (var1 < 0)
            {
                var1 = ~var1;
            }
            for (var_out = 0; var1 < 0x4000; var_out++)
            {
                var1 <<= 1;
            }
        }
    }

    return (var_out);
}


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : div_s                                                   |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Produces a result which is the fractional integer division of var1  by  |
 |   var2; var1 and var2 must be positive and var2 must be greater or equal  |
 |   to var1; the result is positive (leading bit equal to 0) and truncated  |
 |   to 16 bits.                                                             |
 |   If var1 = var2 then div(var1,var2) = 32767.                             |
 |                                                                           |
 |   Complexity weight : 18                                                  |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (I16) whose value falls in the |
 |             range : 0x0000 0000 <= var1 <= var2 and var2 != 0.            |
 |                                                                           |
 |    var2                                                                   |
 |             16 bit short signed integer (I16) whose value falls in the |
 |             range : var1 <= var2 <= 0x0000 7fff and var2 != 0.            |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (I16) whose value falls in the |
 |             range : 0x0000 0000 <= var_out <= 0x0000 7fff.                |
 |             It's a Q15 value (point between b15 and b14).                 |
 |___________________________________________________________________________|
*/
I16 div_s_not_use (I16 var1, I16 var2) 
{
    I16 var_out = 0;
    I16 iteration;
    I32 L_num;
    I32 L_denom;
    if ((var1 > var2) || (var1 < 0) || (var2 < 0))
    {
        printf ("Division Error var1=%d  var2=%d\n", var1, var2);
        abort(); /* exit (0); */
    }
    if (var2 == 0)
    {
        printf ("Division by 0, Fatal error \n");
        abort(); /* exit (0); */
    }
    if (var1 == 0)
    {
        var_out = 0;
    }
    else
    {
        if (var1 == var2)
        {
            var_out = MAX_16;
        }
        else
        {
            L_num = L_deposit_l (var1);
            L_denom = L_deposit_l (var2);
            for (iteration = 0; iteration < 15; iteration++)
            {
                var_out <<= 1;
                L_num <<= 1;
                if (L_num >= L_denom)
                {
                    L_num = L_sub (L_num, L_denom);
                    var_out = add (var_out, 1);
                }
            }
        }
    }

    return (var_out);
}


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : norm_l                                                  |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Produces the number of left shifts needed to normalize the 32 bit varia-|
 |   ble L_var1 for positive values on the interval with minimum of          |
 |   1073741824 and maximum of 2147483647, and for negative values on the in-|
 |   terval with minimum of -2147483648 and maximum of -1073741824; in order |
 |   to normalize the result, the following operation must be done :         |
 |                   norm_L_var1 = L_shl(L_var1,norm_l(L_var1)).             |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var1                                                                 |
 |             32 bit long signed integer (I32) whose value falls in the  |
 |             range : 0x8000 0000 <= var1 <= 0x7fff ffff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (I16) whose value falls in the |
 |             range : 0x0000 0000 <= var_out <= 0x0000 001f.                |
 |___________________________________________________________________________|
*/
I16 norm_l (I32 L_var1)
{
    I16 var_out;

    if (L_var1 == 0)
    {
        var_out = 0;
    }
    else
    {
        if (L_var1 == (I32) 0xffffffffL)
        {
            var_out = 31;
        }
        else
        {
            if (L_var1 < 0)
            {
                L_var1 = ~L_var1;
            }
            for (var_out = 0; L_var1 < (I32) 0x40000000L; var_out++)
            {
                L_var1 <<= 1;
            }
        }
    }

    return (var_out);
}
I16 norm_l_nz (I32 L_var1)
{
    I16 var_out;

    if (L_var1 == 0)
    {
        var_out = 31;
    }
    else
    {
        if (L_var1 == (I32) 0xffffffffL)
        {
            var_out = 31;
        }
        else
        {
            if (L_var1 < 0)
            {
                L_var1 = ~L_var1;
            }
            for (var_out = 0; L_var1 < (I32) 0x40000000L; var_out++)
            {
                L_var1 <<= 1;
            }
        }
    }

    return (var_out);
}

/*
 ******************************************************************************
 * Additional operators extracted from the G.723.1 Library
 * Adapted for WMOPS calculations
 ******************************************************************************
*/

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : L_mls                                                   |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Multiplies a 16 bit word v by a 32 bit word Lv and returns a 32 bit     |
 |   word (multiplying 16 by 32 bit words gives 48 bit word; the function    |
 |   extracts the 32 MSB and shift the result to the left by 1).             |
 |                                                                           |
 |   A 32 bit word can be written as                                         |
 |    Lv = a  +  b * 2^16                                                    |
 |   where a= unsigned 16 LSBs and b= signed 16 MSBs.                        |
 |   The function returns v * Lv  /  2^15  which is equivalent to            |
 |        a*v / 2^15 + b*v*2                                                 |
 |                                                                           |
 |   Complexity weight : 5													 |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |   Lv                                                                      |
 |             32 bit long signed integer (I32) whose value falls in the  |
 |             range : 0x8000 0000 <= var1 <= 0x7fff ffff.                   |
 |   v                                                                       |
 |             16 bit short signed integer (I16) whose value falls in the |
 |             range : 0x8000 <= var1 <= 0x7fff.                             |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             32 bit long signed integer (I32) whose value falls in the  |
 |             range : 0x8000 0000 <= var_out <= 0x7fff ffff.                |
 |                                                                           |
 |___________________________________________________________________________|
*/
I32 L_mls (I32 Lv, I16 v)
{
   I32   Temp  ;

   Temp = Lv & (I32) 0x0000ffff ;
   Temp = Temp * (I32) v ;
   Temp = L_shr( Temp, (I16) 15 ) ;
   Temp = L_mac( Temp, v, extract_h(Lv) ) ;


   return Temp ;
}


/*__________________________________________________________________________
|                                                                           |
|   Function Name : div_l                                                   |
|                                                                           |
|   Purpose :                                                               |
|                                                                           |
|   Produces a result which is the fractional integer division of L_var1 by |
|   var2; L_var1 and var2 must be positive and var2 << 16 must be greater or|
|   equal to L_var1; the result is positive (leading bit equal to 0) and    |
|   truncated to 16 bits.                                                   |
|   If L_var1 == var2 << 16 then div_l(L_var1,var2) = 32767.                |
|                                                                           |
|   Complexity weight : 32                                                  |
|                                                                           |
|   Inputs :                                                                |
|                                                                           |
|    L_var1                                                                 |
|             32 bit long signed integer (I32) whose value falls in the  |
|             range : 0x0000 0000 <= var1 <= (var2 << 16)  and var2 != 0.   |
|             L_var1 must be considered as a Q.31 value                     |
|                                                                           |
|    var2                                                                   |
|             16 bit short signed integer (I16) whose value falls in the |
|             range : var1 <= (var2<< 16) <= 0x7fff0000 and var2 != 0.      |
|             var2 must be considered as a Q.15 value                       |
|                                                                           |
|   Outputs :                                                               |
|                                                                           |
|    none                                                                   |
|                                                                           |
|   Return Value :                                                          |
|                                                                           |
|    var_out                                                                |
|             16 bit short signed integer (I16) whose value falls in the |
|             range : 0x0000 0000 <= var_out <= 0x0000 7fff.                |
|             It's a Q15 value (point between b15 and b14).                 |
|___________________________________________________________________________|
*/
I16 div_l (I32  L_num, I16 den)
{
    I16   var_out = (I16)0;
    I32   L_den;
    I16   iteration;



    if ( den == (I16) 0 ) {
        printf("Division by 0 in div_l, Fatal error \n");
        exit(0);
    }

    if ( (L_num < (I32) 0) || (den < (I16) 0) ) {
        printf("Division Error in div_l, Fatal error \n");
        exit(0);
    }

    L_den = L_deposit_h( den ) ;


    if ( L_num >= L_den ){
        return MAX_16 ;
    }
    else {
        L_num = L_shr(L_num, (I16)1) ;
        L_den = L_shr(L_den, (I16)1);

        for(iteration=(I16)0; iteration< (I16)15;iteration++) {
            var_out = shl( var_out, (I16)1);
            L_num   = L_shl( L_num, (I16)1);

            if (L_num >= L_den) {
                L_num = L_sub(L_num,L_den);
                var_out = add(var_out, (I16)1);
            }
        }

        return var_out;
    }
}


/*__________________________________________________________________________
|                                                                           |
|   Function Name : i_mult                                                  |
|                                                                           |
|   Purpose :                                                               |
|                                                                           |
|   Integer 16-bit multiplication. No overflow protection is performed if   |
|   ORIGINAL_G7231 is defined.                                              |
|                                                                           |
|   Complexity weight : 3                                                   |
|                                                                           |
|   Inputs :                                                                |
|                                                                           |
|    a                                                                      |
|             16 bit short signed integer (I16).                         |
|                                                                           |
|    b                                                                      |
|             16 bit short signed integer (I16).                         |
|                                                                           |
|   Outputs :                                                               |
|                                                                           |
|    none                                                                   |
|                                                                           |
|   Return Value :                                                          |
|                                                                           |
|             16 bit short signed integer (I16). No overflow checks      |
|             are performed if ORIGINAL_G7231 is defined.                   |
|___________________________________________________________________________|
*/
I16 i_mult (I16 a, I16 b)
{

   I32 register c=a*b;

   return saturate(c) ;

}


/*
 ******************************************************************************
 * The following three operators are not part of the original 
 * G.729/G.723.1 set of basic operators and implement shiftless
 * accumulation operation.
 ******************************************************************************
*/

/*___________________________________________________________________________
 |
 |   Function Name : L_mult0
 |
 |   Purpose :
 |
 |   L_mult0 is the 32 bit result of the multiplication of var1 times var2
 |   without one left shift.
 |
 |   Complexity weight : 1
 |
 |   Inputs :
 |
 |    var1     16 bit short signed integer (I16) whose value falls in the
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.
 |
 |    var2     16 bit short signed integer (I16) whose value falls in the
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.
 |
 |   Return Value :
 |
 |    L_var_out
 |             32 bit long signed integer (I32) whose value falls in the
 |             range : 0x8000 0000 <= L_var_out <= 0x7fff ffff.
 |___________________________________________________________________________
*/
I32 L_mult0 (I16 var1,I16 var2)
{
  I32 L_var_out;

  L_var_out = (I32)var1 * (I32)var2;


  return(L_var_out);
}


/*___________________________________________________________________________
 |
 |   Function Name : L_mac0
 |
 |   Purpose :
 |
 |   Multiply var1 by var2 (without left shift) and add the 32 bit result to
 |   L_var3 with saturation, return a 32 bit result:
 |        L_mac0(L_var3,var1,var2) = L_add(L_var3,(L_mult0(var1,var2)).
 |
 |   Complexity weight : 1
 |
 |   Inputs :
 |
 |    L_var3   32 bit long signed integer (I32) whose value falls in the
 |             range : 0x8000 0000 <= L_var3 <= 0x7fff ffff.
 |
 |    var1     16 bit short signed integer (I16) whose value falls in the
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.
 |
 |    var2     16 bit short signed integer (I16) whose value falls in the
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.
 |
 |   Return Value :
 |
 |    L_var_out
 |             32 bit long signed integer (I32) whose value falls in the
 |             range : 0x8000 0000 <= L_var_out <= 0x7fff ffff.
 |___________________________________________________________________________
*/
I32 L_mac0 (I32 L_var3, I16 var1, I16 var2)
{
  I32 L_var_out;
  I32 L_product;

  L_product = L_mult0(var1,var2);
  L_var_out = L_add(L_var3,L_product);

  return(L_var_out);
}


/*___________________________________________________________________________
 |
 |   Function Name : L_msu0
 |
 |   Purpose :
 |
 |   Multiply var1 by var2 (without left shift) and subtract the 32 bit
 |   result to L_var3 with saturation, return a 32 bit result:
 |        L_msu0(L_var3,var1,var2) = L_sub(L_var3,(L_mult0(var1,var2)).
 |
 |   Complexity weight : 1
 |
 |   Inputs :
 |
 |    L_var3   32 bit long signed integer (I32) whose value falls in the
 |             range : 0x8000 0000 <= L_var3 <= 0x7fff ffff.
 |
 |    var1     16 bit short signed integer (I16) whose value falls in the
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.
 |
 |    var2     16 bit short signed integer (I16) whose value falls in the
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.
 |
 |   Return Value :
 |
 |    L_var_out
 |             32 bit long signed integer (I32) whose value falls in the
 |             range : 0x8000 0000 <= L_var_out <= 0x7fff ffff.
 |___________________________________________________________________________
*/
I32 L_msu0 (I32 L_var3, I16 var1, I16 var2)
{
  I32 L_var_out;
  I32 L_product;

  L_product = L_mult0(var1,var2);
  L_var_out = L_sub(L_var3,L_product);

  return(L_var_out);
}


/* end of file */
