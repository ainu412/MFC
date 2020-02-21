
// drawUsView.cpp: CdrawUsView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "drawUs.h"
#endif

#include "drawUsDoc.h"
#include "drawUsView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "DlgDraw.h"


// CdrawUsView

IMPLEMENT_DYNCREATE(CdrawUsView, CView)

BEGIN_MESSAGE_MAP(CdrawUsView, CView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_DRAW_ELLIPSE, &CdrawUsView::OnDrawEllipse)
	ON_COMMAND(ID_DRAW_LINE, &CdrawUsView::OnDrawLine)
	ON_COMMAND(ID_DRAW_PEN, &CdrawUsView::OnDrawPen)
	ON_COMMAND(ID_DRAW_RECT, &CdrawUsView::OnDrawRect)
	ON_WM_NCRBUTTONUP()
	ON_UPDATE_COMMAND_UI(ID_DRAW_ELLIPSE, &CdrawUsView::OnUpdateDrawEllipse)
	ON_UPDATE_COMMAND_UI(ID_DRAW_LINE, &CdrawUsView::OnUpdateDrawLine)
	ON_UPDATE_COMMAND_UI(ID_DRAW_PEN, &CdrawUsView::OnUpdateDrawPen)
	ON_UPDATE_COMMAND_UI(ID_DRAW_RECT, &CdrawUsView::OnUpdateDrawRect)
	ON_COMMAND(ID_SUPERPOWER_FLY, &CdrawUsView::OnSuperpowerFly)
	ON_UPDATE_COMMAND_UI(ID_SUPERPOWER_FLY, &CdrawUsView::OnUpdateSuperpowerFly)
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_PEN_SETUP, &CdrawUsView::OnPenSetup)
END_MESSAGE_MAP()

// CdrawUsView 构造/析构

CdrawUsView::CdrawUsView() noexcept
{
	// TODO: 在此处添加构造代码

}

CdrawUsView::~CdrawUsView()
{
	int cnt = m_pGraphs.GetSize();
	while (cnt--) {
		delete m_pGraphs.GetAt(cnt);
	}
	m_pGraphs.RemoveAll();
}

BOOL CdrawUsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	//m_toolbar.LoadToolBar(IDR_TOOLBAR_DRAW);

	m_DrawPopupMenu.LoadMenu(IDR_MENU_POPUP);//?
	return CView::PreCreateWindow(cs);
}

// CdrawUsView 绘图

void CdrawUsView::OnDraw(CDC* pDC)
{
	CdrawUsDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	// 实现重绘
	for (int i = 0; i < m_pGraphs.GetSize(); i++)
	{
		((Graph*)m_pGraphs.GetAt(i))->draw(pDC);
	}
}


// CdrawUsView 诊断

#ifdef _DEBUG
void CdrawUsView::AssertValid() const
{
	CView::AssertValid();
}

void CdrawUsView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CdrawUsDoc* CdrawUsView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CdrawUsDoc)));
	return (CdrawUsDoc*)m_pDocument;
}
#endif //_DEBUG


// CdrawUsView 消息处理程序


void CdrawUsView::OnLButtonDown(UINT nFlags, CPoint point)
{
	//记录起始点,并开启绘画
	m_pOrigin = point;

	if (DRAW_TYPE::DT_PEN == m_eDrawType) {
		m_bDraw = true;
		m_pTmpGraph = new Graph(m_eDrawType, m_cColref,
				m_nLineWidth, m_nLineStyle, m_nMissMe);
	}

	CView::OnLButtonDown(nFlags, point);
}


