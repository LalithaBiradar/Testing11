

#include"type.h"
#include<absacc.h>

/************Variable Declaration **************************/

uint32 bcd_val,bin_val,acc_bin,bin_ref;  //,max_bin,min_bin;
uint32 r1_lmt_bin,r2_lmt_bin,r3_lmt_bin,r4_lmt_bin,r5_lmt_bin;
uint32 r6_lmt_bin,r7_lmt_bin,r8_lmt_bin,r9_lmt_bin,r10_lmt_bin,r11_lmt_bin;

uint32 bin_trend_netwt;

float dev_result;
/************Function Declaration **************************/
void bcd_asc(uint32 );
void prep_stats(void);
void reset_stats(void);
void calc_range_counts(void);
void bcd_2_ascii(uchar8,uint32);

uint32 ascii_2_binary(uchar8 *,uchar8,uint32);
uint32 binary_2_bcd(uchar8,uint32);
uint32 asc_bcd(void);
uint32 bincon(uint32 );
uint32 bin_bcd(void);

uchar8 avg_stk_ctr = 0,avg_count = 0;
uint32 avg[10];
uint32 avg_bin = 0;


/************Extern Declarations from uart0 File************/
extern uchar8 tx0buf[30],rx0buf[20];

/************Extern Declarations from uart1 file**********/
extern uchar8 tx1buf[64];
extern uchar8 start_date[8];
extern uchar8 start_time[8];
extern void get_avg_xbar(void);


/************Extern Declarations from uart0_application File*/

extern uint32 accp_wt_ctr;
extern uchar8 bin_rej_ctr;
