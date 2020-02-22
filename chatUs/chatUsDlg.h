
// chatUsDlg.h: 头文件
//

#pragma once
#include <atlconv.h>
#include "CMySocket.h"

// CchatUsDlg 对话框
class CchatUsDlg : public CDialogEx
{
// 构造
public:
	CchatUsDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHATUS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedConnectBtn();
	afx_msg void OnBnClickedDisconnectBtn();
	CMySocket* m_sockCli;
	CListBox m_msgListBox;
	afx_msg void OnBnClickedSendBtn();
	afx_msg void OnBnClickedNameBtn();
	CString m_name;
	afx_msg void OnBnClickedAutoresponseRadio();
	afx_msg void OnBnClickedClsBtn();
	CComboBox m_fontColorCombo;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
