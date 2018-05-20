#ifndef _BEEP_H_
#define _BEEP_H_

#include "stm32f10x.h"
#include "systick.h"

/* ������������ӵ�GPIO�˿� */
#define BEEP_GPIO_PORT    	GPIOC			              /* GPIO�˿� */
#define BEEP_GPIO_CLK 	    RCC_APB2Periph_GPIOC		/* GPIO�˿�ʱ�� */
#define BEEP_GPIO_PIN		  	GPIO_Pin_1			        /* ���ӵ���������GPIO */

void BEEP_GPIO_Config(void);
void BEEP_ON(int16_t f);

#endif

