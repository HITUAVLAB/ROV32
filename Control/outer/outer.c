#include "outer.h"

struct PID pidData_pitch_angle;
struct PID pidData_roll_angle;
struct PID pidData_yaw_angle;

int32_t out_pitch_angle;
int32_t out_roll_angle;
int32_t out_yaw_angle;

void Outer_Init(void)
{
	PIDdataInit(&pidData_pitch_angle,1,0,0,5000,10000);      //kp ki kd imax outmax
	PIDdataInit(&pidData_roll_angle,20,20,5,5000,10000);
	PIDdataInit(&pidData_yaw_angle,50,50,0,5000,10000);
}

void Outer_Loop(void)
{
	//��ȡ����������
	pidData_pitch_angle.feedback=JY901_Angle.Angle[0];
	pidData_roll_angle.feedback=JY901_Angle.Angle[1];
	pidData_yaw_angle.feedback=JY901_Angle.Angle[2];
	//����PID����
	PIDdataUpdate(&pidData_pitch_angle);
	PIDdataUpdate(&pidData_roll_angle);
	PIDdataUpdate(&pidData_yaw_angle);
	//����PID
	GetPID_OUT(&pidData_pitch_angle);
	GetPID_OUT(&pidData_roll_angle);
	GetPID_OUT(&pidData_yaw_angle);
	//�⻷PID���
	out_pitch_angle=pidData_pitch_angle.out;
	out_roll_angle=pidData_roll_angle.out;
	out_yaw_angle=pidData_yaw_angle.out;
}
