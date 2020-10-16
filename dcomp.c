

#include <LPC23xx.H>
#include"dcomp.h"
#include"Type.h"
#include"memory.h"
#include"struct.h"
#include<absacc.h>


extern uchar8 dcomp[8]	__at(0x40001234);
extern uchar8 hmi_coils0       __at(0x40001700);
extern uchar8 hmi_coils1       __at(0x40001701);
extern uchar8 hmi_coils2       __at(0x40001702);
extern uchar8 hmi_coils3       __at(0x40001703);
extern uchar8 hmi_coils4       __at(0x40001704);





void dcomp_routine(void)
{
	struct_obj.dcompfg = 1;
	dcomp_bin = 0;
	dcomp_ctr = 0;
	flag_array[0] = 2;
	struct_obj.conv_on_thrdfg = 1;
	conv_thrd_ctr = 1;

//	conv_onoff();
}


void dcomp_calc(void)
{

	uint32 bcd;
	uchar8 ch_temp_val,ch_counter;

	dcomp_bin = dcomp_bin + bin_val;
	dcomp_ctr++;
	if(dcomp_ctr == 20)
	{
		dcomp_bin = dcomp_bin / 20;
		if(target_wt >= dcomp_bin)
		{
			dcomp_bin = target_wt - dcomp_bin;
			uchar8memaddr = (unsigned char *)INT_RAM + 0x134;
			*uchar8memaddr = 0x20;

		}
		else
		{
			dcomp_bin = dcomp_bin - target_wt;
			uchar8memaddr = (unsigned char *)INT_RAM + 0x134;
			*uchar8memaddr = 0x2d;

		}
		bcd = dcomp_bin; 
		bcd = binary_2_bcd(5,bcd);	
		uchar8memaddr = (unsigned char *)INT_RAM + 0x13a;
		ch_temp_val = bcd;
		ch_temp_val = ch_temp_val & 0x0f;
		ch_temp_val = ch_temp_val + 0x30;
		*uchar8memaddr = ch_temp_val;
		uchar8memaddr--;
		bcd = bcd >> 4;
		*uchar8memaddr = 0x2e;	
		ch_counter = 4;
		uchar8memaddr = (unsigned char *)INT_RAM + 0x138;
		bcd_2_ascii(ch_counter,bcd);

		struct_obj.convonfg = 0;
		hmi_coils0=hmi_coils0&0xfe;

		IOCLR0 |= 0x00010000;
		IOCLR0 |= 0x000E0000;
		T2TCR = 0;
		T2TC = 0;
				
		struct_obj.dcompfg = 0;
		uchar8memaddr = (unsigned char *)INT_RAM + 0x134;
		save_EPR(0,1,7);
		

		uchar8memaddr = (unsigned char *)EXT_RAM + 0x134;
		*uchar8memaddr = dcomp[0];
		uchar8memaddr++;
		*uchar8memaddr = dcomp[1];
		uchar8memaddr++;
		*uchar8memaddr = dcomp[2];
		uchar8memaddr++;
		*uchar8memaddr = dcomp[3];
		uchar8memaddr++;
		*uchar8memaddr = dcomp[4];
		uchar8memaddr++;
		*uchar8memaddr = dcomp[5];
		uchar8memaddr++;
		*uchar8memaddr = dcomp[6];
													
	}
}


void chk_dcomp_on(void)
{

	if(ch_dcomp_onfg)
	{
	 	if(dcomp[0] == 0x20)
		{

		 	bin_val = bin_val + dcomp_bin; 

		}
		else
		{
			if(bin_val >= dcomp_bin)
			{
		 		bin_val = bin_val - dcomp_bin; 
			}
			else
			{
			 	bin_val = 0;
			}
		}
	}
}


