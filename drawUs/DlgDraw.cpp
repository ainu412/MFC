// DlgDraw.cpp: 实现文件
//

#include "pch.h"
#include "drawUs.h"
#include "DlgDraw.h"
#include "afxdialogex.h"


// DlgDraw 对话框

IMPLEMENT_DYNAMIC(DlgDraw, CDialogEx)

DlgDraw::DlgDraw(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_DRAW, pParent)
	, m_nLineWidth(1)
	, m_nLineStyle(0)
	, m_nMissMe(1)
{

	//  m_nLineWidth = 0;
	//  m_nMissMe = 0;
	//  m_nLineStyle = 0;
}

DlgDraw::~DlgDraw()
{
}

void DlgDraw::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Slider(pDX, IDC_SLIDER_LINEWIDTH, m_nLineWidth);
	DDV_MinMaxInt(pDX, m_nLineWidth, 1, 30);
	//  DDX_Radio(pDX, IDC_RADIO1, m_nLineStyle);
	DDX_Radio(pDX, IDC_RADIO1, m_nLineStyle);
	DDX_Text(pDX, IDC_EDIT1, m_nMissMe);
	DDV_MinMaxInt(pDX, m_nMissMe, 1, 99);
}


BEGIN_MESSAGE_MAP(DlgDraw, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &DlgDraw::OnBnClickedButton1)
	ON_WM_HSCROLL()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_RADIO2, &DlgDraw::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &DlgDraw::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO1, &DlgDraw::OnBnClickedRadio1)
END_MESSAGE_MAP()


// DlgDraw 消息处理程序


BOOL DlgDraw::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CSliderCtrl* sli = (CSliderCtrl * )GetDlgItem(IDC_SLIDER_LINEWIDTH);
	sli->SetRange(1, 30);
	sli->SetTicFreq(1);




	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void DlgDraw::OnBnClickedButton1()
{
	CColorDialog cdlg;
	cdlg.m_cc.Flags |= CC_RGBINIT;
	cdlg.m_cc.rgbResult = m_cColref;

	if (IDOK == cdlg.DoModal()) {
		m_cColref = cdlg.m_cc.rgbResult;

		//设置新值后要求重画
		Invalidate();
	}
	
}


void DlgDraw::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	Invalidate();
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void DlgDraw::OnPaint()
{
	UpdateData();
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CDialogEx::OnPaint()
	//设置画笔
	CPen pen(m_nLineStyle, m_nLineWidth, m_cColref);
	CPen* pOldPen = dc.SelectObject(&pen);

	//确定画线位置
	CRect rect;
	GetDlgItem(IDC_SAMPLE)->GetWindowRect(&rect);
	ScreenToClient(&rect);

	dc.MoveTo(rect.left + 10, rect.top + rect.Height() / 2);
	dc.LineTo(rect.right - 10, rect.top + rect.Height() / 2);


	dc.SelectObject(pOldPen);
}


void DlgDraw::OnBnClickedRadio2()
{
	Invalidate();
}


void DlgDraw::OnBnClickedRadio3()
{
	Invalidate();
}


void DlgDraw::OnBnClickedRadio1()
{
	Invalidate();
}
