
// MainFrm.h : CMainFrame ��Ľӿ�
//

#pragma once
#include "PlotEditorView.h"
#include "CoordinateView.h"
#include "CurveSheet.h"
#include "CurveInfo.h"
class CMainFrame : public CFrameWnd
{
	
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// ����
protected:
	CSplitterWnd m_wndSplitter;
public:
	CMainFrame*   pFrame; 
	CPlotEditorView*   pPlotEditorView; 
	CCoordinateView * pCoodinateView; 
// ����
public:

// ��д
public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
	CStatusBar        m_wndStatusBar;

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnFileNewplot();
	afx_msg void OnFileSaveplot();
	afx_msg void OnClose();
};


