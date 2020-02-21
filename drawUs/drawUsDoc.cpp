﻿
// drawUsDoc.cpp: CdrawUsDoc 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "drawUs.h"
#endif

#include "drawUsDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "drawUsView.h"

// CdrawUsDoc

IMPLEMENT_DYNCREATE(CdrawUsDoc, CDocument)

BEGIN_MESSAGE_MAP(CdrawUsDoc, CDocument)
END_MESSAGE_MAP()


// CdrawUsDoc 构造/析构

CdrawUsDoc::CdrawUsDoc() noexcept
{
	// TODO: 在此添加一次性构造代码

}

CdrawUsDoc::~CdrawUsDoc()
{
}

BOOL CdrawUsDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CdrawUsDoc 序列化

void CdrawUsDoc::Serialize(CArchive& ar)
{
	POSITION pos = GetFirstViewPosition();
	CdrawUsView* pView = (CdrawUsView*)GetNextView(pos);
	//int cnt;
	//if (ar.IsStoring())
	//{
	//	cnt = pView->m_pGraphs.GetSize();
	//	ar << cnt;
	//	for (size_t i = 0; i < cnt; i++)
	//	{
	//		ar << pView->m_pGraphs.GetAt(i);
	//	}
	//}
	//else
	//{
	//	ar >> cnt;
	//	for (size_t i = 0; i < cnt; i++)
	//	{
	//		//依次读取数据，加入pView->m_pGraphs数组中
	//		Graph* gh;
	//		ar >> gh;
	//		pView->m_pGraphs.Add(gh);
	//	}
	//}
	pView->m_pGraphs.Serialize(ar);
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CdrawUsDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CdrawUsDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:     strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CdrawUsDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CdrawUsDoc 诊断

#ifdef _DEBUG
void CdrawUsDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CdrawUsDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CdrawUsDoc 命令
