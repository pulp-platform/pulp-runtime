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

void set_clear_ntt(void){

  uint32_t volatile * ctrl_reg = (uint32_t*)NTT_INTT_PWM_CTRL(0);

  *ctrl_reg = 1 << NTT_INTT_PWM_CTRL_CLEAR;

  *ctrl_reg = 0 << NTT_INTT_PWM_CTRL_CLEAR;

}

void set_input_ntt(uint32_t Din){
   uint32_t *Din_reg_start = (uint32_t*)NTT_INTT_PWM_DIN(0);

   *Din_reg_start = Din;
}


void trigger_input_ntt(void){

  uint32_t volatile * ctrl_reg = (uint32_t*)NTT_INTT_PWM_CTRL(0);

  *ctrl_reg = 1 << NTT_INTT_PWM_CTRL_LOAD_A_F;

  *ctrl_reg = 0 << NTT_INTT_PWM_CTRL_LOAD_A_F;
  
}


void trigger_ntt(void)
{
  uint32_t volatile *ctrl_reg = (uint32_t*)NTT_INTT_PWM_CTRL(0);
  int cnt = 0;

  asm volatile ("": : : "memory");
  *ctrl_reg = 1 << NTT_INTT_PWM_CTRL_START_NTT;
  asm volatile ("": : : "memory");
  *ctrl_reg = 0 << NTT_INTT_PWM_CTRL_START_NTT;
  asm volatile ("": : : "memory");
  
	
}


void poll_done_ntt_intt_pwm(void){
   uint32_t volatile *status_reg = (uint32_t*)NTT_INTT_PWM_STATUS(0);
   uint32_t volatile * ctrl_reg = (uint32_t*)NTT_INTT_PWM_CTRL(0);
   uint32_t current_status;
   
   do {
   	current_status = (*status_reg)&(1<<NTT_INTT_PWM_STATUS_STATUS);
   } while ( current_status == BUSY);
   
   *ctrl_reg = 1 << NTT_INTT_PWM_CTRL_READ_A;

   *ctrl_reg = 0 << NTT_INTT_PWM_CTRL_READ_A;

}



uint32_t set_output_ntt(){
  uint32_t *Dout_reg_start = (uint32_t*)NTT_INTT_PWM_DOUT(0);
  uint32_t temp;

  temp = *Dout_reg_start;
  return temp;
  
	}



/*************************************************************************/
/***********************  MAIN *******************************************/
/*************************************************************************/
void KYBER_poly_ntt(uint32_t Din[128], uint32_t Dout[128]){

  uint32_t temp;
  
  set_clear_ntt();
	trigger_input_ntt();
 
  for (int i = 0; i<128; i++){
	  set_input_ntt(Din[i]);
  }
     
	trigger_ntt();
  
  poll_done_ntt_intt_pwm();
 
	for (int i = 0; i<128; i++){
  asm volatile ("nop");
	  Dout[i]=set_output_ntt();
  }
  
}
