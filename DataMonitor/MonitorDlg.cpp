
// MonitorDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include<iostream>
#include <fstream>
#include "DataMonitor.h"
#include "MonitorDlg.h"
#include "afxdialogex.h"
#include "PublicInterface.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
using namespace std;
static bool bRunning;
static CTypedPtrList < CPtrList, SEND_COMMAND_LIST * >senCommandList;
//
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnMenuInstrument();
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}



BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	
END_MESSAGE_MAP()


// CMonitorDlg �Ի���




CMonitorDlg::CMonitorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMonitorDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	theApp.activeWnd = NULL;
	fp = NULL;
	initWndList();
}
CMonitorDlg::~CMonitorDlg()
{
	theApp.activeWnd = NULL;
	initWndList();
}
void CMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMonitorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CMonitorDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMonitorDlg::OnBnClickedCancel)
	ON_COMMAND(ID_MENU_ABOUT, &CMonitorDlg::OnMenuAbout)
	ON_UPDATE_COMMAND_UI(ID_MENU_ABOUT, &CMonitorDlg::OnUpdateMenuAbout)
	ON_COMMAND(ID_MENU_EXIT, &CMonitorDlg::OnMenuExit)
	ON_MESSAGE( WM_USER_RECEIVEDATA, OnCommReceive)//���ն˿���Ϣ
	ON_WM_SHOWWINDOW()
	ON_WM_GETMINMAXINFO()
	ON_WM_CLOSE()
	ON_COMMAND(ID_MENU_NEW, &CMonitorDlg::OnMenuNew)
	ON_COMMAND(ID_MENU_OPEN, &CMonitorDlg::OnMenuOpen)
	ON_COMMAND(ID_MENU_WORK, &CMonitorDlg::OnMenuWork)
	ON_COMMAND(ID_MENU_INSTRUMENT, &CMonitorDlg::OnMenuInstrument)
	ON_WM_SIZE()
	ON_COMMAND(ID_TOOLBAR_CONNECT, &CMonitorDlg::OnToolbarConnect)
	ON_COMMAND(ID_TOOLBAR_DISCONNECT, &CMonitorDlg::OnToolbarDisconnect)
END_MESSAGE_MAP()


// CMonitorDlg ��Ϣ�������

BOOL CMonitorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	if (!m_ToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_ToolBar.LoadToolBar(IDR_TOOLBAR_MAIN))  //IDR_TOOLBARΪ��������Դ��ID��
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}
	m_ToolBar.ShowWindow(SW_SHOW);   
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST,   AFX_IDW_CONTROLBAR_LAST,   0); 
	CWnd::SetWindowText(theApp.GetResString(IDS_PRODUCT_NAME));
	
	theApp.commLayer.SetConnectType(TYPE_NONE);

	theApp.commLayer.fatherHwnd = (AfxGetMainWnd()->GetSafeHwnd());//��ȡHWND����ֵ��ͨ�Ų������Ϣ����
	bRunning = true;
	StartThread();//�����߳�

	ChildWndInitialize();//��ʼ���Ӵ���
	wndList.AddTail(childDrawWnd);
	ShowActiveDlg(childDrawWnd, 0);
	//�����������С
	CWnd::SetWindowPos(NULL,0,0,GetSystemMetrics (SM_CXFULLSCREEN),GetSystemMetrics (SM_CYFULLSCREEN),0/*WS_SIZEBOX|SWP_NOZORDER|SWP_NOMOVE*/);
	CenterWindow();//�������
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

bool CMonitorDlg::ChildWndInitialize()
{
	//�����Ӵ���
	childDrawWnd = new CChildDraw;
	childDrawWnd->Create(IDD_CHILD_DRAW);
	return true;
}

void CMonitorDlg::OnMenuAbout()
{
	// TODO: �ڴ���������������
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}


void CMonitorDlg::OnUpdateMenuAbout(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
}

void CMonitorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMonitorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMonitorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

bool CMonitorDlg::StartThread()
{
    if (!(m_Thread = AfxBeginThread(MainThread, this)))
    {
    	m_Thread=NULL;
        return FALSE;
    }
	TRACE(_T("Thread started\n"));
    return TRUE;
}

UINT CMonitorDlg::MainThread(LPVOID pParam)
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

void CMonitorDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
}


void CMonitorDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}

BOOL CMonitorDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		{
			switch (pMsg->wParam)
			{
			case VK_ESCAPE:
				return TRUE;
			case VK_UP://�������·����
			case VK_DOWN:	
			case VK_SPACE:
			case VK_RETURN://���λس�
				return TRUE;
			default:
				break;
			}
		}
		break;
	default:
		break;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}



