
#ifndef I2C_H
#define I2C_H

#include "mcu.h"
#include "I2cDev.h"
#include "clock_def.h"

#include "uart.h"

extern I2cDev_IOB I2cDev[];
extern I2cDev_IOB *i2c;

void Init_i2c(void);

#endif
