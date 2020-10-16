
/**********************************************************
PROJECT NAME    :-	CW - 1200 with HMI & 1-FIT Loadcell

FILE NAME       :-	arith.c

----------
HISTORY
----------

DATE(DD-MM-YYYY)    :-	14-05-2011

Version             :-	V4.1

Author              :-	Devanand

Changes             :-	Implementation of STATS & Ranges Counts.

**********************************************************/


#include <LPC23xx.H>
#include<math.h>
#include"arith.h"
#include"memory.h"
#include"struct.h"
#include<absacc.h>
extern uchar8 *uchar8memaddr,*uchar8memaddr1;
extern uchar8 netwt[6]				__at(0x4000114e);
extern uchar8 rtc_hr_mm_sec[6]		__at(0x40001480);
extern uchar8 rtc_dt_mm_yy[6]		__at(0x40001486);
extern uchar8 avg_xbar_s1[8]		__at(0x4000134a);



/**********************************************************

Function Name   :-	asc_bcd

Description     :-	Arithmatic call to Convert Ascii fromat to BCD. 

Arguments       :-	None

Return Type     :-  BCD Value.

**********************************************************/
uint32 asc_bcd(void)
{
	uchar8 ch_acc,ch_b;
	uint32 temp_bcd_val;
	uchar8 ch_lc_buf[6];


		ch_lc_buf[0] = rx0buf[6]; 	
		ch_lc_buf[1] = rx0buf[5]; 	
		ch_lc_buf[2] = rx0buf[4]; 	
		ch_lc_buf[3] = rx0buf[3]; 	
//		ch_lc_buf[4] = rx0buf[6]; 	
//		ch_lc_buf[5] = rx0buf[7]; 	

		ch_acc = ch_lc_buf[0];		

		ch_b = ch_acc << 4;

		ch_acc = ch_lc_buf[1];		

		ch_acc = ch_acc + ch_b;
		temp_bcd_val = ch_acc;
		temp_bcd_val = temp_bcd_val << 4;
		temp_bcd_val = temp_bcd_val << 4;



	//-------------------

		ch_acc = ch_lc_buf[2];	  	

		ch_b = ch_acc << 4;

		ch_acc = ch_lc_buf[3];		  	

		ch_acc = ch_acc + ch_b;
		temp_bcd_val = temp_bcd_val + ch_acc;
		temp_bcd_val = temp_bcd_val << 4;
		temp_bcd_val = temp_bcd_val << 4;

	//--------------------

		ch_acc = ch_lc_buf[4];	   		

		//ch_acc = ch_acc & 0x0f;
		ch_b = ch_acc << 4;

		ch_acc = ch_lc_buf[5];	   		

		//ch_acc = ch_acc & 0x0f;
		ch_acc = ch_acc + ch_b;
		temp_bcd_val = temp_bcd_val + ch_acc;


		temp_bcd_val = rx0buf[5]; 
		temp_bcd_val = temp_bcd_val << 8;
		temp_bcd_val = temp_bcd_val + rx0buf[6]; 
		temp_bcd_val = temp_bcd_val << 8;
		temp_bcd_val = temp_bcd_val + rx0buf[3]; 
		temp_bcd_val = temp_bcd_val << 8;
		temp_bcd_val = temp_bcd_val + rx0buf[4]; 


		return(temp_bcd_val);

}



/**********************************************************

Function Name   :-	bincon

Description     :-	Arithmatic call for Binary Conversion. 

Arguments       :-	Bcd Value is passed.

Return Type     :-  Binary Value.

**********************************************************/
uint32 bincon(uint32 temp_bcd_buf)
{
	uchar8 ch_temp_val;
	uchar8 ch_val[4];
	uint32 temp_bin_val;

		temp_bcd_buf = temp_bcd_buf >> 4;

		ch_temp_val = temp_bcd_buf & 0x0f;
		ch_val[3] = ch_temp_val;
		temp_bcd_buf = temp_bcd_buf >> 4;

		ch_temp_val = temp_bcd_buf & 0x0f;
		ch_val[2] = ch_temp_val;
		temp_bcd_buf = temp_bcd_buf >> 4;

		ch_temp_val = temp_bcd_buf & 0x0f;
		ch_val[1] = ch_temp_val;
		temp_bcd_buf = temp_bcd_buf >> 4;

		ch_temp_val = temp_bcd_buf & 0x0f;
		ch_val[0] = ch_temp_val;
		temp_bcd_buf = temp_bcd_buf >> 4;

		ch_temp_val = temp_bcd_buf & 0x0f;

		temp_bin_val = ch_temp_val * 10000;
		ch_temp_val = ch_val[0];
		temp_bcd_buf = ch_temp_val * 1000;
		temp_bin_val = temp_bin_val + temp_bcd_buf;

		ch_temp_val = ch_val[1];
		temp_bcd_buf = ch_temp_val * 100;
		temp_bin_val = temp_bin_val + temp_bcd_buf;

		ch_temp_val = ch_val[2];
		temp_bcd_buf = ch_temp_val * 10;
		temp_bin_val = temp_bin_val + temp_bcd_buf;

		ch_temp_val = ch_val[3];
		temp_bin_val = temp_bin_val + ch_temp_val;

		return(temp_bin_val);

}





