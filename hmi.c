
/**********************************************************
PROJECT NAME    :-	CW - 1200 with HMI & 1-FIT Loadcell

FILE NAME       :-	hmi.c

----------
HISTORY
----------

DATE(DD-MM-YYYY)    :-	27-06-2011

Version             :-	V7.11

Author              :-	Devanand

Changes             :-	Checking the status of 3-level password
						Flags.
						Also Calibration SW inout is read to
						show the error as per the convdition. 

**********************************************************/


#include <LPC23xx.H>
#include"hmi.h"
#include"Type.h"
#include"memory.h"
#include"struct.h"
#include"constant.h"


#include<absacc.h>



extern const uint32 luk_speed_mpm[];

/***************  RAM Location of LPC2378 *************************/

uchar8 ch_serialno1[4]		__at(0x40001100);
uchar8 ch_product_code[16]	__at(0x40001104);
uchar8 ch_product_name[16]	__at(0x40001114);
//uchar8 ch_batch_no[16]		__at(0x4000111a);
uchar8 ch_targetwt[6]		__at(0x40001124);
uchar8 ch_tarewt[6]			__at(0x4000112a);
uchar8 ch_length[4]			__at(0x40001130);
uchar8 ch_upper_limit[6]	__at(0x40001134);
uchar8 ch_lower_limit[6]	__at(0x4000113a);
uchar8 ch_speed[4]			__at(0x40001140);
uchar8 ch_operate[2]		__at(0x40001144);
uchar8 ch_hold[2]			__at(0x40001146);
uchar8 ch_cont_reg[2]		__at(0x40001148);

uchar8 netwt[6]				__at(0x4000114e);
uchar8 trend_netwt[6]		__at(0x40001154);

uchar8 uw_counts[6]			__at(0x4000115a);
uchar8 ok_counts[6]			__at(0x40001160);
uchar8 ow_counts[6]			__at(0x40001166);

uchar8 tot_counts[7]		__at(0x4000116c);
uchar8 rej_perctage[5]		__at(0x40001174);
uchar8 max_wt[6]			__at(0x40001192);
uchar8 min_wt[6]			__at(0x40001198);
uchar8 avg_accp_wt[6]		__at(0x4000119e);
uchar8 std_deviation[4]		__at(0x400011a4);

uchar8 range1_limit[6]		__at(0x400011a8);
uchar8 range2_limit[6]		__at(0x400011ae);
uchar8 range3_limit[6]		__at(0x400011b4);
uchar8 range4_limit[6]		__at(0x400011ba);
uchar8 range5_limit[6]		__at(0x400011c0);
uchar8 range6_limit[6]		__at(0x400011c6);
uchar8 range7_limit[6]		__at(0x400011cc);
uchar8 range8_limit[6]		__at(0x400011d2);
uchar8 range9_limit[6]		__at(0x400011d8);
uchar8 range10_limit[6]		__at(0x400011de);
uchar8 range11_limit[6]		__at(0x400011e4);


uchar8 range1_count[6]		__at(0x400011ea);
uchar8 range2_count[6]		__at(0x400011f0);
uchar8 range3_count[6]		__at(0x400011f6);
uchar8 range4_count[6]		__at(0x400011fc);
uchar8 range5_count[6]		__at(0x40001202);
uchar8 range6_count[6]		__at(0x40001208);
uchar8 range7_count[6]		__at(0x4000120e);
uchar8 range8_count[6]		__at(0x40001214);
uchar8 range9_count[6]		__at(0x4000121a);
uchar8 range10_count[6]		__at(0x40001220);
uchar8 range11_count[6]		__at(0x40001226);
uchar8 range12_count[6]		__at(0x4000122c);

uchar8 dcomp[8]				__at(0x40001234);

uchar8 max_pack[3]			__at(0x4000123c);
uchar8 input_ppm[3]			__at(0x40001240);		//input speed calc


uchar8 master_version[20]	__at(0x40001248);


uchar8 prdlib_srno1[4]		__at(0x40001260);
uchar8 prdlib_code1[16]		__at(0x40001264);
uchar8 prdlib_name1[16]		__at(0x40001274);

uchar8 prdlib_srno2[4]		__at(0x40001284);
uchar8 prdlib_code2[16]		__at(0x40001288);
uchar8 prdlib_name2[16]		__at(0x40001298);

uchar8 prdlib_srno3[4]		__at(0x400012a8);
uchar8 prdlib_code3[16]		__at(0x400012ac);
uchar8 prdlib_name3[16]		__at(0x400012bc);

uchar8 prdlib_srno4[4]		__at(0x400012cc);
uchar8 prdlib_code4[16]		__at(0x400012d0);
uchar8 prdlib_name4[16]		__at(0x400012e0);

uchar8 prdlib_srno5[4]		__at(0x400012f0);
uchar8 prdlib_code5[16]		__at(0x400012f4);
uchar8 prdlib_name5[16]		__at(0x40001304);

uchar8 prdlib_srno6[4]		__at(0x40001314);
uchar8 prdlib_code6[16]		__at(0x40001318);
uchar8 prdlib_name6[16]		__at(0x40001328);

//uchar8 log_data[30]			__at(0x40001300);	//used for logging data in print format.
//uchar8 avg_xbar_s1[8]		__at(0x40001330);

uchar8 ch_batch_no[16]		__at(0x4000133a);

uchar8 avg_xbar_s1[8]		__at(0x4000134a);

uchar8 fb_startcount[2]		__at(0x400013a6);
uchar8 fb_avgcount[2]		__at(0x400013a8);
uchar8 fb_neutcount[2]		__at(0x400013aa);
uchar8 fb_errcount[2]		__at(0x400013ac);
uchar8 fb_acplimitp[4]		__at(0x400013ae);
uchar8 fb_acplimitm[4]		__at(0x400013b2);
uchar8 fb_limitp[4]			__at(0x400013b6);
uchar8 fb_limitm[4]			__at(0x400013ba);
uchar8 fb_pulsecount[2]		__at(0x400013be);

uchar8 login_id[16]			__at(0x400013ce);
uchar8 login_pwd[6]			__at(0x400013de);

uchar8 login_set_id[16]		__at(0x40001452);
uchar8 login_set_pwd[6]		__at(0x40001462);

uchar8 rtc_hr_mm_sec[6]		__at(0x40001480);
uchar8 rtc_dt_mm_yy[6]		__at(0x40001486);

/*********************** ram location-check input bit *************************/

uchar8 hmi_flags0 		__at(0x40001600);
uchar8 hmi_flags1 		__at(0x40001601);
uchar8 hmi_flags2 		__at(0x40001602);
uchar8 hmi_flags3 		__at(0x40001603);

/********************** ram location-read coil bit **************************/

uchar8 hmi_coils0       __at(0x40001700);
uchar8 hmi_coils1       __at(0x40001701);
uchar8 hmi_coils2       __at(0x40001702);
uchar8 hmi_coils3       __at(0x40001703);
uchar8 hmi_coils4       __at(0x40001704);



/**********************************************************

Function Name   :-	hmi_dat

Description     :-	HMI Query - Response Serve Logic. 

Arguments       :-	None

Return Type     :-  None

**********************************************************/
void hmi_dat(void)
{

	uchar8 ch_hmifunc_code,ch_hmi_slv_addr;

		ch_hmi_slv_addr = rx1buf[0];
		if(ch_hmi_slv_addr == 0x01)
		{
		 		ch_hmifunc_code = rx1buf[1];

				switch(ch_hmifunc_code)
				{

			 			case 0x01 :
									read_coil_bit();
									break;

			 			case 0x02 :
									chk_input_bit();
									break;
			 			case 0x03 :
									read_hld_reg();
									break;

			 			case 0x05 :
									chk_fc_bit();
									break;

			 			case 0x10 :
									chk_multiple_preset_reg();
									break;

						default :
									break;				
				
				}

		}
}


/**********************************************************

Function Name   :-	read_hld_reg

Description     :-	Reads the Holding Register Value & Transmits to HMI 

Arguments       :-	None

Return Type     :-  None

**********************************************************/
void read_hld_reg(void)
{
	uchar8 ch_hmi_func_code,ch_hmi_slv_addr,ch_hmi_byte_cnt;

	uchar8 ch_temp_ctr,i;
	uint32 mem_ctr,mem_ctr1;

   
		ch_hmi_slv_addr = rx1buf[0];
		tx1buf[0] = ch_hmi_slv_addr;

		ch_hmi_func_code = rx1buf[1];
		tx1buf[1] = ch_hmi_func_code;

		ch_hmi_byte_cnt = rx1buf[5];
		ch_hmi_byte_cnt = 2 * ch_hmi_byte_cnt;
		tx1buf[2] = ch_hmi_byte_cnt;

		ch_crc_len = ch_hmi_byte_cnt + 3;

		mem_ctr = rx1buf[2];
		mem_ctr = mem_ctr * 2;
		mem_ctr = mem_ctr << 8;

		mem_ctr1 = rx1buf[3];
		mem_ctr1 = mem_ctr1 * 2;
		mem_ctr = mem_ctr + mem_ctr1;

		uchar8memaddr = (unsigned char *)INT_RAM + mem_ctr;
		i = 3;
		for(ch_temp_ctr = 0;ch_temp_ctr <= ch_hmi_byte_cnt;ch_temp_ctr++)
		{

 				tx1buf[i] = *uchar8memaddr;
				i++;
				uchar8memaddr++;

		}

		ch_txmit1_len = ch_crc_len + 2;
		get_crc();
		xmitbuf1();  
}





