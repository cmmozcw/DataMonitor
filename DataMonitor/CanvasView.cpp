// CanvasView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataMonitor.h"
#include "CanvasView.h"


// CCanvasView

IMPLEMENT_DYNCREATE(CCanvasView, CView)

CCanvasView::CCanvasView()
{

}

CCanvasView::~CCanvasView()
{
}

BEGIN_MESSAGE_MAP(CCanvasView, CView)
END_MESSAGE_MAP()


// CCanvasView ��ͼ

void CCanvasView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: �ڴ���ӻ��ƴ���
}


// CCanvasView ���

#ifdef _DEBUG
void CCanvasView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CCanvasView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCanvasView ��Ϣ�������
