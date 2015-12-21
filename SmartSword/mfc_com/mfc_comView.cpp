
// mfc_comView.cpp : Cmfc_comView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "mfc_com.h"
#endif

#include "mfc_comDoc.h"
#include "mfc_comView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cmfc_comView

IMPLEMENT_DYNCREATE(Cmfc_comView, CView)

BEGIN_MESSAGE_MAP(Cmfc_comView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &Cmfc_comView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// Cmfc_comView ����/����

Cmfc_comView::Cmfc_comView()
{
	// TODO: �ڴ˴���ӹ������

}

Cmfc_comView::~Cmfc_comView()
{
}

BOOL Cmfc_comView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// Cmfc_comView ����

void Cmfc_comView::OnDraw(CDC* /*pDC*/)
{
	Cmfc_comDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// Cmfc_comView ��ӡ


void Cmfc_comView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL Cmfc_comView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void Cmfc_comView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void Cmfc_comView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void Cmfc_comView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void Cmfc_comView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// Cmfc_comView ���

#ifdef _DEBUG
void Cmfc_comView::AssertValid() const
{
	CView::AssertValid();
}

void Cmfc_comView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Cmfc_comDoc* Cmfc_comView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cmfc_comDoc)));
	return (Cmfc_comDoc*)m_pDocument;
}
#endif //_DEBUG


// Cmfc_comView ��Ϣ�������
