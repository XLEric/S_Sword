#if !defined(AFX_OPENGL_H__38B5D1C8_2DFF_4A7D_9A99_3AC401C19D72__INCLUDED_)
#define AFX_OPENGL_H__38B5D1C8_2DFF_4A7D_9A99_3AC401C19D72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OpenGL.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// COpenGL window
#include <gl/glut.h>

class COpenGL : public CWnd
{
// Construction
public:
 COpenGL();


// Attributes
public:

// Operations
public:

// Overrides
 // ClassWizard generated virtual function overrides
 //{{AFX_VIRTUAL(COpenGL)
 //}}AFX_VIRTUAL

// Implementation
public:
 BOOL SetNormScreen();
 BOOL SetFullScreen(int width, int height, int depth);
 virtual void RenderGLScene(float Ww,float q0,float q1,float q2,float q3,float Hxn,float Hyn,
	                        int Rool,int Pitch, int Yaw,
							GLfloat *Mccx,bool flag_Destory,
							float &pos_x,float &pos_y ,float &pos_z,
							int &Yaw_angle_Offset0,int &Yaw_angle_Offset,int Compass_AngleN,
							bool flag_touch,bool &Compass_StartN,int CompassX,int CompassY,int CompassZ,bool flag_connect,
							bool Compass_Flag_OffsetG
							
							);
  void Create(CRect rect, CWnd *parent);
 virtual ~COpenGL();

 // Generated message map functions
protected:
 CRect m_rect;
 CWnd* m_parent;
 BOOL m_bFullScreen;
 DEVMODE m_DMsaved;
 BOOL m_bInit;
 int InitGL();
 void KillGLWindow();
 HDC m_hDC;
 HGLRC m_hRC;
 //{{AFX_MSG(COpenGL)
 afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
 afx_msg void OnPaint();
 afx_msg void OnSize(UINT nType, int cx, int cy);
 //}}AFX_MSG
 DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPENGL_H__38B5D1C8_2DFF_4A7D_9A99_3AC401C19D72__INCLUDED_)