/**********************************************************

Function Name   :-	chk_multiple_preset_reg

Description     :-  To sort the data present in reg & copy to CPU
					memory.	 

Arguments       :-	None

Return Type     :-  None

**********************************************************/
void chk_multiple_preset_reg(void)
{

	uchar8 ch_st_addr_lo;
	uint32 st_addr;
	uchar8 ch_e2addr_L,ch_e2addr_H;
	uint32 epr_mem;
	uchar8 temp_array[6];

			ch_st_addr_lo = rx1buf[3];
			st_addr = rx1buf[2];
			st_addr = st_addr << 8;
			st_addr = st_addr + ch_st_addr_lo;  
			switch(st_addr)
			{


			 		case 0x00 :
									uchar8memaddr = (unsigned char *)INT_RAM + 0x00;
									store_param(2);

									uchar8memaddr = (unsigned char *)INT_RAM + 0x01;
									temp_array[0] = *uchar8memaddr;
									uchar8memaddr--;
									temp_array[1] = *uchar8memaddr;


									uchar8memaddr = (unsigned char *)INT_RAM + 0x03;
									ch_e2addr_L = temp_array[1];
									*uchar8memaddr = ch_e2addr_L & 0x0f;
									*uchar8memaddr = *uchar8memaddr + 0x30;									
									uchar8memaddr--;
									ch_e2addr_L = temp_array[1];
									ch_e2addr_L = ch_e2addr_L >> 4;
									*uchar8memaddr = ch_e2addr_L & 0x0f;
									*uchar8memaddr = *uchar8memaddr + 0x30;									


									uchar8memaddr--;
									ch_e2addr_L = temp_array[0];
									*uchar8memaddr = ch_e2addr_L & 0x0f;
									*uchar8memaddr = *uchar8memaddr + 0x30;									
									uchar8memaddr--;
									ch_e2addr_L = temp_array[0];
									ch_e2addr_L = ch_e2addr_L >> 4;
									*uchar8memaddr = ch_e2addr_L & 0x0f;
									*uchar8memaddr = *uchar8memaddr + 0x30;									

									
									uchar8memaddr = (unsigned char *)INT_RAM + 0x00;
									s1p1_bin = ascii_2_binary(uchar8memaddr,4,1000);

									if(s1p1_bin >= 199)
									{
										s1p1_bin = 199;
										uchar8memaddr = (unsigned char *)INT_RAM + 0x00;
										*uchar8memaddr = 0x30;
										uchar8memaddr++;
										*uchar8memaddr = 0x31;
										uchar8memaddr++;
										*uchar8memaddr = 0x39;
										uchar8memaddr++;
										*uchar8memaddr = 0x39;


									}
									delay(4); 

									epr_mem = s1p1_bin * E2P_BLOCK_SIZE;	
									epr_mem = epr_mem + e2p_srno;
									ch_e2addr_L = epr_mem;
									ch_e2addr_H = epr_mem >> 8; 
									uchar8memaddr = (unsigned char *)INT_RAM + 0x00;
									save_EPR(ch_e2addr_H,ch_e2addr_L,4);
									reset_stats();
									get_prd();

									break;

			 		case 0x02 :

									uchar8memaddr = (unsigned char *)INT_RAM + 0x04;
									store_param(16);

									uchar8memaddr = (unsigned char *)INT_RAM + 0x04;
									for(epr_mem = 0;epr_mem < 16;epr_mem++)
									{
										if(*uchar8memaddr == 0x00)
										{
											*uchar8memaddr = 0x20;
										}
										uchar8memaddr++;
									}

									epr_mem = s1p1_bin * E2P_BLOCK_SIZE;
									epr_mem = epr_mem + e2p_prd_code;
									ch_e2addr_L = epr_mem;
									ch_e2addr_H = epr_mem >> 8; 


									uchar8memaddr = (unsigned char *)INT_RAM + 0x04;
									save_EPR(ch_e2addr_H,ch_e2addr_L,16);

									break;

			 		case 0x0a :

									uchar8memaddr = (unsigned char *)INT_RAM + 0x14;
									store_param(16);

									uchar8memaddr = (unsigned char *)INT_RAM + 0x14;
									for(epr_mem = 0;epr_mem < 16;epr_mem++)
									{
										if(*uchar8memaddr == 0x00)
										{
											*uchar8memaddr = 0x20;
										}
										uchar8memaddr++;
									}
									epr_mem = s1p1_bin * E2P_BLOCK_SIZE;	
									epr_mem = epr_mem + e2p_prd_name;									
									ch_e2addr_L = epr_mem;
									ch_e2addr_H = epr_mem >> 8; 


									uchar8memaddr = (unsigned char *)INT_RAM + 0x14;
									save_EPR(ch_e2addr_H,ch_e2addr_L,16);


									break;
/*

			 		case 0x0d :

									uchar8memaddr = (unsigned char *)INT_RAM + 0x1a;
									store_param(10);

									uchar8memaddr = (unsigned char *)INT_RAM + 0x1a;
									for(epr_mem = 0;epr_mem < 10;epr_mem++)
									{
										if(*uchar8memaddr == 0x00)
										{
											*uchar8memaddr = 0x20;
										}
										uchar8memaddr++;
									}

									epr_mem = s1p1_bin * E2P_BLOCK_SIZE; //64
									epr_mem = epr_mem + 0x24;

									ch_e2addr_L = epr_mem;
									ch_e2addr_H = epr_mem >> 8; 
									uchar8memaddr = (unsigned char *)INT_RAM + 0x1a;
									save_EPR(ch_e2addr_H,ch_e2addr_L,10);
									reset_stats();

									break;
*/
			 		case 0x12 :

									uchar8memaddr = (unsigned char *)INT_RAM + 0x24;
									store_param(3);

									uchar8memaddr = (unsigned char *)INT_RAM + 0x26;
									temp_array[0] = *uchar8memaddr;
									uchar8memaddr--;
									temp_array[1] = *uchar8memaddr;
									uchar8memaddr--;
									temp_array[2] = *uchar8memaddr;

									uchar8memaddr = (unsigned char *)INT_RAM + 0x29;
									ch_e2addr_L = temp_array[2];
									*uchar8memaddr = ch_e2addr_L & 0x0f;
									*uchar8memaddr = *uchar8memaddr + 0x30;									
									uchar8memaddr--;
									*uchar8memaddr = 0x2e;
									uchar8memaddr--;
									ch_e2addr_L = temp_array[2];
									ch_e2addr_L = ch_e2addr_L >> 4;
									*uchar8memaddr = ch_e2addr_L & 0x0f;
									*uchar8memaddr = *uchar8memaddr + 0x30;									

									uchar8memaddr--;
									ch_e2addr_L = temp_array[1];
									*uchar8memaddr = ch_e2addr_L & 0x0f;
									*uchar8memaddr = *uchar8memaddr + 0x30;									
																		
									uchar8memaddr--;
									ch_e2addr_L = temp_array[1];
									ch_e2addr_L = ch_e2addr_L >> 4;
									*uchar8memaddr = ch_e2addr_L & 0x0f;
									*uchar8memaddr = *uchar8memaddr + 0x30;									

									uchar8memaddr--;
									ch_e2addr_L = temp_array[0];
									*uchar8memaddr = ch_e2addr_L & 0x0f;
									*uchar8memaddr = *uchar8memaddr + 0x30;	
									
									uchar8memaddr = (unsigned char *)INT_RAM + 0x24;
									target_wt = ascii_2_binary(uchar8memaddr,5,10000); 
									
									if(target_wt > OW_ERROR_COUNT)	
									{
									 	uchar8memaddr = (unsigned char *)INT_RAM + 0x24;
										*uchar8memaddr = 0x31;
										uchar8memaddr++;
										*uchar8memaddr = 0x31;
										uchar8memaddr++;
										*uchar8memaddr = 0x30;
										uchar8memaddr++;
										*uchar8memaddr = 0x30;
										uchar8memaddr++;
										*uchar8memaddr = 0x2e;
										uchar8memaddr++;
										*uchar8memaddr = 0x30;
									
									}							

//									uchar8memaddr = (unsigned char *)INT_RAM + 0x24;
//									if(*uchar8memaddr >= 0x32)
//									{
//										*uchar8memaddr = 0x32;
//										uchar8memaddr++;
//										*uchar8memaddr = 0x30;
//										uchar8memaddr++;
//										*uchar8memaddr = 0x30;
//										uchar8memaddr++;
//										*uchar8memaddr = 0x30;
//										uchar8memaddr++;
//										*uchar8memaddr = 0x2e;
//										uchar8memaddr++;
//										*uchar8memaddr = 0x30;
//									}


									epr_mem = s1p1_bin * E2P_BLOCK_SIZE;
									epr_mem = epr_mem + e2p_target_wt;
									ch_e2addr_L = epr_mem;
									ch_e2addr_H = epr_mem >> 8; 

									uchar8memaddr = (unsigned char *)INT_RAM + 0x24;
									save_EPR(ch_e2addr_H,ch_e2addr_L,6);

									uchar8memaddr = (unsigned char *)INT_RAM + 0x24;
									target_wt = ascii_2_binary(uchar8memaddr,5,10000); 
									 
									break;

			 		case 0x15 :
									uchar8memaddr = (unsigned char *)INT_RAM + 0x2a;
									store_param(3);
							
									uchar8memaddr = (unsigned char *)INT_RAM + 0x2c;
									temp_array[0] = *uchar8memaddr;
									uchar8memaddr--;
									temp_array[1] = *uchar8memaddr;
									uchar8memaddr--;
									temp_array[2] = *uchar8memaddr;

									uchar8memaddr = (unsigned char *)INT_RAM + 0x2f;
									ch_e2addr_L = temp_array[2];
									*uchar8memaddr = ch_e2addr_L & 0x0f;
									*uchar8memaddr = *uchar8memaddr + 0x30;									
									uchar8memaddr--;
									*uchar8memaddr = 0x2e;
									uchar8memaddr--;
									ch_e2addr_L = temp_array[2];
									ch_e2addr_L = ch_e2addr_L >> 4;
									*uchar8memaddr = ch_e2addr_L & 0x0f;
									*uchar8memaddr = *uchar8memaddr + 0x30;									

									uchar8memaddr--;
									ch_e2addr_L = temp_array[1];
									*uchar8memaddr = ch_e2addr_L & 0x0f;
									*uchar8memaddr = *uchar8memaddr + 0x30;									
																		
									uchar8memaddr--;
									ch_e2addr_L = temp_array[1];
									ch_e2addr_L = ch_e2addr_L >> 4;
									*uchar8memaddr = ch_e2addr_L & 0x0f;
									*uchar8memaddr = *uchar8memaddr + 0x30;									

									uchar8memaddr--;
									ch_e2addr_L = temp_array[0];
									*uchar8memaddr = ch_e2addr_L & 0x0f;
									*uchar8memaddr = *uchar8memaddr + 0x30;		
									
									uchar8memaddr = (unsigned char *)INT_RAM + 0x2a;
									tare_wt = ascii_2_binary(uchar8memaddr,5,10000);

									
									epr_mem = s1p1_bin * E2P_BLOCK_SIZE;
									epr_mem = epr_mem + e2p_tare_wt;
									ch_e2addr_L = epr_mem;
									ch_e2addr_H = epr_mem >> 8; 
									
									if(tare_wt >= target_wt)
									{
										uchar8memaddr = (unsigned char *)INT_RAM + 0x2a;
										read_EPR1(ch_e2addr_H,ch_e2addr_L,6);

										uchar8memaddr = (unsigned char *)INT_RAM + 0x2a;
										tare_wt = ascii_2_binary(uchar8memaddr,5,10000);
									
									}							


									epr_mem = s1p1_bin * E2P_BLOCK_SIZE;
									epr_mem = epr_mem + e2p_tare_wt;
									ch_e2addr_L = epr_mem;
									ch_e2addr_H = epr_mem >> 8; 
									uchar8memaddr = (unsigned char *)INT_RAM + 0x2a;
									save_EPR(ch_e2addr_H,ch_e2addr_L,6);

									uchar8memaddr = (unsigned char *)INT_RAM + 0x2a;
									tare_wt = ascii_2_binary(uchar8memaddr,5,10000); 

									break;



			 		case 0x18 :
									
									uchar8memaddr = (unsigned char *)INT_RAM + 0x30;
									store_param(4);

									uchar8memaddr = (unsigned char *)INT_RAM + 0x31;
									temp_array[0] = *uchar8memaddr;
									uchar8memaddr--;
									temp_array[1] = *uchar8memaddr;

									uchar8memaddr = (unsigned char *)INT_RAM + 0x32;
									ch_e2addr_L = temp_array[1];
									*uchar8memaddr = ch_e2addr_L & 0x0f;
									*uchar8memaddr = *uchar8memaddr + 0x30;									
									uchar8memaddr--;
									ch_e2addr_L = temp_array[1];
									ch_e2addr_L = ch_e2addr_L >> 4;
									*uchar8memaddr = ch_e2addr_L & 0x0f;
									*uchar8memaddr = *uchar8memaddr + 0x30;									


									uchar8memaddr--;
									ch_e2addr_L = temp_array[0];
									*uchar8memaddr = ch_e2addr_L & 0x0f;
									*uchar8memaddr = *uchar8memaddr + 0x30;									


									uchar8memaddr = (unsigned char *)INT_RAM + 0x30;
									pack_length = ascii_2_binary(uchar8memaddr,3,100); 
																		
									if(pack_length <= 20)
									{

										rx1buf[7] = 0x30;
										rx1buf[8] = 0x32;
										rx1buf[9] = 0x30;
										uchar8memaddr = (unsigned char *)INT_RAM + 0x30;
										store_param(4);
									 	pack_length = 20;
									}
									
									if(pack_length >= MAX_PACK_LENGTH)
									{
									 	struct_obj.err6_hmifg = 1;
										hmi_flags2=hmi_flags2|0x10;
										pack_length = 290;
										rx1buf[7] = 0x32;
										rx1buf[8] = 0x39;
										rx1buf[9] = 0x30;
										uchar8memaddr = (unsigned char *)INT_RAM + 0x30;
										store_param(4);

									}
									max_pack_calc();

									epr_mem = s1p1_bin * E2P_BLOCK_SIZE;
									epr_mem = epr_mem + e2p_max_pack_ln;
									ch_e2addr_L = epr_mem;
									ch_e2addr_H = epr_mem >> 8; 
									uchar8memaddr = (unsigned char *)INT_RAM + 0x30;
									save_EPR(ch_e2addr_H,ch_e2addr_L,4);
									flag_array[0] = 2;
									break;

			 		case 0x1a :

									uchar8memaddr = (unsigned char *)INT_RAM + 0x34;
									store_param(4);
								//	uchar8memaddr = (unsigned char *)INT_RAM + 0x35;

									uchar8memaddr = (unsigned char *)INT_RAM + 0x35;
									temp_array[0] = *uchar8memaddr;
									uchar8memaddr--;
									temp_array[1] = *uchar8memaddr;

									uchar8memaddr = (unsigned char *)INT_RAM + 0x37;
									ch_e2addr_L = temp_array[1];
									*uchar8memaddr = ch_e2addr_L & 0x0f;
									*uchar8memaddr = *uchar8memaddr + 0x30;									
									uchar8memaddr--;
									*uchar8memaddr = 0x2e;
									uchar8memaddr--;
									ch_e2addr_L = temp_array[1];
									ch_e2addr_L = ch_e2addr_L >> 4;
									*uchar8memaddr = ch_e2addr_L & 0x0f;
									*uchar8memaddr = *uchar8memaddr + 0x30;									


									uchar8memaddr--;
									ch_e2addr_L = temp_array[0];
									*uchar8memaddr = ch_e2addr_L & 0x0f;
									*uchar8memaddr = *uchar8memaddr + 0x30;									

									epr_mem = s1p1_bin * E2P_BLOCK_SIZE;
									epr_mem = epr_mem + e2p_upper_lmt;
									ch_e2addr_L = epr_mem;					
									ch_e2addr_H = epr_mem >> 8; 

									uchar8memaddr = (unsigned char *)INT_RAM + 0x38;
									*uchar8memaddr = 0x20;
									uchar8memaddr++;
									*uchar8memaddr = 0x20;

									uchar8memaddr = (unsigned char *)INT_RAM + 0x34;
									save_EPR(ch_e2addr_H,ch_e2addr_L,6);

									uchar8memaddr = (unsigned char *)INT_RAM + 0x34;
									upper_limit = ascii_2_binary(uchar8memaddr,3,100); 

/*								
									
									uchar8memaddr = (unsigned char *)INT_RAM + 0x24;
									target_wt = ascii_2_binary(uchar8memaddr,5,10000); 

									uchar8memaddr = (unsigned char *)INT_RAM + 0x34;
									upper_limit = ascii_2_binary(uchar8memaddr,3,100); 

									if((target_wt + upper_limit) >= 1000)
									{
										uchar8memaddr = (unsigned char *)INT_RAM + 0x34;
									    *uchar8memaddr = 0x30;
										uchar8memaddr++;
										*uchar8memaddr = 0x30;
										uchar8memaddr++;
										*uchar8memaddr = 0x2e;
										uchar8memaddr++;
										*uchar8memaddr = 0x30;
									}

									uchar8memaddr = (unsigned char *)INT_RAM + 0x34;
									save_EPR(ch_e2addr_H,ch_e2addr_L,6);

									uchar8memaddr = (unsigned char *)INT_RAM + 0x34;
									upper_limit = ascii_2_binary(uchar8memaddr,3,100); 
 */

//									upper_limit_calc();									

									break;

			 		case 0x1d :

									uchar8memaddr = (unsigned char *)INT_RAM + 0x3a;
									store_param(4);

									uchar8memaddr = (unsigned char *)INT_RAM + 0x3b;
									temp_array[0] = *uchar8memaddr;
									uchar8memaddr--;
									temp_array[1] = *uchar8memaddr;

									uchar8memaddr = (unsigned char *)INT_RAM + 0x3d;
									ch_e2addr_L = temp_array[1];
									*uchar8memaddr = ch_e2addr_L & 0x0f;
									*uchar8memaddr = *uchar8memaddr + 0x30;									
									uchar8memaddr--;
									*uchar8memaddr = 0x2e;
									uchar8memaddr--;
									ch_e2addr_L = temp_array[1];
									ch_e2addr_L = ch_e2addr_L >> 4;
									*uchar8memaddr = ch_e2addr_L & 0x0f;
									*uchar8memaddr = *uchar8memaddr + 0x30;									


									uchar8memaddr--;
									ch_e2addr_L = temp_array[0];
									*uchar8memaddr = ch_e2addr_L & 0x0f;
									*uchar8memaddr = *uchar8memaddr + 0x30;									


									uchar8memaddr = (unsigned char *)INT_RAM + 0x3b;
									epr_mem = s1p1_bin * E2P_BLOCK_SIZE;
									epr_mem = epr_mem + e2p_lower_lmt;
									ch_e2addr_L = epr_mem;
									ch_e2addr_H = epr_mem >> 8;
									
									uchar8memaddr = (unsigned char *)INT_RAM + 0x3e;
									*uchar8memaddr = 0x20;
									uchar8memaddr++;
									*uchar8memaddr = 0x20;
									 
									uchar8memaddr = (unsigned char *)INT_RAM + 0x3a;
									save_EPR(ch_e2addr_H,ch_e2addr_L,6);

									uchar8memaddr = (unsigned char *)INT_RAM + 0x3a;
									lower_limit = ascii_2_binary(uchar8memaddr,3,100); 

									break;


			 		case 0x20 :

									uchar8memaddr = (unsigned char *)INT_RAM + 0x40;
									store_param(4);


									uchar8memaddr = (unsigned char *)INT_RAM + 0x41;
									temp_array[0] = *uchar8memaddr;
									uchar8memaddr--;
									temp_array[1] = *uchar8memaddr;

									uchar8memaddr = (unsigned char *)INT_RAM + 0x42;
									ch_e2addr_L = temp_array[1];
									*uchar8memaddr = ch_e2addr_L & 0x0f;
									*uchar8memaddr = *uchar8memaddr + 0x30;									
									uchar8memaddr--;
									ch_e2addr_L = temp_array[1];
									ch_e2addr_L = ch_e2addr_L >> 4;
									*uchar8memaddr = ch_e2addr_L & 0x0f;
									*uchar8memaddr = *uchar8memaddr + 0x30;									


									uchar8memaddr--;
									ch_e2addr_L = temp_array[0];
									*uchar8memaddr = ch_e2addr_L & 0x0f;
									*uchar8memaddr = *uchar8memaddr + 0x30;									

									epr_mem = s1p1_bin * E2P_BLOCK_SIZE;
									epr_mem = epr_mem + e2p_speed;
									ch_e2addr_L = epr_mem;
									ch_e2addr_H = epr_mem >> 8; 

									flag_array[0] = 2;
									
									uchar8memaddr = (unsigned char *)INT_RAM + 0x40;
									if(*uchar8memaddr == 0x30)
									{
									 	uchar8memaddr++;
										if(*uchar8memaddr <= 0x34)	   //35       min spedd 40 PPM nitin 19/1/15
										{
											*uchar8memaddr = 0x35;	   //36
											uchar8memaddr++;
											*uchar8memaddr = 0x30;

										}
																		
									}									


									uchar8memaddr = (unsigned char *)INT_RAM + 0x40;
									if(*uchar8memaddr == 0x32)
									{
									 	uchar8memaddr++;
										if(*uchar8memaddr >= 0x35)
										{

											uchar8memaddr = (unsigned char *)INT_RAM + 0x40;
											*uchar8memaddr = 0x32;
											uchar8memaddr++;
											*uchar8memaddr = 0x35;
											uchar8memaddr++;
											*uchar8memaddr = 0x30;

										}
																		
									}									
									else if(*uchar8memaddr > 0x32)
									{

											uchar8memaddr = (unsigned char *)INT_RAM + 0x40;
											*uchar8memaddr = 0x32;
											uchar8memaddr++;
											*uchar8memaddr = 0x35;
											uchar8memaddr++;
											*uchar8memaddr = 0x30;
																		
									}									


									
									uchar8memaddr = (unsigned char *)INT_RAM + 0x40;
									pack_permin = ascii_2_binary(uchar8memaddr,3,100); 

									max_pack_calc();

									uchar8memaddr = (unsigned char *)INT_RAM + 0x40;
									save_EPR(ch_e2addr_H,ch_e2addr_L,4);
									break;

			 		case 0x22 :

									uchar8memaddr = (unsigned char *)INT_RAM + 0x44;
									store_param(2);

									uchar8memaddr = (unsigned char *)INT_RAM + 0x44;
									temp_array[0] = *uchar8memaddr;


									uchar8memaddr = (unsigned char *)INT_RAM + 0x45;
									ch_e2addr_L = temp_array[0];
									*uchar8memaddr = ch_e2addr_L & 0x0f;
									*uchar8memaddr = *uchar8memaddr + 0x30;									
									uchar8memaddr--;
									ch_e2addr_L = temp_array[0];
									ch_e2addr_L = ch_e2addr_L >> 4;
									*uchar8memaddr = ch_e2addr_L & 0x0f;
									*uchar8memaddr = *uchar8memaddr + 0x30;									

									epr_mem = s1p1_bin * E2P_BLOCK_SIZE;
									epr_mem = epr_mem + e2p_operate_dly;
									ch_e2addr_L = epr_mem;
									ch_e2addr_H = epr_mem >> 8; 

									uchar8memaddr = (unsigned char *)INT_RAM + 0x44;
									operate_dly = ascii_2_binary(uchar8memaddr,2,10); 
	 								if(operate_dly >= 71)
									{
										uchar8memaddr = (unsigned char *)INT_RAM + 0x44;
										*uchar8memaddr = 0x37;
										uchar8memaddr++;
										*uchar8memaddr = 0x30;
										uchar8memaddr = (unsigned char *)INT_RAM + 0x44;
										operate_dly = ascii_2_binary(uchar8memaddr,2,10); 

									}
									if(operate_dly == 0)
									{
										uchar8memaddr = (unsigned char *)INT_RAM + 0x44;
										*uchar8memaddr = 0x30;
										uchar8memaddr++;
										*uchar8memaddr = 0x31;
										operate_dly = 1;//ascii_2_binary(uchar8memaddr,2,10); 
									}

									uchar8memaddr = (unsigned char *)INT_RAM + 0x44;
									save_EPR(ch_e2addr_H,ch_e2addr_L,2);

									break;


			 		case 0x23 :

									uchar8memaddr = (unsigned char *)INT_RAM + 0x46;
									store_param(2);

									uchar8memaddr = (unsigned char *)INT_RAM + 0x46;
									temp_array[0] = *uchar8memaddr;


									uchar8memaddr = (unsigned char *)INT_RAM + 0x47;
									ch_e2addr_L = temp_array[0];
									*uchar8memaddr = ch_e2addr_L & 0x0f;
									*uchar8memaddr = *uchar8memaddr + 0x30;									
									uchar8memaddr--;
									ch_e2addr_L = temp_array[0];
									ch_e2addr_L = ch_e2addr_L >> 4;
									*uchar8memaddr = ch_e2addr_L & 0x0f;
									*uchar8memaddr = *uchar8memaddr + 0x30;									


									uchar8memaddr = (unsigned char *)INT_RAM + 0x46;
									hold_dly = ascii_2_binary(uchar8memaddr,2,10); 
									if(hold_dly <= 5)
									{
										uchar8memaddr = (unsigned char *)INT_RAM + 0x46;
										*uchar8memaddr = 0x30;
										uchar8memaddr++;
										*uchar8memaddr = 0x35;

										hold_dly = 5;

									}


									epr_mem = s1p1_bin * E2P_BLOCK_SIZE;
									epr_mem = epr_mem + e2p_hold_dly;
									ch_e2addr_L = epr_mem;
									ch_e2addr_H = epr_mem >> 8; 
									uchar8memaddr = (unsigned char *)INT_RAM + 0x46;
									save_EPR(ch_e2addr_H,ch_e2addr_L,2);



									break;


			 		case 0x24 :

									uchar8memaddr = (unsigned char *)INT_RAM + 0x48;
									store_param(2);

									uchar8memaddr = (unsigned char *)INT_RAM + 0x48;
									temp_array[0] = *uchar8memaddr;


									uchar8memaddr = (unsigned char *)INT_RAM + 0x49;
									ch_e2addr_L = temp_array[0];
									*uchar8memaddr = ch_e2addr_L & 0x0f;
									*uchar8memaddr = *uchar8memaddr + 0x30;									
									uchar8memaddr--;
									ch_e2addr_L = temp_array[0];
									ch_e2addr_L = ch_e2addr_L >> 4;
									*uchar8memaddr = ch_e2addr_L & 0x0f;
									*uchar8memaddr = *uchar8memaddr + 0x30;	
									
									uchar8memaddr = (unsigned char *)INT_RAM + 0x48;
									bin_rej_cnt = ascii_2_binary(uchar8memaddr,2,10); 
									if(bin_rej_cnt == 0)
									{
										uchar8memaddr = (unsigned char *)INT_RAM + 0x48;
										*uchar8memaddr = 0x30;
										uchar8memaddr++;
										*uchar8memaddr = 0x31;

										bin_rej_cnt = 1;

									}

									uchar8memaddr1 = (unsigned char *)INT_RAM + 0x48;
									uchar8memaddr = (unsigned char *)EXT_RAM + 0x48;
									for(ch_e2addr_L = 0;ch_e2addr_L < 2;ch_e2addr_L++)
									{
	 									*uchar8memaddr = *uchar8memaddr1;
		 								uchar8memaddr++;
		 								uchar8memaddr1++;

									}
									bin_rej_ctr = 0;
									break;




			 		case 0x153 :
									uchar8memaddr = (unsigned char *)INT_RAM + 0x2a6;
									store_param(2);

									uchar8memaddr = (unsigned char *)INT_RAM + 0x2a6;
									temp_array[0] = *uchar8memaddr;


									uchar8memaddr = (unsigned char *)INT_RAM + 0x2a7;
									ch_e2addr_L = temp_array[0];
									*uchar8memaddr = ch_e2addr_L & 0x0f;
									*uchar8memaddr = *uchar8memaddr + 0x30;									
									uchar8memaddr--;
									ch_e2addr_L = temp_array[0];
									ch_e2addr_L = ch_e2addr_L >> 4;
									*uchar8memaddr = ch_e2addr_L & 0x0f;
									*uchar8memaddr = *uchar8memaddr + 0x30;									


									uchar8memaddr = (unsigned char *)INT_RAM + 0x2a6;
									fb_start_count = ascii_2_binary(uchar8memaddr,2,10); 

									epr_mem = s1p1_bin * E2P_BLOCK_SIZE;
									epr_mem = epr_mem + e2p_fb_start_count;
									ch_e2addr_L = epr_mem;
									ch_e2addr_H = epr_mem >> 8; 
									uchar8memaddr = (unsigned char *)INT_RAM + 0x2a6;
									save_EPR(ch_e2addr_H,ch_e2addr_L,2);


									break;


			 		case 0x154 :
									uchar8memaddr = (unsigned char *)INT_RAM + 0x2a8;
									store_param(2);

									uchar8memaddr = (unsigned char *)INT_RAM + 0x2a8;
									temp_array[0] = *uchar8memaddr;


									uchar8memaddr = (unsigned char *)INT_RAM + 0x2a9;
									ch_e2addr_L = temp_array[0];
									*uchar8memaddr = ch_e2addr_L & 0x0f;
									*uchar8memaddr = *uchar8memaddr + 0x30;									
									uchar8memaddr--;
									ch_e2addr_L = temp_array[0];
									ch_e2addr_L = ch_e2addr_L >> 4;
									*uchar8memaddr = ch_e2addr_L & 0x0f;
									*uchar8memaddr = *uchar8memaddr + 0x30;									


									uchar8memaddr = (unsigned char *)INT_RAM + 0x2a8;
									fb_avg_bincount = ascii_2_binary(uchar8memaddr,2,10); 
									fb_avg_count = fb_avg_bincount;

									if(fb_avg_bincount == 0)
									{
										*uchar8memaddr = 0x30;
										uchar8memaddr++;
										*uchar8memaddr = 0x31;
										fb_avg_bincount = 1;
										fb_avg_count = fb_avg_bincount;


									}
									fb_bin_avg = 0;

									epr_mem = s1p1_bin * E2P_BLOCK_SIZE;
									epr_mem = epr_mem + e2p_fb_avg_count;
									ch_e2addr_L = epr_mem;
									ch_e2addr_H = epr_mem >> 8; 
									uchar8memaddr = (unsigned char *)INT_RAM + 0x2a8;
									save_EPR(ch_e2addr_H,ch_e2addr_L,2);

									break;
			 		case 0x155 :
									uchar8memaddr = (unsigned char *)INT_RAM + 0x2aa;
									store_param(2);

									uchar8memaddr = (unsigned char *)INT_RAM + 0x2aa;
									temp_array[0] = *uchar8memaddr;


									uchar8memaddr = (unsigned char *)INT_RAM + 0x2ab;
									ch_e2addr_L = temp_array[0];
									*uchar8memaddr = ch_e2addr_L & 0x0f;
									*uchar8memaddr = *uchar8memaddr + 0x30;									
									uchar8memaddr--;
									ch_e2addr_L = temp_array[0];
									ch_e2addr_L = ch_e2addr_L >> 4;
									*uchar8memaddr = ch_e2addr_L & 0x0f;
									*uchar8memaddr = *uchar8memaddr + 0x30;									


									epr_mem = s1p1_bin * E2P_BLOCK_SIZE;
									epr_mem = epr_mem + e2p_fb_neutral_count;
									ch_e2addr_L = epr_mem;
									ch_e2addr_H = epr_mem >> 8; 
									uchar8memaddr = (unsigned char *)INT_RAM + 0x2aa;
									save_EPR(ch_e2addr_H,ch_e2addr_L,2);

									uchar8memaddr = (unsigned char *)INT_RAM + 0x2aa;
									fb_neutral_bincount = ascii_2_binary(uchar8memaddr,2,10); 
									fb_neutral_count = fb_neutral_bincount;
									break;
			 		case 0x156 :

									uchar8memaddr = (unsigned char *)INT_RAM + 0x2ac;
									store_param(2);

									uchar8memaddr = (unsigned char *)INT_RAM + 0x2ac;
									temp_array[0] = *uchar8memaddr;


									uchar8memaddr = (unsigned char *)INT_RAM + 0x2ad;
									ch_e2addr_L = temp_array[0];
									*uchar8memaddr = ch_e2addr_L & 0x0f;
									*uchar8memaddr = *uchar8memaddr + 0x30;									
									uchar8memaddr--;
									ch_e2addr_L = temp_array[0];
									ch_e2addr_L = ch_e2addr_L >> 4;
									*uchar8memaddr = ch_e2addr_L & 0x0f;
									*uchar8memaddr = *uchar8memaddr + 0x30;									


									epr_mem = s1p1_bin * E2P_BLOCK_SIZE;
									epr_mem = epr_mem + e2p_fb_err_count;
									ch_e2addr_L = epr_mem;
									ch_e2addr_H = epr_mem >> 8; 
									uchar8memaddr = (unsigned char *)INT_RAM + 0x2ac;
									save_EPR(ch_e2addr_H,ch_e2addr_L,2);

									uchar8memaddr = (unsigned char *)INT_RAM + 0x2ac;
									fb_err_bincount = ascii_2_binary(uchar8memaddr,2,10); 
									fb_err_count = fb_err_bincount;

					
									break;
			 		case 0x157 :
									uchar8memaddr = (unsigned char *)INT_RAM + 0x2ae;
									store_param(4);

									uchar8memaddr = (unsigned char *)INT_RAM + 0x2af;
									temp_array[0] = *uchar8memaddr;
									uchar8memaddr--;
									temp_array[1] = *uchar8memaddr;

									uchar8memaddr = (unsigned char *)INT_RAM + 0x2b1;
									ch_e2addr_L = temp_array[1];
									*uchar8memaddr = ch_e2addr_L & 0x0f;
									*uchar8memaddr = *uchar8memaddr + 0x30;									
									uchar8memaddr--;
									*uchar8memaddr = 0x2e;
									uchar8memaddr--;
									ch_e2addr_L = temp_array[1];
									ch_e2addr_L = ch_e2addr_L >> 4;
									*uchar8memaddr = ch_e2addr_L & 0x0f;
									*uchar8memaddr = *uchar8memaddr + 0x30;									


									uchar8memaddr--;
									ch_e2addr_L = temp_array[0];
									*uchar8memaddr = ch_e2addr_L & 0x0f;
									*uchar8memaddr = *uchar8memaddr + 0x30;									


									epr_mem = s1p1_bin * E2P_BLOCK_SIZE;
									epr_mem = epr_mem + e2p_accp_limit_plus;
									ch_e2addr_L = epr_mem;
									ch_e2addr_H = epr_mem >> 8; 
									uchar8memaddr = (unsigned char *)INT_RAM + 0x2ae;
									save_EPR(ch_e2addr_H,ch_e2addr_L,4);

									uchar8memaddr = (unsigned char *)INT_RAM + 0x2ae;
									accp_limit_plus = ascii_2_binary(uchar8memaddr,3,100); 

									break;
			 		case 0x159 :
									uchar8memaddr = (unsigned char *)INT_RAM + 0x2b2;
									store_param(4);

									uchar8memaddr = (unsigned char *)INT_RAM + 0x2b3;
									temp_array[0] = *uchar8memaddr;
									uchar8memaddr--;
									temp_array[1] = *uchar8memaddr;

									uchar8memaddr = (unsigned char *)INT_RAM + 0x2b5;
									ch_e2addr_L = temp_array[1];
									*uchar8memaddr = ch_e2addr_L & 0x0f;
									*uchar8memaddr = *uchar8memaddr + 0x30;									
									uchar8memaddr--;
									*uchar8memaddr = 0x2e;
									uchar8memaddr--;
									ch_e2addr_L = temp_array[1];
									ch_e2addr_L = ch_e2addr_L >> 4;
									*uchar8memaddr = ch_e2addr_L & 0x0f;
									*uchar8memaddr = *uchar8memaddr + 0x30;									


									uchar8memaddr--;
									ch_e2addr_L = temp_array[0];
									*uchar8memaddr = ch_e2addr_L & 0x0f;
									*uchar8memaddr = *uchar8memaddr + 0x30;									


									epr_mem = s1p1_bin * E2P_BLOCK_SIZE;
									epr_mem = epr_mem + e2p_accp_limit_minus;
									ch_e2addr_L = epr_mem;
									ch_e2addr_H = epr_mem >> 8; 
									uchar8memaddr = (unsigned char *)INT_RAM + 0x2b2;
									save_EPR(ch_e2addr_H,ch_e2addr_L,4);

									uchar8memaddr = (unsigned char *)INT_RAM + 0x2b2;
									accp_limit_minus = ascii_2_binary(uchar8memaddr,3,100); 

									break;
			 		case 0x15b :
									uchar8memaddr = (unsigned char *)INT_RAM + 0x2b6;
									store_param(4);

									uchar8memaddr = (unsigned char *)INT_RAM + 0x2b7;
									temp_array[0] = *uchar8memaddr;
									uchar8memaddr--;
									temp_array[1] = *uchar8memaddr;

									uchar8memaddr = (unsigned char *)INT_RAM + 0x2b9;
									ch_e2addr_L = temp_array[1];
									*uchar8memaddr = ch_e2addr_L & 0x0f;
									*uchar8memaddr = *uchar8memaddr + 0x30;									
									uchar8memaddr--;
									*uchar8memaddr = 0x2e;
									uchar8memaddr--;
									ch_e2addr_L = temp_array[1];
									ch_e2addr_L = ch_e2addr_L >> 4;
									*uchar8memaddr = ch_e2addr_L & 0x0f;
									*uchar8memaddr = *uchar8memaddr + 0x30;									


									uchar8memaddr--;
									ch_e2addr_L = temp_array[0];
									*uchar8memaddr = ch_e2addr_L & 0x0f;
									*uchar8memaddr = *uchar8memaddr + 0x30;									


									epr_mem = s1p1_bin * E2P_BLOCK_SIZE;
									epr_mem = epr_mem + e2p_feedbk_limit_plus;
									ch_e2addr_L = epr_mem;
									ch_e2addr_H = epr_mem >> 8; 
									uchar8memaddr = (unsigned char *)INT_RAM + 0x2b6;
									save_EPR(ch_e2addr_H,ch_e2addr_L,4);

									uchar8memaddr = (unsigned char *)INT_RAM + 0x2b6;
									feedbk_limit_plus = ascii_2_binary(uchar8memaddr,3,100); 

									break;
			 		case 0x15d :

									uchar8memaddr = (unsigned char *)INT_RAM + 0x2ba;
									store_param(4);

									uchar8memaddr = (unsigned char *)INT_RAM + 0x2bb;
									temp_array[0] = *uchar8memaddr;
									uchar8memaddr--;
									temp_array[1] = *uchar8memaddr;

									uchar8memaddr = (unsigned char *)INT_RAM + 0x2bd;
									ch_e2addr_L = temp_array[1];
									*uchar8memaddr = ch_e2addr_L & 0x0f;
									*uchar8memaddr = *uchar8memaddr + 0x30;									
									uchar8memaddr--;
									*uchar8memaddr = 0x2e;
									uchar8memaddr--;
									ch_e2addr_L = temp_array[1];
									ch_e2addr_L = ch_e2addr_L >> 4;
									*uchar8memaddr = ch_e2addr_L & 0x0f;
									*uchar8memaddr = *uchar8memaddr + 0x30;									


									uchar8memaddr--;
									ch_e2addr_L = temp_array[0];
									*uchar8memaddr = ch_e2addr_L & 0x0f;
									*uchar8memaddr = *uchar8memaddr + 0x30;									

									epr_mem = s1p1_bin * E2P_BLOCK_SIZE;
									epr_mem = epr_mem + e2p_feedbk_limit_minus;
									ch_e2addr_L = epr_mem;
									ch_e2addr_H = epr_mem >> 8; 
									uchar8memaddr = (unsigned char *)INT_RAM + 0x2ba;
									save_EPR(ch_e2addr_H,ch_e2addr_L,4);

									uchar8memaddr = (unsigned char *)INT_RAM + 0x2ba;
									feedbk_limit_minus = ascii_2_binary(uchar8memaddr,3,100); 

									break;

			 		case 0x15f :
									uchar8memaddr = (unsigned char *)INT_RAM + 0x2be;
									store_param(2);

									uchar8memaddr = (unsigned char *)INT_RAM + 0x2be;
									temp_array[0] = *uchar8memaddr;


									uchar8memaddr = (unsigned char *)INT_RAM + 0x2bf;
									ch_e2addr_L = temp_array[0];
									*uchar8memaddr = ch_e2addr_L & 0x0f;
									*uchar8memaddr = *uchar8memaddr + 0x30;									
									uchar8memaddr--;
									ch_e2addr_L = temp_array[0];
									ch_e2addr_L = ch_e2addr_L >> 4;
									*uchar8memaddr = ch_e2addr_L & 0x0f;
									*uchar8memaddr = *uchar8memaddr + 0x30;									


									uchar8memaddr = (unsigned char *)INT_RAM + 0x2be;
									pulses_count = ascii_2_binary(uchar8memaddr,2,10); 

									if(pulses_count >= 11)
									{
										*uchar8memaddr = 0x31;
										uchar8memaddr++;
										*uchar8memaddr = 0x30;
										pulses_count = 10;

									}
									else if(pulses_count == 0)
									{
										*uchar8memaddr = 0x30;
										uchar8memaddr++;
										*uchar8memaddr = 0x31;
										pulses_count = 1;

									}

									epr_mem = s1p1_bin * E2P_BLOCK_SIZE;
									epr_mem = epr_mem + e2p_pulses_count;
									ch_e2addr_L = epr_mem;
									ch_e2addr_H = epr_mem >> 8; 
									uchar8memaddr = (unsigned char *)INT_RAM + 0x2be;
									save_EPR(ch_e2addr_H,ch_e2addr_L,2);
									break;

			 		case 0x11d :

									uchar8memaddr = (unsigned char *)INT_RAM + 0x23a;
									store_param(16);

									uchar8memaddr = (unsigned char *)INT_RAM + 0x23a;
									for(epr_mem = 0;epr_mem < 16;epr_mem++)
									{
										if(*uchar8memaddr == 0x00)
										{
											*uchar8memaddr = 0x20;
										}
										uchar8memaddr++;
									}

									epr_mem = s1p1_bin * E2P_BLOCK_SIZE; 
									epr_mem = epr_mem + e2p_batch_no;								

									ch_e2addr_L = epr_mem;
									ch_e2addr_H = epr_mem >> 8; 
									uchar8memaddr = (unsigned char *)INT_RAM + 0x23a;
									save_EPR(ch_e2addr_H,ch_e2addr_L,16);
									reset_stats();
									break;


			 		case 0x167 :
									if(struct_obj.operator_id1_fg == 1)
									{
										uchar8memaddr = (unsigned char *)INT_RAM + 0x2ce;
										store_param(16);

										uchar8memaddr = (unsigned char *)EXT_RAM + 0x390;
										store_param(16);

									}
									else if(struct_obj.operator_id1_fg == 2)
									{
										uchar8memaddr = (unsigned char *)INT_RAM + 0x2ce;
										store_param(16);

										uchar8memaddr = (unsigned char *)EXT_RAM + 0x3a6;
										store_param(16);
									}
									else if(struct_obj.operator_id1_fg == 4)
									{
										uchar8memaddr = (unsigned char *)INT_RAM + 0x2ce;
										store_param(16);

										uchar8memaddr = (unsigned char *)EXT_RAM + 0x3bc;
										store_param(16);

									}
									else if(struct_obj.operator_id1_fg == 10)
									{
										uchar8memaddr = (unsigned char *)INT_RAM + 0x2ce;
										store_param(16);

										uchar8memaddr = (unsigned char *)EXT_RAM + 0x3d2;
										store_param(16);

									}
									else if(struct_obj.operator_id1_fg == 20)
									{
										uchar8memaddr = (unsigned char *)INT_RAM + 0x2ce;
										store_param(16);

										uchar8memaddr = (unsigned char *)EXT_RAM + 0x3e8;
										store_param(16);

									}

									else if(struct_obj.operator_id1_fg == 40)
									{
										uchar8memaddr = (unsigned char *)INT_RAM + 0x2ce;
										store_param(16);

										uchar8memaddr = (unsigned char *)EXT_RAM + 0x3fe;
										store_param(16);

									}


									break;
			 		case 0x16f :
					
									
									uchar8memaddr = (unsigned char *)INT_RAM + 0x2de;
									store_param(6);
									chk_pwd_similarity();
									if(struct_obj.pas_invladfg)
									{
										struct_obj.operator_id1_fg = 0;
									}
																		

									if(struct_obj.operator_id1_fg == 1)
									{
										uchar8memaddr = (unsigned char *)INT_RAM + 0x2de;
										store_param(6);

										uchar8memaddr = (unsigned char *)EXT_RAM + 0x3a0;
										store_param(6);

									}
									else if(struct_obj.operator_id1_fg == 2)
									{
										uchar8memaddr = (unsigned char *)INT_RAM + 0x2de;
										store_param(6);

										uchar8memaddr = (unsigned char *)EXT_RAM + 0x3b6;
										store_param(6);


									}
									else if(struct_obj.operator_id1_fg == 4)
									{
										uchar8memaddr = (unsigned char *)INT_RAM + 0x2de;
										store_param(6);

										uchar8memaddr = (unsigned char *)EXT_RAM + 0x3cc;
										store_param(6);


									}
									else if(struct_obj.operator_id1_fg == 10)
									{
										uchar8memaddr = (unsigned char *)INT_RAM + 0x2de;
										store_param(6);

										uchar8memaddr = (unsigned char *)EXT_RAM + 0x3e2;
										store_param(6);

									}
									else if(struct_obj.operator_id1_fg == 20)
									{
										uchar8memaddr = (unsigned char *)INT_RAM + 0x2de;
										store_param(6);

										uchar8memaddr = (unsigned char *)EXT_RAM + 0x3f8;
										store_param(6);

									}
									else if(struct_obj.operator_id1_fg == 40)
									{
										uchar8memaddr = (unsigned char *)INT_RAM + 0x2de;
										store_param(6);

										uchar8memaddr = (unsigned char *)EXT_RAM + 0x40e;
										store_param(6);

									}


									break;



			 		case 0x1a9 :
									
									struct_obj.set_managerfg = 0;
									hmi_flags0=hmi_flags0&0xDF;
									uchar8memaddr = (unsigned char *)INT_RAM + 0x352;
									store_param(16);
									break;

			 		case 0x1b1 :
									
									struct_obj.set_managerfg = 0;
									hmi_flags0=hmi_flags0&0xDF;
									struct_obj.pswrd_edtfg = 1;
									uchar8memaddr = (unsigned char *)INT_RAM + 0x362;
									store_param(6);

									break;
			 		case 0x1b3 :

									struct_obj.level_pwdfg = 1;
									uchar8memaddr = (unsigned char *)EXT_RAM + 0x366;
									store_param(6);

									break;

			 		case 0x1c0 :

									uchar8memaddr = (unsigned char *)INT_RAM + 0x380;
									store_param(12);
									break;


					default :
									break;			
			}
			

			ch_crc_len = 6;
			ch_txmit1_len = ch_crc_len + 2;
			get_crc();


			xmitbuf1();

  
}


