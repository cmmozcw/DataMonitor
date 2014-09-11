
// DMonitorView.h : CDataMonitorView ��Ľӿ�
//

#pragma once

#include "DataMonitorDoc.h"
#include "clPlot.h"
class CDataMonitorView : public CScrollView
{
protected: // �������л�����
	CDataMonitorView();
	DECLARE_DYNCREATE(CDataMonitorView)

// ����
public:
	CDataMonitorDoc* GetDocument() const;

// ����
public:

protected:
	//why add ��ͼ����
	int			unitPixel;//ÿ�׶�Ӧ����ֵ
	CRect		m_screenRect;// m_screenRect - view�Ŀɼ���Χ�����Ļ���Ͻ�����
	CRect		m_clientRect;// m_clientRect - view�Ŀɼ���Χ�ͻ���
	CRect		m_totalRect;// m_totalRect - ����Ҫ��������ķ�Χ���������ɼ�

	CRect		m_plot1Rect;// m_plot1Rect - borderspace1
	CRect		m_plot2Rect;// m_plot2Rect - borderspace2
	CRect		m_plot3Rect;// m_plot3Rect - borderspace3

	SIZE sizeTotal;//����Ҫ��������ķ�Χ���������ɼ�

	double		minDepth;//�ɼ���Χ��С���
	double		maxDepth;//�ɼ���Χ������

	bool		m_bAutoScrollY;		// automatic y range scrolling
	bool		m_bDirectDown;		// automatic x range scrolling
	//��ɫ
	COLORREF	m_gridColor;		// grid line color
	//��������
	CFont		m_font;
	LOGFONT		m_zoomFont;
	//˫����
	CDC   MemDC;          //����һ����ʾ�豸����  
	CBitmap   MemBitmap;  //����һ��λͼ����  

	unsigned long tempArray[3];
	unsigned long gmArray[3];
	unsigned long rmArray[3];
	unsigned long cclArray[3];
	unsigned long magArray[3];

	DATA_TEMP olddeptArray;
	DATA_TEMP oldtempArray;
	DATA_TEMP oldgmArray;
	DATA_TEMP oldrmArray;
	DATA_TEMP oldcclArray;
	DATA_TEMP oldmagArray;
#ifdef FEATURE_TEST_DRAW
	clPlot m_Plot;
#endif

protected:
	//why add ��ͼ����
	void DrawBasic(CDC * pDC);
	void DrawGrid(CDC * pDC);//������������
	void DrawXYAxisGrid(CDC * pDC);//���ƺ�����������
	void DrawPlot(CDC* pDC);//������������

	void DrawData(CDC* pDC);
	void DrawDataFile(CDC* pDC);
	void SetScrollTotalSizes(CRect rect);
	void InitArrayData();
	void InitOldArrayData();
	void GetMaxMinData();
	
#ifdef FEATURE_TEST_DRAW
	void InitPlot(CRect Rect);
#endif
	void DrawCoordinateSystemFile(CDC* pDC);
#if 0
	void DrawDataArray(CDC* pDC);
	void DrawDataArrayFile(CDC* pDC);
#endif
	unsigned long GetMinData(DATA_PART tmp, unsigned long m);
	
// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void StartTimer();
	void StopTimer();
	void KillDrawTimer();

protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

private:
	unsigned long base;//������
	unsigned long bias;//���ƫ����
	long counter;//������
	POSITION pos;//��ǰ
	CRect rect;
	CRect rectTotal;
	CRect rectTotalFile;
	
	CPetroData* pPData;//�õ���������
	CPetroData* pOldPData;//����ǰһ�����ݶ���
// ʵ��
public:
	virtual ~CDataMonitorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual void OnInitialUpdate();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // DMonitorView.cpp �еĵ��԰汾
inline CDataMonitorDoc* CDataMonitorView::GetDocument() const
   { return reinterpret_cast<CDataMonitorDoc*>(m_pDocument); }
#endif