void CMonitorDlg::OnMenuExit()
{
	// TODO: �ڴ���������������
	SendMessage(WM_CLOSE);
}
//===========================================================================
//void CreatConnect( )
// ������ȡ������
// ������
// ����ֵ��void
// ����������
// Author��wuhaoyong
// Date��20140708
//===========================================================================
DWORD CMonitorDlg::CreatConnect( )
{
	theApp.m_CommResault=theApp.commLayer.CreatConnect();
	if(theApp.m_CommResault==COMM_SUCCESS)
	{
		TRACE(_T("Communication OK!\n"));

	}
	if(theApp.m_CommResault==COMM_ERROE_HARDWARE_CONNECT_FAIL)
	{
		TRACE(_T("Communication Fail!\n"));

	}
	return 0;
}

bool isConnected()
{
	return theApp.commLayer.m_bConnectEffective;
}
//===========================================================================
//void DestroyConnect( )
// �������Ͽ�����
// ������
// ����ֵ��DWORD
// ����������
// Author��wuhaoyong
// Date��20140708
//===========================================================================
DWORD CMonitorDlg::DestroyConnect( )
{
	theApp.commLayer.m_SerialPort.ClosePort();//�رմ���
    if (theApp.commLayer.m_SerialPort.m_bThreadAlive)
    {
        if(theApp.commLayer.m_SerialPort.m_Thread->m_hThread!=NULL)
        {
            TerminateThread (theApp.commLayer.m_SerialPort.m_Thread->m_hThread, 0);
            theApp.commLayer.m_SerialPort.m_Thread=NULL;
            theApp.commLayer.m_SerialPort.m_bThreadAlive=FALSE;
        }
    }
    return 0;
}

//===========================================================================
// void DataReceive(BYTE* inbuff, DWORD* dwSize)
// ���������������ⲿ�ӿ�
// ������ [in] inbuff:��������buf;dwSize:�������ݴ�С
// ����ֵ��void
// �������������������е����ݴ��ݡ�
// Author��wuhaoyong
// Date��20140708
//===========================================================================
void CMonitorDlg::DataReceive(BYTE* inbuff, DWORD* dwSize)
{
    TRACE(_T("DataReceive\n"));

    return;
}
//===========================================================================
// WORD DataSend(BYTE* outbuff, DWORD dwSize)
// ���������������ⲿ�ӿ�
// ������ [in] outbuff:��������buf;dwSize:�������ݴ�С
// ����ֵ��void
// �������������������е����ݴ��ݡ�
// Author��wuhaoyong
// Date��20140708
//===========================================================================
WORD CMonitorDlg::DataSend(BYTE* outbuff, DWORD dwSize)
{
    TRACE(_T("DataTrans\n"));
    return theApp.commLayer.TransData(outbuff,dwSize);
}

/**************************************************************************/
/***  specifications;              ***/
/***  NAME   = OnCommReceive;       ***/
/***  FUNCTION  = �����ֻ������ݵ��������� ***/
/***  DATE   = 2014/07/11;           ***/
/***  AUTHOR  = wuhaoyong;              ***/
/***  INPUT   = ;               ***/
/***  OUTPUT  = ;               ***/
/***  END OF SPECIFICATIONS;            ***/
/**************************************************************************/
LRESULT CMonitorDlg::OnCommReceive(WPARAM wParam, LPARAM lParam)
{
    TRACE(_T("Communication Receive!\n"));
	TRACE0("RX = ");
	TRACE(_T(" %02X\n"),wParam);
    for(WORD cont = 0; cont < wParam ; cont++)
    {
        TRACE(_T(" %02X"),theApp.commLayer.m_ReceiveBuff[cont]);
    }
    TRACE0("\n");
	writeDataFile(&theApp.commLayer.m_ReceiveBuff[0],wParam);
    return 0;
}


void CMonitorDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: �ڴ˴������Ϣ����������
	
}

void CMonitorDlg::startWork()
{
	//open and write file
	//ִ�е�������
	CString sGetFileName;
	CString strTime;
	//��ȡϵͳʱ��
	CTime tm;
	tm=CTime::GetCurrentTime();
	strTime=tm.Format(_T("(%Y-%m-%d-%H-%M-%S)"));
	CString fileName = _T("export ")+strTime;
	CFileDialog dlg (FALSE, _T("dmor"), fileName, OFN_HIDEREADONLY | OFN_EXPLORER | OFN_OVERWRITEPROMPT, NULL);
	if (dlg.DoModal () == IDOK)
	{
		sGetFileName = dlg.GetPathName ();
		openDataFile(sGetFileName);
	}
		
	sendConnectCmd();
}

void CMonitorDlg::stopWork()
{
	//close file
	closeDataFile(sGetFileName);
}

void CMonitorDlg::sendConnectCmd()
{
	SEND_COMMAND_LIST* pCurrent= new SEND_COMMAND_LIST;
	pCurrent->cmd = _T("connect");
	pCurrent->buf = theApp.sendCmd;
	pCurrent->size = 0;
	senCommandList.AddTail(pCurrent);
	pCurrent = NULL;
}

