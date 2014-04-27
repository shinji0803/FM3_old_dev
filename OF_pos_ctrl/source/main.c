
#include <math.h>

#include "mcu.h"
#include "timer.h"
#include "uart.h"
#include "rc_io.h"
#include "i2c.h"
#include "fram.h"

#include "param.h"
#include "console.h"
#include "px4_flow.h"

static void InitLED(void);
void loop_1hz(void);
void loop_20hz(void);
void loop_100hz(void);
void loop_200hz(void);

static void read_radio(radio *r);
static void load_param(gain *g);

uint8_t p_flg = 0, menu_flg = 0, flow_update = 0;
uint8_t input_detect = 0;
uint8_t input;

int32_t main(void){
	flow_data px4f;
	gain f_gain;
	radio rc;
	
	//initialize
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
	
	load_param(&f_gain);	
	
	px4f_init(&px4f);
	init_cli(&px4f, &f_gain, &rc);
		
	top_menu();
	
	while(1){
		if(flow_update == 1){
			read_radio(&rc);
			px4f_update();
			flow_update = 0;
		}
		
		if(input_detect == 1){
			if(menu_flg == 0){
				if(input == '\n'){
					input_detect = 0;
					top_menu();
				}
				else if(input != '\r')	top_menu_branch(input);
			}	
			
			else if(menu_flg == 1){
				input_detect = 0;
				if(input == '\n') gain_menu();
				if(input != '\r' && input != '\n') gain_menu_branch(input);	
			}
		}
	}
}

static void load_param(gain *g){
	gain *temp = g;
	//Load parameter
	temp->p_gain = read_float(P_ADD);
	temp->i_gain = read_float(I_ADD);
	temp->d_gain = read_float(D_ADD);
	uart0_printf("Parameters Load Complete\r\n");
}

static void read_radio(radio *r){
	uint8_t ch = 0;
	for(ch = 0; ch < 3; ch++){
		r->input[ch] = rc_read( ch + 1);
		//through output
		rc_write( ch + 1, r->input[ch]);
		r->output[ch] = rcout_read( ch + 1);
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

void loop_50hz(){
	
}

void loop_100hz(){
	//detect user input
	int32_t size;
	uint8_t rx_buf[8];
	
	size = 1;
	uart0->BufRx(rx_buf, &size, UartDev_FLAG_NONBLOCKING);
	
	if(size >= 1){
		input_detect = 1;
		input = rx_buf[0];
	}
	//
	flow_update = 1;
}

void loop_200hz(){
	
}

