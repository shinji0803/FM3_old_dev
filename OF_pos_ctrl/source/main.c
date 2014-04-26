
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

static void read_radio(void);
static void load_param(void);
static void top_menu(void);
static void gain_menu(void);
static void print_data(uint8_t data_type);
static void gain_menu_branch(uint8_t com_type);

uint8_t p_flg = 0, menu_flg = 0, flow_update = 0;
uint8_t input_detect = 0;
uint8_t input;

uint16_t rc_input_trim[2] = { 0, 0};
uint16_t rc_input[2] = { 0, 0};

//Command type
#define	RADIO						'r'
#define	FLOW						'f'
#define	RADIO_FLOW	'w'
#define	SET_GAIN 			'g'
#define	DISPLAY				'a'
#define	SET_P						'p'
#define	SET_I						'i'
#define	SET_D						'd'
#define	SAVE						's'
#define  EXIT							'e'

float p_gain = 0.0f, i_gain = 0.0f, d_gain = 0.0f;
//gain address on fram
#define P_ADD 0
#define I_ADD 4
#define D_ADD 8

flow_data *px4f;

int32_t main(void){
	
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
	
	px4f = px4f_init();
	
	load_param();	
	
	top_menu();
	
	while(1){
		if(flow_update == 1){
			read_radio();
			px4f_update();
			flow_update = 0;
		}
		
		if(input_detect == 1){
			if(menu_flg == 0){
				if(input == '\n'){
					input_detect = 0;
					top_menu();
				}
				else if(input != '\r')	print_data(input);
			}	
			
			if(menu_flg == 1){
				input_detect = 0;
				if(input == '\n') gain_menu();
				if(input != '\r' && input != '\n') gain_menu_branch(input);	
			}
		}
	}
}

static void load_param(){
	//Load parameter
	p_gain = read_float(P_ADD);
	i_gain = read_float(I_ADD);
	d_gain = read_float(D_ADD);
	uart0_printf("Parameters Load Complete\r\n");
}

static void read_radio(){
	uint8_t ch = 0;
	for(ch = 0; ch < 2; ch++){
		rc_input[ch] = rc_read( ch + 1);
		//through output
		rc_write( ch + 1, rc_input[ch]);
	}
}

static void top_menu(){
	uart0_printf("---------------------------\r\n");
	uart0_printf("-- Test Console ver1.1 ---\r\n");
	uart0_printf("---------------------------\r\n");
	uart0_printf("r: read radio\r\n");
	uart0_printf("f: read flow\r\n");
	uart0_printf("w: read radio and flow\r\n");
	uart0_printf("g: set gain\r\n");
	uart0_printf("---------------------------\r\n");
}	

static void gain_menu(){
	uart0_printf("-----Gain Setting Menu-----\r\n");
	uart0_printf("a: display all gain\r\n");
	uart0_printf("p: set p_gain\r\n");
	uart0_printf("i: set i_gain\r\n");
	uart0_printf("d: set d_gain\r\n");
	uart0_printf("s: save all gain setting\r\n");
	uart0_printf("e: return top menu\r\n");
	uart0_printf("---------------------------\r\n");
}

static void print_data(uint8_t com_type){
	if(p_flg == 1){
		switch(com_type){
			case RADIO:
				uart0_printf("IN: %d, %d OUT: %d, %d\r\n", rc_input[0], rc_input[1], rcout_read(1), rcout_read(2));
				break;
			
			case FLOW:
				calc_flow();
				uart0_printf("%5d, %5d, %5d, %3d, ", px4f->frame_count, px4f->flow_comp_m_x, px4f->flow_comp_m_y, px4f->qual);
				uart0_printf("%5d, %5d, %5d, %5d\r\n", px4f->gyro_x_rate, px4f->gyro_y_rate, px4f->gyro_z_rate, px4f->ground_distance);
			  break;
			
			case RADIO_FLOW:
				uart0_printf("radio and flow output\r\n");
			  break;
			
			case SET_GAIN:
				menu_flg = 1;
				gain_menu();
			  break;
			
			default:
				uart0_printf("invalid command\r\n");
				input_detect = 0;
				break;
		}
		p_flg = 0;
	}
}

static void gain_menu_branch(uint8_t com_type){
	
	switch(com_type){
		case DISPLAY:
			uart0_printf("P: %f, I: %f, D: %f\r\n", p_gain, i_gain, d_gain);
			break;
		
		case SET_P:
			uart0_printf("P Gain is %f, input new value.\r\n", p_gain);
			p_gain = uart0_get_float_input();
			break;
		
		case SET_I:
			uart0_printf("I Gain is %f, input new value.\r\n", i_gain);
			i_gain = uart0_get_float_input();
			break;
		
		case SET_D:
			uart0_printf("D Gain is %f, input new value.\r\n", d_gain);
			d_gain = uart0_get_float_input();
			break;
		
		case SAVE:
			write_float(P_ADD, p_gain);
			write_float(I_ADD, i_gain);
			write_float(D_ADD, d_gain);
			uart0_printf("Saved!\r\n");
			break;
		
		case EXIT:
			menu_flg = 0;
			input_detect = 0;
			top_menu();
			break;
		
		default:
			uart0_printf("invalid command\r\n");
			input_detect = 0;
			break;
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

