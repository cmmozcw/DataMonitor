
// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "DataMonitor.h"

#include "MainFrm.h"
#include "DataMonitorView.h"
#include "PanelView.h"
#include "ScaleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static bool bRunning;
static CTypedPtrList < CPtrList, SEND_COMMAND_LIST * >senCommandList;
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_TOOLBAR_CONNECT, &CMainFrame::OnToolbarConnect)
	ON_COMMAND(ID_TOOLBAR_DISCONNECT, &CMainFrame::OnToolbarDisconnect)
	ON_MESSAGE( WM_USER_RECEIVEDATA, OnCommReceive)//���ն˿���Ϣ
	ON_WM_CLOSE()
	ON_COMMAND(ID_MENU_WORK, &CMainFrame::OnMenuWork)
	ON_COMMAND(ID_MENU_INSTRUMENT, &CMainFrame::OnMenuInstrument)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_CONNECT, &CMainFrame::OnUpdateToolbarConnect)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_DISCONNECT, &CMainFrame::OnUpdateToolbarDisconnect)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_INFO, &CMainFrame::OnUpdateIndicatorInfo)
	ON_COMMAND(ID_MENU_NEWJOB, &CMainFrame::OnMenuNewjob)
	ON_COMMAND(ID_MENU_JOBLOAD, &CMainFrame::OnMenuJobload)
	ON_COMMAND(ID_MENU_MEASUREUP, &CMainFrame::OnMenuMeasureup)
	ON_COMMAND(ID_MENU_MEASUREDOWN, &CMainFrame::OnMenuMeasuredown)
	ON_COMMAND(ID_MENU_DRAWMODEL, &CMainFrame::OnMenuDrawmodel)
	ON_COMMAND(ID_MENU_DEMO, &CMainFrame::OnMenuDemo)
	ON_COMMAND(ID_MENU_DEBUGCMD, &CMainFrame::OnMenuDebugcmd)
	ON_COMMAND(ID_EDIT_COPY, &CMainFrame::OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, &CMainFrame::OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, &CMainFrame::OnEditPaste)
	ON_COMMAND(ID_MENU_TARGETDEEPTH, &CMainFrame::OnMenuTargetdeepth)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_INDICATOR_INFO,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
	bConnect = false;
	parameterFlag = 0;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
	m_wndStatusBar.SetPaneInfo(1,ID_INDICATOR_INFO,SBPS_STRETCH,40);
	// TODO: �������Ҫ��ͣ������������ɾ��������
	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	//EnableDocking(CBRS_ALIGN_ANY);
	//DockControlBar(&m_wndToolBar);


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	cs.style = WS_OVERLAPPED | WS_CAPTION /*| FWS_ADDTOTITLE*/
		 | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE | WS_SYSMENU;

	return TRUE;
}

// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame ��Ϣ�������


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: �ڴ����ר�ô����/����û���
	if(!m_wndSplitter.CreateStatic(this,1,2))
	{
		return FALSE;
	}
	 
	CRect rect;
	GetClientRect(&rect);
	CPanelView* pPanelView; 
	CScaleView* pScaleView; 
	CDataMonitorView* pDataMonitorView; 

	pFrame=(CMainFrame*)AfxGetMainWnd();  

	if(!m_wndSplitter.CreateView(0,0,RUNTIME_CLASS(CPanelView),CSize(200,rect.Height()),pContext))
	{
		return FALSE;
	}
	pPanelView = (CPanelView*)m_wndSplitter.GetPane(0,0); 
	if(!m_wndSplitterSub.CreateStatic(&m_wndSplitter,2,1,WS_CHILD|WS_VISIBLE,m_wndSplitter.IdFromRowCol(0,1)))
	{
		return FALSE;
	}

	if(!m_wndSplitterSub.CreateView(0,0,RUNTIME_CLASS(CDataMonitorView), CSize(rect.Width()-200,::GetSystemMetrics(SM_CYSCREEN)-200),pContext))
	{
		return FALSE;
	}
	pDataMonitorView = (CDataMonitorView*)m_wndSplitterSub.GetPane(0,0); 
	if(!m_wndSplitterSub.CreateView(1,0,RUNTIME_CLASS(CScaleView), CSize(rect.Width()-200,200),pContext))
	{
		return FALSE;
	}
	pScaleView = (CScaleView*)m_wndSplitterSub.GetPane(1,0); 
	return TRUE;
	//return CFrameWnd::OnCreateClient(lpcs, pContext);
}


