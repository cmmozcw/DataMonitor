
// MonitorDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataMonitor.h"
#include "MonitorDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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
	//�����������С
	CWnd::SetWindowPos(NULL,0,0,GetSystemMetrics (SM_CXFULLSCREEN),GetSystemMetrics (SM_CYFULLSCREEN),0/*WS_SIZEBOX|SWP_NOZORDER|SWP_NOMOVE*/);
	CenterWindow();//�������
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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
