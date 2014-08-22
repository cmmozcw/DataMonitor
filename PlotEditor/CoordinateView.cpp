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
	DrawCoordinateSystem(pDC);
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
	OnPrepareDC(&dc);
	OnDraw(&dc);
}


void CCoordinateView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
}
void CCoordinateView::DrawCoordinateSystem(CDC* pDC)
{
	TRACE(_T("DrawCoordinateSystem"));
	CRect rect;
	GetClientRect(rect);
	UpdateData(FALSE);
	CString str;
	CFont font;
	VERIFY(font.CreateFont(
		14,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Arial")));                 // lpszFacename

	int i = 0;
	int iDrawType = PS_SOLID;
	COLORREF colorR = RGB(127,127,127);
	int count = theApp.curveList.GetCount();
	if(count > 0)
	{
		CPen pen(iDrawType, 1, colorR); //����
		CPen* pOldPen = pDC->SelectObject(&pen);//���ʺͻ���������
		POSITION pos = theApp.curveList.GetHeadPosition();
		while(pos)
		{
			CCurveInfo* plist = theApp.curveList.GetNext(pos);
			COLORREF colorR = plist->curveColor;
			iDrawType = plist->lineType;
			CPen pen(iDrawType, 1, colorR); //����
			pDC->SelectObject(&pen);//���ʺͻ���������
			
			pDC->MoveTo(0, 30*i+15);
			pDC->LineTo(rect.Width(), 30*i+15);

			pDC->SelectObject(&font);
			pDC->SetTextColor(colorR);   //�ı�������ɫΪ��ɫ
            pDC->TextOut(rect.Width()/2,30*i,plist->strCurveName);   //����ı�ֵ

			str.Format(_T("%d"),plist->leftLimit);
			pDC->TextOut(1,30*i+16,str);   //����ı�ֵ
			str.Format(_T("%d"),plist->rightLimit);
			pDC->TextOut(rect.Width()-30,30*i+16,str);   //����ı�ֵ
			i++;
		}
		//����ˢ��
		pDC->SelectObject(pOldPen);  //[���Բ���Ҫ]
	}
}
