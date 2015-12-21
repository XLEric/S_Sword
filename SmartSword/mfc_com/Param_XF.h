#pragma once
#ifndef __PARAM_XF_H__
#define __PARAM_XF_H__
#include"iostream"
#include <gl/glut.h>
#include <windows.h>
#include <opencv2/opencv.hpp>
#include "CvvImage.h"
using namespace cv;
using namespace std;
#define PI 3.1415926

float pos_x_Glob=0;
float pos_y_Glob=0;
float pos_z_Glob=-240;

int Yaw_angle_Offset=0;
int Yaw_angle_Offset0=0;

//´Å³¡º½Ïò½Ç¶È
int Compass_Angle=0;
#endif