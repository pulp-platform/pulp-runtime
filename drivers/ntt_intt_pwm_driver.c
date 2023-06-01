#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <ntt_intt_pwm_driver.h>
#include <ntt_intt_pwm_auto.h>

#define BUSY 0 
#define CC_input_NTT 256
#define CC_NTT 904
#define CC_INTT 904
#define CC_PWM 647



void set_input_ntt_intt_pwm(void){

  uint32_t volatile * ctrl_reg = (uint32_t*)NTT_INTT_PWM_CTRL(0);

  asm volatile ("": : : "memory");
  *ctrl_reg = 1 << NTT_INTT_PWM_CTRL_LOAD_A_F;
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
  printf("set_input_ntt_intt_pwm()\n");

}

void wait_for_input(void){
  int cnt = 0;
  uint32_t volatile * ctrl_reg = (uint32_t*)NTT_INTT_PWM_CTRL(0);
  *ctrl_reg = 0 << NTT_INTT_PWM_CTRL_LOAD_A_F;

  while ( cnt != CC_input_NTT ){
		cnt += 1;
	 	printf("%d\n", cnt);
	}
  printf("wait_for_input()\n");
}


void trigger_ntt(void)
{
  uint32_t volatile * ctrl_reg = (uint32_t*)NTT_INTT_PWM_CTRL(0);

  asm volatile ("": : : "memory");
  *ctrl_reg = 1 << NTT_INTT_PWM_CTRL_START_NTT;
  asm volatile ("": : : "memory");
}


void poll_done_ntt_intt_pwm(void){
   uint32_t volatile *status_reg = (uint32_t*)NTT_INTT_PWM_STATUS(0);
   uint32_t current_status;
 
   do {
	current_status = (*status_reg)&(1<<NTT_INTT_PWM_STATUS_STATUS);
   } while ( current_status == BUSY);
   printf("NTT driver ends!\n");
}

/*
void set_input_ntt_intt_pwm(int16_t* Din){
   int16_t volatile *Din_reg_start = (int16_t*)NTT_INTT_PWM_DIN(0);
   for (int i = 0; i<255; i++)
   {
     Din_reg_start[i] = Din[i];
   }
}


void trigger_ntt(void)
{
  uint32_t volatile * ctrl_reg = (uint32_t*)NTT_INTT_PWM_CTRL(0);
  asm volatile ("": : : "memory");
  *ctrl_reg = 1 << NTT_INTT_PWM_CTRL_START_NTT;
  asm volatile ("": : : "memory");
}

void poll_done_ntt_intt_pwm(void){
   uint32_t volatile *status_reg = (uint32_t*)NTT_INTT_PWM_STATUS(0);
   uint32_t current_status;
 
   do {
	current_status = (*status_reg)&(1<<NTT_INTT_PWM_STATUS_STATUS);
   } while ( current_status == BUSY);
   //printf("Keccak : ready\n");
}

void get_result_ntt_intt_pwm(int16_t* Dout){
   
   int16_t volatile *Dout_reg_start = (int16_t*)NTT_INTT_PWM_DOUT(0);
   for (volatile int i = 0; i<255; i++){
     Dout[i] = Dout_reg_start[i];	
		
   }
}

*/
void KYBER_poly_ntt(int16_t Din[256], int16_t Dout[256]){
	printf("NTT driver starts!\n");
	
    /*for (int i=0; i<256; i++){
		printf("%x\n", Din[256]);
	}*/
 	
	//set_input(Din);
	set_input_ntt_intt_pwm();
    wait_for_input();
	trigger_ntt();
	poll_done();
	//get_result(Dout);	
}
