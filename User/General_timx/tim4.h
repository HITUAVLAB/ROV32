#ifndef _TIM4_H_
#define _TIM4_H_

#include "stm32f10x.h"

/************ͨ�ö�ʱ��TIM�������壬ֻ��TIM2��3��4��5************/
// ��ʹ�ò�ͬ�Ķ�ʱ����ʱ�򣬶�Ӧ��GPIO�ǲ�һ���ģ����Ҫע��
// ��������Ĭ��ʹ��TIM4

#define            LED_TIM                   TIM4
#define            LED_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            LED_TIM_CLK               RCC_APB1Periph_TIM4
#define            LED_TIM_Period            (20000-1)
#define            LED_TIM_Prescaler         (72-1)

// TIM4 CH3
#define            LED1_TIM_CH3_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            LED1_TIM_CH3_PORT          GPIOB
#define            LED1_TIM_CH3_PIN           GPIO_Pin_8

// TIM4 CH4
#define            LED2_TIM_CH4_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            LED2_TIM_CH4_PORT          GPIOB
#define            LED2_TIM_CH4_PIN           GPIO_Pin_9

void LED_TIM4_Init(void);

#endif 
