
// mfc_comView.cpp : Cmfc_comView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &Cmfc_comView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// Cmfc_comView 构造/析构

Cmfc_comView::Cmfc_comView()
{
	// TODO: 在此处添加构造代码

}

Cmfc_comView::~Cmfc_comView()
{
}

BOOL Cmfc_comView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// Cmfc_comView 绘制

void Cmfc_comView::OnDraw(CDC* /*pDC*/)
{
	Cmfc_comDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// Cmfc_comView 打印


void Cmfc_comView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL Cmfc_comView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void Cmfc_comView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void Cmfc_comView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
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


// Cmfc_comView 诊断

#ifdef _DEBUG
void Cmfc_comView::AssertValid() const
{
	CView::AssertValid();
}

void Cmfc_comView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Cmfc_comDoc* Cmfc_comView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cmfc_comDoc)));
	return (Cmfc_comDoc*)m_pDocument;
}
#endif //_DEBUG


// Cmfc_comView 消息处理程序
