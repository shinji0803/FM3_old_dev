
#ifndef TIMER_H
#define TIMER_H

#include "mcu.h"

//SysTick�^�C�}�̐ݒ�i���Ԍv���p�j
void Init_timer(void);

//�^�C�}�l��Ԃ�
uint32_t get_micros(void);
uint32_t get_millis(void);

//Wait�֐��i����usec�j
void wait_usec(uint32_t length);

void wait(uint32_t length);

//�f���A���^�C�}�̐ݒ�
void Init_DT(void);

//�f���A���^�C�}�n���h�����Ŏ��s����֐�
extern void loop_1hz(void);
extern void loop_20hz(void);
extern void loop_50hz(void);
extern void loop_100hz(void);
extern void loop_200hz(void);

#endif