void CMonitorDlg::openDataFile(CString strFile)  
{   
#ifdef FEATURE_C_LANGUAGE
	if(fp != NULL)
	{
		fclose(fp);  
	}
	fp = _wfopen(strFile, _T("w")); 
#else
	if (!fAddressTransOut.Open (strFile, CFile::modeCreate | CFile::modeWrite, &eFileException))
	{
		MessageBox (_T("Create file fail"), _T("Error"), MB_OK + MB_ICONERROR);
		bFileOk = false;
	}
	bFileOk = true;
#endif
}  

void CMonitorDlg::writeDataFile(BYTE* tmp, WPARAM wParam)  
{     
#ifdef FEATURE_C_LANGUAGE
    if(fp != NULL)
	{ 
		fseek(fp,0L,2);
		for(unsigned int i=0; i < wParam; ++i)
		{
			unsigned int nSize = fwrite( &tmp[i], 1, 1, fp);  
		}
    }   
#else 
	if(bFileOk)
	{
		fAddressTransOut.SeekToEnd ();
		fAddressTransOut.Write (tmp, wParam);
	}
#endif
}  
void CMonitorDlg::closeDataFile(CString strFile)  
{    
#ifdef FEATURE_C_LANGUAGE
	if(fp != NULL)
	{
		fclose(fp);   
	}
#else
	if(bFileOk)
	{
		TRY
		{
			fAddressTransOut.Close ();
		}
		CATCH (CFileException, eFileException)
		{
			TRACE0 ("File close Failed!\n");
		}
		END_CATCH
		bFileOk = false;
	}
#endif
}  
void CMonitorDlg::OnToolbarConnect()
{
	// TODO: �ڴ���������������
	startWork();
}


void CMonitorDlg::OnToolbarDisconnect()
{
	// TODO: �ڴ���������������
	stopWork();
}

void CMonitorDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	theApp.commLayer.m_SerialPort.ClosePort();//�رմ���
	bRunning = false;
	Sleep(500);
	initCmdList();
	CDialogEx::OnClose();
}

void CMonitorDlg::initCmdList()
{
	SEND_COMMAND_LIST* pCurrent=NULL;
	while(senCommandList.IsEmpty()==false)
    {
        pCurrent=senCommandList.RemoveHead();
        delete pCurrent;
        pCurrent=NULL;
    }
}
void CMonitorDlg::initWndList()
{
	CDialog* pCurrent=NULL;
	while(wndList.IsEmpty()==false)
    {
        pCurrent=wndList.RemoveHead();
        delete pCurrent;
        pCurrent=NULL;
    }
}
void CMonitorDlg::OnMenuNew()
{
	// TODO: �ڴ���������������
}


void CMonitorDlg::OnMenuOpen()
{
	// TODO: �ڴ���������������
	
}

void CMonitorDlg::OnMenuWork()
{
	// TODO: �ڴ���������������
		CJobDlg m_jDlg;
		m_jDlg.m_Path=theApp.JobPath;
		m_jDlg.m_Title=_T("������ҵ����");
		m_jDlg.m_TreeTitle=_T("��ҵ�б�");
		m_jDlg.DoModal();
}
void CMonitorDlg::OnMenuInstrument()
{
	// TODO: �ڴ���������������
		CJobDlg m_jDlg;
		m_jDlg.m_Path=theApp.ToolPath;
		m_jDlg.m_Title=_T("������������");
		m_jDlg.m_TreeTitle=_T("�����б�");
		m_jDlg.DoModal();
}


bool CMonitorDlg::ShowActiveDlg(CWnd* activeWnd,int id)
{
	if(activeWnd == theApp.activeWnd)
	{
		return false;
	}
	if(theApp.activeWnd != NULL)
	{
		theApp.activeWnd->ShowWindow(SW_HIDE);
	}
	theApp.activeWnd = activeWnd;
	
	theApp.activeWnd->ShowWindow(SW_SHOW);
	theApp.activeWnd->SetActiveWindow();
	theApp.activeWnd->SetFocus();
	return true;
}

void CMonitorDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	CRect rect,rectMain;
	GetClientRect(&rect);
	rectMain.left = rect.left + 3;
	rectMain.right = rect.right - 3;
	rectMain.top = rect.top + 30;
	rectMain.bottom = rect.bottom - 30;
	if(wndList.GetCount()>0)
	{
		POSITION pos = wndList.GetHeadPosition();
		while(pos)
		{
			CDialog * pCurrent = wndList.GetNext(pos);
			if(pCurrent != NULL)
			{
				pCurrent->SetWindowPos(NULL,rectMain.left,rectMain.top,rectMain.Width(),rectMain.Height(),0);
				//TRACE(_T("Child Dialog OnSize\n"));
			}
		}
	}
}
void CMonitorDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: Add your message handler code here and/or call default
	lpMMI-> ptMinTrackSize.x = 800 ; //�� 
	lpMMI-> ptMinTrackSize.y = 600 ; //�� 
	CDialogEx::OnGetMinMaxInfo(lpMMI);
}



