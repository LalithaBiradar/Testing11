

#define platform 350
#define platform_plus 375
#define pully_count 85
#define start_delay 60
#define MAX_PACK_LENGTH 291
#define CALIB_COUNTS_HIGH 9982
#define CALIB_COUNTS_LOW 9942 
#define FB_ERR_TIME 2000
#define E2_TIME 375
#define OW_ERROR_COUNT 11000     //0x2af8

#define one_minute 1000 
#define convon_rly_p0_16 0x00010000 
#define uwrly_p0_17 0x00020000
#define okrly_p0_18 0x00040000 
#define ow_rly_p0_19 0x00080000 
#define ld_soft_trigg_p0_4  0x00000010	 
#define binfull_err_time_5sec 500

#define ip_p1_29 	0x20000000 


#define E2P_BLOCK_SIZE			 0x78										 //120 dec
#define e2p_srno  	     		 0x0a										 //0x0a
#define e2p_prd_code	 		(e2p_srno +0x04)							 //0x0e
#define e2p_prd_name	 		(e2p_prd_code+0x10)							 //0x1e																						 
#define	e2p_batch_no	    	(e2p_prd_name +0x10)						 //0x2e
#define e2p_target_wt	 		(e2p_batch_no+0x10)							 //0x3e 
#define e2p_tare_wt 			(e2p_target_wt+0x06	)						 //0x44
#define e2p_max_pack_ln 		(e2p_tare_wt+0x06)							 //0x4a
#define e2p_upper_lmt			(e2p_max_pack_ln+0x04)						 //0x4e
#define e2p_lower_lmt			(e2p_upper_lmt+0x06)						 //0x54
#define e2p_speed				(e2p_lower_lmt+0x06)						 //0x5a
#define e2p_operate_dly			(e2p_speed+0x04)							 //0x5e
#define e2p_hold_dly			(e2p_operate_dly+0x02)						 //0x60
#define e2p_fb_start_count		(e2p_hold_dly+0x02)							 //0x62
#define e2p_fb_avg_count		(e2p_fb_start_count+0x02)					 //0x64
#define e2p_fb_neutral_count	(e2p_fb_avg_count+0x02)						 //0x66
#define e2p_fb_err_count	 	(e2p_fb_neutral_count+0x02)				     //0x68
#define e2p_accp_limit_plus	 	(e2p_fb_err_count+0x02)						 //0x6a
#define e2p_accp_limit_minus  	(e2p_accp_limit_plus+0x04)					 //0x6e
#define e2p_feedbk_limit_plus 	(e2p_accp_limit_minus+0x04)					 //0x72
#define e2p_feedbk_limit_minus 	(e2p_feedbk_limit_plus+0x04)				 //0x76
#define e2p_pulses_count	   	(e2p_feedbk_limit_minus+0x04)				 //0x7a

#define LIB_NUM_OF_PAGES   0xff		
#define LIB_BYTES_ROW	   0x24
#define LIB_BYTES_PAGE	   0xd8

 