/**********************************************************

Function Name   :-	chk_fc_bit	

Description     :-	Identifies the Force Coil Bit.
					(Identifies which button is pressed.) 	

Arguments       :-	None

Return Type     :-  None

**********************************************************/
void chk_fc_bit(void)
{
	uchar8 ch_hmi_func_code,ch_hmi_slv_addr,ch_hmi_hi_addr,ch_hmi_lo_addr;


		ch_hmi_slv_addr = rx1buf[0];
		tx1buf[0] = ch_hmi_slv_addr;

		ch_hmi_func_code = rx1buf[1];
		tx1buf[1] = ch_hmi_func_code;

		ch_hmi_hi_addr = rx1buf[2];
		tx1buf[2] = ch_hmi_hi_addr;

		ch_hmi_lo_addr = rx1buf[3];
		tx1buf[3] = ch_hmi_lo_addr;

		ch_hmi_hi_addr = rx1buf[4];
		tx1buf[4] = ch_hmi_hi_addr;

		ch_hmi_lo_addr = rx1buf[5];
		tx1buf[5] = ch_hmi_lo_addr;


		ch_crc_len = 6;
		ch_txmit1_len = ch_crc_len + 2;
		
		get_crc();
		xmitbuf1();

		force_coil();

}



/**********************************************************

Function Name   :-	chk_input_bit	

Description     :-	Identifies the Force Coil Bit.
					(Identifies which button is pressed.) 	

Arguments       :-	None

Return Type     :-  None

**********************************************************/
void chk_input_bit(void)
{

	uchar8 ch_tmp,ch_byt_cnt,ch_lp_ctr;
	uint32 tmp_mem_offset;

					ch_tmp = rx1buf[0];
					tx1buf[0] = ch_tmp;
					ch_tmp = rx1buf[1]; 
					tx1buf[1] =	ch_tmp;
					ch_tmp = rx1buf[5]; 
					ch_byt_cnt = ch_tmp / 8;
					tx1buf[2] = ch_byt_cnt;
					ch_crc_len = ch_byt_cnt + 3;
					ch_tmp = rx1buf[3]; 
					ch_tmp = ch_tmp / 8;
					tmp_mem_offset = ch_tmp + 0x500;

					uchar8memaddr = (unsigned char *)INT_RAM + tmp_mem_offset;
					ch_tmp = 3;

					for(ch_lp_ctr = 0 ; ch_lp_ctr < ch_byt_cnt ; ch_lp_ctr++)
					{
							tx1buf[ch_tmp] = *uchar8memaddr;
							ch_tmp++;
							uchar8memaddr++;
					}

					ch_txmit1_len = ch_crc_len + 2;
					get_crc();
					xmitbuf1();  
}