/**********************************************************

Function Name   :-	bin_bcd

Description     :-	Arithmatic call for Binary to BCD Conversion. 

Arguments       :-	None

Return Type     :-  BCD Value 

**********************************************************/
uint32 bin_bcd()
{
	uchar8 ch_temp_val;
	uchar8 ch_val[5];

		ch_temp_val = bin_val % 10;
		ch_val[4] = ch_temp_val;
		bin_val = bin_val / 10;


		ch_temp_val = bin_val % 10;
		ch_val[3] = ch_temp_val;
		bin_val = bin_val / 10;


		ch_temp_val = bin_val % 10;
		ch_val[2] = ch_temp_val;
		bin_val = bin_val / 10;
		ch_temp_val = bin_val % 10;
		ch_val[1] = ch_temp_val;

		bin_val = bin_val / 10;
		ch_temp_val = bin_val % 10;
		ch_val[0] = ch_temp_val;

		bin_val = bin_val / 10;
		ch_temp_val = bin_val % 10;
		bcd_val = ch_temp_val;
		bcd_val = bcd_val << 4;

		ch_temp_val = ch_val[0];
		bcd_val = bcd_val | ch_temp_val;
		bcd_val = bcd_val << 4;


		ch_temp_val = ch_val[1];
		bcd_val = bcd_val | ch_temp_val;
		bcd_val = bcd_val << 4;
		ch_temp_val = ch_val[2];
		bcd_val = bcd_val | ch_temp_val;
		bcd_val = bcd_val << 4;
		ch_temp_val = ch_val[3];
		bcd_val = bcd_val | ch_temp_val;


		bcd_val = bcd_val << 4;
		ch_temp_val = ch_val[4];
		bcd_val = bcd_val | ch_temp_val;

		return(bcd_val);

}



/**********************************************************

Function Name   :-	bcd_asc

Description     :-	Arithmatic call for BCD to Ascii conversion. 	

Arguments       :-	Bcd Value is passed.

Return Type     :-  void

**********************************************************/
void bcd_asc(uint32 temp_valbcd1)
{
 	uchar8 ch_temp_val1;
	

		temp_valbcd1 = bcd_val;
//		temp_valbcd1 = temp_valbcd1 >> 4;
		ch_temp_val1 = temp_valbcd1 & 0x0f;
		ch_temp_val1 = ch_temp_val1 + 0x30;
		netwt[5] = ch_temp_val1;

		netwt[4] = 0x2e;

		temp_valbcd1 = temp_valbcd1 >> 4;		
		ch_temp_val1 = temp_valbcd1 & 0x0f;
		ch_temp_val1 = ch_temp_val1 + 0x30;
		netwt[3] = ch_temp_val1;

		temp_valbcd1 = temp_valbcd1 >> 4;		
		ch_temp_val1 = temp_valbcd1 & 0x0f;
		ch_temp_val1 = ch_temp_val1 + 0x30;
		netwt[2] = ch_temp_val1;

		temp_valbcd1 = temp_valbcd1 >> 4;		
		ch_temp_val1 = temp_valbcd1 & 0x0f;
		ch_temp_val1 = ch_temp_val1 + 0x30;
		netwt[1] = ch_temp_val1;

		temp_valbcd1 = temp_valbcd1 >> 4;		
		ch_temp_val1 = temp_valbcd1 & 0x0f;
		ch_temp_val1 = ch_temp_val1 + 0x30;
		netwt[0] = ch_temp_val1;


		uchar8memaddr = (unsigned char *)INT_RAM + 0x4e;

		*uchar8memaddr = netwt[0];
		uchar8memaddr++;
		*uchar8memaddr = netwt[1];
		uchar8memaddr++;
		*uchar8memaddr = netwt[2];
		uchar8memaddr++;
		*uchar8memaddr = netwt[3];
		uchar8memaddr++;
		*uchar8memaddr = netwt[4];
		uchar8memaddr++;
		*uchar8memaddr = netwt[5];



}



