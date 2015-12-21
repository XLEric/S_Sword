#pragma once
#include "mscomm1.h"
#include "OpenGL.h"

// CCommTest 对话框

class CCommTest : public CDialog
{
	DECLARE_DYNAMIC(CCommTest)

public:
	CCommTest(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCommTest();
	COpenGL m_openGL;
// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CMscomm1 m_mscomm;
	CString m_EditSend;
	CString m_EditReceive;
	DECLARE_EVENTSINK_MAP()
	void OnCommMscomm1();
	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton1();

	POINT pt;
	void MouseAction(unsigned char m_get);
	virtual BOOL OnInitDialog();
	CString C_Time;
//	virtual HRESULT accDoDefaultAction(VARIANT varChild);
	afx_msg void OnBnClickedReset();
};
