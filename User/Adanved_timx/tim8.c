#include "tim8.h"

static void ADVANCED_TIM_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(ADVANCE_TIM_CH1_GPIO_CLK,ENABLE);
	RCC_APB2PeriphClockCmd(ADVANCE_TIM_BKIN_GPIO_CLK, ENABLE);
	
	/* 输出比较通道GPIO初始化 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = ADVANCE_TIM_CH1_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(ADVANCE_TIM_CH1_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = ADVANCE_TIM_CH2_PIN;
	GPIO_Init(ADVANCE_TIM_CH2_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = ADVANCE_TIM_CH3_PIN;
	GPIO_Init(ADVANCE_TIM_CH3_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = ADVANCE_TIM_CH4_PIN;
	GPIO_Init(ADVANCE_TIM_CH4_PORT, &GPIO_InitStructure);

//	/* 输出比较通道刹车通道GPIO初始化 */
//	GPIO_InitStructure.GPIO_Pin = ADVANCE_TIM_BKIN_PIN;
//	GPIO_Init(ADVANCE_TIM_BKIN_PORT, &GPIO_InitStructure);

//	/* 刹车通道引脚默认先输出低电平 */
//	GPIO_ResetBits(ADVANCE_TIM_BKIN_PORT, ADVANCE_TIM_BKIN_PIN);
}

///*
// * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
// * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
// * 另外三个成员是通用定时器和高级定时器才有.
// *-----------------------------------------------------------------------------
// *typedef struct
// *{ TIM_Prescaler            都有
// *	TIM_CounterMode			     TIMx,x[6,7]没有，其他都有
// *  TIM_Period               都有
// *  TIM_ClockDivision        TIMx,x[6,7]没有，其他都有
// *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]才有
// *}TIM_TimeBaseInitTypeDef; 
// *-----------------------------------------------------------------------------
// */

/* ----------------   PWM信号 周期和占空比的计算--------------- */
// ARR ：自动重装载寄存器的值
// CLK_cnt：计数器的时钟，等于 Fck_int / (psc+1) = 72M/(psc+1)
// PWM 信号的周期 T = (ARR+1) * (1/CLK_cnt) = (ARR+1)*(PSC+1) / 72M
// 占空比P=CCR/(ARR+1)

static void ADVANCE_TIM_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
	
	RCC_APB2PeriphClockCmd(ADVANCE_TIM_CLK, ENABLE);
	
/*--------------------时基结构体初始化-------------------------*/	
	// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
	TIM_TimeBaseStructure.TIM_Period = ADVANCE_TIM_PERIOD;
	// 驱动CNT计数器的时钟 = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler = ADVANCE_TIM_PSC;
	// 时钟分频因子 ，配置死区时间时需要用到
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	// 计数器计数模式，设置为向上计数
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	// 重复计数器的值，没用到不用管
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	// 初始化定时器
	TIM_TimeBaseInit(ADVANCE_TIM, &TIM_TimeBaseStructure);
	
/*--------------------输出比较结构体初始化-------------------*/
	// 配置为PWM模式1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	// 输出使能
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	// 输出通道电平极性配置
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	// 输出通道空闲电平极性配置
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	
	//CH1
	TIM_OCInitStructure.TIM_Pulse = 50;
	TIM_OC1Init(ADVANCE_TIM, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(ADVANCE_TIM, TIM_OCPreload_Enable);
	
	//CH2
	TIM_OCInitStructure.TIM_Pulse = 40;
	TIM_OC2Init(ADVANCE_TIM, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(ADVANCE_TIM, TIM_OCPreload_Enable);
	
	//CH3
	TIM_OCInitStructure.TIM_Pulse = 30;
	TIM_OC3Init(ADVANCE_TIM, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(ADVANCE_TIM, TIM_OCPreload_Enable);
	
	//CH4
	TIM_OCInitStructure.TIM_Pulse = 20;
	TIM_OC4Init(ADVANCE_TIM, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(ADVANCE_TIM, TIM_OCPreload_Enable);

///*-------------------刹车和死区结构体初始化-------------------*/
//// 有关刹车和死区结构体的成员具体可参考BDTR寄存器的描述
//  TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
//  TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
//  TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_1;
//	// 输出比较信号死区时间配置，具体如何计算可参考 BDTR:UTG[7:0]的描述
//	// 这里配置的死区时间为0ns
////  TIM_BDTRInitStructure.TIM_DeadTime = 0;
//  TIM_BDTRInitStructure.TIM_Break = TIM_Break_Enable;
//	// 当BKIN引脚检测到高电平的时候，输出比较信号被禁止，就好像是刹车一样
//  TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;
//  TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
//  TIM_BDTRConfig(ADVANCE_TIM, &TIM_BDTRInitStructure);
	
	// 使能计数器
	TIM_Cmd(ADVANCE_TIM, ENABLE);	
	// 主输出使能，当使用的是通用定时器时，这句不需要
	TIM_CtrlPWMOutputs(ADVANCE_TIM, ENABLE);
}

void TIM8_Init(void)
{
	ADVANCED_TIM_GPIO_Config();
	ADVANCE_TIM_Mode_Config();
}


