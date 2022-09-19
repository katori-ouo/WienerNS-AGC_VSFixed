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

#include "ceva_typedef.h"
//#include "CEVA_TL4_basic_op.h"


//*****************************************************************************
//                                                                            *
//  y[n] = -a[1]*y[n-2] + b[2]*x[n-2] + b[1]*x[n-1] + b[0]*x[n] - a[0]*y[n-1] *
//                                                                            *
//*****************************************************************************
void CEVA_TL4_iir_bqd32_410(
	I32 buf_len,
	I32 shift_val,
	I32 *x,
	I32 *y,
	I32 *a_b,		// -a[0], -a[1], b[0], b[1], b[2]
	I32 *x_scr,
	I32 *y_scr
)
{
	I32 x1, x2, y1, y2;
	I32 i, acc;
	
	x1 = x_scr[0];
	x2 = x_scr[1];
	y1 = y_scr[0];
	y2 = y_scr[1];

	for(i=0; i < buf_len; i++)
	{
		acc = 0;
		acc = L_mac_ll_sh_sat(acc, y2,   a_b[1], shift_val);
		acc = L_mac_ll_sh_sat(acc, x2,   a_b[4], shift_val);
		acc = L_mac_ll_sh_sat(acc, x1,   a_b[3], shift_val);
		acc = L_mac_ll_sh_sat(acc, x[i], a_b[2], shift_val);
		acc = L_mac_ll_sh_sat(acc, y1,   a_b[0], shift_val);

		x2 = x1;
		x1 = x[i];
		y2 = y1;
		y1 = acc;
		y[i] = acc;
	}

	x_scr[0] = x1;
	x_scr[1] = x2;
	y_scr[0] = y1;
	y_scr[1] = y2;
}

//--------------------------------------------------------------------
void CEVA_TL4_iir_bqd32_411(
	I32 buf_len,
	I32 shift_val,
	I32 *x,
	I32 *y,
	I32 *a_b,		// -a[0], -a[1], b[0], b[1], b[2]
	I32 *x_scr,
	I32 *y_scr
)
{
	I32 x1, x2, y1, y2;
	I32 i, acc;
	
	x1 = x_scr[0];
	x2 = x_scr[1];
	y1 = y_scr[0];
	y2 = y_scr[1];

	for(i=0; i < buf_len; i++)
	{
		acc = 0;
		acc = L_mac3_ll_sh_sat(acc, y2, a_b[1], x2,   a_b[4], shift_val);
		acc = L_mac3_ll_sh_sat(acc, x1, a_b[3], x[i], a_b[2], shift_val);
		acc = L_mac3_ll_sh_sat(acc, y1, a_b[0], 0,    0,      shift_val);

		x2 = x1;
		x1 = x[i];
		y2 = y1;
		y1 = acc;
		y[i] = acc;
	}

	x_scr[0] = x1;
	x_scr[1] = x2;
	y_scr[0] = y1;
	y_scr[1] = y2;
}
