#pragma once
#include<math.h>
#include <windows.h>
#include <cv.h>
#include <highgui.h>


//---------------------------------------------------------------------------------------------------
// 变量定义

//---------------------------------------------------------------------------------------------------
// Variable definitions
#define Kp 2.0001
#define Ki 0.00033

float Rotx,Roty,Rotz;

//float exInt=0,eyInt=0,ezInt=0;
float q0 = 1, q1 = 0, q2 = 0, q3 = 0;        // quaternion elements representing the estimated orientation
float q0b = 1, q1b = 0, q2b = 0, q3b = 0;

float q0_data[6]={1};
float q1_data[6]={0};
float q2_data[6]={0};
float q3_data[6]={0};

volatile float exInt = 0, eyInt = 0, ezInt = 0;

// 偏航角——Yaw,俯仰角——Pitch,翻滚角——Rool
	float Pitch;
	float Rool ;
	float Yaw  ;

	int Pitch_angle;
	int Rool_angle ;
	int Yaw_angle  ;

	int Pitch_angle_S;
	int Rool_angle_S ;
	int Yaw_angle_S  ;

	int Compass_X,Compass_Y,Compass_Z;
	int Compass_X_Old[4]={0};
	int Compass_Y_Old[4]={0};
	int Compass_Z_Old[4]={0};

	/*int Compass_X_Old;
	int Compass_Y_Old;
	int Compass_Z_Old;*/

	int Compass_X_Min=6000,Compass_Y_Min=6000,Compass_Z_Min=6000;
	int Compass_X_Max=-6000,Compass_Y_Max=-6000,Compass_Z_Max=-6000;

	bool Compass_Start=0;//磁场范围采集开始信号

	//磁场 数组
	int m_x[7]={0};
	int m_y[7]={0};
	int m_z[7]={0};

	int hx_d[3]={0};
	int hy_d[3]={0};
	int hz_d[3]={0};

	float G0=9.8;

	float Hx=0,Hy=0;
//====================================================================================================
// Function
//加数度为3轴采样数据，做归一化
//输出为4元数

void LXW_IMU_6( int Rool_angle0,int Pitch_angle0,int Yaw_angle0)
{
	float rot_xx= float(Rool_angle0)*3.1415926/180;//弧度 单位
	float rot_yy= float(Pitch_angle0)*3.1415926/180;
	float rot_zz= float(Yaw_angle0)*3.1415926/180;

	q0 = cos(rot_xx/2)*cos(rot_yy/2)*cos(rot_zz/2)+sin(rot_xx/2)*sin(rot_yy/2)*sin(rot_zz/2);

	q1 = sin(rot_xx/2)*cos(rot_yy/2)*cos(rot_zz/2)-cos(rot_xx/2)*sin(rot_yy/2)*sin(rot_zz/2);

	q2 = cos(rot_xx/2)*sin(rot_yy/2)*cos(rot_zz/2)+sin(rot_xx/2)*cos(rot_yy/2)*sin(rot_zz/2);

	q3 = cos(rot_xx/2)*cos(rot_yy/2)*sin(rot_zz/2)-sin(rot_xx/2)*sin(rot_yy/2)*cos(rot_zz/2);

	/*rot_xx=Rool;
	rot_yy=Pitch;
	rot_zz=Yaw;
	q0 = cos(rot_xx/2)*cos(rot_yy/2)*cos(rot_zz/2)+sin(rot_xx/2)*sin(rot_yy/2)*sin(rot_zz/2);

	q1 = sin(rot_xx/2)*cos(rot_yy/2)*cos(rot_zz/2)-cos(rot_xx/2)*sin(rot_yy/2)*sin(rot_zz/2);

	q2 = cos(rot_xx/2)*sin(rot_yy/2)*cos(rot_zz/2)+sin(rot_xx/2)*cos(rot_yy/2)*sin(rot_zz/2);

	q3 = cos(rot_xx/2)*cos(rot_yy/2)*sin(rot_zz/2)-sin(rot_xx/2)*sin(rot_yy/2)*cos(rot_zz/2);*/

}

/********************************* MPU9250 坐标系欧拉角 转 OpenGL坐标系四元数  **********************************/
void LXW_IMU_6B( int Rool_angle0,int Pitch_angle0,int Yaw_angle0)
{
	float p= float(Pitch_angle0)*3.1415926/180/2;//弧度 单位
	float r= float(Rool_angle0)*3.1415926/180/2;
	float y= float(Yaw_angle0)*3.1415926/180/2;

	q0 = cos(r)*cos(p)*cos(y) + sin(r)*sin(p)*sin(y);

	q1 = cos(r)*sin(p)*cos(y) + sin(r)*cos(p)*sin(y);

	q2 = cos(r)*cos(p)*sin(y) - sin(r)*sin(p)*cos(y);

	q3 = sin(r)*cos(p)*cos(y) - cos(r)*sin(p)*sin(y);

}
/***************************************** 磁场航向 *****************************************/
int GLB_Angle_Compass_YawB(float mx,float my,float mz,float Rr,int PitchN,int RollN,int YawN)
{
	////滤波
	
	float p=3.1415926*float(PitchN)/180;
	float r=3.1415926*float(RollN)/180;
	/*float norm =mx*mx+my*my+mz*mz;
	norm=sqrt(norm);*/

	/*if(norm!=0)
	{
		mx/=norm;
		my/=norm;
		mz/=norm;
	}*/


	

#if 1

	Hx=mx*sin(r)*sin(p)+my*cos(p)-mz*cos(r)*sin(p);
	Hy=mx*cos(r)+mz*sin(r);
#else

	float Hx=mx*cos(r) + my*sin(r)*sin(p) - mz*cos(p)*sin(r);
	float Hy=my*cos(p) + mz*sin(p);
#endif


	// for(int i=3;i>0;i--)
	//{
	//	hx_d[i]=hx_d[i-1];
	//	hy_d[i]=hy_d[i-1];

	//}

	// hx_d[0]=Hx;
	// hy_d[0]=Hy;

	// Hx=0;Hy=0;

	// for(int i=0;i<3;i++)
	// {
	// Hx+=hx_d[i];
	// Hy+=hy_d[i];

	// }
	// Hx/=3;
	// Hy/=3;


	float anglef=0;
	anglef=atan2(Hx,Hy);
	int Angle=180.0*anglef/3.1415926;

	/*if(Hy==0&&Hx<0)Angle=90;
	else if(Hy==0&&Hx>0)Angle=270;*/

	if(Angle<=0)Angle=360+Angle;


	return Angle;
}

