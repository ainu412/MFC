#pragma once


// DlgDraw 对话框

class DlgDraw : public CDialogEx
{
	DECLARE_DYNAMIC(DlgDraw)

public:
	DlgDraw(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~DlgDraw();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DRAW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	COLORREF m_cColref;
	int m_nLineWidth;
	int m_nLineStyle;
	int m_nMissMe;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio1();
};
