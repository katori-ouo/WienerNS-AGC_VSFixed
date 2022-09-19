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


void CEVA_TL4_BitRev_int32(I32 data[], I32 data_out[], I32 nLog2np, I32 tmp[])
{
	I32 np = (1<<nLog2np);
	I32 *px = data;
	I32 *pxk, *pxi;
	I32 *px_out = data_out;
	I32 *pxk_out, *pxi_out;
	I32 i, j, k;

	if (np > 2)
	{
		I32 n21, nn2;
		I32 tmp;

		nn2 = np / 2;  
		n21 = np + 1;  
		j = 0;
		for (i = 0; i < np; i += 4) { 

			if (i <= j) {
				// swap 4 pairs of values (2 complex pairs with 2 others)
				pxi = &px[i];
				pxk = &px[j];
				pxi_out = &px_out[i];
				pxk_out = &px_out[j];

				tmp = *pxi++;
				*pxi_out++ = *pxk++;
				*pxk_out++ = tmp;



				tmp = *pxi;
				*pxi_out = *pxk;
				*pxk_out = tmp;

				pxi  += n21;
				pxk  += n21;
				pxi_out  += n21;
				pxk_out  += n21;

				tmp = *pxi++;
				*pxi_out++ = *pxk++;
				*pxk_out++ = tmp;

				tmp = *pxi;
				*pxi_out = *pxk;
				*pxk_out = tmp;
			}

			// swap 2 pairs of values (1 complex pair with another)
			pxi = &px[i+2];
			pxk = &px[j+np];
			pxi_out = &px_out[i+2];
			pxk_out = &px_out[j+np];

			tmp = *pxi++;
			*pxi_out++ = *pxk++;
			*pxk_out++ = tmp;

			tmp = *pxi;
			*pxi_out = *pxk;
			*pxk_out = tmp;

			k = nn2;
			while (k <= j) {     
				j -= k;
				k = k / 2;
			}
			j += k;              
		}
	}
}