/**********************************************************

Function Name   :-	read_coil_bit

Description     :-  Reading the Bit Status from controller &
                    transmit to HMI.		 

Arguments       :-	None

Return Type     :-  None

**********************************************************/
void read_coil_bit(void)
{

					uchar8 ch_tmp,ch_byt_cnt,ch_lp_ctr;
					uint32 tmp_mem_offset;

					ch_tmp = rx1buf[0];
					tx1buf[0] = ch_tmp;
					ch_tmp = rx1buf[1]; 
					tx1buf[1] =	ch_tmp;
					ch_tmp = rx1buf[5]; 
					ch_byt_cnt = ch_tmp / 8;
					tx1buf[2] = ch_byt_cnt;
					ch_crc_len = ch_byt_cnt + 3;
					ch_tmp = rx1buf[3]; 
					ch_tmp = ch_tmp / 8;
					tmp_mem_offset = ch_tmp + 0x600;

					uchar8memaddr = (unsigned char *)INT_RAM + tmp_mem_offset;
					ch_tmp = 3;

					for(ch_lp_ctr = 0 ; ch_lp_ctr < ch_byt_cnt ; ch_lp_ctr++)
					{
							tx1buf[ch_tmp] = *uchar8memaddr;
							ch_tmp++;
							uchar8memaddr++;
					}
					ch_txmit1_len = ch_crc_len + 2;
					get_crc();
					xmitbuf1();  
	
}

