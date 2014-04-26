
#ifndef PX4_FLOW_H
#define PX4_FLOW_H

//PX4 FlowÇÃÉfÅ[É^èàóùïîï™

#include "i2c.h"
#include "rc_io.h"
#include "fram.h"

#define PX4F_ADD 0x42
#define PX4F_BAUD 400000

typedef struct{
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
} flow_data;

void px4f_init(void);

void px4f_update(void);

void calc_flow(void);

flow_data *get_flow_data(void);

#endif
