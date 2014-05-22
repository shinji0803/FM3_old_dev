
#ifndef CONSOLE_H
#define CONSOLE_H

//コンソールとか　いわゆるCLI

#include "param.h"
#include "uart.h"
#include "px4_flow.h"
#include "fram.h"


//Command type
#define	RADIO						'r'
#define	FLOW						'f'
#define	RADIO_FLOW	'w'
#define	SET_GAIN 			'g'
#define	DISPLAY				'a'
#define	SET_P						'p'
#define	SET_I						'i'
#define	SET_D						'd'
#define  SET_Q_TH			'q'
#define	SAVE						's'
#define	RESTORE			'r'
#define  EXIT							'e'


extern uint8_t p_flg;
extern uint8_t menu_flg;
extern uint8_t input_detect;

void init_cli(flow_data *f, gain *g, radio *r);

void top_menu(void); //print top menu
	
void gain_menu(void); //print gain set menu

void gain_menu_branch(uint8_t com_type);

void top_menu_branch(uint8_t com_type);

#endif
