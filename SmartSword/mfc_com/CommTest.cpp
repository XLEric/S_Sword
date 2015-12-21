// CommTest.cpp : 实现文件
//

#include "stdafx.h"
#include "mfc_com.h"
#include "CommTest.h"
#include "afxdialogex.h"
#include "OpenGL.h"
#include "LXW_IMU_6.h"
#include "windows.h"
#include "Param_XF.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include <winsock.h>
//#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
/******************************************************
*基于TCP客户端程序设计 
******************************************************/
#include <iostream>
#include <string>
#include <windows.h>

long fpsnum=0;
float Q0=1;//OpenGL四元数
float Q1=0;
float Q2=0;
float Q3=0;

float Q0b=1;//OpenGL四元数
float Q1b=0;
float Q2b=0;
float Q3b=0;

float Q0r=1;//传感器四元数
float Q1r=0;
float Q2r=0;
float Q3r=0;

bool flag_com=0;
bool flag_Ncommon=0;
bool flag_connect=0;

double timex=0;

long int start_time;
long int end_time;

int LXW_control=0;
short Cnt_flag=0;

GLfloat Mccx[16]={0};

bool flag_axyz[3]={0};

int G_ax=0,G_ay=0,G_az=0;
float G0_Pitch=0,G0_Roll=0;
CvCapture* capture1,*capture2;
IplImage* frame1=cvCreateImage(cvSize(320,240) , 8 , 3);
IplImage* frame2=cvCreateImage(cvSize(320,240) , 8 , 3);

bool Compass_Flag_OffsetG=0;

/*初始化套接字*/
WSADATA wsa;
SOCKET s ;//= socket(AF_INET,SOCK_STREAM,0);
struct sockaddr_in sa;
char sendBuf[1024];
// CCommTest 对话框

IMPLEMENT_DYNAMIC(CCommTest, CDialog)

CCommTest::CCommTest(CWnd* pParent /*=NULL*/)
	: CDialog(CCommTest::IDD, pParent)
	, m_EditSend(_T(""))
	, m_EditReceive(_T(""))
	, C_Time(_T(""))
{

}

CCommTest::~CCommTest()
{
}

void CCommTest::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSCOMM1, m_mscomm);
	DDX_Text(pDX, IDC_EDIT1, m_EditSend);
	DDX_Text(pDX, IDC_EDIT2, m_EditReceive);
	DDX_Text(pDX, IDC_EDIT3, C_Time);
}


BEGIN_MESSAGE_MAP(CCommTest, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CCommTest::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CCommTest::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CCommTest::OnBnClickedButtonOpen)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, &CCommTest::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_ReSet, &CCommTest::OnBnClickedReset)
END_MESSAGE_MAP()


// CCommTest 消息处理程序
BEGIN_EVENTSINK_MAP(CCommTest, CDialog)
	ON_EVENT(CCommTest, IDC_MSCOMM1, 1, CCommTest::OnCommMscomm1, VTS_NONE)
END_EVENTSINK_MAP()


