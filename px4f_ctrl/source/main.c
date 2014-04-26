
#include <math.h>

#include "mcu.h"
#include "timer.h"
#include "uart.h"
#include "rc_io.h"
#include "i2c.h"
#include "fram.h"

#include "px4_flow.h"


static void InitLED(void);
void loop_1hz(void);
void loop_20hz(void);
void loop_100hz(void);
void loop_200hz(void);

uint8_t p_flg = 0;


int32_t main(void){
	flow_data *px4f;
	
	Init_timer();
	Init_uart();
	InitLED();
	
	Init_rcin_port();
	Init_rcout_port();
	
	Init_rcin(IN1);
	Init_rcin(IN2);
	Init_rcin(IN3);
	
	Init_rcout(OUT1);
	Init_rcout(OUT2);
	Init_rcout(OUT3);
	
	Init_i2c();
	Init_fram();
	Init_DT();
	
	px4f_init();
	px4f = get_flow_data();
	while (1) {
		px4f_update();
		
		if(p_flg == 1){
			calc_flow();
			
			uart0_printf("%5d, %5d, %5d, %3d, ", px4f->frame_count, px4f->flow_comp_m_x, px4f->flow_comp_m_y, px4f->qual);
			uart0_printf("%5d, %5d, %5d, %5d\r\n", px4f->gyro_x_rate, px4f->gyro_y_rate, px4f->gyro_z_rate, px4f->ground_distance);
			p_flg = 0;
		}
  } 
}



static void InitLED()	//for Heartbeat
{
	FM3_GPIO->PFRF_f.P3 = 0; 
  FM3_GPIO->PZRF_f.P3 = 1; 
  FM3_GPIO->DDRF_f.P3 = 1;
  FM3_GPIO->PDORF_f.P3 = 0;
}

void loop_1hz(){
	FM3_GPIO->PDORF_f.P3 = ~FM3_GPIO->PDORF_f.P3;	
}

void loop_20hz(){
	p_flg = 1;
}

void loop_100hz(){
	
}

void loop_200hz(){
	
}