void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);
	
	// TODO: �ڴ˴������Ϣ����������
	CRect rect;
	GetClientRect(&rect);
	if(m_wndSplitter && m_wndSplitterSub)  // m_bSplitterCreated set in OnCreateClient
    {
		m_wndStatusBar.SetPaneInfo(1,ID_INDICATOR_INFO,SBPS_NORMAL,200);
		if(rect.Width()>=200 && rect.Height()>=200)
		{
			m_wndSplitter.SetColumnInfo(0, 200, 10);
			m_wndSplitter.SetColumnInfo(1, rect.Width()-200, 10);
			m_wndSplitter.RecalcLayout();

			m_wndSplitterSub.SetRowInfo(0, rect.Height()-200, 10);
			m_wndSplitterSub.SetRowInfo(1, 200, 10);
			m_wndSplitterSub.RecalcLayout();
		}
    }
}

void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: Add your message handler code here and/or call default
	lpMMI-> ptMinTrackSize.x = 800 ; //�� 
	lpMMI-> ptMinTrackSize.y = 600 ; //�� 
	CFrameWnd::OnGetMinMaxInfo(lpMMI);
}

void CMainFrame::OnToolbarConnect()
{
	// TODO: �ڴ���������������
	bConnect = true;
	theApp.commLayer.SetConnectType(TYPE_NONE);
	theApp.commLayer.fatherHwnd = (AfxGetMainWnd()->GetSafeHwnd());//��ȡHWND����ֵ��ͨ�Ų������Ϣ����
	bRunning = true;
	fp = NULL;
	StartThread();//�����߳�
	startWork();
}


void CMainFrame::OnToolbarDisconnect()
{
	// TODO: �ڴ���������������
	bConnect = false;
	stopWork();
}

void CMainFrame::OnUpdateToolbarConnect(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->SetCheck(bConnect);
}


void CMainFrame::OnUpdateToolbarDisconnect(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->SetCheck(!bConnect);
}
bool CMainFrame::StartThread()
{
    if (!(m_Thread = AfxBeginThread(MainThread, this)))
    {
    	m_Thread=NULL;
        return false;
    }
	TRACE(_T("Thread started\n"));
    return true;
}

LRESULT CMainFrame::OnCommReceive(WPARAM wParam, LPARAM lParam)
{
    TRACE(_T("Communication Receive!\n"));
	TRACE0("CMainFrame RX = ");
	TRACE(_T(" %02X\n"),wParam);
    for(WORD cont = 0; cont < wParam ; cont++)
    {
        TRACE(_T(" %02X"),theApp.commLayer.m_ReceiveBuff[cont]);
    }
    TRACE0("\n");
	CString str;
	totalReceiveByte += wParam;
	str.Format(_T("�ѽ���:%d�ֽ�"),totalReceiveByte);
	m_wndStatusBar.SetPaneText(m_wndStatusBar.CommandToIndex(ID_INDICATOR_INFO),str);
	writeDataFile(&theApp.commLayer.m_ReceiveBuff[0],wParam);
    return 0;
}

