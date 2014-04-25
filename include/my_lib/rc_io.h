
//RC���o�͊֐�
//PWM��ǂށCPWM���o�͂��� etc...

#include "mcu.h"

#define USE_PPM

#define PWM_FREQ 100.f
#define INTERRUPT_FREQ 2250000.f

#define PWM_MAX_PULSE_WIDTH 2100
#define PWM_MIN_PULSE_WIDTH 900

//�|�[�g�������֐�
void Init_rcin_port(void);
void Init_rcout_port(void);

//PWM�Ǎ��ݒ�֐�
void Init_rcin(FM3_BT_PWC_TypeDef *rcin);
//������
#define IN1 FM3_BT0_PWC
#define IN2 FM3_BT1_PWC
#define IN3 FM3_BT2_PWC
#define IN4 FM3_BT3_PWC
#define IN5 FM3_BT4_PWC
#define IN6 FM3_BT5_PWC
#define IN7 FM3_BT6_PWC
#define IN8 FM3_BT7_PWC

//PWM�o�͐ݒ�֐�
void Init_rcout(FM3_BT_PWM_TypeDef *rcout);
//������
#define OUT1 FM3_BT8_PWM
#define OUT2 FM3_BT9_PWM
#define OUT3 FM3_BT10_PWM
#define OUT4 FM3_BT11_PWM
#define OUT5_FM3_BT12_PWM
#define OUT6 FM3_BT13_PWM
#define OUT7 FM3_BT14_PWM
#define OUT8 FM3_BT15_PWM

//�w��Ch�Ɏw��p���X����PWM�o��(ch = 1~8)
void rc_write(uint8_t ch, uint16_t pulse_usec);

//�w��Ch��PWM���̓p���X���擾(ch = 1~8)
uint16_t rc_read(uint8_t ch);

//8ch���̓��̓p���X���擾
void rc_multiread(uint16_t *data);

//�w��Ch��PWM�o�̓p���X���擾(ch = 1~8)
uint16_t rcout_read(uint8_t ch);
