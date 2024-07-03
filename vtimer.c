#include "vtimer.h"

/*this array holds the virtual timers tick value, callback function  */
static volatile Vtimer_t sVtimer[VTIMER_NUM];
__IO uint8_t counter100ms; /* systick counter */

// in stm32f4xx_it.c
//	extern void _100ms_IRQHandler (void);
///* USER CODE BEGIN SysTick_IRQn 1 */
//	_100ms_IRQHandler();
///* USER CODE END SysTick_IRQn 1 */
void _100ms_IRQHandler(void)
{
	counter100ms++;
	if ( counter100ms > 99 )
	{		
			counter100ms = 0;
			Timer_100ms_Callback();
	}
}
void Timer_100ms_Callback(void)
{
	uint8_t i;
	for (i = 0; i < VTIMER_NUM; i++)
	{
		if (sVtimer[i].timebase > 0)/* if timer is active */
		{
			if (sVtimer[i].tick != 0)/* if not overflow yet */
			{
				sVtimer[i].tick--;
				if (sVtimer[i].tick == 0)/* if timer overflow */
				{
					sVtimer[i].elapsed_flag = SET;
					if (sVtimer[i].pCallback != 0)/* if any callback exists */
					{
						((PFN_Callback_t)sVtimer[i].pCallback)();
					}				
				}			
			}					
		}
	}
}

void vtimer_InitAllTimers(void)
{
	uint8_t i;
	for (i = 0; i < VTIMER_NUM; i++)
	{
		vtimer_DeInit((VtimerName_t)i);
	}
}

void vtimer_Config(VtimerName_t vtim,timer_res_t  msec,void ( *pCallback)(void))
{
	sVtimer[vtim].timebase = msec;
	sVtimer[vtim].elapsed_flag = RESET;
	sVtimer[vtim].pCallback = pCallback;
}

void vtimer_Start(VtimerName_t vtim)
{
	sVtimer[vtim].tick = sVtimer[vtim].timebase;
}

void vtimer_Reload(VtimerName_t vtim)
{
	vtimer_Start(vtim);
}

void vtimer_Stop(VtimerName_t vtim)
{
	sVtimer[vtim].tick = 0;
}

void vtimer_DeInit(VtimerName_t vtim)
{
	sVtimer[vtim].timebase = 0;
	sVtimer[vtim].tick = 0;
	sVtimer[vtim].elapsed_flag = RESET;
	sVtimer[vtim].pCallback = 0;
}

void vtimer_Clear_ElapsedFlag(VtimerName_t vtim)
{
	if (sVtimer[vtim].elapsed_flag == SET)
	{
			sVtimer[vtim].elapsed_flag = RESET;
	}
}

/* Check timer elapsed in polling mode */
FlagStatus vtimer_TimerElapsed(VtimerName_t vtim)
{
	if (sVtimer[vtim].elapsed_flag == SET) return SET;
	else return RESET;
}
