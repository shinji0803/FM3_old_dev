
#ifndef FRAM_H
#define FRAM_H

//FRAM�p�̊֐�
//MB85RC16�p�@�e��2048�~8bit

#include "mcu.h"
#include "I2cDev.h"
#include "clock_def.h"

#include "uart.h"

typedef union __generic_16bit{
	uint8_t b[2];
	int16_t i;
} generic_16bit;

typedef union __generic_32bit{
	uint8_t b[4];
	float f;
	int32_t i;
} generic_32bit;

typedef union __generic_64bit{
	uint8_t b[8];
	int64_t ll;
} generic_64bit;


extern I2cDev_IOB I2cDev[];
extern I2cDev_IOB *fram;

//�������@FRAM�ƂȂ���I2C�|�[�g��ݒ�
void Init_fram(void);

//�������݊֐�
void write_byte(uint16_t address, uint8_t data);
void write_int16(uint16_t address, int16_t data);
void write_uint16(uint16_t address, uint16_t data);
void write_int32(uint16_t address, int32_t data);
void write_uint32(uint16_t address, uint32_t data);
void write_float(uint16_t address, float data);

//�ǂݍ��݊֐�
uint8_t read_byte(uint16_t address);
int16_t read_int16(uint16_t address);
uint16_t read_uint16(uint16_t address);
int32_t read_int32(uint16_t address);
uint32_t read_uint32(uint16_t address);
float read_float(uint16_t address);


#endif