/**********************************************************

Function Name   :-	prep_stats

Description     :-	Arithmatic call for BCD to Ascii conversion. 	

Arguments       :-	Bcd Value is passed.

Return Type     :-  void

**********************************************************/
void prep_stats(void)
{

	uchar8 ch_pass_ascii[7];
	unsigned long multiplier;
	uint32 bin_ok_cnts,bin_ow_cnts,bin_uw_cnts,bin_tot_cnts;

	uchar8 ch_counter,ch_temp_val;
	uint32 bcd;
	float range_lo,range_hi,delta,delta1;

	uchar8memaddr1 = (unsigned char *)EXT_RAM + 0x5a;
	uchar8memaddr = (unsigned char *)INT_RAM + 0x5a;
	for(ch_temp_val = 0;ch_temp_val < 25;ch_temp_val++)
	{
	 	*uchar8memaddr1 = *uchar8memaddr;
		 uchar8memaddr++;
		 uchar8memaddr1++;

	}

	uchar8memaddr = (unsigned char *)INT_RAM + 0x60;
	ch_pass_ascii[0] = *uchar8memaddr;
	uchar8memaddr++;
	ch_pass_ascii[1] = *uchar8memaddr;
	uchar8memaddr++;
	ch_pass_ascii[2] = *uchar8memaddr;
	uchar8memaddr++;
	ch_pass_ascii[3] = *uchar8memaddr;
	uchar8memaddr++;
	ch_pass_ascii[4] = *uchar8memaddr;
	uchar8memaddr++;
	ch_pass_ascii[5] = *uchar8memaddr;

	multiplier = 100000;
	ch_counter = 6;
	bin_ok_cnts = ascii_2_binary(ch_pass_ascii,ch_counter,multiplier);


	uchar8memaddr = (unsigned char *)INT_RAM + 0x5a;
	ch_pass_ascii[0] = *uchar8memaddr;
	uchar8memaddr++;
	ch_pass_ascii[1] = *uchar8memaddr;
	uchar8memaddr++;
	ch_pass_ascii[2] = *uchar8memaddr;
	uchar8memaddr++;
	ch_pass_ascii[3] = *uchar8memaddr;
	uchar8memaddr++;
	ch_pass_ascii[4] = *uchar8memaddr;
	uchar8memaddr++;
	ch_pass_ascii[5] = *uchar8memaddr;

	multiplier = 100000;
	ch_counter = 6;
	bin_uw_cnts = ascii_2_binary(ch_pass_ascii,ch_counter,multiplier);

	uchar8memaddr = (unsigned char *)INT_RAM + 0x66;
	ch_pass_ascii[0] = *uchar8memaddr;
	uchar8memaddr++;
	ch_pass_ascii[1] = *uchar8memaddr;
	uchar8memaddr++;
	ch_pass_ascii[2] = *uchar8memaddr;
	uchar8memaddr++;
	ch_pass_ascii[3] = *uchar8memaddr;
	uchar8memaddr++;
	ch_pass_ascii[4] = *uchar8memaddr;
	uchar8memaddr++;
	ch_pass_ascii[5] = *uchar8memaddr;

	multiplier = 100000;
	ch_counter = 6;
	bin_ow_cnts = ascii_2_binary(ch_pass_ascii,ch_counter,multiplier);

	bin_tot_cnts = bin_ok_cnts + bin_uw_cnts + bin_ow_cnts;
	bcd = binary_2_bcd(ch_counter,bin_tot_cnts);
	ch_counter = 7;

	uchar8memaddr = (unsigned char *)EXT_RAM + 0x73;
	*uchar8memaddr = 0x20;	

	uchar8memaddr = (unsigned char *)EXT_RAM + 0x72;
	bcd_2_ascii(ch_counter,bcd);
	
	bcd = bin_uw_cnts + bin_ow_cnts;
	bcd = bcd * 1000 / bin_tot_cnts;
	ch_counter = 4;   
	bcd = binary_2_bcd(ch_counter,bcd);

	uchar8memaddr = (unsigned char *)EXT_RAM + 0x79;
	*uchar8memaddr = 0x20;	


	uchar8memaddr = (unsigned char *)EXT_RAM + 0x78;
	ch_temp_val = bcd;
	ch_temp_val = ch_temp_val & 0x0f;
	ch_temp_val = ch_temp_val + 0x30;
	*uchar8memaddr = ch_temp_val;
	uchar8memaddr--;
	bcd = bcd >> 4;
	*uchar8memaddr = 0x2e;	
	ch_counter = 3;
	uchar8memaddr = (unsigned char *)EXT_RAM + 0x76;
	bcd_2_ascii(ch_counter,bcd);


	ch_counter = 5;	
	bcd = binary_2_bcd(ch_counter,max_bin);	
	uchar8memaddr = (unsigned char *)EXT_RAM + 0x97;
	ch_temp_val = bcd;
	ch_temp_val = ch_temp_val & 0x0f;
	ch_temp_val = ch_temp_val + 0x30;
	*uchar8memaddr = ch_temp_val;

	uchar8memaddr--;
	bcd = bcd >> 4;
	*uchar8memaddr = 0x2e;	
	ch_counter = 4;
	uchar8memaddr = (unsigned char *)EXT_RAM + 0x95;
	bcd_2_ascii(ch_counter,bcd);


	ch_counter = 5;	
	bcd = binary_2_bcd(ch_counter,min_bin);	
	uchar8memaddr = (unsigned char *)EXT_RAM + 0x9d;
	ch_temp_val = bcd;
	ch_temp_val = ch_temp_val & 0x0f;
	ch_temp_val = ch_temp_val + 0x30;
	*uchar8memaddr = ch_temp_val;
	uchar8memaddr--;
	bcd = bcd >> 4;
	*uchar8memaddr = 0x2e;	
	ch_counter = 4;
	uchar8memaddr = (unsigned char *)EXT_RAM + 0x9b;
	bcd_2_ascii(ch_counter,bcd);

	bcd = avg_accp_bin / bin_ok_cnts;
	ch_counter = 5;	
	bcd = binary_2_bcd(ch_counter,bcd);	
	uchar8memaddr = (unsigned char *)EXT_RAM + 0xa3;
	ch_temp_val = bcd;
	ch_temp_val = ch_temp_val & 0x0f;
	ch_temp_val = ch_temp_val + 0x30;
	*uchar8memaddr = ch_temp_val;
	uchar8memaddr--;
	bcd = bcd >> 4;
	*uchar8memaddr = 0x2e;	
	ch_counter = 4;
	uchar8memaddr = (unsigned char *)EXT_RAM + 0xa1;
	bcd_2_ascii(ch_counter,bcd);

//------------------------------------------------------------------------------

	dev_result = (float)std_dev_buffer / bin_ok_cnts;  //accp_wt_ctr; 			//std deviation 
	dev_result = dev_result / 100;
	dev_result = sqrt(dev_result);
	bcd = dev_result * 100;
	ch_counter = 3;	
	bcd = binary_2_bcd(ch_counter,bcd);	
	ch_counter = 2;
	uchar8memaddr = (unsigned char *)EXT_RAM + 0xa7;
	bcd_2_ascii(ch_counter,bcd);
	*uchar8memaddr = 0x2e;	
	uchar8memaddr--;
	bcd = bcd >> 8;
	ch_temp_val = bcd;
	ch_temp_val = ch_temp_val & 0x0f;
	ch_temp_val = ch_temp_val + 0x30;
	*uchar8memaddr = ch_temp_val;


	//--------------------------ranges--------------------

	range_hi = target_wt + upper_limit;
	range_lo = target_wt - lower_limit;
	range_hi = range_hi / 10;
	range_lo = range_lo / 10;
	delta =  range_hi - range_lo;
	delta = delta / 8;
	delta1 = delta / 2;
	delta = delta + delta1;
	range_lo = range_lo - delta;
	bcd = range_lo * 10;

	r1_lmt_bin = bcd;

	ch_counter = 5;			   							//1
	bcd = binary_2_bcd(ch_counter,bcd);	
	uchar8memaddr = (unsigned char *)EXT_RAM + 0xad;
	ch_temp_val = bcd;
	ch_temp_val = ch_temp_val & 0x0f;
	ch_temp_val = ch_temp_val + 0x30;
	*uchar8memaddr = ch_temp_val;
	uchar8memaddr--;
	bcd = bcd >> 4;
	*uchar8memaddr = 0x2e;	
	ch_counter = 4;
	uchar8memaddr = (unsigned char *)EXT_RAM + 0xab;
	bcd_2_ascii(ch_counter,bcd);

	range_lo = range_lo + delta;
	bcd = range_lo * 10;
	r2_lmt_bin = bcd;

	ch_counter = 5;						  		//2
	bcd = binary_2_bcd(ch_counter,bcd);	
	uchar8memaddr = (unsigned char *)EXT_RAM + 0xb3;
	ch_temp_val = bcd;
	ch_temp_val = ch_temp_val & 0x0f;
	ch_temp_val = ch_temp_val + 0x30;
	*uchar8memaddr = ch_temp_val;
	uchar8memaddr--;
	bcd = bcd >> 4;
	*uchar8memaddr = 0x2e;	
	ch_counter = 4;
	uchar8memaddr = (unsigned char *)EXT_RAM + 0xb1;
	bcd_2_ascii(ch_counter,bcd);

	delta = delta - delta1;
	range_lo = range_lo + delta;
	bcd = range_lo * 10;
	r3_lmt_bin = bcd;


	ch_counter = 5;						   			//3
	bcd = binary_2_bcd(ch_counter,bcd);	
	uchar8memaddr = (unsigned char *)EXT_RAM + 0xb9;
	ch_temp_val = bcd;
	ch_temp_val = ch_temp_val & 0x0f;
	ch_temp_val = ch_temp_val + 0x30;
	*uchar8memaddr = ch_temp_val;
	uchar8memaddr--;
	bcd = bcd >> 4;
	*uchar8memaddr = 0x2e;	
	ch_counter = 4;
	uchar8memaddr = (unsigned char *)EXT_RAM + 0xb7;
	bcd_2_ascii(ch_counter,bcd);

	range_lo = range_lo + delta;
	bcd = range_lo * 10;
	r4_lmt_bin = bcd;


	ch_counter = 5;								  	//4
	bcd = binary_2_bcd(ch_counter,bcd);	
	uchar8memaddr = (unsigned char *)EXT_RAM + 0xbf;
	ch_temp_val = bcd;
	ch_temp_val = ch_temp_val & 0x0f;
	ch_temp_val = ch_temp_val + 0x30;
	*uchar8memaddr = ch_temp_val;
	uchar8memaddr--;
	bcd = bcd >> 4;
	*uchar8memaddr = 0x2e;	
	ch_counter = 4;
	uchar8memaddr = (unsigned char *)EXT_RAM + 0xbd;
	bcd_2_ascii(ch_counter,bcd);


	range_lo = range_lo + delta;
	bcd = range_lo * 10;
	r5_lmt_bin = bcd;


	ch_counter = 5;								  	//5
	bcd = binary_2_bcd(ch_counter,bcd);	
	uchar8memaddr = (unsigned char *)EXT_RAM + 0xc5;
	ch_temp_val = bcd;
	ch_temp_val = ch_temp_val & 0x0f;
	ch_temp_val = ch_temp_val + 0x30;
	*uchar8memaddr = ch_temp_val;
	uchar8memaddr--;
	bcd = bcd >> 4;
	*uchar8memaddr = 0x2e;	
	ch_counter = 4;
	uchar8memaddr = (unsigned char *)EXT_RAM + 0xc3;
	bcd_2_ascii(ch_counter,bcd);


	range_lo = range_lo + delta;
	bcd = range_lo * 10;
	r6_lmt_bin = bcd;

	ch_counter = 5;								  	//6
	bcd = binary_2_bcd(ch_counter,bcd);	
	uchar8memaddr = (unsigned char *)EXT_RAM + 0xcb;
	ch_temp_val = bcd;
	ch_temp_val = ch_temp_val & 0x0f;
	ch_temp_val = ch_temp_val + 0x30;
	*uchar8memaddr = ch_temp_val;
	uchar8memaddr--;
	bcd = bcd >> 4;
	*uchar8memaddr = 0x2e;	
	ch_counter = 4;
	uchar8memaddr = (unsigned char *)EXT_RAM + 0xc9;
	bcd_2_ascii(ch_counter,bcd);


	range_lo = range_lo + delta;
	bcd = range_lo * 10;
	r7_lmt_bin = bcd;

	ch_counter = 5;								  	//7
	bcd = binary_2_bcd(ch_counter,bcd);	
	uchar8memaddr = (unsigned char *)EXT_RAM + 0xd1;
	ch_temp_val = bcd;
	ch_temp_val = ch_temp_val & 0x0f;
	ch_temp_val = ch_temp_val + 0x30;
	*uchar8memaddr = ch_temp_val;
	uchar8memaddr--;
	bcd = bcd >> 4;
	*uchar8memaddr = 0x2e;	
	ch_counter = 4;
	uchar8memaddr = (unsigned char *)EXT_RAM + 0xcf;
	bcd_2_ascii(ch_counter,bcd);

	range_lo = range_lo + delta;
	bcd = range_lo * 10;
	r8_lmt_bin = bcd;

	ch_counter = 5;								  	//8
	bcd = binary_2_bcd(ch_counter,bcd);	
	uchar8memaddr = (unsigned char *)EXT_RAM + 0xd7;
	ch_temp_val = bcd;
	ch_temp_val = ch_temp_val & 0x0f;
	ch_temp_val = ch_temp_val + 0x30;
	*uchar8memaddr = ch_temp_val;
	uchar8memaddr--;
	bcd = bcd >> 4;
	*uchar8memaddr = 0x2e;	
	ch_counter = 4;
	uchar8memaddr = (unsigned char *)EXT_RAM + 0xd5;
	bcd_2_ascii(ch_counter,bcd);

	range_lo = range_lo + delta;
	bcd = range_lo * 10;
	r9_lmt_bin = bcd;

	ch_counter = 5;								  	//9
	bcd = binary_2_bcd(ch_counter,bcd);	
	uchar8memaddr = (unsigned char *)EXT_RAM + 0xdd;
	ch_temp_val = bcd;
	ch_temp_val = ch_temp_val & 0x0f;
	ch_temp_val = ch_temp_val + 0x30;
	*uchar8memaddr = ch_temp_val;
	uchar8memaddr--;
	bcd = bcd >> 4;
	*uchar8memaddr = 0x2e;	
	ch_counter = 4;
	uchar8memaddr = (unsigned char *)EXT_RAM + 0xdb;
	bcd_2_ascii(ch_counter,bcd);

	range_lo = range_lo + delta;
	bcd = range_lo * 10;
	r10_lmt_bin = bcd;

	ch_counter = 5;								  	//10
	bcd = binary_2_bcd(ch_counter,bcd);	
	uchar8memaddr = (unsigned char *)EXT_RAM + 0xe3;
	ch_temp_val = bcd;
	ch_temp_val = ch_temp_val & 0x0f;
	ch_temp_val = ch_temp_val + 0x30;
	*uchar8memaddr = ch_temp_val;
	uchar8memaddr--;
	bcd = bcd >> 4;
	*uchar8memaddr = 0x2e;	
	ch_counter = 4;
	uchar8memaddr = (unsigned char *)EXT_RAM + 0xe1;
	bcd_2_ascii(ch_counter,bcd);


	delta = delta + delta1;
	range_lo = range_lo + delta;
	bcd = range_lo * 10;
	r11_lmt_bin = bcd;

	ch_counter = 5;								  	//11
	bcd = binary_2_bcd(ch_counter,bcd);	
	uchar8memaddr = (unsigned char *)EXT_RAM + 0xe9;
	ch_temp_val = bcd;
	ch_temp_val = ch_temp_val & 0x0f;
	ch_temp_val = ch_temp_val + 0x30;
	*uchar8memaddr = ch_temp_val;
	uchar8memaddr--;
	bcd = bcd >> 4;
	*uchar8memaddr = 0x2e;	
	ch_counter = 4;
	uchar8memaddr = (unsigned char *)EXT_RAM + 0xe7;
	bcd_2_ascii(ch_counter,bcd);

	//-------------------------- counts ----------------------------


	bcd = r1_cnt_bin; 
	ch_counter = 6;								  	
	bcd = binary_2_bcd(ch_counter,bcd);	
	ch_counter = 6;
	uchar8memaddr = (unsigned char *)EXT_RAM + 0xef; 		//1
	bcd_2_ascii(ch_counter,bcd);

	bcd = r2_cnt_bin; 
	ch_counter = 6;								  	
	bcd = binary_2_bcd(ch_counter,bcd);	
	ch_counter = 6;
	uchar8memaddr = (unsigned char *)EXT_RAM + 0xf5;		//2
	bcd_2_ascii(ch_counter,bcd);

	bcd = r3_cnt_bin; 
	ch_counter = 6;								  	
	bcd = binary_2_bcd(ch_counter,bcd);	
	ch_counter = 6;
	uchar8memaddr = (unsigned char *)EXT_RAM + 0xfb; 		//3
	bcd_2_ascii(ch_counter,bcd);

	bcd = r4_cnt_bin; 
	ch_counter = 6;								  	
	bcd = binary_2_bcd(ch_counter,bcd);	
	ch_counter = 6;
	uchar8memaddr = (unsigned char *)EXT_RAM + 0x101;  		//4
	bcd_2_ascii(ch_counter,bcd);

	bcd = r5_cnt_bin; 
	ch_counter = 6;								  	
	bcd = binary_2_bcd(ch_counter,bcd);	
	ch_counter = 6;
	uchar8memaddr = (unsigned char *)EXT_RAM + 0x107;  		//5
	bcd_2_ascii(ch_counter,bcd);

	bcd = r6_cnt_bin; 
	ch_counter = 6;								  	
	bcd = binary_2_bcd(ch_counter,bcd);	
	ch_counter = 6;
	uchar8memaddr = (unsigned char *)EXT_RAM + 0x10d; 		//6
	bcd_2_ascii(ch_counter,bcd);

	bcd = r7_cnt_bin; 
	ch_counter = 6;								  	
	bcd = binary_2_bcd(ch_counter,bcd);	
	ch_counter = 6;
	uchar8memaddr = (unsigned char *)EXT_RAM + 0x113;  		//7
	bcd_2_ascii(ch_counter,bcd);

	bcd = r8_cnt_bin; 
	ch_counter = 6;								  	
	bcd = binary_2_bcd(ch_counter,bcd);	
	ch_counter = 6;
	uchar8memaddr = (unsigned char *)EXT_RAM + 0x119; 		//8
	bcd_2_ascii(ch_counter,bcd);

	bcd = r9_cnt_bin; 
	ch_counter = 6;								  	
	bcd = binary_2_bcd(ch_counter,bcd);	
	ch_counter = 6;
	uchar8memaddr = (unsigned char *)EXT_RAM + 0x11f;		//9
	bcd_2_ascii(ch_counter,bcd);

	bcd = r10_cnt_bin; 
	ch_counter = 6;								  	
	bcd = binary_2_bcd(ch_counter,bcd);	
	ch_counter = 6;
	uchar8memaddr = (unsigned char *)EXT_RAM + 0x125; 		//10
	bcd_2_ascii(ch_counter,bcd);

	bcd = r11_cnt_bin; 
	ch_counter = 6;								  	
	bcd = binary_2_bcd(ch_counter,bcd);	
	ch_counter = 6;
	uchar8memaddr = (unsigned char *)EXT_RAM + 0x12b;		//11
	bcd_2_ascii(ch_counter,bcd);
  
	bcd = r12_cnt_bin; 
	ch_counter = 6;								  	
	bcd = binary_2_bcd(ch_counter,bcd);	
	ch_counter = 6;
	uchar8memaddr = (unsigned char *)EXT_RAM + 0x131;  		//12
	bcd_2_ascii(ch_counter,bcd);


	uchar8memaddr1 = (unsigned char *)EXT_RAM + 0x5a;		
	uchar8memaddr = (unsigned char *)INT_RAM + 0x5a;
	for(ch_temp_val = 0;ch_temp_val < 216;ch_temp_val++)
	{
	 	*uchar8memaddr = *uchar8memaddr1;
		 uchar8memaddr++;
		 uchar8memaddr1++;

	}


}



