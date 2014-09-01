
// DMonitorView.cpp : CDataMonitorView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "DataMonitor.h"
#endif

#include "DataMonitorDoc.h"
#include "DataMonitorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDataMonitorView

IMPLEMENT_DYNCREATE(CDataMonitorView, CScrollView)

BEGIN_MESSAGE_MAP(CDataMonitorView, CScrollView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_TIMER()
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CDataMonitorView ����/����

CDataMonitorView::CDataMonitorView()
{
	// TODO: �ڴ˴���ӹ������
	tempArray[0] = 0;
	tempArray[1] = 20000;

	gmArray[0] = 0;
	gmArray[1] = 20000;

	base = 0;
	bias = 0;
	pPData = NULL;
	pOldPData = NULL;
}

CDataMonitorView::~CDataMonitorView()
{
}

BOOL CDataMonitorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ
	
	return CScrollView::PreCreateWindow(cs);
}

// CDataMonitorView ����

void CDataMonitorView::OnDraw(CDC* pDC)
{
	CDataMonitorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
	if(theApp.processType == REALTIME_PROCESSING)
	{
		DrawData(pDC);
	}
	else if(theApp.processType == FILEDATA_PROCESSING)
	{
		DrawData(pDC);
	}
	else
	{
		//nothing to do
	}
}


// CDataMonitorView ��ӡ

BOOL CDataMonitorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CDataMonitorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CDataMonitorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CDataMonitorView ���

#ifdef _DEBUG
void CDataMonitorView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CDataMonitorView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CDataMonitorDoc* CDataMonitorView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDataMonitorDoc)));
	return (CDataMonitorDoc*)m_pDocument;
}
#endif //_DEBUG


// CDataMonitorView ��Ϣ�������


