
// mfc_comView.h : Cmfc_comView ��Ľӿ�
//

#pragma once


class Cmfc_comView : public CView
{
protected: // �������л�����
	Cmfc_comView();
	DECLARE_DYNCREATE(Cmfc_comView)

// ����
public:
	Cmfc_comDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~Cmfc_comView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // mfc_comView.cpp �еĵ��԰汾
inline Cmfc_comDoc* Cmfc_comView::GetDocument() const
   { return reinterpret_cast<Cmfc_comDoc*>(m_pDocument); }
#endif