void CCommTest::OnCommMscomm1()
{
	// TODO: 在此处添加消息处理程序代码
    static unsigned int cnt=0;
	VARIANT variant_inp;
	COleSafeArray safearray_inp;
	long len,k;
	unsigned int data[1024]={0};
	char rxdata[1024]; //设置 BYTE 数组
	//四元数
	char rxdata_q0[6]; //设置 BYTE 数组
	char rxdata_q1[6]; //设置 BYTE 数组
	char rxdata_q2[6]; //设置 BYTE 数组
	char rxdata_q3[6]; //设置 BYTE 数组
	//加速度
	char rxdata_ax[4]; //设置 BYTE 数组
	char rxdata_ay[4]; //设置 BYTE 数组
	char rxdata_az[4]; //设置 BYTE 数组

	//角速度场
	char rxdata_rx[4];
	char rxdata_ry[4];
	char rxdata_rz[4];

	//磁场
	char rxdata_mx[4];
	char rxdata_my[4];
	char rxdata_mz[4];

	char rxdataflagx[1]; //设置 BYTE 数组
	char rxdataflagy[1]; //设置 BYTE 数组
	char rxdataflagz[1]; //设置 BYTE 数组
	
	

	CString strtemp;
	char command_a=0,command_b=0;
	
	if(m_mscomm.get_CommEvent()==2)//值为 2 表示接收缓冲区内有字符
	{
		cnt++;
		variant_inp=m_mscomm.get_Input();//读缓冲区消息
		safearray_inp=variant_inp;///变量转换
		len=safearray_inp.GetOneDimSize();//得到有效的数据长度
	
		for(k=0;k<len;k++)
		{
			safearray_inp.GetElement(&k,rxdata+k);
			//帧头
			if(k==0)command_a=rxdata[k];
			if(k==1)command_b=rxdata[k];
			//四元数
			if(k>=2&&k<=7)
			rxdata_q0[k-2]=rxdata[k];
			if(k>=8&&k<=13)
			rxdata_q1[k-8]=rxdata[k];
			if(k>=14&&k<=19)
			rxdata_q2[k-14]=rxdata[k];
			if(k>=20&&k<=25)
			rxdata_q3[k-20]=rxdata[k];
			
			////磁场
			if(k>=26&&k<=29)
				rxdata_mx[k-26]=rxdata[k];
			if(k>=30&&k<=33)
				rxdata_my[k-30]=rxdata[k];
			if(k>=34&&k<=37)
				rxdata_mz[k-34]=rxdata[k];


			//角速度

			if(k>=38&&k<=41)
				rxdata_rx[k-38]=rxdata[k];
			if(k>=42&&k<=45)
				rxdata_ry[k-42]=rxdata[k];
			if(k>=46&&k<=49)
				rxdata_rz[k-46]=rxdata[k];

			
			//重力场
			if(k>=50&&k<=53)
				rxdata_ax[k-50]=rxdata[k];
			if(k>=54&&k<=57)
				rxdata_ay[k-54]=rxdata[k];
			if(k>=58&&k<=61)
				rxdata_az[k-58]=rxdata[k];
				
		}
		flag_connect=0;
		if(command_a=='X' && command_b=='W')//帧头校验
		{
			//frame1 = cvQueryFrame( capture1 );
			////if( !frame1 ) 
			//cvShowImage( "Camera_1", frame1 );

			//frame2 = cvQueryFrame( capture2 );
			////if( !frame2 ) 
			//	cvShowImage( "Camera_2", frame2 );

			flag_connect=1;//通信 正确标志
			SetDlgItemText(IDC_EDIT2," ");
			UpdateData(true);
			//四元数显示
			Q0r=float(atoi(rxdata_q0))/10000;
			Q1r=float(atoi(rxdata_q1))/10000;
			Q2r=float(atoi(rxdata_q2))/10000;
			Q3r=float(atoi(rxdata_q3))/10000;

			strtemp.Format(_T("%f"),Q0r);
			m_EditReceive+=strtemp;
			strtemp=_T("\r\n");//换行
			m_EditReceive+=strtemp;
			strtemp.Format(_T(" %f"),Q1r);
			m_EditReceive+=strtemp;
			strtemp=_T("\r\n");//换行
			m_EditReceive+=strtemp;
			strtemp.Format(_T(" %f"),Q2r);
			m_EditReceive+=strtemp;
			strtemp=_T("\r\n");//换行
			m_EditReceive+=strtemp;
			strtemp.Format(_T(" %f"),Q3r);
			m_EditReceive+=strtemp;

			//---------------------------------- 重力场
			strtemp=_T("\r\n");//换行
			m_EditReceive+=strtemp;
			strtemp.Format(_T("------>>>\r\n"));
			m_EditReceive+=strtemp;

			G_ax=atoi(rxdata_ax);
			G_ay=atoi(rxdata_ay);
			G_az=atoi(rxdata_az);
			strtemp=_T("\r\n");//换行
			strtemp.Format(_T("%04d  %04d  %04d"),G_ax,G_ay,G_az);
			m_EditReceive+=strtemp;
			// 用于 Gaint Tracking
			float gaxf=float(G_ax)/1000*0.981;
			float gayf=float(G_ay)/1000*0.981;
			float gazf=float(G_az)/1000*0.981;

			//---------------------------------- 角速度
			strtemp=_T("\r\n");//换行
			m_EditReceive+=strtemp;
			strtemp.Format(_T("------>>>\r\n"));
			m_EditReceive+=strtemp;

			Rotx=float(atoi(rxdata_rx))/100;
			Roty=float(atoi(rxdata_ry))/100;
			Rotz=float(atoi(rxdata_rz))/100;

			strtemp=_T("\r\n");//换行
			m_EditReceive+=strtemp;
			strtemp.Format(_T("%f \n"),Rotx);
			m_EditReceive+=strtemp;
			strtemp=_T("\r\n");//换行
			m_EditReceive+=strtemp;
			strtemp.Format(_T("%f \n"),Roty);
			m_EditReceive+=strtemp;
			strtemp=_T("\r\n");//换行
			m_EditReceive+=strtemp;
			strtemp.Format(_T("%f \n"),Rotz);
			m_EditReceive+=strtemp;

			//----------------------------------- 磁场

			strtemp=_T("\r\n");//换行
			m_EditReceive+=strtemp;
			strtemp.Format(_T("------>>>\r\n"));
			m_EditReceive+=strtemp;

			int Compass_X_R=atoi(rxdata_mx);
			int Compass_Y_R=atoi(rxdata_my);
			int Compass_Z_R=atoi(rxdata_mz);

			
			if(abs(Compass_X_R)<500  && abs(Compass_Y_R)<500  && abs(Compass_Z_R)<500)
			{
				Compass_X=Compass_X_R;
				Compass_Y=Compass_Y_R;
				Compass_Z=Compass_Z_R;
			}

			strtemp=_T("\r\n");//换行
			m_EditReceive+=strtemp;
			strtemp.Format(_T(" %d, %d, %d"),Compass_X,Compass_Y,Compass_Z);
			m_EditReceive+=strtemp;
			
			//-----------------------------------------------------------------
			//if(abs(Compass_X)<500  && abs(Compass_Y)<500  && abs(Compass_Z)<500)
			//{
			//	FILE *file=fopen("DataCsv.csv","a+");
			//	
			//	//while(!feof(file))
			//	{
			//	fpsnum++;
			//	fprintf(file,"%d,%f,%f,%f,%f,%f,%f,%f,%f,%f\n",fpsnum,
			//		Rotx*180/PI,Roty*180/PI,Rotz*180/PI,
			//		gaxf,gayf,gazf,
			//		float(Compass_X)/1000,float(Compass_Y)/1000,float(Compass_Z)/1000
			//		);
			//	
			//	}
			//	fclose(file);
			//}
			strtemp=_T("\r\n");//换行
			m_EditReceive+=strtemp;
			strtemp.Format(_T("fps: %d"),fpsnum);
			m_EditReceive+=strtemp;
			if(Compass_Start && abs(Compass_X)<500  && abs(Compass_Y)<500  && abs(Compass_Z)<500)
			{
				FILE *file=fopen(".//NiHe//Compass.txt","a+");
				{
					//fprintf(file,"(%d,%d,%d) (%f,%f,%f) (%d,%d,%d)\n",G_ax,G_ay,G_az,Rotx,Roty,Rotz,Compass_X,Compass_Y,Compass_Z);
					fprintf(file,"(%d,%d,%d)\n",Compass_X,Compass_Y,Compass_Z);
				}
				fclose(file);
			}

			if(!Compass_Start)
			{
				strtemp=_T("\r\n");//换行
				strtemp=_T("\r\n");//换行
				m_EditReceive+=strtemp;
				strtemp.Format(_T("Non start"));
				m_EditReceive+=strtemp;
			}
			else
			{
				strtemp=_T("\r\n");//换行
				strtemp=_T("\r\n");//换行
				m_EditReceive+=strtemp;
				strtemp.Format(_T("Start!"));
				m_EditReceive+=strtemp;
			}

			
		}
			
		CString temp2;
		end_time=clock();
		timex=float(end_time-start_time)/1000;
		temp2.Format("%f",timex);C_Time=temp2;
		UpdateData(false);//显示编辑框的内容
		//LXW_control++;
		start_time=clock();
		UpdateData(FALSE);//更新编辑框内容 
		
	}
	
	flag_com=1;
	//m_mscomm.put_InputLen(0);
	
}

