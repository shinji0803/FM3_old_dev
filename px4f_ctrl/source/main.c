
#include <math.h>

#include "mcu.h"
#include "timer.h"
#include "uart.h"
#include "rc_io.h"
#include "i2c.h"
#include "fram.h"

//#include "optical_flow.h"

#define UART_WAIT_COUNT  (SysFreHCLK / 20)

static void InitLED(void);
void loop_1hz(void);
void loop_20hz(void);
void loop_100hz(void);
void loop_200hz(void);

uint8_t p_flg = 0;

//PX4 Flow output data
static void calc_flow_i2c(uint8_t *data);
uint16_t frame_count;// counts created I2C frames
int16_t pixel_flow_x_sum;// accumulated x flow in pixels*10 since last I2C frame
int16_t pixel_flow_y_sum;// accumulated y flow in pixels*10 since last I2C frame
int16_t flow_comp_m_x;// x velocity*1000 in meters / timestep
int16_t flow_comp_m_y;// y velocity*1000 in meters / timestep
int16_t qual;// Optical flow quality / confidence 0: bad, 255: maximum quality
int16_t gyro_x_rate; //gyro x rate
int16_t gyro_y_rate; //gyro y rate
int16_t gyro_z_rate; //gyro z rate
uint8_t gyro_range; // gyro range
uint8_t sonar_timestamp;// timestep in milliseconds between I2C frames
int16_t ground_distance;// Ground distance in meters*1000. Positive value: distance known. Negative value: Unknown distance


int32_t main(void){
	
	int32_t size;
	uint8_t data_rx[64];
	uint8_t data_tx = 0x00;
	
	//‰Šú‰»ŠJŽn
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
	//‰Šú‰»I—¹
	
	//PX4 Flow‚Æ‚ÌI2cÚ‘±Ý’è
	i2c->Cfg.SlaveAddr = 0x42; 
	i2c->Cfg.BaudRate = 400000;
	//

	while (1) {
		size = 1;
		i2c->DataTx(&data_tx, &size);
		size= 22;
		i2c->DataRx(data_rx, &size);
		
		if(p_flg == 1){
			
			calc_flow_i2c(data_rx);
			
			uart0_printf("%5d, %5d, %5d, %3d, ", frame_count, flow_comp_m_x, flow_comp_m_y, qual);
			uart0_printf("%5d, %5d, %5d, %5d\r\n", gyro_x_rate, gyro_y_rate, gyro_z_rate, ground_distance);
			
			/*
			for(i = 0; i < 22; i++){
				uart0_printf("%2x, ", data_rx[i]);
			}
			uart0_printf("\r\n");
			*/
			p_flg = 0;
		}
  } 
}

static void calc_flow_i2c(uint8_t *data){
	generic_16bit r;
	
	r.b[0] = data[0];
	r.b[1] = data[1];
	frame_count = (uint16_t)r.i;

	r.b[0] = data[6];
	r.b[1] = data[7];
	flow_comp_m_x = (int16_t)r.i;
	r.b[0] = data[8];
	r.b[1] = data[9];
	flow_comp_m_y = (int16_t)r.i;
	r.b[0] = data[10];
	r.b[1] = data[11];
	qual =(int16_t)r.i;
	
	r.b[0] = data[12];
	r.b[1] = data[13];
	gyro_x_rate = (int16_t)r.i;
	r.b[0] = data[14];
	r.b[1] = data[15];
	gyro_y_rate = (int16_t)r.i;
	r.b[0] = data[16];
	r.b[1] = data[17];
	gyro_z_rate = (int16_t)r.i;
	
	r.b[0] = data[20];
	r.b[1] = data[21];
	ground_distance = (int16_t)r.i;
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
	
}

void loop_200hz(){
	
}

