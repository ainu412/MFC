
// chatSrvDlg.h: 头文件
//

#pragma once
#include "CSockSrv.h"
#include "CSockChat.h"
class CSockSrv;
class CSockChat;
// CchatSrvDlg 对话框
class CchatSrvDlg : public CDialogEx
{
// 构造
public:
	CchatSrvDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHATSRV_DIALOG };
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
	afx_msg void OnBnClickedStartBtn();
	CListBox m_msgListBox;
	CTime m_time;
	CSockChat* m_sockChat;
	CSockSrv* m_sockSrv;
};