void CCommTest::OnBnClickedButtonClose()
{
	// TODO: 在此添加控件通知处理程序代码
	//m_mscomm.put_PortOpen(FALSE);//关闭串口
	//AfxMessageBox(_T("串口1已关闭"));
}

void CCommTest::OnBnClickedButtonSend()
{
	// TODO: 在此添加控件通知处理程序代码
#if 0
	CString str;
	str="a";
	
	CByteArray hexdata;//发送的数据	
    hexdata.Add(0x31);

	UpdateData(true);//读取编辑框内容
	m_mscomm.put_Output(COleVariant(hexdata));//发送数据
	m_EditSend.Empty();//发送后清空输入框
	UpdateData(false);//更新编辑框内容
#endif
}


void CCommTest::OnBnClickedButtonOpen()
{
	// TODO: 在此添加控件通知处理程序代码
	const short com_num[10]={1,2,3,4,5,6,7,8,9,10};
	bool com_enable[10]={0,0,0,0,0,0,0,0,0,0};
	short Com_Num_En=0;
	HANDLE M_COM;
	CString com[10]={"COM1","COM2","COM3","COM4","COM5","COM6","COM7","COM8","COM9","COM10"};
	CString str="";
	int cnt=0;
	for(int i=0;i<10;i++)
	{
		M_COM=CreateFile(com[i],GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,NULL);
		if(M_COM==INVALID_HANDLE_VALUE)// 如果没有该设备，或者被其他应用程序在用    *******************
		{ 
			//str+=com[i];// 记录下该串口名称，以备后面提示用
			//  str+=" ";

			com_enable[i]=0;

		}
		else
		{
			cnt=i+1;// 如果存在，则记录下来。这里只记录了一个，也可以采用一个数组来记录所有存在串口
			com_enable[i]=1;
			Com_Num_En=i+1;
		}
		CloseHandle(M_COM);
	}
	//------------------------------------------------------------


	if(m_mscomm.get_PortOpen())//如果串口是打开的，则行关闭串口
	{m_mscomm.put_PortOpen(FALSE);}

	m_mscomm.put_CommPort(Com_Num_En);//选择COM1
	m_mscomm.put_InBufferSize(1024);//接收缓冲区
	m_mscomm.put_OutBufferSize(1024);//发送缓冲区 
	m_mscomm.put_InputLen(0);//设置当前接收区数据长度为0,表示全部读取
	m_mscomm.put_InputMode(1);//以二进制方式读写数据
	m_mscomm.put_RThreshold(26+24);//(26+24);//26+36//接收缓冲区有1个及1个以上字符时，将引发接收数据的OnComm事件
	//m_mscomm.put_SThreshold(0);          //每发送一个字符时，不触发OnComm事件
	m_mscomm.put_Settings(_T("19200,n,8,1"));//波特率9600无检验位，8个数据位，1个停止位
	if(!m_mscomm.get_PortOpen())//如果串口没有打开则打开
	{
		m_mscomm.put_PortOpen(TRUE);//打开串口
		CString str;
		str.Format(_T("串口%d打开成功"),Com_Num_En);
		AfxMessageBox(str);

	}
	else
	{
		m_mscomm.put_OutBufferCount(0);
		AfxMessageBox(_T("串口打开失败"));
	}
}


