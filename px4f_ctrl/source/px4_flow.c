
#include "px4_flow.h"

static uint8_t data[64];

flow_data flow;

void px4f_init(){
			
	//PX4 Flow
	i2c->Cfg.SlaveAddr = PX4F_ADD; 
	i2c->Cfg.BaudRate = PX4F_BAUD;
	
}

void px4f_update(){
	int32_t size;
	uint8_t data_tx = 0x00;
	
	size = 1;
	i2c->DataTx(&data_tx, &size);
	size= 22;
	i2c->DataRx(data, &size);
}

void calc_flow(){
	generic_16bit r;
	
	r.b[0] = data[0];
	r.b[1] = data[1];
	flow.frame_count = (uint16_t)r.i;

	r.b[0] = data[6];
	r.b[1] = data[7];
	flow.flow_comp_m_x = (int16_t)r.i;
	r.b[0] = data[8];
	r.b[1] = data[9];
	flow.flow_comp_m_y = (int16_t)r.i;
	r.b[0] = data[10];
	r.b[1] = data[11];
	flow.qual =(int16_t)r.i;
	
	r.b[0] = data[12];
	r.b[1] = data[13];
	flow.gyro_x_rate = (int16_t)r.i;
	r.b[0] = data[14];
	r.b[1] = data[15];
	flow.gyro_y_rate = (int16_t)r.i;
	r.b[0] = data[16];
	r.b[1] = data[17];
	flow.gyro_z_rate = (int16_t)r.i;
	
	r.b[0] = data[20];
	r.b[1] = data[21];
	flow.ground_distance = (int16_t)r.i;
}

flow_data *get_flow_data(){
	return &flow;
}
