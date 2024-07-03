/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __VTIMER_H
#define __VTIMER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

typedef enum{
	VTIM0 = 0U,
	VTIM1,
	VTIM2,
	VTIM3,
	VTIM4,
	/*VTIM5,VTIM6,VTIM7,VTIM8,VTIM9,VTIM10,*/
	VTIMER_NUM
}VtimerName_t;

typedef uint16_t timer_res_t;
typedef struct
{
	timer_res_t timebase;
	timer_res_t tick;
	FlagStatus elapsed_flag; 
	void (*pCallback)(void);
}Vtimer_t,*PVtimer;

typedef void(*PFN_Callback_t)(void);

void _100ms_IRQHandler(void);
void Timer_100ms_Callback(void);
void vtimer_InitAllTimers(void);
void vtimer_Config(VtimerName_t vtim,timer_res_t msec,void(*pCallback)(void));
void vtimer_Start(VtimerName_t vtim);
void vtimer_Reload(VtimerName_t vtim);
void vtimer_Stop(VtimerName_t vtim);
void vtimer_DeInit(VtimerName_t vtim);
void vtimer_Clear_ElapsedFlag(VtimerName_t vtim);
FlagStatus vtimer_TimerElapsed(VtimerName_t vtim);

#ifdef __cplusplus
}
#endif

#endif /* __VTIMER_H */
