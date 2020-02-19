// LineSetup.cpp: 实现文件
//

#include "pch.h"
#include "drawBySingleItem.h"
#include "LineSetup.h"
#include "afxdialogex.h"


// LineSetup 对话框

IMPLEMENT_DYNAMIC(LineSetup, CDialogEx)

LineSetup::LineSetup(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_LINESETUP, pParent)
	, m_nLineWidth(0)
{

}

LineSetup::~LineSetup()
{
}

void LineSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LINEWIDTH, m_nLineWidth);
}


BEGIN_MESSAGE_MAP(LineSetup, CDialogEx)
END_MESSAGE_MAP()


// LineSetup 消息处理程序
