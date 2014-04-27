
#ifndef PX4_FLOW_H
#define PX4_FLOW_H

//PX4 Flow‚Ìƒf[ƒ^ˆ—•”•ª

#include "i2c.h"
#include "rc_io.h"
#include "fram.h"
#include "param.h"

#define PX4F_ADD 0x42
#define PX4F_BAUD 400000

void px4f_init(flow_data *f);

void px4f_update(void);

void calc_flow(void);


#endif
