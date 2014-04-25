#ifndef UART_H
#define UART_H


//UART�֘A�֐�

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "mcu.h"
#include "UartDev.h"
#include "clock_def.h"


#define PRINT_MAX_SIZE 128


//FRAM�g���Ƃ��͗L��
//MFS ch.1��I2C�Ƃ��Ďg��
#define USE_FRAM

extern UartDev_IOB UartDev[];
extern UartDev_IOB *uart0;
extern UartDev_IOB *uart1;
extern UartDev_IOB *uart2;

//UART�������֐�
void Init_uart(void);

//UART0��printf�o��
void uart0_printf(const char* format, ...);

//�������͂��擾 �����͎~�܂�
float uart0_get_float_input(void);

#endif
