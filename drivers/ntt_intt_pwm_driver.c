#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <ntt_intt_pwm_driver.h>
#include <ntt_intt_pwm_auto.h>

#define BUSY 0 
#define CC_input_NTT 25
#define CC_output_NTT 25
#define CC_NTT 904
#define CC_INTT 904
#define CC_PWM 647


void set_input_ntt(uint32_t* Din){
   uint32_t *Din_reg = (uint32_t*)NTT_INTT_PWM_DIN_0(0);
   
   for (int i = 0; i<128; i++){
	 printf("%08x-", Din[i]);
     Din_reg[i] = Din[i];
   }
}


void trigger_input_ntt(void){

  uint32_t volatile * ctrl_reg = (uint32_t*)NTT_INTT_PWM_CTRL(0);

  asm volatile ("": : : "memory");
  *ctrl_reg = 1 << NTT_INTT_PWM_CTRL_LOAD_A_F;
  asm volatile ("": : : "memory");
  *ctrl_reg = 0 << NTT_INTT_PWM_CTRL_LOAD_A_I;
  *ctrl_reg = 0 << NTT_INTT_PWM_CTRL_LOAD_B_F;
  *ctrl_reg = 0 << NTT_INTT_PWM_CTRL_LOAD_B_I;
  *ctrl_reg = 0 << NTT_INTT_PWM_CTRL_READ_A;
  *ctrl_reg = 0 << NTT_INTT_PWM_CTRL_READ_B;
  *ctrl_reg = 0 << NTT_INTT_PWM_CTRL_START_AB;
  *ctrl_reg = 0 << NTT_INTT_PWM_CTRL_START_NTT;
  *ctrl_reg = 0 << NTT_INTT_PWM_CTRL_START_PWM;
  *ctrl_reg = 0 << NTT_INTT_PWM_CTRL_START_INTT;

  asm volatile ("": : : "memory");
  *ctrl_reg = 0 << NTT_INTT_PWM_CTRL_LOAD_A_F;
  asm volatile ("": : : "memory");
	
}

/*
void wait_for_input(int32_t Din){
  int32_t volatile *DIN_reg = (int32_t*)NTT_INTT_PWM_DIN(0);

  *DIN_reg = (int32_t)Din;
}*/


void trigger_ntt(void)
{
  uint32_t volatile * ctrl_reg = (uint32_t*)NTT_INTT_PWM_CTRL(0);
  int cnt = 0;

  asm volatile ("": : : "memory");
  *ctrl_reg = 1 << NTT_INTT_PWM_CTRL_START_NTT;
  asm volatile ("": : : "memory");
  *ctrl_reg = 0 << NTT_INTT_PWM_CTRL_START_NTT;
  asm volatile ("": : : "memory");
  
	
}


/*
void set_output_ntt_intt_pwm(void){

  uint32_t volatile * ctrl_reg = (uint32_t*)NTT_INTT_PWM_CTRL(0);

  asm volatile ("": : : "memory");
  *ctrl_reg = 1 << NTT_INTT_PWM_CTRL_READ_A;
  asm volatile ("": : : "memory");
  printf("set_output_ntt_intt_pwm()\n");

}



void wait_for_output(int32_t Dout[128]){
  int cnt = 0;
  uint32_t volatile * ctrl_reg = (uint32_t*)NTT_INTT_PWM_CTRL(0);
  int32_t volatile *DOUT_reg = (int32_t*)NTT_INTT_PWM_DOUT(0);

  asm volatile ("": : : "memory");
  *ctrl_reg = 0 << NTT_INTT_PWM_CTRL_READ_A;
  asm volatile ("": : : "memory");
 
  
   for (int i = 0; i<128; i++)
   {
     Dout[i] = DOUT_reg[i] ;
   }

  while ( cnt != CC_output_NTT ){
		cnt += 1;
	 	printf("%d-", cnt);
	}

  printf("\nwait_for_output()\n");
}*/



/*************************************************************************/
/***********************  MAIN *******************************************/
/*************************************************************************/
void KYBER_poly_ntt(uint32_t Din[128], uint32_t Dout[128]){

	set_input_ntt(Din);

	trigger_input_ntt();
    
	trigger_ntt();

	//set_output_ntt_intt_pwm();
	//wait_for_output(Dout[128]);

}