/******************************************/
void GLB_G0_Pitch_Roll(float &G0_Pitch,float &G0_Roll,float G_ax,float G_ay,float G_az)
{
	float norm=G_ax*G_ax+G_ay*G_ay+G_az*G_az;
	if(norm==0) return ;

	norm=G_ax*G_ax+G_az*G_az;
	norm=sqrt(norm);

	if(norm!=0)
	G0_Pitch=atan(G_ay/norm);

	norm=G_ay*G_ay+G_az*G_az;
	if(norm==0) return ;
	norm=sqrt(norm);
	if(norm!=0)
	G0_Roll=atan(G_ax/norm);
}


/******************************************/
void Quarter_T_OlA(float qq0,float qq1,float qq2,float qq3)
{
	////方向余弦矩阵寄存器
	//更新方向余弦矩阵
	float t11,t12,t13,t21,t22,t23,t31,t32,t33;
		t11=qq0*qq0+qq1*qq1-qq2*qq2-qq3*qq3;
		t12=2.0*(qq1*qq2+qq0*qq3);
		t13=2.0*(qq1*qq3-qq0*qq2);
		t21=2.0*(qq1*qq2-qq0*qq3);
		t22=qq0*qq0-qq1*qq1+qq2*qq2-qq3*qq3;
		t23=2.0*(qq2*qq3+qq0*qq1);
		t31=2.0*(qq1*qq3+qq0*qq2);
		t32=2.0*(qq2*qq3-qq0*qq1);
		t33=qq0*qq0-qq1*qq1-qq2*qq2+qq3*qq3;
	
		//------ 偏航角——Yaw,俯仰角——Pitch,翻滚角——Rool  （弧度）
		Rool = atan2(t23,t33);
		Pitch = -asin(t13);
	//	if(abs(gx)<1/3.1159 && abs(gy)<1/3.1159 && abs(gz)<1/3.1159)
		{
		Yaw = atan2(t12,t11);
	
		if (Yaw < 0) Yaw += 360/180*3.1415926;
		}
		//Yaw += 360/180*PI;
	//	else	if (Yaw > 2*PI) Yaw = Yaw-360/180*PI;
	
		//--- 弧度转为角度
		Rool_angle = Rool/3.1415926*180;
		Pitch_angle = Pitch/3.1415926*180;
		Yaw_angle = Yaw/3.1415926*180;

	if (Yaw_angle < 0) Yaw_angle += 360;
}

/******************************************/
void OlA_T_Quarter(int Rool_angleN,int Pitch_angleN,int Yaw_angleN)
{
	float p= float(Pitch_angleN)*3.1415926/180/2;//弧度 单位
	float r= float(Rool_angleN)*3.1415926/180/2;
	float y= float(Yaw_angleN)*3.1415926/180/2;

	q0=cos(r)*cos(p)*cos(y) + sin(r)*sin(p)*sin(y);
	q1=sin(r)*cos(p)*cos(y) - cos(r)*sin(p)*sin(y);
	q2=cos(r)*sin(p)*cos(y) + sin(r)*cos(p)*sin(y);
	q3=cos(r)*cos(p)*sin(y) - sin(r)*sin(p)*cos(y);
}



/********************* xyzw *********************/
void slerp( float result[4], float starting[4], float ending[4], float t )
{
	float cosa = starting[0]*ending[0] + starting[1]*ending[1] + 
		starting[2]*ending[2] + starting[3]*ending[3];
	if ( cosa < 0.0f ) {
		ending[0] = -ending[0];
		ending[1] = -ending[1];
		ending[2] = -ending[2];
		ending[3] = -ending[3];
		cosa = -cosa;
	}
	float k0, k1;
	if ( cosa > 0.9999f ) {
		k0 = 1.0f - t;
		k1 = t;
	}
	else {
		float sina = sqrt( 1.0f - cosa*cosa );
		float a = atan2( sina, cosa );
		float invSina = 1.0f / sina;
		k0 = sin((1.0f - t)*a) * invSina;
		k1 = sin(t*a) * invSina;
	}
	result[0] = starting[0]*k0 + ending[0]*k1;
	result[1] = starting[1]*k0 + ending[1]*k1;
	result[2] = starting[2]*k0 + ending[2]*k1;
	result[3] = starting[3]*k0 + ending[3]*k1;
}