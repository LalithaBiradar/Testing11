
#include"Type.h"


uchar8 dcomp_ctr;
uint32 dcomp_bin;

void dcomp_routine(void);
void chk_dcomp_on(void);
void dcomp_calc(void);
extern void conv_onoff(void);
extern uint32 bin_val;
extern void bcd_2_ascii(uchar8,uint32);
extern uint32 binary_2_bcd(uchar8,uint32);
extern uchar8 *uchar8memaddr,*uchar8memaddr1;

extern void save_EPR(uint32, uint32,uchar8);
extern uchar8 conv_thrd_ctr;
extern uchar8 flag_array[10];