/**********************************************************

Function Name   :-	calc_range_count

Description     :-		

Arguments       :-	

Return Type     :-  void

**********************************************************/
void calc_range_count()
{

	uint32 temp_cnt;

	if(struct_obj.ch_underwtfg)
	{
		if(r1_lmt_bin <= bin_val)
		{
		 	temp_cnt = r2_cnt_bin;
			temp_cnt++; 
			r2_cnt_bin = temp_cnt;

		}
		else
		{
		 	temp_cnt = r1_cnt_bin;
			temp_cnt++; 
			r1_cnt_bin = temp_cnt;

		}
	}

	else if(struct_obj.ch_overwtfg)
	{
		if(r11_lmt_bin >= bin_val)
		{
		 	temp_cnt = r11_cnt_bin;
			temp_cnt++; 
			r11_cnt_bin = temp_cnt;

		}
		else
		{
		 	temp_cnt = r12_cnt_bin;
			temp_cnt++; 
			r12_cnt_bin = temp_cnt;

		}



	}



	else if(struct_obj.ch_acceptwtfg)
	{
		if(bin_val <= r3_lmt_bin)
		{
		 	temp_cnt = r3_cnt_bin;
			temp_cnt++; 
			r3_cnt_bin = temp_cnt;

		}
		else if(bin_val <= r4_lmt_bin)
		{
		 	temp_cnt = r4_cnt_bin;
			temp_cnt++; 
			r4_cnt_bin = temp_cnt;

		}
		else if(bin_val <= r5_lmt_bin)
		{
		 	temp_cnt = r5_cnt_bin;
			temp_cnt++; 
			r5_cnt_bin = temp_cnt;

		}
		else if(bin_val <= r6_lmt_bin)
		{
		 	temp_cnt = r6_cnt_bin;
			temp_cnt++; 
			r6_cnt_bin = temp_cnt;
						 
		}
		else if(bin_val <= r7_lmt_bin)
		{
		 	temp_cnt = r7_cnt_bin;
			temp_cnt++; 
			r7_cnt_bin = temp_cnt;

		}
		else if(bin_val <= r8_lmt_bin)
		{
		 	temp_cnt = r8_cnt_bin;
			temp_cnt++; 
			r8_cnt_bin = temp_cnt;

		}
		else if(bin_val <= r9_lmt_bin)
		{
		 	temp_cnt = r9_cnt_bin;
			temp_cnt++; 
			r9_cnt_bin = temp_cnt;

		}
		else
		{
		 	temp_cnt = r10_cnt_bin;
			temp_cnt++; 
			r10_cnt_bin = temp_cnt;

		}

	}


}





