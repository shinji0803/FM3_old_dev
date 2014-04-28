
#include "timer.h"

static uint32_t usec = 0;

void Init_timer(){
	
	uint32_t period;
	period = SystemCoreClock / 1000000;
	
	SysTick_Config(period); //1usec�Ŋ��荞�ݐݒ�
}

void Init_DT(void){
	///�^�C�}�N���b�N(TIMCLK)=APB�o�X�N���b�N=36MHz
	
	FM3_DTIM->TIMER1CONTROL_f.TIMEREN = 0; //Timer1��~
	FM3_DTIM->TIMER1CONTROL_f.TIMERMODE = 1; //�������[�h(�����V���b�g���Ɗ֌W�Ȃ��j
	FM3_DTIM->TIMER1CONTROL_f.ONESHOT = 1; //�����V���b�g
	FM3_DTIM->TIMER1CONTROL_f.INTENABLE = 1; //���荞�݋���
	FM3_DTIM->TIMER1CONTROL_f.TIMERPRE0 = 1; //�N���b�N16����
	FM3_DTIM->TIMER1CONTROL_f.TIMERPRE1 = 0;
	FM3_DTIM->TIMER1CONTROL_f.TIMERSIZE = 0; //16bit�J�E���^
	
	NVIC_EnableIRQ(DTIM_QDU_IRQn);	//DualTimer���荞�݋N��
	//�J�E���^�����l�ݒ�
	FM3_DTIM->TIMER1LOAD = 0x000008CA;
	//FM3_DTIM->TIMER1BGLOAD = 0x000008CA;
	FM3_DTIM->TIMER1CONTROL_f.TIMEREN = 1; //Timer1�N��
}

void SysTick_Handler(void){
	usec ++;
}

void DT_Handler(void){	//DT�����݃n���h��: 0.001sec��
	static uint16_t counter_1hz = 0, counter_20hz = 0, counter_50hz = 0, counter_100hz = 0, counter_200hz = 0;
	
	FM3_DTIM->TIMER1INTCLR = 0x01; //�Ȃ�ł��������珑�����ނƊ����݃N���A�H
	
	if(counter_1hz >= 1000){	//1Hz : LED�_�Ŏ���
		loop_1hz();
		counter_1hz  = 0;
	}
	
	if(counter_20hz >= 50){	//20Hz : �\������
		loop_20hz();
		counter_20hz = 0;
	}
	
	if(counter_50hz >= 20){	//50Hz
		loop_50hz();
		counter_50hz = 0;
	}
	
	if(counter_100hz >= 10){	//100Hz
		loop_100hz();
		counter_100hz = 0;
	}
	
	if(counter_200hz >= 5){	//200Hz : Flow�f�[�^��M����
		loop_200hz();
		counter_200hz = 0;
	}
		
	FM3_DTIM->TIMER1LOAD = 0x000008CA;
	counter_1hz ++;
	counter_20hz ++;
	counter_50hz ++;
	counter_100hz ++;
	counter_200hz ++;
}


uint32_t get_micros(){
	return usec;
}

uint32_t get_millis(){
	return (uint32_t)(usec / 1000);
}

void wait(uint32_t length){
	uint32_t now = usec;
	while(usec <= (now + length));	
	
	return;
}

