#pragma once


// LineSetup 对话框

class LineSetup : public CDialogEx
{
	DECLARE_DYNAMIC(LineSetup)

public:
	LineSetup(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~LineSetup();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_LINESETUP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 设置线宽
	UINT m_nLineWidth;
};
