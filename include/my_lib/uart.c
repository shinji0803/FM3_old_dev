
#include "uart.h"

extern UartDev_IOB UartDev[];

static void initPort(void);
static void init_uart0_port(void);
#ifndef USE_FRAM
static void init_uart1_port(void);
#endif
static void init_uart2_port(void);

UartDev_IOB *uart0;
UartDev_IOB *uart1;
UartDev_IOB *uart2;


void Init_uart(){
	int32_t ret0 = SUCCESS;
#ifndef USE_FRAM
	int32_t ret1 = SUCCESS;
#endif
	int32_t ret2 = SUCCESS;
	
	initPort();
    
  uart0  = &UartDev[0];
#ifndef USE_FRAM
	uart1 = &UartDev[1];
#endif
	uart2 = &UartDev[2];
    
  uart0->Cfg.BitOrder = UartDev_BITORDER_LSB;
  uart0->Cfg.BufSize  = 128;  //byte;
	uart0->Cfg.BaudRate = 57600;

#ifndef USE_FRAM
	uart1->Cfg.BitOrder = UartDev_BITORDER_LSB;
  uart1->Cfg.BufSize  = 256;  //byte;
	uart1->Cfg.BaudRate = 115200;
#endif
	
	uart2->Cfg.BitOrder = UartDev_BITORDER_LSB;
  uart2->Cfg.BufSize  = 128;  //byte;
	uart2->Cfg.BaudRate = 57600;
	
	ret0 = uart0->Init();
#ifndef USE_FRAM
	ret1 = uart1->Init();
#endif
	ret2 = uart2->Init();

#ifndef USE_FRAM
	if (ret0 == SUCCESS && ret1 == SUCCESS && ret2 == SUCCESS){
#else
	if (ret0 == SUCCESS && ret2 == SUCCESS){
#endif
		uart0_printf("Initialized...\r\n");
	}
}
		

static void initPort(){
	init_uart0_port();
	
#ifndef USE_FRAM
  init_uart1_port();
#endif
	
	init_uart2_port();
}

static void init_uart0_port(){
	FM3_GPIO->ADE = 0x00000000;
  FM3_GPIO->PFR2 = FM3_GPIO->PFR2 | 0x0006;
  FM3_GPIO->EPFR07 = FM3_GPIO->EPFR07 | 0x00000140;
}

#ifndef USE_FRAM
static void init_uart1_port(){
	FM3_GPIO->ADE = 0x00000000;
	FM3_GPIO->PFRF = FM3_GPIO->PFRF | 0x03;
  FM3_GPIO->EPFR07 = FM3_GPIO->EPFR07 | 0x03C00;
}
#endif

static void init_uart2_port(){
	FM3_GPIO->ADE = 0x00000000;
	FM3_GPIO->PFR7 = FM3_GPIO->PFR7 | 0x0C;
  FM3_GPIO->EPFR07 = FM3_GPIO->EPFR07 | 0x40000;
}


void uart0_printf(const char* format, ...){
	va_list args;
	
	uint8_t convert[PRINT_MAX_SIZE];
	int32_t size = 0;
	uint8_t error_msg[] = "Print size is too big.\r\n";
	
	//出力文字列が長すぎたら警告
	if(strlen(format) > (PRINT_MAX_SIZE - 1)){
		size = strlen((char *)error_msg);
		uart0->BufTx(error_msg, &size, UartDev_FLAG_BLOCKING);
		return;
	}
		
	va_start( args, format);
	vsprintf((char *)convert, format, args);
	va_end(args);
	size = strlen((char *)convert);
	uart0->BufTx(convert, &size, UartDev_FLAG_BLOCKING);
}
//sprintf,vsprintfはサイズがでかいとかなんとか　要改善



