
// drawUsView.h: CdrawUsView 类的接口
//

#pragma once

#include"Graph.h"

class CdrawUsView : public CView
{
protected: // 仅从序列化创建
	CdrawUsView() noexcept;
	DECLARE_DYNCREATE(CdrawUsView)

// 特性
public:
	CdrawUsDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CdrawUsView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
	bool m_bDraw = false;
	DRAW_TYPE m_eDrawType;
	CPoint m_pOrigin;
	bool m_bValidate = true;
	CMenu m_DrawPopupMenu;

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
protected:
	//画图所用右键上下文菜单
	CMenu m_menu;
	COLORREF m_cColref = RGB(0, 0, 0);//TODO初始化
	int m_nLineWidth = 1;
	int m_nLineStyle = 0;
	int m_nMissMe = 1;
	//	CToolBar m_toolbar;
public:
	afx_msg void OnDrawEllipse();
	afx_msg void OnDrawLine();
	afx_msg void OnDrawPen();
	afx_msg void OnDrawRect();
	afx_msg void OnUpdateDrawEllipse(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDrawLine(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDrawPen(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDrawRect(CCmdUI* pCmdUI);
	afx_msg void OnSuperpowerFly();
	afx_msg void OnUpdateSuperpowerFly(CCmdUI* pCmdUI);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPenSetup();
protected:
	//存有一次鼠标按抬形成的图形
	Graph* m_pTmpGraph;
public:
	//存有所有图形的数组
	//CArray<Graph*> m_pGraphs;//存储类对象指针,节约空间;doc中序列化需访问
	CObArray m_pGraphs;
};

#ifndef _DEBUG  // drawUsView.cpp 中的调试版本
inline CdrawUsDoc* CdrawUsView::GetDocument() const
   { return reinterpret_cast<CdrawUsDoc*>(m_pDocument); }
#endif

