#pragma once
#include <afx.h>
#include "drawUs.h"
class Graph :
	public CObject
{
public:
	Graph();
	Graph(DRAW_TYPE m_eDrawType, COLORREF m_cColref,
	int m_nLineWidth,
	int m_nLineStyle,
	int m_nMissMe);
	void addPoint(CPoint point);
	void addPoint(CPoint origin, CPoint end);
	void draw(CDC* pDC);
	DECLARE_SERIAL(Graph);
protected:
	//只存点
	CArray<CPoint> m_pPoints;
	//初始化时设定笔型
	DRAW_TYPE m_eDrawType;
	COLORREF m_cColref;
	int m_nLineWidth;
	int m_nLineStyle;
	int m_nMissMe;
public:
	virtual void Serialize(CArchive& ar);
};