UINT CMainFrame::MainThread(LPVOID pParam)
{
	while(bRunning)
	{
		if(!senCommandList.IsEmpty())
		{
			SEND_COMMAND_LIST* pCurrent=NULL;
			pCurrent=senCommandList.RemoveHead();
			if(pCurrent->cmd == _T("connect"))
			{
				//connect device
				theApp.m_CommResault=theApp.commLayer.CreatConnect();
				if(theApp.m_CommResault==COMM_SUCCESS)
				{
					TRACE(_T("Communication OK!\n"));

				}
				if(theApp.m_CommResault==COMM_ERROE_HARDWARE_CONNECT_FAIL)
				{
					TRACE(_T("Communication Fail!\n"));

				}
			}
			else if(pCurrent->cmd == _T("senddata"))
			{
				//connect device
				theApp.m_CommResault=theApp.commLayer.SendCommand(pCurrent->buf,pCurrent->size);
			}
			delete pCurrent;
			pCurrent=NULL;

		}
		//TRACE(_T("MainThread is running \n"));
		Sleep(100);
	}
	TRACE(_T("Exit MainThread \n"));
	return 0;
}

void CMainFrame::initCmdList()
{
	SEND_COMMAND_LIST* pCurrent=NULL;
	while(senCommandList.IsEmpty()==false)
    {
        pCurrent=senCommandList.RemoveHead();
        delete pCurrent;
        pCurrent=NULL;
    }
}

void CMainFrame::startWork()
{
	//open and write file
	//ִ�е�������
	CString sGetFileName;
	CString strTime;
	//��ȡϵͳʱ��
	CTime tm;
	tm=CTime::GetCurrentTime();
	strTime=tm.Format(_T("(%Y-%m-%d-%H-%M-%S)"));
	CString fileName = theApp.DataPath + _T("export ")+strTime;
	CFileDialog dlg (FALSE, _T("dmor"), fileName, OFN_HIDEREADONLY | OFN_EXPLORER | OFN_OVERWRITEPROMPT, NULL);
	if (dlg.DoModal () == IDOK)
	{
		sGetFileName = dlg.GetPathName ();
		openDataFile(sGetFileName);
	}
	totalReceiveByte = 0;
	sendConnectCmd();
}

void CMainFrame::stopWork()
{
	//close file
	closeDataFile(sGetFileName);
}

void CMainFrame::sendConnectCmd()
{
	SEND_COMMAND_LIST* pCurrent= new SEND_COMMAND_LIST;
	pCurrent->cmd = _T("connect");
	pCurrent->buf = theApp.sendCmd;
	pCurrent->size = 0;
	senCommandList.AddTail(pCurrent);
	pCurrent = NULL;
}

void CMainFrame::openDataFile(CString strFile)  
{   
	if(fp != NULL)
	{
		fclose(fp);  
	}
	fp = _wfopen(strFile, _T("w")); 
}  

void CMainFrame::writeDataFile(BYTE* tmp, WPARAM wParam)  
{     
    if(fp != NULL)
	{ 
		fseek(fp,0L,2);
		//for(unsigned int i=0; i < wParam; ++i)
		{
			unsigned int nSize = fwrite( &tmp[0], wParam, 1, fp);  
		}
    }   
}  
void CMainFrame::closeDataFile(CString strFile)  
{    
	if(fp != NULL)
	{
		fclose(fp);   
	}
}  


void CMainFrame::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (MessageBox(theApp.GetResString(IDS_EXIT_APP_QUESTION),_T("��ʾ"),MB_ICONQUESTION|MB_OKCANCEL)==IDOK)
	{
		theApp.commLayer.m_SerialPort.ClosePort();//�رմ���
		bRunning = false;
		Sleep(500);
		initCmdList();
		CFrameWnd::OnClose();
	}
}


void CMainFrame::OnMenuWork()
{
	// TODO: �ڴ���������������
	parameterFlag=0;
	myOnMenuWork(parameterFlag);
}

void CMainFrame::myOnMenuWork(int myparameterFlag){	
	CJobDlg m_jDlg;
	m_jDlg.m_Path=theApp.JobPath;
	m_jDlg.receiveFlag = myparameterFlag;
	m_jDlg.m_Title=_T("������ҵ����");
	m_jDlg.m_TreeTitle=_T("��ҵ�б�");
	m_jDlg.DoModal();
}


