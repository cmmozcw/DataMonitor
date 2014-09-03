
// DMonitorView.h : CDataMonitorView ��Ľӿ�
//

#pragma once

#include "DataMonitorDoc.h"

class CDataMonitorView : public CScrollView
{
protected: // �������л�����
	CDataMonitorView();
	DECLARE_DYNCREATE(CDataMonitorView)

	void DrawData(CDC* pDC);
	void DrawDataFile(CDC* pDC);
	void SetScrollTotalSizes(CRect rect);
	void InitArrayData();
	void InitOldArrayData();
	void GetDataLimit();
// ����
public:
	CDataMonitorDoc* GetDocument() const;

// ����
public:
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

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void StartTimer();
	void StopTimer();
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	void DrawCoordinateSystem(CDC* pDC);
	void DrawCoordinateSystemFile(CDC* pDC);
	void DrawDataArray(CDC* pDC);
	void DrawDataArrayFile(CDC* pDC);
	void DrawData();
	unsigned long GetMinData(DATA_PART tmp, unsigned long m);
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
};

#ifndef _DEBUG  // DMonitorView.cpp �еĵ��԰汾
inline CDataMonitorDoc* CDataMonitorView::GetDocument() const
   { return reinterpret_cast<CDataMonitorDoc*>(m_pDocument); }
#endif

