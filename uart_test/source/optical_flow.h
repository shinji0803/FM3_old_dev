
#ifndef OPTICAL_FLOW_H
#define OPTICAL_FLOW_H

//PX4 Flow�̃f�[�^��������

#include "uart.h"
#include "rc_io.h"
#include "fram.h"

#define ALL_BYTE	34
#define RECV_MAX_SIZE 256

//FlowData��揈��
void receive_uart(void);

//FlowData
extern uint32_t time_usec32;

extern float flow_comp_m_x;
extern float flow_comp_m_y;

extern uint8_t quality;
//

void print_mav(void);
void print_flow(void);
void calc_flow(void);

//UART0�̓���(PC����̓��́j���`�F�b�N
uint8_t check_user_input(void);


#endif