void CCommTest::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
#if 1
	
	if(flag_com==0 )
	{
		CByteArray hexdata;//发送的数据	
		hexdata.Add(0x43);
		hexdata.Add(0xa1);
		hexdata.Add(0x0d);
		hexdata.Add(0x0a);
		m_mscomm.put_Output(COleVariant(hexdata));//发送数据
		//Sleep(6);
	}
	
	if(flag_com==1)
	{
		
		// 当不引入磁场时 需要 传感器四元数坐标系 转 OpenGL坐标系；
		//不然当有磁场矫正引入时，欧拉转四元数OlA_T_Quarter()，已经包括 传感器四元数坐标系 转 OpenGL坐标系功能。
		
		//Q0=Q0r;//传感器四元数坐标系 转 OpenGL坐标系
		//Q1=Q2r;
		//Q2=Q3r;
		//Q3=Q1r;

		//Q0=Q0r;//传感器四元数坐标系 转 OpenGL坐标系
		//Q1=Q2r;
		//Q2=Q3r;
		//Q3=Q1r;

		q0=Q0r;
		q1=Q1r;
		q2=Q2r;
		q3=Q3r;

		Quarter_T_OlA(Q0r,Q1r,Q2r,Q3r);	


	Yaw_angle_Offset0=Yaw_angle;//获取航向角矫正偏移量
	Yaw_angle-=Yaw_angle_Offset;//航向角矫正

	OlA_T_Quarter(Rool_angle,Pitch_angle,Yaw_angle);//矫正偏移后的 欧拉角转 四元数

#if 1
	//平滑
	float starting[4];
	starting[0]=q1b;
	starting[1]=q2b;
	starting[2]=q3b;
	starting[3]=q0b;

	float ending[4];
	ending[0]=q1;
	ending[1]=q2;
	ending[2]=q3;
	ending[3]=q0;
	float result[4];

	//参数越小越平滑
	slerp( result, starting, ending, 0.30999 );

	Quarter_T_OlA(result[3],result[0],result[1],result[2]);

	q0b=result[3];
	q1b=result[0];
	q2b=result[1];
	q3b=result[2];


	LXW_IMU_6B(Rool_angle,Pitch_angle,Yaw_angle);//传感器四元数坐标系 转 OpenGL坐标系
	if(abs(Rotx)>0.0065 | abs(Roty)>0.0065 | abs(Rotz)>0.0065)
	{
		Q0=q0; Q1=q1;Q2=q2;Q3=q3; 
	}
#else

	if(abs(Rotx)>0.0065 | abs(Roty)>0.0065 | abs(Rotz)>0.0065)
	{
		Q0=q0; Q1=q2;Q2=q3;Q3=q1; //传感器四元数坐标系 转 OpenGL坐标系
	}

#endif
	
			//四元数 角
		float w=acos(Q0)*2*180/PI;//---- 融合 磁偏角 q0 改为 qq0

			//// 旋转角度
			//float s=acos(Q0)*2;
			//// 之前 参数 q1  q3 -q2
			//// 之前 参数 -q2  q3 -q1
			//float x=Q1;
			//float y=Q2;
			//float z=Q3;

			////旋转 向量单位化
			//float length = sqrt( x * x + y * y + z * z );
			//x /= length;
			//y /= length;
			//z /= length;
			//
			////------ 旋转矩阵求得
			//Mccx[0]=(1-cos(s))*x*x+cos(s);
			//Mccx[1]=(1-cos(s))*x*y+sin(s)*z;
			//Mccx[2]=(1-cos(s))*x*z-sin(s)*y;
			//Mccx[3]=0;

			//Mccx[4]=(1-cos(s))*x*y-sin(s)*z;
			//Mccx[5]=(1-cos(s))*y*y+cos(s);
			//Mccx[6]=(1-cos(s))*y*z+sin(s)*x;
			//Mccx[7]=0;

			//Mccx[8]=(1-cos(s))*x*z+sin(s)*y;
			//Mccx[9]=(1-cos(s))*y*z-sin(s)*x;
			//Mccx[10]=(1-cos(s))*z*z+cos(s);
			//Mccx[11]=0;

			//Mccx[12]=0;
			//Mccx[13]=0;
			//Mccx[14]=0;
			//Mccx[15]=1;
		/****************************传输**************************/
		//memset(sendBuf,'\0',sizeof(sendBuf));// 传输 TCP 数据
		////39
		//sprintf(sendBuf, "LiID:Q4:39:%6d %6d %6d %6d\n",int(Q0*10000),int(Q1*10000),int(Q2*10000),int(Q3*10000));
		////（22+48+8）/2=39
		//send(s,sendBuf,strlen(sendBuf),0);

		
		/****************************传输**************************/


		m_openGL.RenderGLScene(w,Q0,Q1,Q2,Q3,Hx,Hy,Rool_angle,Pitch_angle, Yaw_angle,
			                   Mccx,1,pos_x_Glob,pos_y_Glob,pos_z_Glob,Yaw_angle_Offset0,Yaw_angle_Offset,
			                   Compass_Angle,0,Compass_Start,Compass_X,Compass_Y,Compass_Z,flag_connect,
							   Compass_Flag_OffsetG); //---- 融合 磁偏角 q1 q2 q3  改为 qq1 qq2 qq3
		flag_com=0;	    
		Compass_Flag_OffsetG=0;
	}

	