void CdrawUsView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_eDrawType!= DRAW_TYPE::DT_PEN)
	{
		//设备上下文就位
		CClientDC dc(this);
		//设置画笔
		CPen pen(m_nLineStyle, m_nLineWidth, m_cColref);
		CPen* pOldPen = dc.SelectObject(&pen);

		//设置透明画刷
		CBrush* brush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
		dc.SelectObject(brush);
		//根据不同ID切换绘画类型
		switch (m_eDrawType)
		{
		case DRAW_TYPE::DT_LINE:
			dc.MoveTo(m_pOrigin);
			dc.LineTo(point);
			break;
		case DRAW_TYPE::DT_ELLIPSE:
			dc.Ellipse(CRect(m_pOrigin, point));
			break;
		case DRAW_TYPE::DT_RECT:
			dc.Rectangle(CRect(m_pOrigin, point));
			break;
		default:
			break;
		}

		dc.SelectObject(pOldPen);

		Graph* gh = new Graph(m_eDrawType, m_cColref,
			m_nLineWidth, m_nLineStyle, m_nMissMe);
		gh->addPoint(m_pOrigin, point);
		m_pGraphs.Add(gh);
	}

	if (m_bDraw && m_eDrawType == DRAW_TYPE::DT_PEN)
	{
		//因鼠标抬起点和鼠标移动点都要存入一个graph中,块中临时变量仅能在块内存活,故创建临时graph
		//将最后一点存入临时graph(一次鼠标按抬形成的图形)
		m_pTmpGraph->addPoint(point);
		//将这一点存入graph*数组
		m_pGraphs.Add(m_pTmpGraph);
	}

	m_bDraw = false;
	CView::OnLButtonUp(nFlags, point);
}


void CdrawUsView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bDraw && DRAW_TYPE::DT_PEN == m_eDrawType)
	{
		//存起始点
		m_pTmpGraph->addPoint(m_pOrigin);

		//设备上下文就位
		CClientDC dc(this);

		//选择设置好的画笔
		CPen pen(m_nLineStyle, m_nLineWidth, m_cColref);
		CPen* pOldPen = dc.SelectObject(&pen);
		//连续画线
		dc.MoveTo(m_pOrigin);
		dc.LineTo(point);

		//存移动到的新的点
		m_pTmpGraph->addPoint(point);
		//将前一点设为当前点
		m_pOrigin = point;
		//返回原笔
		dc.SelectObject(pOldPen);
	}
	CView::OnMouseMove(nFlags, point);
}

//ID响应直接写在view中
//直接保存在view中以便绘图
void CdrawUsView::OnDrawEllipse()
{
	m_eDrawType = DRAW_TYPE::DT_ELLIPSE;
}


void CdrawUsView::OnDrawLine()
{
	m_eDrawType = DRAW_TYPE::DT_LINE;
}


void CdrawUsView::OnDrawPen()
{
	m_eDrawType = DRAW_TYPE::DT_PEN;
}


void CdrawUsView::OnDrawRect()
{
	m_eDrawType = DRAW_TYPE::DT_RECT;
}


void CdrawUsView::OnUpdateDrawEllipse(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_eDrawType == DRAW_TYPE::DT_ELLIPSE);
}


void CdrawUsView::OnUpdateDrawLine(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_eDrawType == DRAW_TYPE::DT_LINE);
}


void CdrawUsView::OnUpdateDrawPen(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_eDrawType == DRAW_TYPE::DT_PEN);
}


void CdrawUsView::OnUpdateDrawRect(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_eDrawType == DRAW_TYPE::DT_RECT);
}

//点过一次后禁用
void CdrawUsView::OnSuperpowerFly()
{
	MessageBox(_T("You can't fly!"));
	m_bValidate = false;
}

void CdrawUsView::OnUpdateSuperpowerFly(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bValidate);
}


void CdrawUsView::OnRButtonUp(UINT nFlags, CPoint point)
{
	CMenu* pPop = m_DrawPopupMenu.GetSubMenu(0);
	ClientToScreen(&point);

	//跟踪鼠标显示popup menu
	pPop->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);

	CView::OnRButtonUp(nFlags, point);
}


void CdrawUsView::OnPenSetup()
{
	DlgDraw conf;
	conf.m_nLineWidth = m_nLineWidth;
	conf.m_nLineStyle = m_nLineStyle;
	conf.m_nMissMe = m_nMissMe;
	conf.m_cColref = m_cColref;

	if (IDOK == conf.DoModal())
	{
		m_nLineWidth = conf.m_nLineWidth;
		m_nLineStyle = conf.m_nLineStyle;
		m_nMissMe = conf.m_nMissMe;
		m_cColref = conf.m_cColref;
	}
}
