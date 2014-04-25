
#include "optical_flow.h"


//ŽóMˆ——p
extern uint8_t p_flg;
uint8_t rx_mav[ALL_BYTE];

//FlowData
uint32_t time_msec;
float flow_comp_m_x;
float flow_comp_m_y;
uint8_t quality;

void receive_uart(){
	int32_t size = 0;
	uint8_t rx_buf[RECV_MAX_SIZE];
	uint32_t count = 0;
	uint32_t byte_index = 0;
	uint16_t i;
	
	while (1) {
		while (1) {
			
			size = 256;
			uart1->BufRx(rx_buf, &size, UartDev_FLAG_NONBLOCKING);
			
			for(count = 0; count < size; count++){
						
				if(rx_buf[count] == 0xFE)  {
					byte_index = 0;
					for(i = 0; i < ALL_BYTE; i++) rx_mav[i] = '\0';
				}
						
				rx_mav[byte_index] = rx_buf[count];
				byte_index ++;
						
				if(byte_index >= (ALL_BYTE - 1)){
					calc_flow();
					byte_index = 0;
					
					if(p_flg == 1){
						//print_mav();
						print_flow();
						p_flg = 0;
					}
				}
			}
		}
	}
}


void print_mav(){
	uint8_t i;

	for(i = 0; i < ALL_BYTE; i++){
		uart0_printf("%2x,", rx_mav[i]);
	}
	
#ifdef USE_PPM
	for(i = 0; i < 6; i++){
		uart0_printf("%4d,", rc_in[i]);
	}
#else
	for(i = 0; i < 3; i++){
		uart0_printf("%4d,", rc_read(i + 1));
	}
#endif
	for(i = 0; i < 3; i++){
		uart0_printf("%4d,", rcout_read(i + 1));
	}
	
	uart0_printf("\r\n");
}


void print_flow(){
	uint8_t i;
		
	uart0_printf("%d,", time_msec);
	uart0_printf("%5.3f,", flow_comp_m_x);
	uart0_printf("%5.3f,", flow_comp_m_y);
	uart0_printf("%3d,", quality);
	
	for(i = 0; i < 3; i++){
		uart0_printf("%4d,", rc_read(i + 1));
	}
	for(i = 0; i < 3; i++){
		uart0_printf("%4d,", rcout_read(i + 1));
	}
	uart0_printf("\r\n");
}
	

void calc_flow(){
	generic_32bit r;
	
	r.b[0] = rx_mav[6];
  r.b[1] = rx_mav[7];
  r.b[2] = rx_mav[8];
  r.b[3] = rx_mav[9];
  time_msec = (uint32_t)( r.i / 1000);
	
  r.b[0] = rx_mav[14];
  r.b[1] = rx_mav[15];
  r.b[2] = rx_mav[16];
  r.b[3] = rx_mav[17];
  flow_comp_m_x = (float)r.f;

  r.b[0] = rx_mav[18];
  r.b[1] = rx_mav[19];
  r.b[2] = rx_mav[20];
  r.b[3] = rx_mav[21];
  flow_comp_m_y = (float)r.f;
  
  quality = rx_mav[31];
}

uint8_t check_user_input(){
	int32_t size = 0;
	uint8_t rx_buf[128];
	
	size = 128;
	uart0->BufRx(rx_buf, &size, UartDev_FLAG_NONBLOCKING);
	
	if(size > 0) return 1;
	else return 0;
}

