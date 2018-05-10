#include "loop.h"

static short cnt_Inner,cnt_Remote,cnt_Outer,cnt_Databack,cnt_Ms5837,cnt_Heartbeat;
int8_t Lock_flag;           //0Ϊ������1Ϊ����
int cnt_MS5837;
//static MS5837_ValueTypeDef MS5837_temp={0,0,0,0.03};

static mavlink_message_t msg;
static uint8_t buf[BUF_LENGTH];

void loop_cnt(void)
{
	cnt_Inner++;
	cnt_Remote++;
	cnt_Outer++;
	cnt_Databack++;
	cnt_Ms5837++;
	cnt_Heartbeat++;
	cnt_MS5837++;
}	


//��̬�ڻ�����
static void Loop_Inner(void)
{
	Inner_Loop();
}

//����ң���ź��Լ��������洢����
static void Loop_Remote(void)
{
	Updata_set();
}

//��̬�⻷����
static void Loop_Outer(void)
{
#ifdef OUTTERDEBUG
  Usart_SendString( NEO_USARTx, "out loop debug\n");
#endif
	Outer_Loop();
}

//���˻����ݷ����Լ��Լ�
static void Loop_Databack(void)
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
static void Loop_Ms5837(void)
{
	MS5837_Read_From_Part();
}

//��Nanopi����������,ͬʱ��⴫���������˻�״̬
static void Loop_Heartbeat(void)
{
	int len = 0;
	mavlink_msg_heartbeat_pack(1,200,&msg,MAV_TYPE_SUBMARINE,MAV_AUTOPILOT_GENERIC,MAV_MODE_GUIDED_ARMED,0,MAV_STATE_ACTIVE);
	len = mavlink_msg_to_send_buffer(buf, &msg);   //�����������һ��������ǣ����lenֻ�Ǽ򵥷�ֵ����ô�ǻ���Ϊlenû�б�ʹ��
	Usart_SendArray(NEO_USARTx,(uint8_t *)buf,len);
}	

void ROV_Loop(void)
{
	if( cnt_Inner >= 20 )
	{
		if( Lock_flag )
			Loop_Inner();  //��̬�ڻ����� 50Hz
		cnt_Inner = 0;
	}
	
	if( cnt_Remote >= 10 )
	{
		Loop_Remote(); //����ң���ź��Լ��������洢���� 100Hz
		cnt_Remote = 0;
	}
	
	if( cnt_Outer >= 100 )
	{
		if( Lock_flag )
			Loop_Outer();     //��̬�⻷���� 10Hz
		cnt_Outer = 0;
	}
	
	if( cnt_Databack >= 50 )
	{
		Loop_Databack();    //���ݻش�20Hz
		cnt_Databack = 0;
	}
	
	if( cnt_Ms5837 >= 67 )
	{
		Loop_Ms5837();      //MS5837��ȡ 15Hz
		cnt_Ms5837 = 0;
	}
	
	if( cnt_Heartbeat >= 1000)
	{
		Loop_Heartbeat();   //������ 1Hz
		cnt_Heartbeat = 0;
	}
}


void cnt_init(void)
{
	cnt_Inner = 0;
	cnt_Remote = 0;
	cnt_Outer = 81;
	cnt_Databack = 0;
	cnt_Ms5837 = 0;
	cnt_Heartbeat = 0;
	cnt_MS5837 = 0;
	Lock_flag = 1;
}