void CMainFrame::OnMenuInstrument()
{
	// TODO: �ڴ���������������
	CJobDlg m_jDlg;
	m_jDlg.m_Path=theApp.ToolPath;
	m_jDlg.m_Title=_T("������������");
	m_jDlg.m_TreeTitle=_T("�����б�");
	m_jDlg.DoModal();
}






void CMainFrame::OnUpdateIndicatorInfo(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
}


void CMainFrame::OnMenuNewjob()
{
	// TODO: �ڴ���������������
	parameterFlag = 1;
	myOnMenuWork(parameterFlag);
}


void CMainFrame::OnMenuJobload()
{
	// TODO: �ڴ���������������
	parameterFlag = 0;
	myOnMenuWork(parameterFlag);
}


void CMainFrame::OnMenuMeasureup()
{
	// TODO: �ڴ���������������
}


void CMainFrame::OnMenuMeasuredown()
{
	// TODO: �ڴ���������������
}


void CMainFrame::OnMenuDrawmodel()
{
	// TODO: �ڴ���������������

	SHELLEXECUTEINFO  ShExecInfo = {0};
	CString strFile = theApp.ModuleFilePath + _T("\\PlotEditor.exe");  
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask    = NULL;
	ShExecInfo.hwnd      = NULL;
	ShExecInfo.lpVerb    = NULL;
	ShExecInfo.lpFile      = strFile;      // ִ�еĳ�����
	ShExecInfo.lpParameters = NULL;
	ShExecInfo.lpDirectory    = NULL;
	ShExecInfo.nShow          = SW_NORMAL;                // Ĭ�Ϸ�ʽ��ʾ�������
	ShExecInfo.hInstApp = NULL;
	ShellExecuteEx(&ShExecInfo);
	/*
	if(!ShellExecuteEx(&ShExecInfo))
	{
		DWORD dErr = GetLastError();
		if(dErr == ERROR_FILE_NOT_FOUND)
		{
			MessageBox(_T("ָ���ļ�������"),_T("��ʾ"),MB_ICONQUESTION|MB_OK);
		}
		else if(dErr == ERROR_NO_ASSOCIATION)
		{
			MessageBox(_T("û�����ƶ��ļ�����չ��Ӧ��Ӧ�ó���"),_T("��ʾ"),MB_ICONQUESTION|MB_OK);
		}
		else if(dErr == ERROR_ACCESS_DENIED)
		{
			MessageBox(_T("����ָ���ļ����ܾ�"),_T("��ʾ"),MB_ICONQUESTION|MB_OK);
		}
		else if(dErr == ERROR_DLL_NOT_FOUND)
		{
			MessageBox(_T("�޷��ҵ�����Ӧ�ó���������Ŀ��ļ�"),_T("��ʾ"),MB_ICONQUESTION|MB_OK);
		}
		else if(dErr == ERROR_NOT_ENOUGH_MEMORY)
		{
			MessageBox(_T("û���㹻���ڴ���ִ�в���"),_T("��ʾ"),MB_ICONQUESTION|MB_OK);
		}
	}
	*/
}


void CMainFrame::OnMenuDemo()
{
	// TODO: �ڴ���������������
}


void CMainFrame::OnMenuDebugcmd()
{
	// TODO: �ڴ���������������
}


void CMainFrame::OnEditCopy()
{
	// TODO: �ڴ���������������
}


void CMainFrame::OnEditCut()
{
	// TODO: �ڴ���������������
}


void CMainFrame::OnEditPaste()
{
	// TODO: �ڴ���������������
}


void CMainFrame::OnMenuTargetdeepth()
{
	// TODO: �ڴ���������������
	CTargetDepth targetDlg;
	targetDlg.DoModal();
}
