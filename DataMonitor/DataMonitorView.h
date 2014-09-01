
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
	void SetScrollTotalSizes(CRect rect);
// ����
public:
	CDataMonitorDoc* GetDocument() const;

// ����
public:
	int tempArray[2];
	int gmArray[2];
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
	void DrawDataArray(CDC* pDC);
	void DrawData();
private:
	unsigned long base;//������
	unsigned long bias;//���ƫ����
	CRect rect;
	CRect rectTotal;

	
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

