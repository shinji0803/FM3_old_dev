#ifndef UART_H
#define UART_H


//UART関連関数

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "mcu.h"
#include "UartDev.h"
#include "clock_def.h"


#define PRINT_MAX_SIZE 128


//FRAM使うときは有効
//MFS ch.1をI2Cとして使う
#define USE_FRAM

extern UartDev_IOB UartDev[];
extern UartDev_IOB *uart0;
extern UartDev_IOB *uart1;
extern UartDev_IOB *uart2;

//UART初期化関数
void Init_uart(void);

//UART0にprintf出力
void uart0_printf(const char* format, ...);

//小数入力を取得 処理は止まる
float uart0_get_float_input(void);

#endif