#endif

	CDialog::OnTimer(nIDEvent);
}


void CCommTest::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	

	SetTimer(0,7,NULL);
	//CByteArray hexdata;//发送的数据	
	//hexdata.Add(0x43);
	//hexdata.Add(0xa1);
	//hexdata.Add(0x0d);
	//hexdata.Add(0x0a);
	//m_mscomm.put_Output(COleVariant(hexdata));//发送数据
}

void CCommTest::MouseAction(unsigned char m_get)  //控制鼠标动作主程序
{
#if 0
	if (m_get==0xa1)          //左移
	{
		pt.x-=2;
		::SetCursorPos(pt.x,pt.y);
		::GetCursorPos(&pt);
	}
	else if (m_get==0xa2)    //右移
	{
		pt.x+=2;
		::SetCursorPos(pt.x,pt.y);
		::GetCursorPos(&pt);
	}
	else if (m_get==0xa3)    //上移
	{
		pt.y-=2;
		::SetCursorPos(pt.x,pt.y);
		::GetCursorPos(&pt);
	}
	else if (m_get==0xa4)    //下移
	{
		pt.y+=2;
		::SetCursorPos(pt.x,pt.y);
		::GetCursorPos(&pt);
	}
	else if (m_get==0xb1)   //左键单击
	{

		mouse_event (MOUSEEVENTF_LEFTDOWN, pt.x, pt.y, 0, 0 );
		mouse_event (MOUSEEVENTF_LEFTUP, pt.x, pt.y, 0, 0 );

		

	}
	else if (m_get==0xb2)    //右键单击
	{

		mouse_event (MOUSEEVENTF_RIGHTDOWN, pt.x, pt.y, 0, 0 );
		mouse_event (MOUSEEVENTF_RIGHTUP, pt.x, pt.y, 0, 0 );

	}
	else if (m_get==0xb4)    //鼠标双击
	{
		mouse_event (MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, pt.x, pt.y, 0, 0 );
		mouse_event (MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, pt.x, pt.y, 0, 0 );


	}
#endif
}