void CDataMonitorView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CScrollView::OnPaint()
	OnPrepareDC(&dc);
	OnDraw(&dc);
}
void CDataMonitorView::DrawData(CDC* pDC)
{
	TRACE(_T("DrawCoordinateSystem\r\n"));
	GetClientRect(rect);
	UpdateData(FALSE);
	rectTotal = rect;
	DrawCoordinateSystem(pDC);
	DrawDataArray(pDC);
	
}
void CDataMonitorView::DrawDataArray(CDC* pDC)
{
	int dept =0,temp = 0,olddept =0,oldtemp = 0;
	int oldgr = 0,gr = 0;
	int x1=0,y1=0,dx1=0,dy1=0,dgry1=0;
	int x2=0,y2=0,dx2=0,dy2=0,dgry2=0;
	int iDrawType = PS_SOLID;
	COLORREF color = RGB(255,0,0);
	COLORREF colorBlue = RGB(0,0,255);
	CPen pen(iDrawType, 1,color); //����
	CPen pen2(iDrawType, 1,colorBlue); //����
	CPen* pOldPen = pDC->SelectObject(&pen);//���ʺͻ���������
	int gap = 20, gap1 = 300, gap2 = 350;
	if(pPData)
	{
		dept = pPData->dept.integer;
		dx1=pPData->dept.integer;
		dx2=pPData->dept.decimal;
		dx1-=base;

		if(pOldPData != NULL)
		{
			pDC->SelectObject(&pen);
			x1=pOldPData->dept.integer;
			x2=pOldPData->dept.decimal;
			x1-=base;

			y1=pOldPData->temp.integer;
			y2=pOldPData->temp.decimal;

			int iTemp = (int)(x2*20/10);
			int iTemp2 = (int)(pOldPData->temp.integer*gap1/(tempArray[1]-tempArray[0])+pOldPData->temp.decimal*gap1/(10*(tempArray[1]-tempArray[0])));

			int iTemp3 = (int)(dx2*20/10);
			int iTemp4 = (int)(pPData->temp.integer*gap1/(tempArray[1]-tempArray[0])+pPData->temp.decimal*gap1/(10*(tempArray[1]-tempArray[0])));

			pDC->MoveTo(iTemp2,x1+iTemp); // [0,0]
			pDC->LineTo(iTemp4,dx1+iTemp3);
			//gr
#if 0
			pDC->SelectObject(&pen2);

			oldgr = pOldPData->gr;
			if(oldgr - 0 > 0.01 && gr - 0 > 0.01 )
			{
				y1=(int)oldgr;    
				y2=oldgr-(float)y1;

				iTemp = (int)(x2*20/10);
				iTemp2 = (int)(oldgr*gap1/(gmArray[1]-gmArray[0]));

				iTemp3 = (int)(dx2*20/10);
				iTemp4 = (int)(gr*gap1/(gmArray[1]-gmArray[0]));

				pDC->MoveTo(x1+iTemp,iTemp2); // [0,0]
				pDC->LineTo(dx1+iTemp3,iTemp4);
			}
#endif
		}
		
		pOldPData = pPData;
	}

#if 0
	POSITION pos = theApp.petroList.GetHeadPosition();

	while(pos)
	{
		pOldPData = pPData;
		pPData = theApp.petroList.GetNext(pos);
		dept = pPData->dept;
		dx1=(int)dept;    
		dx2=dept-(float)dx1;
		dx1-=theApp.nStartCoordinate;

		temp = pPData->temp;
		//dy1=(int)temp;    
		//dy2=temp-(float)dy1;

		gr = pPData->gr;
		if(pOldPData != NULL)
		{
			pDC->SelectObject(&pen);
			olddept = pOldPData->dept;
			x1=(int)olddept;    
			x2=olddept-(float)x1;
			x1-=theApp.nStartCoordinate;

			oldtemp = pOldPData->temp;
			y1=(int)oldtemp;    
			y2=oldtemp-(float)y1;

			int iTemp = (int)(x2*20/10);
			int iTemp2 = (int)(oldtemp*gap1/(tempArray[1]-tempArray[0]));

			int iTemp3 = (int)(dx2*20/10);
			int iTemp4 = (int)(temp*gap1/(tempArray[1]-tempArray[0]));

			pDC->MoveTo(x1+iTemp,iTemp2); // [0,0]
			pDC->LineTo(dx1+iTemp3,iTemp4);
			//gr
			pDC->SelectObject(&pen2);
			oldgr = pOldPData->gr;
			if(oldgr - 0 > 0.01 && gr - 0 > 0.01 )
			{
				y1=(int)oldgr;    
				y2=oldgr-(float)y1;

				iTemp = (int)(x2*20/10);
				iTemp2 = (int)(oldgr*gap1/(gmArray[1]-gmArray[0]));

				iTemp3 = (int)(dx2*20/10);
				iTemp4 = (int)(gr*gap1/(gmArray[1]-gmArray[0]));

				pDC->MoveTo(x1+iTemp,iTemp2); // [0,0]
				pDC->LineTo(dx1+iTemp3,iTemp4);
			}
		}
	}
#endif
	pDC->SelectObject(pOldPen);  //[���Բ���Ҫ]
}
void CDataMonitorView::DrawCoordinateSystem(CDC* pDC)
{
	TRACE(_T("DrawCoordinateSystem"));
	if(pPData->dept.integer > bias)
	{
		base = pPData->dept.integer;
	}
	//����ߵ���ɫ��ɫ
	COLORREF color = RGB(127,127,127);
	CRect textRect;
	int gap = 20, gap1 = 300, gap2 = 350;
	CString str;
	CFont font;
	VERIFY(font.CreateFont(
		18,                        // nHeight
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

	 LOGBRUSH logBrush;
	//����ߵķ��
	int iDrawType = PS_SOLID;
	//iDrawType = _wtoi(_T("1"));
	
    //�����ߵĻ���"���"��"��ɫ"����"��ϸ"
	CPen pen(iDrawType, 1,color); //����
	CPen* pOldPen = pDC->SelectObject(&pen);//���ʺͻ���������
	
	CRect rect;
	//�������佲��ÿؼ�����Ļ�е�λ��
	//GetDlgItem(IDC_STATIC_PIC)->GetWindowRect(&rect);//��ȡ�ؼ��������Ļ��λ��,����ؼ���������
	//ScreenToClient(rect);//ת��Ϊ�Ի����ϵ����λ��
	GetClientRect(rect);
	rectTotal = rect;
	rectTotal.right = rectTotal.left + 750;
	rectTotal.bottom = rectTotal.top + gap*25;
	UpdateData(FALSE);

	int iCount=0;

	for (int i=0,iCount = 0;i<=gap1;i+=gap,iCount++)
	{
		pDC->MoveTo(i,0); // [0,0]
		pDC->LineTo(i,rectTotal.Height());
		if(iCount != 0 && iCount%5 == 0)
		{
			pDC->MoveTo(i+1,0); // [0,0]
			pDC->LineTo(i+1,rectTotal.Height());
		}
	}
	for (int i=gap2,iCount = 0;i<=rectTotal.Width();i+=gap,iCount++)
	{
		pDC->MoveTo(i,0); // [0,0]
		pDC->LineTo(i,rectTotal.Height());
		if(iCount != 0 && iCount%5 == 0)
		{
			pDC->MoveTo(i+1,0); // [0,0]
			pDC->LineTo(i+1,rectTotal.Height());
		}
	}
	for (int j=0,iCount = 0;j<=rectTotal.Height();j+=gap,iCount++)
	{
		pDC->MoveTo(0,j); // [0,0]
		pDC->LineTo(gap1,j);
		if(iCount != 0 && (iCount+base)%10 == 0)
		{
			pDC->MoveTo(0,j+1); // [0,0]
			pDC->LineTo(gap1,j+1);
		}
	}
	for (int j=0,iCount = 0;j<=rectTotal.Height();j+=gap,iCount++)
	{
		pDC->MoveTo(gap2,j); // [0,0]
		pDC->LineTo(rectTotal.Width(),j);
		if(iCount != 0 && (iCount+base)%10 == 0)
		{
			pDC->MoveTo(gap2,j+1); // [0,0]
			pDC->LineTo(rectTotal.Width(),j+1);
		}
	}
	bias = base + rectTotal.Height()/gap;//��ȵ���Ļ�ײ�������ƫ��

	for (int j=(base%10),iCount = 0;j<=rectTotal.Height();j+=gap*10,iCount++)
	{
		textRect = rectTotal;
		textRect.left = gap1+2;
		textRect.right = gap2-2;
		textRect.top = rectTotal.top + j-10;
		textRect.bottom = textRect.top +gap;
		pDC->SelectObject(&font);
		pDC->SetTextAlign(TA_LEFT);
		str.Format(_T("%d"),(j/gap+base));
		//pDC->TextOut(1,30*i+16,str);   //����ı�ֵ
		pDC->DrawText(str,textRect,DT_LEFT);

	}
	
	//����ˢ��
	pDC->SelectObject(pOldPen);  //[���Բ���Ҫ]

	SetScrollTotalSizes(rectTotal);
}

void CDataMonitorView::SetScrollTotalSizes(CRect rect)
{
	SIZE sizeTotal;
	sizeTotal.cx = rect.Width();
	sizeTotal.cy = rect.Height();
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CDataMonitorView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	SIZE sizeTotal;
	sizeTotal.cx = rect.Width();
	sizeTotal.cy = rect.Height();

    SetScrollSizes(MM_TEXT, sizeTotal);
}


void CDataMonitorView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	SCROLLINFO	scrollinfo;
	GetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);
	
#if 1
	pScrollBar = GetScrollBarCtrl(SB_VERT);
	int temp_01=GetScrollPos(SB_VERT);//ȡ�õ�ǰֵ

	switch(nSBCode)
	{
	case SB_THUMBTRACK://�϶�
		SetScrollPos(SB_VERT,nPos);
		break;
	case SB_ENDSCROLL://�϶����
		temp_01=GetScrollPos(SB_VERT);
		break;//ȡ�õ�ǰֵ
	case SB_PAGEUP:
		//SetScrollPos(SB_VERT,temp_01-100);
		scrollinfo.nPos-=10;
		SetScrollInfo(SB_VERT,&scrollinfo);
		break;
	case SB_PAGEDOWN:
		//SetScrollPos(SB_VERT,temp_01+100);
		scrollinfo.nPos+=10;
		SetScrollInfo(SB_VERT,&scrollinfo);
		break;
	case SB_LINEUP:
		//SetScrollPos(SB_VERT,temp_01-1);
		scrollinfo.nPos-=10;
		SetScrollInfo(SB_VERT,&scrollinfo);
		break;
	case SB_LINEDOWN:
		//SetScrollPos(SB_VERT,temp_01+1);
		scrollinfo.nPos+=10;
		SetScrollInfo(SB_VERT,&scrollinfo);
		break;

	}
	
	CScrollView::OnVScroll(nSBCode, scrollinfo.nPos, pScrollBar);
#else
	if((nSBCode == SB_THUMBTRACK || nSBCode == SB_THUMBPOSITION) && ((nPos & 0xffff) > 0x7d00)){
		int yOrig, y;
		yOrig = GetScrollPos(SB_VERT);
		y = nPos;
		ScrollWindow(0, -(y - yOrig));
		SetScrollPos(SB_VERT, nPos & 0xFFFF);
		return;
	}

	CScrollView::OnVScroll(nSBCode, nPos, pScrollBar);
#endif
}
void CDataMonitorView::StartTimer()
{
	base = 0;
	bias = 0;
	SetTimer(TIMER_CMD_DRAW,TIME_REFRESH,NULL);
}

void CDataMonitorView::StopTimer()
{
	KillTimer(TIMER_CMD_DRAW);
}

void CDataMonitorView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//KillTimer(nIDEvent);
	switch(nIDEvent)
	{
	case TIMER_CMD_DRAW:
		{
			TRACE(_T("\r\n OnTimer() for + %d + ms \r\n"),TIME_REFRESH);
			if(theApp.petroList.IsEmpty() && theApp.processType == FILEDATA_PROCESSING)
			{
				KillTimer(nIDEvent);
			}
			else
			{
				DrawData();
			}
			SetTimer(TIMER_CMD_DRAW,TIME_REFRESH,NULL);
		}
		break;
	default:
		break;
	}
	CScrollView::OnTimer(nIDEvent);
}

void CDataMonitorView::DrawData()
{
	if(!theApp.petroList.IsEmpty())
	{
		pPData = theApp.petroList.RemoveHead();
		//if(theApp.bStart)
		{
			theApp.nStartCoordinate = pPData->dept.integer/10;
			//theApp.bStart = false;
		}
	}
	Invalidate();
}
int CDataMonitorView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	return 0;
}
