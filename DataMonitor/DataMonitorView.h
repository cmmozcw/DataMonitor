
// DMonitorView.h : CDataMonitorView ��Ľӿ�
//

#pragma once

#include "DataMonitorDoc.h"

class CDataMonitorView : public CView
{
protected: // �������л�����
	CDataMonitorView();
	DECLARE_DYNCREATE(CDataMonitorView)

// ����
public:
	CDataMonitorDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

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
};

#ifndef _DEBUG  // DMonitorView.cpp �еĵ��԰汾
inline CDataMonitorDoc* CDataMonitorView::GetDocument() const
   { return reinterpret_cast<CDataMonitorDoc*>(m_pDocument); }
#endif