/**********************************************************

Function Name   :-	force_coil

Description     :-	Identifies which Button is pressed On HMI. 

Arguments       :-	None

Return Type     :-  None

**********************************************************/
void force_coil(void)
{
	uchar8 ch_hmi_lo_addr,ch_hmi_hi_bit;                 
	uint32 calib_swbit;

		ch_hmi_lo_addr = rx1buf[3];
		switch(ch_hmi_lo_addr)
		{


			case 0x00 :
							ch_hmi_hi_bit = rx1buf[4];
							if(ch_hmi_hi_bit == 0xff)
							{
								//conv_onoff();
								struct_obj.conv_on_thrdfg = 1;
								conv_thrd_ctr = 1;

							}
							else
							{
						 		struct_obj.convonfg = 0;
								hmi_coils0=hmi_coils0&0xfe;
								struct_obj.conv_on_thrdfg = 0;
								IOCLR0 |= 0x00010000;
								IOCLR0 |= 0x000E0000;
								T2TCR = 0;
								T2TC = 0;										
							}				 			
							break;

			case 0x01 : 			
							if(!struct_obj.convonfg)
							{
								static_wt();
							}
							break;

			case 0x02 : 			
							if(!struct_obj.convonfg)
							{
								set_zero();
							}
							break;

			case 0x03 : 			
							sort_enter();
							break;

			case 0x04 : 			
							calib_swbit = FIO2PIN;
							if(calib_swbit & 0x00000040)
							{							
								fr_clb_pas = 1;
							}
							else
							{
								struct_obj.calibsw_hmifg = 1;
								hmi_flags2=hmi_flags2|0x80;

							}
							break;


			case 0x05 : 	
//							uchar8memaddr = (unsigned char *)EXT_RAM + 0x390;
//							for(i=0;i<132;i++)
//							{
//								*uchar8memaddr = 0x20;
//								uchar8memaddr++;	
//							}		
//					
							break;

			case 0x06 : 			
							struct_obj.for_stats_rstfg = 1;
							break;


			case 0x07 : 			
							prep_stats();
							break;

			case 0x09 : 			
							struct_obj.printerfg = 1;

							break;

			case 0x0b : 			
							dcomp_routine();
							break;



			case 0x0c : 			
							struct_obj.login_pasfg = 0;
							hmi_flags0=hmi_flags0&0x7f;
							struct_obj.logfg = 0;
							struct_obj.pswrd_edtfg = 0;
							struct_obj.fr_prd_libfg = 0;

							struct_obj.set_managerfg = 0;
							hmi_flags0=hmi_flags0&0xDF;
					////		hmi_flags0=hmi_flags0&0xef; 
							struct_obj.set_supervisorfg = 0;
					////		hmi_flags0=hmi_flags0&0xDF;
							hmi_flags0=hmi_flags0&0xef;


							struct_obj.set_operatorfg = 0;
							hmi_flags0=hmi_flags0&0xF7;
							struct_obj.for_stats_rstfg = 0;
							struct_obj.pass_stats_rstfg = 0;
							hmi_flags1=hmi_flags1&0xfb;
							struct_obj.pas_prd_libfg = 0;
							hmi_flags1=hmi_flags1&0xfd;
							pas_calb = 0;
							hmi_flags0=hmi_flags0&0xbf;
							fr_clb_pas = 0;
							rtc_pwd_pass = 0;
							hmi_flags3=hmi_flags3&0xFB;
							rtc_pwd_chk = 0;
							decomp_pwd_pass = 0;
							decomp_pwd_chk = 0;
							break;

			case 0x0d : 			

							sort_nxt_libpage();
							break;
			case 0x0e : 			
							sort_prev_libpage();
							break;

			case 0x0f : 			
							struct_obj.fr_prd_libfg = 1;
							break;

			case 0x10 :
							ch_lib_sr_no = 0x31;
							break;
			case 0x11 : 			
							ch_lib_sr_no = 0x32; 			

							break;
			case 0x12 : 			
							ch_lib_sr_no = 0x33; 			

							break;
			case 0x13 : 			
							ch_lib_sr_no = 0x34; 			

							break;
			case 0x14 : 			
							ch_lib_sr_no = 0x35; 			

							break;
			case 0x15 : 			
							ch_lib_sr_no = 0x36; 			

							break;
			case 0x16 :
							sel_prd(); 			
							break;
			case 0x17 :
							del_prd(); 			
							break;


			case 0x1a : 			
							struct_obj.logfg = 1;    								
							break;
			case 0x1b : 			
							struct_obj.errs3_fg = 0; 
							hmi_flags2=hmi_flags2&0xfd;
							struct_obj.pas_invladfg = 0;
							hmi_flags0=hmi_flags0&0xfe;
							struct_obj.set_operatorfg = 0;
							hmi_flags0=hmi_flags0&0xF7;
							struct_obj.login_pasfg = 0;
							hmi_flags0=hmi_flags0&0x7f;
							struct_obj.pass_stats_rstfg = 0;
							hmi_flags1=hmi_flags1&0xfb;
							struct_obj.pas_prd_libfg = 0;
							hmi_flags1=hmi_flags1&0xfd;
							struct_obj.set_managerfg = 0;
							hmi_flags0=hmi_flags0&0xDF;
				////			hmi_flags0=hmi_flags0&0xef; 
							IOCLR0 |= 0x00000200;
														   								
							break;

	 		case 0x1d :								
							struct_obj.level_pwdfg = 0;							
							break;

			case 0x1e : 			
							struct_obj.staticwtfg = 0;
							hmi_coils0=hmi_coils0&0xfd;
							pass_feedbk = 0;
							hmi_flags1=hmi_flags1&0xF7;
							feedbk_pwd = 0;
							log_fg_ctr = 0;    								
							break;

			case 0x1f : 			
							ch_hmi_hi_bit = rx1buf[4];
							ch_hmi_hi_bit = ch_hmi_hi_bit & 0x01;
							ch_dcomp_onfg = ch_hmi_hi_bit; 

							if(ch_dcomp_onfg)
							{
							 	hmi_coils3=hmi_coils3|0x80;
							}
							else
							{
							 	hmi_coils3=hmi_coils3&0x7F;

							}

							break;

			case 0x20 : 			
							ch_hmi_hi_bit = rx1buf[4];
							ch_hmi_hi_bit = ch_hmi_hi_bit & 0x01;
							feedbk_onfg = ch_hmi_hi_bit; 

							if(feedbk_onfg)
							{
							 	hmi_coils4=hmi_coils3|0x01;
							}
							else
							{
							 	hmi_coils4=hmi_coils3&0xFE;

							}

							break;

			case 0x21 : 			
							struct_obj.operator_id1_fg = 1; 
							break;
			case 0x22 : 			
							struct_obj.operator_id1_fg = 2; 
							break;

			case 0x23 : 			
							struct_obj.operator_id1_fg = 4; 
							break;
			case 0x24 : 			
							struct_obj.operator_id1_fg = 0; 	
							break;
			case 0x25 : 			
							struct_obj.operator_id1_fg = 10; 
							break;
			case 0x26 : 			
							struct_obj.operator_id1_fg = 20; 
							break;

			case 0x27 : 			
							struct_obj.operator_id1_fg = 40; 
							break;


			case 0x32 : 			
							struct_obj.err2_hmifg = 0;
							hmi_flags1=hmi_flags1&0xEF;
							ch_err_ctr = 0;
							struct_obj.err1fg = 0; 
							struct_obj.lowprs_hmifg = 0;
							hmi_flags0=hmi_flags0&0xfb;
							IOCLR0 |= 0x00000200;							   								
							break;

			case 0x33 : 			
							struct_obj.err1_hmifg = 0;   
							hmi_flags1=hmi_flags1&0xdf; 								
							break;

			case 0x34 : 			
							struct_obj.err7_hmifg = 0; 
							hmi_flags1=hmi_flags1&0xBF;   								
							break;

			case 0x35 : 			
							struct_obj.s2_failfg = 0;
							hmi_flags1=hmi_flags1&0x7f;
							
							IOCLR0 |= ow_rly_p0_19;					//Pusher OFF at S2 fail      Rohit 6/9/17
							IOCLR0 |= 0x00000200;						    								
							break;

			case 0x36 : 			
							struct_obj.owerr12_hmifg = 0; 
							hmi_flags2=hmi_flags2&0xfe;   								
							break;
																	  

			case 0x37 : 			
							struct_obj.cont_rej_fg = 0;  
							hmi_flags3=hmi_flags3&0xEF; 
							IOCLR0 |= 0x00000200; 								
							break;

			case 0x38 : 	
			
							if(dooropen_swfg)
							{
								dooropen_swfg = 0;
								hmi_flags3=hmi_flags3&0xBF;
								door_open_ctr = 0;
								IOCLR0 |= 0x00000200;
							}	
							
							if(Q_failedfg)	
							{
							Q_failedfg = 0;	
							hmi_flags3=hmi_flags3&0xDF;			
							IOCLR0 |= 0x00000100;  
							IOCLR0 |= 0x00000200;
							//IOSET0 |= syson_blue_conv_on_green;
							}  								
							break;


			case 0x3a : 			
							struct_obj.ch_data_to_hhtfg = 1;
							ch_step = 0;
							break;


			case 0x3b : 			
							struct_obj.calib_hmierrfg = 0;  
							hmi_flags2=hmi_flags2&0xF7;  								
							break;

			case 0x3c : 			
							struct_obj.err6_hmifg = 0;   
							hmi_flags2=hmi_flags2&0xef; 								
							break;

			case 0x3d : 			
							struct_obj.err4_hmifg = 0;   
							hmi_flags2=hmi_flags2&0xDF; 								
							break;

			case 0x3e : 			
							struct_obj.comm_failfg = 0;
							struct_obj.staticwtfg = 0;
							hmi_coils0=hmi_coils0&0xfd;
							struct_obj.serialfg = 0;
							struct_obj.err1fg = 0;							    								
							break;

			case 0x3f :
							struct_obj.err8_hmifg = 0; 
							hmi_flags2=hmi_flags2&0xBF;			
							break;

			case 0x40 :
							struct_obj.binfull_err22hmifg = 0;
							hmi_flags3=hmi_flags3&0x7F;
							IOCLR0 |= 0x00000200;	
							break;


			case 0x41 : 			
							struct_obj.calibsw_hmifg = 0;    
							hmi_flags2=hmi_flags2&0x7F;								
							break;
			case 0x42 : 			
							struct_obj.cur_dltfg = 0;   
							hmi_flags3=hmi_flags3&0xFE; 								
							break;
			case 0x43 : 			
							feedbk_pwd = 1;    								
							break;

			case 0x44 : 			
							struct_obj.err1fg = 0;
							break;

			case 0x45 : 			
							struct_obj.feedbk_hmierrfg = 0;
							hmi_flags3=hmi_flags3&0xFD;
							break;

			case 0x46 :	 	rtc_pwd_chk = 1;
							break;

			case 0x48 : 			
							rtc_pwd_chk = 1;
							break;


			default :
							break;

		}
}


