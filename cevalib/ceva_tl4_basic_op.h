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

/*
  ===========================================================================
   File: BASOP32.H                                       v.2.1 - March 2006
  ===========================================================================

            ITU-T STL  BASIC OPERATORS

            GLOBAL FUNCTION PROTOTYPES

   History:
   26.Jan.00   v1.0     Incorporated to the STL from updated G.723.1/G.729 
                        basic operator library (based on basic_op.h) and 
                        G.723.1's basop.h.
   05.Jul.00    v1.1    Added 32-bit shiftless mult/mac/msub operators

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

  ============================================================================
*/


#ifndef _CEVA_G729AB_BASIC_OP_H
#define _CEVA_G729AB_BASIC_OP_H

#include "CEVA_typedef.h"

/*___________________________________________________________________________
 |                                                                           |
 |   Constants and Globals                                                   |
 | $Id $
 |___________________________________________________________________________|
*/

#define MAX_32 (I32)0x7fffffffL
#define MIN_32 (I32)0x80000000L

#define MAX_16 (I16)0x7fff
#define MIN_16 (I16)0x8000

#define shrNI(x,y) (shr(x,y))

/*___________________________________________________________________________
 |                                                                           |
 |   Prototypes for basic arithmetic operators                               |
 |___________________________________________________________________________|
*/
I32 L_msu_ll_sh_sat(I32 out, I32 inp1, I32 inp2, I16 sh_val);
I32 L_mac3_nsp(I32 out, I16 inp1, I16 inp2, I16 inp3, I16 inp4);
I32 L_mult_ll_sh_sat(I32 inp1, I32 inp2, I16 sh_val);
acc_t L_mult_sh(I32 inp1, I32 inp2, I16 scale);
I32 L_mult_sh_r(I32 inp1, I32 inp2, I16 scale);
long long L_mult_ll(I32 inp1, I32 inp2);
I32 L_mpy_ll_sat(I32 inp1, I32 inp2);
I32 L_mac_ll_sat(I32 out, I32 inp1, I32 inp2);
I32 L_mac_ll_sh_sat(I32 out, I32 inp1, I32 inp2, I16 sh_val);
I32 L_mac3_ll_sh_sat(I32 out, I32 inp1, I32 inp2, I32 inp3, I32 inp4, I16 sh_val);
I32 L_mac3(I32 out, I16 inp1, I16 inp2, I16 inp3, I16 inp4);
void L_mac3_double(I32 *out1, I32 *out2, I16 inp1, I16 inp2, I16 inp3, I16 inp4, I16 inp1a, I16 inp2a, I16 inp3a, I16 inp4a);
I32 L_mac_nsp (I32 out, I16 inp1, I16 inp2); // not sat p

I16 add (I16 var1, I16 var2);    /* Short add,           1   */
I16 sub (I16 var1, I16 var2);    /* Short sub,           1   */
I16 abs_s (I16 var1);               /* Short abs,           1   */
I16 shl (I16 var1, I16 var2);    /* Short shift left,    1   */
I16 shr (I16 var1, I16 var2);    /* Short shift right,   1   */
I16 mult (I16 var1, I16 var2);   /* Short mult,          1   */
I32 L_mult (I16 var1, I16 var2); /* Long mult,           1   */
I16 negate (I16 var1);              /* Short negate,        1   */
I16 extract_h (I32 L_var1);         /* Extract high,        1   */
I16 extract_l (I32 L_var1);         /* Extract low,         1   */

I16 round (I32 L_var1);             /* Round,               1   */

I32 L_mac (I32 L_var3, I16 var1, I16 var2);   /* Mac,  1  */
I32 L_msu (I32 L_var3, I16 var1, I16 var2);   /* Msu,  1  */
I32 L_macNs (I32 L_var3, I16 var1, I16 var2); /* Mac without
                                                             sat, 1   */
I32 L_msuNs (I32 L_var3, I16 var1, I16 var2); /* Msu without
                                                             sat, 1   */
I32 L_add (I32 L_var1, I32 L_var2);    /* Long add,        1 */
I32 L_sub (I32 L_var1, I32 L_var2);    /* Long sub,        1 */
I32 L_add_c (I32 L_var1, I32 L_var2);  /* Long add with c, 2 */
I32 L_sub_c (I32 L_var1, I32 L_var2);  /* Long sub with c, 2 */
I32 L_negate (I32 L_var1);                /* Long negate,     1 */
I16 mult_r (I16 var1, I16 var2);       /* Mult with round, 1 */
I32 L_shl (I32 L_var1, I16 var2);      /* Long shift left, 1 */
I32 L_shr (I32 L_var1, I16 var2);      /* Long shift right, 1 */
I16 shr_r (I16 var1, I16 var2);        /* Shift right with
                                                   round, 2           */
I16 mac_r (I32 L_var3, I16 var1, I16 var2); /* Mac with
                                                           rounding, 1 */
I16 msu_r (I32 L_var3, I16 var1, I16 var2); /* Msu with
                                                           rounding, 1 */
I32 L_deposit_h (I16 var1);        /* 16 bit var1 -> MSB,     1 */
I32 L_deposit_l (I16 var1);        /* 16 bit var1 -> LSB,     1 */

I32 L_shr_r (I32 L_var1, I16 var2); /* Long shift right with
                                                round,             3  */
I32 L_abs (I32 L_var1);            /* Long abs,              1  */
I32 L_sat (I32 L_var1);            /* Long saturation,       4  */
I16 norm_s (I16 var1);             /* Short norm,            1  */
I16 div_s (I16 var1, I16 var2); /* Short division,       18  */
I16 norm_l (I32 L_var1);           /* Long norm,             1  */
I16 norm_l_nz (I32 L_var1);           /* Long norm,             1  */


/*
 * Additional G.723.1 operators
*/
I32 L_mls( I32, I16 ) ;    /* Weight FFS; currently assigned 5 */
I16 div_l( I32, I16 ) ;    /* Weight FFS; currently assigned 32 */
I16 i_mult(I16 a, I16 b);  /* Weight FFS; currently assigned 3 */

/*
 *  New shiftless operators, not used in G.729/G.723.1
*/
I32 L_mult0(I16 v1, I16 v2); /* 32-bit Multiply w/o shift         1 */
I32 L_mac0(I32 L_v3, I16 v1, I16 v2); /* 32-bit Mac w/o shift  1 */
I32 L_msu0(I32 L_v3, I16 v1, I16 v2); /* 32-bit Msu w/o shift  1 */


#endif /* ifndef _CEVA_G729AB_BASIC_OP_H */


/* end of file */
