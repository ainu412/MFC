
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
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CdrawBySingleItemView 构造/析构

CdrawBySingleItemView::CdrawBySingleItemView() noexcept
{
	// TODO: 在此处添加构造代码

	m_nWidth = 0;
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
	//m_pOrigin = point;//point是依据屏幕分辨率生成的像素数的点

	//按下就缺点圆心
	//m_bDraw = true;

	//按下切换画与不画
	//m_bDraw=!m_bDraw;

	//按住时画
	//m_bDraw = true;
	
	CView::OnLButtonDown(nFlags, point);
}


void CdrawBySingleItemView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	/*//获得上下文
	CDC* pDC = GetDC();
	
	//设置画笔
	CPen pen(PS_DASHDOT, 1, RGB(0, 150, 0));
	CPen* oldPen = pDC->SelectObject(&pen);

	//画线
	pDC->MoveTo(m_pOrigin);
	pDC->LineTo(point);

	//恢复原画笔
	pDC->SelectObject(&oldPen);
	//释放设备上下文(写获得的同时写释放!)
	ReleaseDC(pDC);*/

	//法二:CClient
	//CClientDC dc(this);

	//默认黑框,默认白底
	/*dc.Rectangle(CRect(m_pOrigin, point));*/

	//绿底框
	/*CBrush brush(RGB(0, 255, 0));
	dc.FillRect(CRect(m_pOrigin, point), &brush);*/

	//黑框,透明底
	//CBrush* pHollowBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	////dc.FillRect(CRect(m_pOrigin, point), pHollowBrush);
	//CBrush* oldBrush = dc.SelectObject(pHollowBrush);//select后记得释放,而fillRect只是画的时候调用了一次
	//dc.Rectangle(CRect(m_pOrigin, point));//可直接画框,画笔默认1像素黑色直线
	//dc.SelectObject(oldBrush);

	//bitmap图像格式填充画刷
	//CBitmap bitmap;
	////从项目资源中加载bitmap资源
	//bitmap.LoadBitmap(IDB_BITMAP2);//红线不管...
	//CBrush bitmapBrush(&bitmap);
	//dc.FillRect(CRect(m_pOrigin, point), &bitmapBrush);//默认无框

	//画圆:两点x坐标差值/距离
	//CClientDC dc(this);
	//dc.Ellipse(m_pOrigin.x, m_pOrigin.y, point.x, m_pOrigin.y + point.x - m_pOrigin.x);


	//画椭圆
	//CClientDC dc(this);
	//CPen pen(PS_DASH, 1, RGB(255, 0, 70));
	//CPen* oldPen = dc.SelectObject(&pen);
	//dc.Ellipse(CRect(m_pOrigin, point));
	//dc.SelectObject(oldPen);
	//松开的最后,设置不画
	//m_bDraw = false;
	CView::OnLButtonUp(nFlags, point);
}


void CdrawBySingleItemView::OnMouseMove(UINT nFlags, CPoint point)
{
	//CClientDC dc(this);

	////切换画笔
	//CPen pen(PS_SOLID, 4, RGB(100,0,0));
	//
	////一旦selectObject就要记得恢复
	//CPen* oldPen = dc.SelectObject(&pen);
	////跟随鼠标移动轨迹画线
	////问题:会在m_pOrgin设置的初始值(默认左上角)起点,一闪而过
	////新添变量m_bDraw
	//if (m_bDraw) {
	//	dc.MoveTo(m_pOrigin);
	//	dc.LineTo(point);

	//	//m_pOrigin = point;
	//}

	//dc.SelectObject(oldPen);
	//CView::OnMouseMove(nFlags, point);

}


void CdrawBySingleItemView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CClientDC dc(this);
	//文本:
	//设置字体
	CFont font;
	font.CreatePointFont(300, _T("Algerian"));//英文也需要转码...
	//采用字体
	CFont* oldFont = dc.SelectObject(&font);
	CString str(_T("is it love? "));
	//设置字体颜色
	dc.SetTextColor(RGB(255, 0, 0));
	//得到字体高度宽度
	CSize cz = dc.GetTextExtent(str);

	//文本输出:
	m_nWidth += 20;
	//输出字符
	CRect rect;
	rect.top = 100;
	rect.left = 200;
	rect.right = rect.left + m_nWidth;
	rect.bottom = rect.top + cz.cy;
	//dc.TextOut(point.x, point.y, str, str.GetLength() - 1);//不设定默认原长输出


	//屏幕动态打字效果:
	//wm_create: 程序创建的同时产生
	//设置定时器
	//ontimer消息, 设置闹钟触发事件

	switch (nIDEvent)
	{
	case TIMER_TEXT:
		//绘制文本:
		//在指定区域绘制文本
		dc.DrawText(str, rect, DT_LEFT);
		break;
	default:
		break;
	}

	if (m_nWidth > cz.cx) {
		m_nWidth = 0;
		Invalidate();
	}

	//释放原字符
	dc.SelectObject(oldFont);

	CView::OnTimer(nIDEvent);
}


int CdrawBySingleItemView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	SetTimer(TIMER_TEXT, 200, NULL);

	return 0;
}