/**********************************************************

Function Name   :-	get_crc

Description     :-	Code Redundancy Checking Logic. 

Arguments       :-	None

Return Type     :-  None

**********************************************************/
void get_crc(void)
{

	uint16 crc16;
	uchar8 ch_byte_cntr,ch_bit_cntr,chcarry,ch_crc;

		crc16 = 0xffff;

		for(ch_byte_cntr = 0; ch_byte_cntr < ch_crc_len; ch_byte_cntr++)
		{
		 		ch_crc = tx1buf[ch_byte_cntr];
				crc16 = crc16 ^ ch_crc;

				for(ch_bit_cntr = 0; ch_bit_cntr < 8; ch_bit_cntr++)
				{

			 			chcarry = crc16 & 0x0001;
						crc16 = crc16 >> 1;

						if(chcarry)
						{

				 			crc16 = crc16 ^ 0xA001;

						}

				}

		}


		ch_crc = crc16 & 0xff;
		tx1buf[ch_crc_len] = ch_crc;
		ch_crc_len++;
		crc16 = crc16 >> 8;
		ch_crc = crc16 & 0xff;
		tx1buf[ch_crc_len] = ch_crc;

		

}


/**********************************************************

Function Name   :-	store_param

Description     :-	To store Product Data in External RAM Location. 

Arguments       :-	None

Return Type     :-  None

***********************************************************/
void store_param(uchar8 ch_fill_ctr)
{

	uchar8 ch_hmictr,ch_ctr;

		ch_hmictr = 7;
		for(ch_ctr = 1;ch_ctr <= ch_fill_ctr;ch_ctr++)
		{
				*uchar8memaddr = rx1buf[ch_hmictr];
				uchar8memaddr++;
				ch_hmictr++;	 		
		}
}

