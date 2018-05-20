#include "beep.h"

void BEEP_GPIO_Config(void)
{		
		/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*�������Ʒ�������GPIO�Ķ˿�ʱ��*/
		RCC_APB2PeriphClockCmd( BEEP_GPIO_CLK, ENABLE); 

		/*ѡ��Ҫ���Ʒ�������GPIO*/															   
		GPIO_InitStructure.GPIO_Pin = BEEP_GPIO_PIN;	

		/*����GPIOģʽΪͨ���������*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*����GPIO����Ϊ50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*���ÿ⺯������ʼ�����Ʒ�������GPIO*/
		GPIO_Init(BEEP_GPIO_PORT, &GPIO_InitStructure);			 
    
    /* �رշ�����*/
		GPIO_ResetBits(BEEP_GPIO_PORT, BEEP_GPIO_PIN);
		GPIO_SetBits(BEEP_GPIO_PORT, BEEP_GPIO_PIN);
}

void BEEP_ON(int16_t f)
{
	int i = 0;
	for( i = 0;i <= 100;i++ )
	{
		if(i/2)
			GPIO_SetBits(BEEP_GPIO_PORT, BEEP_GPIO_PIN);
		else
			GPIO_ResetBits(BEEP_GPIO_PORT, BEEP_GPIO_PIN);
		Delay_ms(1000/f);
	}
}

