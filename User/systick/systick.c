#include "systick.h"

static __IO u32 TimingDelay = 0;

void Systick_Init(void)
{
	/* SystemCoreClock / 1000    1ms�ж�һ��
	 * SystemCoreClock / 100000	 10us�ж�һ��
	 * SystemCoreClock / 1000000 1us�ж�һ��
	 * static __INLINE uint32_t SysTick_Config(uint32_t ticks)��core_cm3.h�ж��壬����������ת�ؼĴ�����ֵ��
	 * ����Ҳ�����ж����ȼ����á�
	*/
	if(SysTick_Config(SystemCoreClock/1000))
	{
		while(1);
	}
}

/**
  * @brief   ms��ʱ����,10usΪһ����λ
  * @param  
  *		@arg nTime: Delay_ms( 1 ) ��ʵ�ֵ���ʱΪ 1 * 1ms = 1ms
  * @retval  ��
  */
void Delay_ms(__IO u32 nTime)
{ 
	TimingDelay = nTime;	

	// ʹ�ܵδ�ʱ��  
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;

	while(TimingDelay != 0);
}