/**********************************************************

Function Name   :-	ascii_2_binary

Description     :-	Arithmatic call for BCD to Ascii conversion. 	

Arguments       :-	Bcd Value is passed.

Return Type     :-  void

**********************************************************/
void reset_stats(void)
{
	uchar8 ctr3,value2,bcd2;

	uchar8memaddr = (unsigned char *)EXT_RAM + 0xf03;

		ctr3 = rtc_dt_mm_yy[0];
		value2 = ctr3;
		bcd2 = value2 & 0x0f;
		bcd2 = bcd2 + 0x30;
		start_date[1] = bcd2;
		value2 = value2 >> 4;
		bcd2 = value2 & 0x0f;
		bcd2 = bcd2 + 0x30;
		start_date[0] = bcd2;
		start_date[2] = '/';

		ctr3 = rtc_dt_mm_yy[2];
		value2 = ctr3;
		bcd2 = value2 & 0x0f;
		bcd2 = bcd2 + 0x30;
		start_date[4] = bcd2;
		value2 = value2 >> 4;
		bcd2 = value2 & 0x0f;
		bcd2 = bcd2 + 0x30;
		start_date[3] = bcd2;
		start_date[5] = '/';


		ctr3 = rtc_dt_mm_yy[4];
		value2 = ctr3;
		bcd2 = value2 & 0x0f;
		bcd2 = bcd2 + 0x30;
		start_date[7] = bcd2;
		value2 = value2 >> 4;
		bcd2 = value2 & 0x0f;
		bcd2 = bcd2 + 0x30;
		start_date[6] = bcd2;

		*uchar8memaddr = start_date[0];
		uchar8memaddr++;
		*uchar8memaddr = start_date[1];
		uchar8memaddr++;
		*uchar8memaddr = start_date[3];
		uchar8memaddr++;
		*uchar8memaddr = start_date[4];
		uchar8memaddr++;
		*uchar8memaddr = start_date[6];
		uchar8memaddr++;
		*uchar8memaddr = start_date[7];


	uchar8memaddr = (unsigned char *)EXT_RAM + 0xf09;


		ctr3 = rtc_hr_mm_sec[0];//1
		value2 = ctr3;
		bcd2 = value2 & 0x0f;
		bcd2 = bcd2 + 0x30;
		start_time[1] = bcd2;
		value2 = value2 >> 4;
		bcd2 = value2 & 0x0f;
		bcd2 = bcd2 + 0x30;
		start_time[0] = bcd2;
		start_time[2] = ':';


		ctr3 = rtc_hr_mm_sec[2];//3
		value2 = ctr3;
		bcd2 = value2 & 0x0f;
		bcd2 = bcd2 + 0x30;
		start_time[4] = bcd2;
		value2 = value2 >> 4;
		bcd2 = value2 & 0x0f;
		bcd2 = bcd2 + 0x30;
		start_time[3] = bcd2;
		start_time[5] = ':';

		ctr3 = rtc_hr_mm_sec[4];//5
		value2 = ctr3;
		bcd2 = value2 & 0x0f;
		bcd2 = bcd2 + 0x30;
		start_time[7] = bcd2;
		value2 = value2 >> 4;
		bcd2 = value2 & 0x0f;
		bcd2 = bcd2 + 0x30;
		start_time[6] = bcd2;

		*uchar8memaddr = start_time[0];
		uchar8memaddr++;
		*uchar8memaddr = start_time[1];
		uchar8memaddr++;
		*uchar8memaddr = start_time[3];
		uchar8memaddr++;
		*uchar8memaddr = start_time[4];
		uchar8memaddr++;
		*uchar8memaddr = start_time[6];
		uchar8memaddr++;
		*uchar8memaddr = start_time[7];


		r1_cnt_bin = 0;
		r2_cnt_bin = 0;
		r3_cnt_bin = 0;
		r4_cnt_bin = 0;
		r5_cnt_bin = 0;
		r6_cnt_bin = 0;
		r7_cnt_bin = 0;
		r8_cnt_bin = 0;
		r9_cnt_bin = 0;
		r10_cnt_bin = 0;
		r11_cnt_bin = 0;
		r12_cnt_bin = 0;
		max_bin = 0;
		min_bin = 0;
		std_dev_buffer = 0;
		avg_accp_bin = 0;
		ch_comparefg = 0;
		accp_wt_ctr = 0;
		bin_rej_ctr = 0;

		uchar8memaddr = (unsigned char *)EXT_RAM + 0x5a;
		uchar8memaddr1 = (unsigned char *)INT_RAM + 0x5a;
		for(ctr3 = 0;ctr3 < 25;ctr3++)
		{

		 	if(*uchar8memaddr != 0x2e)
			{
				*uchar8memaddr = 0x30;
				*uchar8memaddr1 = 0x30;

			}
			else
			{
				*uchar8memaddr = 0x30;
				*uchar8memaddr1 = 0x30;

			}
			uchar8memaddr++;
			uchar8memaddr1++;

		}

}


