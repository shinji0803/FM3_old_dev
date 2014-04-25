
#include <math.h>

#include "mcu.h"
#include "timer.h"
#include "uart.h"
#include "rc_io.h"
#include "i2c.h"
#include "fram.h"


static void InitLED(void);
void loop_1hz(void);
void loop_20hz(void);
void loop_100hz(void);
void loop_200hz(void);

static void top_menu(void);
static void gain_menu(void);
static void print_data(uint8_t data_type);
static void gain_menu_branch(uint8_t com_type);

uint8_t p_flg = 0, menu_flg = 0;;
uint8_t input_detect = 0;
uint8_t input;

//Command type
#define	RADIO						'r'
#define	FLOW						'f'
#define	RADIO_FLOW	'w'
#define	SET_GAIN 			'g'
#define	SET_P						'p'
#define	SET_I						'i'
#define	SET_D						'd'
#define	SAVE						's'
#define  EXIT							'e'

float p_gain = 0.0f, i_gain = 0.0f, d_gain = 0.0f;

int32_t main(void){
	
	
	
	//初期化開始
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
	//初期化終了
	
	top_menu();
	
	while(1){
		if(input_detect == 1 && menu_flg == 0){
			if(input == '\n'){
				input_detect = 0;
				top_menu();
			}
			else	print_data(input);
		}	
		
		if(input_detect == 1 && menu_flg == 1){
			input_detect = 0;
			gain_menu();
			gain_menu_branch(input);	
		}
	}
}

static void top_menu(){
	uart0_printf("------------------------\r\n");
	uart0_printf("- Test Console ver1.0 -\r\n");
	uart0_printf("------------------------\r\n");
	uart0_printf("r: read radio\r\n");
	uart0_printf("f: read flow\r\n");
	uart0_printf("w: read radio and flow\r\n");
	uart0_printf("g: set gain\r\n");
	uart0_printf("------------------------\r\n");
}	

static void gain_menu(){
	uart0_printf("-----Gain Setting Menu-----\r\n");
	uart0_printf("p: set p_gain\r\n");
	uart0_printf("i: set i_gain\r\n");
	uart0_printf("d: set d_gain\r\n");
	uart0_printf("s: save all gain setting\r\n");
	uart0_printf("e: return top menu\r\n");
	uart0_printf("-------------------------\r\n");
}

static void print_data(uint8_t com_type){
	if(p_flg == 1){
		switch(com_type){
			case RADIO:
				uart0_printf("radio output\r\n");
				break;
			
			case FLOW:
				uart0_printf("flow output\r\n");
			  break;
			
			case RADIO_FLOW:
				uart0_printf("radio and flow output\r\n");
			  break;
			
			case SET_GAIN:
				//uart0_printf("set gain\r\n");
				menu_flg = 1;
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

void loop_100hz(){
	//入力検出
	int32_t size;
	uint8_t rx_buf[8];
	
	size = 1;
	uart0->BufRx(rx_buf, &size, UartDev_FLAG_NONBLOCKING);
	
	if(size >= 1){
		input_detect = 1;
		input = rx_buf[0];
	}
}

void loop_200hz(){
	
}

