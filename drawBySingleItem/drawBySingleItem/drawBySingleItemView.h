
// drawBySingleItemView.h: CdrawBySingleItemView 类的接口
//

#pragma once

#define TIMER_TEXT 9122

class CdrawBySingleItemView : public CView
{
protected: // 仅从序列化创建
	CdrawBySingleItemView() noexcept;
	DECLARE_DYNCREATE(CdrawBySingleItemView)

// 特性
public:
	CdrawBySingleItemDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CdrawBySingleItemView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
protected:
	CPoint m_pOrigin;
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
protected:
	bool m_bDraw = false;
public:
	int m_nWidth;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

#ifndef _DEBUG  // drawBySingleItemView.cpp 中的调试版本
inline CdrawBySingleItemDoc* CdrawBySingleItemView::GetDocument() const
   { return reinterpret_cast<CdrawBySingleItemDoc*>(m_pDocument); }
#endif

