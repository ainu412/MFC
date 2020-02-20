#pragma once
#include <afx.h>
#include "drawUs.h"
class Graph :
	public CObject
{
public:
	Graph(DRAW_TYPE m_eDrawType, COLORREF m_cColref,
	int m_nLineWidth,
	int m_nLineStyle,
	int m_nMissMe);
	void addPoint(CPoint point);
	void addPoint(CPoint origin, CPoint end);
	void draw(CDC* pDC);
protected:
	//ֻ���
	CArray<CPoint> m_pPoints;
	//��ʼ��ʱ�趨����
	DRAW_TYPE m_eDrawType;
	COLORREF m_cColref;
	int m_nLineWidth;
	int m_nLineStyle;
	int m_nMissMe;
};

