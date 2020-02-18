
// drawBySingleItemView.cpp: CdrawBySingleItemView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "drawBySingleItem.h"
#endif

#include "drawBySingleItemDoc.h"
#include "drawBySingleItemView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CdrawBySingleItemView

IMPLEMENT_DYNCREATE(CdrawBySingleItemView, CView)

BEGIN_MESSAGE_MAP(CdrawBySingleItemView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CdrawBySingleItemView 构造/析构

CdrawBySingleItemView::CdrawBySingleItemView() noexcept
{
	// TODO: 在此处添加构造代码

}

CdrawBySingleItemView::~CdrawBySingleItemView()
{
}

BOOL CdrawBySingleItemView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CdrawBySingleItemView 绘图

void CdrawBySingleItemView::OnDraw(CDC* /*pDC*/)
{
	CdrawBySingleItemDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}

void CdrawBySingleItemView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CdrawBySingleItemView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CdrawBySingleItemView 诊断

#ifdef _DEBUG
void CdrawBySingleItemView::AssertValid() const
{
	CView::AssertValid();
}

void CdrawBySingleItemView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CdrawBySingleItemDoc* CdrawBySingleItemView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CdrawBySingleItemDoc)));
	return (CdrawBySingleItemDoc*)m_pDocument;
}
#endif //_DEBUG


// CdrawBySingleItemView 消息处理程序


void CdrawBySingleItemView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//设置成员变量的初始值
	m_pOrigin = point;//point是依据屏幕分辨率生成的像素数的点

	CView::OnLButtonDown(nFlags, point);
}


void CdrawBySingleItemView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//画线自初始值至buttonUp取得的值
	//获得上下文
	CDC* pDC = GetDC();

	pDC->MoveTo(m_pOrigin);
	pDC->LineTo(point);

	//释放设备上下文(写获得的同时写释放!)
	ReleaseDC(pDC);

	CView::OnLButtonUp(nFlags, point);
}
