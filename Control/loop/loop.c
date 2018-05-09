#include "loop.h"

static short cnt_200Hz,cnt_100Hz,cnt_50Hz,cnt_20Hz,cnt_15Hz,cnt_1Hz;
int8_t Lock_flag;           //0Ϊ������1Ϊ����
int cnt_MS5837;
//static MS5837_ValueTypeDef MS5837_temp={0,0,0,0.03};

static mavlink_message_t msg;
static uint8_t buf[BUF_LENGTH];

void loop_cnt(void)
{
	cnt_200Hz++;
	cnt_100Hz++;
	cnt_50Hz++;
	cnt_20Hz++;
	cnt_15Hz++;
	cnt_1Hz++;
	cnt_MS5837++;
}	


//��̬�ڻ�����
static void Loop_200Hz(void)
{
	Inner_Loop();
}

//����ң���ź��Լ��������洢����
static void Loop_100Hz(void)
{
	Updata_set();
}

//��̬�⻷����
static void Loop_50Hz(void)
{
#ifdef OUTTERDEBUG
  Usart_SendString( NEO_USARTx, "out loop debug\n");
#endif
	Outer_Loop();
}

//���˻����ݷ����Լ��Լ�
static void Loop_20Hz(void)
{
	  int len;
	
#ifdef JY901DEBUG
	  char str[100];
#endif
	
#ifdef DATASENDDEBUG
	Usart_SendString( NEO_USARTx, "20Hz\n");
#endif
	//�������˻��Լ죬������������
    
	 
	  //������ȴ�������Ϣ,if depth has changed
	  //�������ԣ�MS5837�ܹ���ȡ��������ֵ�����з���
		 len = MS5837_Send_MAVLink_Message(&msg,buf);
	
	  //����JY901��Ϣ
	  //�������ԣ�JY901�ܹ�������ȡ��������ֵ�����з���
#ifdef JY901DEBUG		
    sprintf(str,"pitch=%d,roll=%d,yaw=%d\n",JY901_Angle.Angle[0],JY901_Angle.Angle[1],JY901_Angle.Angle[2]);
		Usart_SendString(NEO_USARTx,str);
#endif
     len = JY901_Send_MAVLink_Message(&msg,buf);
    
		 
	
//����JY901��Ϣ
//�������ԣ�JY901�ܹ�������ȡ��������ֵ�����з���
#ifdef JY901DEBUG		
	sprintf(str,"roll=%f,yaw=%f,pitch=%f\n",JY901_Angle.Angle[0],JY901_Angle.Angle[1],JY901_Angle.Angle[2]);
	Usart_SendString(NEO_USARTx,str);
#endif
	len = JY901_Send_MAVLink_Message(&msg,buf);
    
}

//MS5837 Deep Sensor Data collection
static void Loop_15Hz(void)
{
	MS5837_Read_From_Part();
}

//��Nanopi����������,ͬʱ��⴫���������˻�״̬
static void Loop_1Hz(void)
{
	int len = 0;
	mavlink_msg_heartbeat_pack(1,200,&msg,MAV_TYPE_SUBMARINE,MAV_AUTOPILOT_GENERIC,MAV_MODE_GUIDED_ARMED,0,MAV_STATE_ACTIVE);
	len = mavlink_msg_to_send_buffer(buf, &msg);   //�����������һ��������ǣ����lenֻ�Ǽ򵥷�ֵ����ô�ǻ���Ϊlenû�б�ʹ��
	Usart_SendArray(NEO_USARTx,(uint8_t *)buf,len);
}	

void ROV_Loop(void)
{
	if( cnt_200Hz >= 5 )
	{
		if( Lock_flag )
			Loop_200Hz();  //��̬�ڻ�����
		cnt_200Hz = 0;
	}
	
	if( cnt_100Hz >= 10 )
	{
		Loop_100Hz(); //����ң���ź��Լ��������洢����
		cnt_100Hz = 0;
	}
	
	if( cnt_50Hz >= 20 )
	{
		if( Lock_flag )
			Loop_50Hz();     //��̬�⻷����
		cnt_50Hz = 0;
	}
	
	if( cnt_20Hz >= 50 )
	{
		Loop_20Hz();
		cnt_20Hz = 0;
	}
	
	if( cnt_15Hz >= 67 )
	{
		Loop_15Hz();
		cnt_15Hz = 0;
	}
	
	if( cnt_1Hz >= 1000)
	{
		Loop_1Hz();
		cnt_1Hz = 0;
	}
}


void cnt_init(void)
{
	cnt_200Hz = 0;
	cnt_100Hz = 0;
	cnt_50Hz = 16;
	cnt_20Hz = 0;
	cnt_15Hz = 0;
	cnt_MS5837 = 0;
	Lock_flag = 1;
}


