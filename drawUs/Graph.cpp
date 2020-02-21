#include "pch.h"
#include "Graph.h"

IMPLEMENT_SERIAL(Graph, CObject, 1);

Graph::Graph()
{
}

Graph::Graph(DRAW_TYPE m_eDrawType, COLORREF m_cColref, int m_nLineWidth, int m_nLineStyle, int m_nMissMe):
	m_eDrawType(m_eDrawType), m_cColref(m_cColref), m_nLineWidth(m_nLineWidth), m_nLineStyle(m_nLineStyle),
	m_nMissMe(m_nMissMe)
{
}

void Graph::addPoint(CPoint point)
{
	m_pPoints.Add(point);
}

void Graph::addPoint(CPoint origin, CPoint end)
{
	m_pPoints.Add(origin);
	m_pPoints.Add(end);
}

void Graph::draw(CDC* pDC)
{
	//设置画笔
	CPen pen(m_nLineStyle, m_nLineWidth, m_cColref);
	CPen* pOldPen = pDC->SelectObject(&pen);

	//如果不为画笔
	if (m_eDrawType != DRAW_TYPE::DT_PEN)
	{

		//设置透明画刷
		CBrush* brush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
		pDC->SelectObject(brush);

		switch (m_eDrawType)
		{
		case DRAW_TYPE::DT_LINE:
			pDC->MoveTo(m_pPoints.GetAt(0));
			pDC->LineTo(m_pPoints.GetAt(1));
			break;
		case DRAW_TYPE::DT_ELLIPSE:
			pDC->Ellipse(CRect(m_pPoints.GetAt(0), m_pPoints.GetAt(1)));
			break;
		case DRAW_TYPE::DT_RECT:
			pDC->Rectangle(CRect(m_pPoints.GetAt(0), m_pPoints.GetAt(1)));
			break;
		default:
			break;
		}

	}
	//若为画笔,画两点间连线
	else
	{
		for (int i = 0; i < m_pPoints.GetSize();)
		{
			pDC->MoveTo(m_pPoints.GetAt(i));

			if (++i < m_pPoints.GetSize())
			{
				pDC->LineTo(m_pPoints.GetAt(i));
			}
		}
	}

	pDC->SelectObject(pOldPen);
}


void Graph::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	
		ar  << int(m_eDrawType) << m_cColref
			<< m_nLineWidth	<< m_nLineStyle
			<< m_nMissMe;
	}
	else
	{	
		int tmp;
		ar  >> tmp >> m_cColref
			>> m_nLineWidth >> m_nLineStyle
			>> m_nMissMe;
		m_eDrawType = (DRAW_TYPE)tmp;
	}
	m_pPoints.Serialize(ar);
}
