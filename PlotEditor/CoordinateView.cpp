// CoordinateView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PlotEditor.h"
#include "CoordinateView.h"


// CCoordinateView

IMPLEMENT_DYNCREATE(CCoordinateView, CView)

CCoordinateView::CCoordinateView()
{

}

CCoordinateView::~CCoordinateView()
{
}

BEGIN_MESSAGE_MAP(CCoordinateView, CView)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CCoordinateView ��ͼ

void CCoordinateView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: �ڴ���ӻ��ƴ���
}


// CCoordinateView ���

#ifdef _DEBUG
void CCoordinateView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CCoordinateView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCoordinateView ��Ϣ�������


void CCoordinateView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CView::OnLButtonDblClk(nFlags, point);
}


void CCoordinateView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CView::OnPaint()
}


void CCoordinateView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
}
