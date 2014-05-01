
#include "px4_flow.h"

static uint8_t data[64];
static flow_data *flow;

void px4f_init(flow_data *f){
	//PX4 Flow
	i2c->Cfg.SlaveAddr = PX4F_ADD; 
	i2c->Cfg.BaudRate = PX4F_BAUD;
	
	flow = f;
	flow->x = 0.0f;
	flow->y = 0.0f;
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
	flow->frame_count = (uint16_t)r.i;
	/*
	r.b[0] = data[2];
	r.b[1] = data[3];
	flow->pixel_flow_x_sum = (int16_t)r.i;
	r.b[0] = data[4];
	r.b[1] = data[5];
	flow->pixel_flow_y_sum = (int16_t)r.i;
	*/
	r.b[0] = data[6];
	r.b[1] = data[7];
	flow->flow_comp_m_x = (int16_t)r.i;
	r.b[0] = data[8];
	r.b[1] = data[9];
	flow->flow_comp_m_y = (int16_t)r.i;
	r.b[0] = data[10];
	r.b[1] = data[11];
	flow->qual =(int16_t)r.i;
	
	flow->x += (flow->flow_comp_m_x / 1000.f) * 0.02f;
	flow->y += (flow->flow_comp_m_y / 1000.f) * 0.02f;
	
	
	r.b[0] = data[12];
	r.b[1] = data[13];
	flow->gyro_x_rate = (int16_t)r.i;
	r.b[0] = data[14];
	r.b[1] = data[15];
	flow->gyro_y_rate = (int16_t)r.i;
	r.b[0] = data[16];
	r.b[1] = data[17];
	flow->gyro_z_rate = (int16_t)r.i;
		
	r.b[0] = data[20];
	r.b[1] = data[21];
	flow->ground_distance = (int16_t)r.i;
}
