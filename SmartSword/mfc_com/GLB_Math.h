#pragma once
#ifndef __GLB_MAT_H__
#define __GLB_MAT_H__
#include <glut.h>
#include <windows.h>
#include <math.h>

int Compass_Yaw_Data[8]={0};

typedef struct _GL_Vector_
{
	float x;
	float y;
	float z;
}GL_Vector;

typedef struct _GL_Point_
{
	float x;
	float y;
	float z;
}GL_Point;

typedef struct _GL_Quater_ //四元数简易 结构体
{
	float q0;
	float q1;
	float q2;
	float q3;
}GL_Quater;

/*********************************************************************/
//计算对应 Normals Vector1 X Vector2  
GL_Vector GLB_CHAJI(float ax,float ay,float az,float bx,float by,float bz)
{
	GL_Vector v;
	v.x=0;
	v.y=0;
	v.z=0;

	float N_X=(ay*bz-az*by);
	float N_Y=(az*bx-ax*bz);
	float N_Z=(ax*by-ay*bx);

	float norm=N_X*N_X + N_Y*N_Y + N_Z*N_Z;
	norm = sqrt(norm);
	
	if(norm!=0)
	{
		v.x=N_X/norm;
		v.y=N_Y/norm;
		v.z=N_Z/norm;
		return v;
	}

	return v;
}

/************************************三维向量夹角*********************************/
float GLB_Angle(float a1,float a2,float a3,float b1,float b2,float b3)
{
	float angle=0;
	float a=a1*b1+a2*b2+a3*b3;

	float b=a1*a1+a2*a2+a3*a3;
	b=sqrt(b);

	float c=b1*b1+b2*b2+b3*b3;
	c=sqrt(c);

	if(b*c!=0)
	{
		float angle_r=a/(b*c);
		angle=acos(angle_r);

		return angle;
	}
	return angle;
}
/************************************三维点绕任意轴旋转*********************************/
GL_Point GLB_Point(float old_x,float old_y,float old_z,float angle,float x,float y,float z)
{
	GL_Point New_Pt;
	float new_x,new_y,new_z;

	//angle=3.1415926*20/180;
	float c=cos(angle);
	float s=sin(angle);

	new_x = (x*x*(1-c)+c) * old_x + (x*y*(1-c)-z*s) * old_y + (x*z*(1-c)+y*s) * old_z;

	new_y = (y*x*(1-c)+z*s) * old_x + (y*y*(1-c)+c) * old_y + (y*z*(1-c)-x*s) * old_z;

	new_z = (x*z*(1-c)-y*s) * old_x + (y*z*(1-c)+x*s) * old_y + (z*z*(1-c)+c) * old_z;

	New_Pt.x=new_x;
	New_Pt.y=new_y;
	New_Pt.z=new_z;

	return New_Pt;
}


/***************************************** 磁场旋转 *****************************************/
int GLB_Angle_Compass_Yaw(float mx,float my,float mz,float Rr,int PitchN,int RollN,int YawN)
{
	float p=-3.1415926*float(PitchN)/180;
	float r=-3.1415926*float(RollN)/180;
	float norm =mx*mx+my*my+mz*mz;
	norm=sqrt(norm);

	if(norm!=0)
	{
		mx/=norm;
		my/=norm;
		mz/=norm;
	}
	

	float Hx=mx*cos(p)+my*sin(r)*sin(p)-mz*cos(r)*sin(p);
	float Hy=my*cos(r)+mz*sin(r);

	//float Hx=HM_x*cos(Pitch)+HM_y*sin(Rool)*sin(Pitch)-HM_z*cos(Rool)*sin(Pitch);
	//float Hy=HM_y*cos(Rool)+HM_z*sin(Rool);


	float anglef=0;
	anglef=atan2(Hx,Hy);

	int Angle=180.0*anglef/3.1415926;

	if(Angle<=0)Angle=360+Angle;

	
	return Angle;
}

/***********************共轭求逆**************************/
//输入参数 ：四元数
//输出参数 : 四元数的逆
//公式查看： 交接文档 ——————手势建模 -姿态运算说明
GL_Quater Conjugate_Q(GL_Quater QN)
{
	GL_Quater QN_C;//共轭
	float norm = sqrt(QN.q0*QN.q0+QN.q1*QN.q1+QN.q2*QN.q2+QN.q3*QN.q3);//求模

	QN_C.q0= QN.q0;
	QN_C.q1=-QN.q1;
	QN_C.q2=-QN.q2;
	QN_C.q3=-QN.q3;

	GL_Quater QN_I;//求逆

	QN_I.q0=QN_C.q0/norm;
	QN_I.q1=QN_C.q1/norm;
	QN_I.q2=QN_C.q2/norm;
	QN_I.q3=QN_C.q3/norm;

	return QN_I;
}

/***********************四元数相乘**************************/
//公式查看： 交接文档 ——————手势建模 -姿态运算说明
//输入参数：两个四元数
//输出参数：两个四元数的乘积
GL_Quater MUL_Q(GL_Quater Q1,GL_Quater Q2)
{
	CvMat* A1 = cvCreateMat(4,4,CV_32FC1);//构造4*4四元数矩阵
	CvMat* A2 = cvCreateMat(4,1,CV_32FC1);//构造4*1四元数矩阵
	CvMat* MM = cvCreateMat(4,1,CV_32FC1);//构造4*1四元数矩阵

	float lm=Q1.q0;
	float p1=Q1.q1;
	float p2=Q1.q2;
	float p3=Q1.q3;
	//------4*4矩阵赋值
	cvmSet(A1,0,0,lm);
	cvmSet(A1,0,1,-p1);
	cvmSet(A1,0,2,-p2);
	cvmSet(A1,0,3,-p3);

	cvmSet(A1,1,0,p1);
	cvmSet(A1,1,1,lm);
	cvmSet(A1,1,2,-p3);
	cvmSet(A1,1,3,p2);

	cvmSet(A1,2,0,p2);
	cvmSet(A1,2,1,p3);
	cvmSet(A1,2,2,lm);
	cvmSet(A1,2,3,-p1);

	cvmSet(A1,3,0,p3);
	cvmSet(A1,3,1,-p2);
	cvmSet(A1,3,2,p1);
	cvmSet(A1,3,3,lm);
	//------4*1矩阵赋值

	cvmSet(A2,0,0,Q2.q0);
	cvmSet(A2,1,0,Q2.q1);
	cvmSet(A2,2,0,Q2.q2);
	cvmSet(A2,3,0,Q2.q3);
	//------

	cvMatMulAdd( A1, A2, 0, MM );//矩阵相乘

	GL_Quater QN;
	//输出四元数 赋值
	QN.q0=cvmGet(MM,0,0);
	QN.q1=cvmGet(MM,1,0);
	QN.q2=cvmGet(MM,2,0);
	QN.q3=cvmGet(MM,3,0);
	//释放矩阵缓存
	cvReleaseMat(&A1);
	cvReleaseMat(&A2);
	cvReleaseMat(&MM);

	return QN;//返回求得 四元数
}
#endif