/**********************************************************

Function Name   :-	ascii_2_binary

Description     :-	Arithmatic call for BCD to Ascii conversion. 	

Arguments       :-	Bcd Value is passed.

Return Type     :-  void

**********************************************************/
uint32 ascii_2_binary(uchar8 *ptr,uchar8 ch_counter,uint32 ctr2)
{
	uchar8 i,ch_val;
	uint32 chtr1,chtr2,chtr3;

		for(i =ch_counter; i > 0;i-- )
		{
		 	ch_val = *ptr;
			if(ch_val == 0x2e)
			{
			 	ptr++;
			 	ch_val = *ptr;
			} 
		 	chtr1 = ch_val & 0x0f;
		 	chtr2 = chtr1 * ctr2;
		 	chtr3 = chtr3 + chtr2;
		 	ctr2 = ctr2 / 10;
		 	ptr++;
		}

	return chtr3;
}


/**********************************************************

Function Name   :-	binary_2_bcd

Description     :-	Arithmatic call for BCD to Ascii conversion. 	

Arguments       :-	Bcd Value is passed.

Return Type     :-  void

**********************************************************/
uint32 binary_2_bcd(uchar8 ch_counter,uint32 ctr3)
{

	uchar8 value2[10],value1,i,j;
	uint32 bcd1,bcd2;
	j = 0;
	for(i=ch_counter;i>0;i-- )
	{
	 	value1 = ctr3 % 10;
		value2[i] = value1;
		ctr3 = ctr3 / 10;
		j++;

	}
	bcd2 = value1;
	bcd2 = bcd2 << 4;
	for(i = 2;i <= j;i++)
	{

	 	 bcd1 = value2[i];
		 bcd2 = bcd2 | bcd1;
		 bcd2 = bcd2 << 4;

	}
	bcd2 = bcd2 >> 4;
	return bcd2;
}


