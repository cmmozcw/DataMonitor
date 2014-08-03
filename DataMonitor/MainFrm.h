
// MainFrm.h : CMainFrame ��Ľӿ�
//

#pragma once
#include "jobdlg.h"
#include "jobeditdlg.h"
#include "DMSplitterWnd.h"
#include "PanelView.h"
#include "ScaleView.h"
#include "DataMonitorView.h"
#include "tinyxml.h"
#include "TargetDepth.h"
class CMainFrame : public CFrameWnd
{
	
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// ����
public:

protected:
    CSplitterWnd m_wndSplitter;
	CSplitterWnd m_wndSplitterSub;
	bool bConnect;

	CMainFrame*   pFrame;  
	//get view point ,sample
	//CPanelView* pPanelView=(CPanelView*) GetActiveView(); 
	CPanelView* pPanelView; 
	CScaleView* pScaleView; 
	CDataMonitorView* pDataMonitorView; 
	int parameterFlag;//send flag to jobdlg for some action, 0-nothing;1-new job
// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;

	CString sGetFileName;//����򿪵��ļ���
	FILE* fp;//ָ��򿪵��ļ�

	CWinThread*   m_Thread;

	unsigned long totalReceiveByte;
protected:
	static UINT MainThread(LPVOID pParam);
	bool StartThread();
	void sendConnectCmd();
	void startWork();
	void stopWork();
	void openDataFile(CString strFile);
	void writeDataFile(BYTE* tmp, WPARAM wParam)  ;
	void closeDataFile(CString strFile);

	void initCmdList();
// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	afx_msg void OnToolbarConnect();
	afx_msg void OnToolbarDisconnect();
	afx_msg LRESULT OnCommReceive(WPARAM wParam, LPARAM lParam);//���ն˿���Ϣ
	afx_msg void OnClose();
	afx_msg void OnMenuWork();
	afx_msg void OnMenuInstrument();
	afx_msg void OnUpdateToolbarConnect(CCmdUI *pCmdUI);
	afx_msg void OnUpdateToolbarDisconnect(CCmdUI *pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnUpdateIndicatorInfo(CCmdUI *pCmdUI);
	afx_msg void OnMenuNewjob();
	afx_msg void OnMenuJobload();
	afx_msg void OnMenuMeasureup();
	afx_msg void OnMenuMeasuredown();
	afx_msg void OnMenuDrawmodel();
	afx_msg void OnMenuDemo();
	afx_msg void OnMenuDebugcmd();
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	afx_msg void OnMenuTargetdeepth();
};


