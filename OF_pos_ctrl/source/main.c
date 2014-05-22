
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
void loop_50hz(void);
void loop_100hz(void);
void loop_200hz(void);

static void read_trim(radio *r);
static void read_radio(radio *r);
static void load_param(gain *g, flow_data *f);
static void calc_flow_ctrl( flow_data *f, gain *g, radio *r);
static uint8_t check_radio_input(radio *r);

uint8_t p_flg = 0, menu_flg = 0, flow_update = 0, ctrl_update = 0;
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
	
	rcin_enable(0);
	rcin_enable(1);
	rcin_enable(2);
	
	rcout_enable(0);
	rcout_enable(1);
		
	Init_i2c();
	Init_fram();
	Init_DT();
	
	load_param(&f_gain, &px4f);	
	read_trim(&rc);
	px4f_init(&px4f);
	init_cli(&px4f, &f_gain, &rc);
		
	top_menu();
	
	while(1){
		if(flow_update == 1){
			read_radio(&rc);
			px4f_update();
			flow_update = 0;
		}
		if(ctrl_update == 1){
			calc_flow_ctrl( &px4f, &f_gain, &rc);
			ctrl_update = 0;
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

static void load_param(gain *g, flow_data *f){
	gain *temp = g;
	flow_data *temp1 = f;
	//Load parameter
	temp->p_gain = read_float(P_ADD);
	temp->i_gain = read_float(I_ADD);
	temp->d_gain = read_float(D_ADD);
	
	temp1->qual_th = read_uint16(Q_TH_ADD);
	uart0_printf("Parameters Load Complete\r\n");
}

static void read_radio(radio *r){
	uint8_t ch = 0;
	for(ch = 0; ch < 3; ch++){
		r->input[ch] = rc_read(ch);
	}
	for(ch = 0; ch < 2; ch++){
		r->output[ch] = rcout_read(ch);
	}
}

#define TRIM_LOW 1400
#define TRIM_HIGH 1600
static void read_trim(radio *r){
	uint8_t count = 0;
	uint16_t data[2] = { 0, 0};
	
	uart0_printf("Reading Radio Trim. Don't move sticks.\r\n");
	uart0_printf("Reading");

	while(count < 5){
		data[0] += rc_read(0);
		data[1] += rc_read(1);
		uart0_printf(".");
		wait(300);
		count ++;
	}
	uart0_printf("\r\n");
	
	r->trim[0] = data[0] / 5;
	r->trim[1] = data[1] / 5;
	if((r->trim[0] < TRIM_LOW || r->trim[0] > TRIM_HIGH) || (r->trim[1] < TRIM_LOW || r->trim[1] > TRIM_HIGH)){
		uart0_printf("Invalid Trim Value... CH1: %d, CH2: %d\r\n", r->trim[0], r->trim[1]);
		uart0_printf("Please Reset!!\r\n");
		while(1){};
	}
	else uart0_printf("CH1 Trim: %d, CH2 Trim: %d\r\n", r->trim[0], r->trim[1]);
}	
	

#define RADIO_TRIM 1520
#define DZ 5
static void calc_flow_ctrl( flow_data *f, gain *g, radio *r){
	float error, rate_error;
	int16_t ctrl_value[2];
	static float old_error[2] = { 0, 0};

	error = 0 - (f->flow_comp_m_x / 1000.f);
	rate_error = (error - old_error[0]) / 0.02f;
	ctrl_value[0] = (int16_t)((g->p_gain * error) - (g->i_gain * f->x) - (g->d_gain * rate_error));
	old_error[0] = error;

	error = 0 - (f->flow_comp_m_y / 1000.f);
	rate_error = (error - old_error[1]) / 0.02f;
	ctrl_value[1] = (int16_t)((g->p_gain * error) - (g->i_gain * f->y) - (g->d_gain * rate_error));
	old_error[1] = error;
	
	if(check_radio_input(r) == 0 && r->input[2] > RADIO_TRIM && f->qual > f->qual_th){
		FM3_GPIO->PDORA_f.P3 = 1; //test led on
		rc_write( 0, r->trim[0] + ctrl_value[0]);
		rc_write( 1, r->trim[1] + ctrl_value[1]);
	}
	else{
		FM3_GPIO->PDORA_f.P3 = 0; //test led off
		rc_write( 0, r->input[0]);
		rc_write( 1, r->input[1]);
		f->x = 0.0f;
		f->y = 0.0f;
	}
}

static uint8_t check_radio_input(radio *r){
	uint8_t count = 0;
	if( r->input[0] > (RADIO_TRIM + DZ)) count ++;
	if( r->input[0] < (RADIO_TRIM - DZ)) count ++;
	if( r->input[1] > (RADIO_TRIM + DZ)) count ++;
	if( r->input[1] < (RADIO_TRIM - DZ)) count ++;
	
	return count;
}
	
	
static void InitLED(){
	//for Heartbeat
	FM3_GPIO->PFRF_f.P3 = 0; 
  FM3_GPIO->PZRF_f.P3 = 1; 
  FM3_GPIO->DDRF_f.P3 = 1;
  FM3_GPIO->PDORF_f.P3 = 0;
	
	//for test
	FM3_GPIO->PFRA_f.P3 = 0; 
  FM3_GPIO->DDRA_f.P3 = 1;
  FM3_GPIO->PDORA_f.P3 = 0;
}

void loop_1hz(){
	FM3_GPIO->PDORF_f.P3 = ~FM3_GPIO->PDORF_f.P3;	
}

void loop_20hz(){
	p_flg = 1;
}

void loop_50hz(){
	calc_flow();
	ctrl_update = 1;
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