/**********************************************************

Function Name   :-	bcd_2_ascii

Description     :-	Arithmatic call for BCD to Ascii conversion. 	

Arguments       :-	Bcd Value is passed.

Return Type     :-  void

**********************************************************/
void bcd_2_ascii(uchar8 ch_counter,uint32 bcd)
{
	uchar8 ch_temp_ctr,ch_temp_val;
	for(ch_temp_ctr=ch_counter;ch_temp_ctr>0;ch_temp_ctr--)
	{
		ch_temp_val = bcd;
		ch_temp_val = ch_temp_val & 0x0f;
		ch_temp_val = ch_temp_val + 0x30;
		*uchar8memaddr = ch_temp_val;
		uchar8memaddr--;
		bcd = bcd >> 4;	

	}
}

void get_avg_xbar(void)
{

	uchar8 ch_temp_val,ch_counter;
	uint32 bcd_1;	

	avg[avg_stk_ctr] = bin_val;		
	if(avg_count == 9)
	{
		if(avg_stk_ctr >= 9)
		{
			avg_stk_ctr = 0;
			avg_bin = 0;				
		}
		else
		{
			avg_stk_ctr++;
			avg_bin = 0;
		}

		for(ch_counter = 0 ; ch_counter < 10 ; ch_counter++)
		{
			avg_bin = avg_bin + avg[ch_counter];							
		}
			avg_bin = avg_bin/10;
   			uchar8memaddr = (unsigned char *)INT_RAM + 0x24a; 					//230
	   		*uchar8memaddr = 0x20;
			bcd_1 = avg_bin; 
			bcd_1 = binary_2_bcd(5,bcd_1);	
			uchar8memaddr = (unsigned char *)INT_RAM + 0x24f;  					//235
			ch_temp_val = bcd_1;
			ch_temp_val = ch_temp_val & 0x0f;
			ch_temp_val = ch_temp_val + 0x30;
			*uchar8memaddr = ch_temp_val;
			uchar8memaddr--;
			bcd_1 = bcd_1 >> 4;
			*uchar8memaddr = 0x2e;	
			ch_counter = 4;
			uchar8memaddr = (unsigned char *)INT_RAM + 0x24d;	 				//233
			bcd_2_ascii(ch_counter,bcd_1);
			 		
			uchar8memaddr = (unsigned char *)EXT_RAM + 0x24a;					//230
			*uchar8memaddr = avg_xbar_s1[0];
			uchar8memaddr++;
			*uchar8memaddr = avg_xbar_s1[1];
			uchar8memaddr++;
			*uchar8memaddr = avg_xbar_s1[2];
			uchar8memaddr++;
			*uchar8memaddr = avg_xbar_s1[3];
			uchar8memaddr++;
			*uchar8memaddr = avg_xbar_s1[4];
			uchar8memaddr++;
			*uchar8memaddr = avg_xbar_s1[5];
			uchar8memaddr++;
			*uchar8memaddr = avg_xbar_s1[6];
	
	}
	else
	{
		avg_stk_ctr++;
		avg_count++;
	}

}