BOOL CCommTest::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	 CRect rect;
	GetDlgItem(IDC_OPENGL)->GetWindowRect(rect);
	ScreenToClient(rect);
	m_openGL.Create(rect, this); 

	//Socket
	/*初始化套接字*/
	//wsa;
	//WSAStartup(MAKEWORD(2,0),&wsa);
	//s = socket(AF_INET,SOCK_STREAM,0);
	//if(s == INVALID_SOCKET)
	//{
	//	printf("创建套接字失败!\n");
	//	WSACleanup();
	//	return 1;
	//}
	///*填写本地数据*/
	//struct sockaddr_in sa;
	//sa.sin_family = AF_INET;
	//sa.sin_port = htons(8080);
	//sa.sin_addr.S_un.S_addr = inet_addr("192.168.1.142");
	//int nAddrlen = sizeof(sa);

	///*等待被连接*/
	//if (connect(s,(sockaddr *)(&sa),nAddrlen) == -1)
	//{
	//	printf("连接服务器失败!\n");
	//	WSACleanup();
	//	return -1;
	//}

	//
	//memset(sendBuf,'\0',sizeof(sendBuf));
	//strcpy(sendBuf,"I'm from client\n");
	//int n = send(s,sendBuf,strlen(sendBuf),0);
	//--------------------------------------------

	//capture1 = cvCreateCameraCapture( 0 );
	//capture2 = cvCreateCameraCapture( 1 );
	//int w = 320, h = 240;
	//cvSetCaptureProperty ( capture1, CV_CAP_PROP_FRAME_WIDTH,  w );  
	//cvSetCaptureProperty ( capture1, CV_CAP_PROP_FRAME_HEIGHT, h );
	//cvSetCaptureProperty ( capture2, CV_CAP_PROP_FRAME_WIDTH,  w );  
	//cvSetCaptureProperty ( capture2, CV_CAP_PROP_FRAME_HEIGHT, h );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


//HRESULT CCommTest::accDoDefaultAction(VARIANT varChild)
//{
//	// TODO: 在此添加专用代码和/或调用基类
//
//	return CDialog::accDoDefaultAction(varChild);
//}


void CCommTest::OnBnClickedReset()
{
	// TODO: 在此添加控件通知处理程序代码
	Compass_Flag_OffsetG=1;
}
