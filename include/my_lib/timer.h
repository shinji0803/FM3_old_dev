
#ifndef TIMER_H
#define TIMER_H

#include "mcu.h"

//SysTickタイマの設定（時間計測用）
void Init_timer(void);

//タイマ値を返す
uint32_t get_micros(void);
uint32_t get_millis(void);

//Wait関数（引数usec）
void wait(uint32_t length);

//デュアルタイマの設定
void Init_DT(void);

//デュアルタイマハンドラ内で実行する関数
extern void loop_1hz(void);
extern void loop_20hz(void);
extern void loop_100hz(void);
extern void loop_200hz(void);

#endif

