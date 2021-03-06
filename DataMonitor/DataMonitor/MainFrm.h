
// MainFrm.h : CMainFrame 类的接口
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
#include "PetroData.h"
#include "CurveInfo.h"
#include <string>
class CMainFrame : public CFrameWnd
{
	
protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 特性
public:
	CPanelView* pPanelView; 
	CScaleView* pScaleView; 
	CDataMonitorView* pDataMonitorView; 
protected:
    CSplitterWnd m_wndSplitter;
	CSplitterWnd m_wndSplitterSub;
	bool bConnect;

	CMainFrame*   pFrame;  
	//get view point ,sample
	//CPanelView* pPanelView=(CPanelView*) GetActiveView(); 
	
	CString fileName;
	//CString sGetFileName;
	
	int parameterFlag;//send flag to jobdlg for some action, 0-nothing;1-new job
// 操作
public:
	
// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;

	CString sGetFileName;//保存打开的文件路径
	int fileNum;
	CString sGetFilePreName;//保存打开的文件名
	FILE* fp;//指向打开的文件

	CWinThread*   m_Thread;
	BYTE* pData;//存储文件数据
	unsigned long totalReceiveByte;
	unsigned long fileLimit;//文件大小限制，超过的话则自动生成下一个文件

	CCurveInfo curveInfo;
protected:
	static UINT MainThread(LPVOID pParam);
	
	bool StartThread();
	bool StartDrawThread();
	void sendConnectCmd();
	void startWork();
	void stopWork();
	void openDataFile(CString strFile);
	void writeDataFile(BYTE* tmp, WPARAM wParam)  ;
	void ParseData(BYTE* tmp, WPARAM wParam) ;
	void closeDataFile(CString strFile);
	void myOnMenuWork(int myparameterFlag);
	CPanelView* GetPanelView();
	CDataMonitorView* GetDataMonitorView();
	CScaleView* GetScaleView();
	void initCmdList();
	int CheckString( std::string str );
// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	afx_msg void OnToolbarConnect();
	afx_msg void OnToolbarDisconnect();
	afx_msg LRESULT OnCommReceive(WPARAM wParam, LPARAM lParam);//接收端口消息
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
	afx_msg void OnFileOpen();
	afx_msg void OnUpdateMenuMeasuredown(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMenuMeasureup(CCmdUI *pCmdUI);
	afx_msg void OnUpdateToolbarMeasuredown(CCmdUI *pCmdUI);
	afx_msg void OnToolbarMeasuredown();
	afx_msg void OnToolbarMeasureup();
	afx_msg void OnUpdateToolbarMeasureup(CCmdUI *pCmdUI);
	afx_msg void OnToolbarStop();
	afx_msg void OnUpdateToolbarStop(CCmdUI *pCmdUI);
};


