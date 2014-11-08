
// DMoniterDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DMonitor.h"
#include "DMonitorDlg.h"
#include "afxdialogex.h"
#include "JobDlg.h"
#include <string>
#include <iostream> 
using namespace std; 
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int maxqueue=1000;
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


// CDMoniterDlg �Ի���




CDMoniterDlg::CDMoniterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDMoniterDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//�����������С
	m_SM_CXFULLSCREEN = GetSystemMetrics (SM_CXFULLSCREEN);
	m_SM_CYFULLSCREEN = GetSystemMetrics (SM_CYFULLSCREEN);
	minMainDialogWidth = 960;
	minMainDialogHeight = minMainDialogWidth * 9 / 16;
	processType = NO_PROCESSING;

	ClearPetroData();
	InitColorVariable();
	pData = NULL;
	bConnect = false;
	unitPixel = 5;//20 pixel = 1 m
	unitRatio = 4;
	m_bDirectDown = true;
	//bOperating = false;
	base = 0;
	bias = 0;
	
	minDepth = 0;
	maxDepth = 0;
	maxPreDepth = 0;
	minDepthLimit = 0;
	maxDepthLimit = 0;
	
	m_drawCount = 0;
	bScroll = false;
	m_step = 5;//ÿ���ƶ�����Ϊ5��
	fp = NULL;
	pPData = NULL;
	pOldPData = NULL;

	bTimer = false;//��ʱ���Ƿ���
	pos = NULL;//��ǰ��¼λ��

	pData = NULL;
	fileLimit = 1024*1024*4;
	processType == NO_PROCESSING;

	VERIFY(m_font.CreateFont(
		18,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Arial")));                 // lpszFacename

}

void CDMoniterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SCROLLBAR_V, mScrollV);
	DDX_Control(pDX, IDC_STATIC_01, cs01);
	DDX_Text(pDX, IDC_STATIC_01, value01);
	DDX_Control(pDX, IDC_STATIC_02, cs02);
	DDX_Text(pDX, IDC_STATIC_02, value02);
	DDX_Control(pDX, IDC_STATIC_03, cs03);
	DDX_Text(pDX, IDC_STATIC_03, value03);
	DDX_Control(pDX, IDC_STATIC_04, cs04);
	DDX_Text(pDX, IDC_STATIC_04, value04);
	DDX_Control(pDX, IDC_EDIT_01, Edit01);
	DDX_Text(pDX, IDC_EDIT_01, strEdit01);
	DDX_Control(pDX, IDC_EDIT_02, Edit02);
	DDX_Text(pDX, IDC_EDIT_02, strEdit02);
	DDX_Control(pDX, IDC_EDIT_03, Edit03);
	DDX_Text(pDX, IDC_EDIT_03, strEdit03);
	DDX_Control(pDX, IDC_EDIT_04, Edit04);
	DDX_Text(pDX, IDC_EDIT_04, strEdit04);
	DDX_Control(pDX, IDC_LIST_DETAIL, listView);
}

BEGIN_MESSAGE_MAP(CDMoniterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CDMoniterDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDMoniterDlg::OnBnClickedCancel)
	ON_MESSAGE( WM_USER_RECEIVEDATA, OnCommReceive)//���ն˿���Ϣ
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_FILE_OPEN, &CDMoniterDlg::OnFileOpen)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, &CDMoniterDlg::OnUpdateFileOpen)
	ON_WM_TIMER()
	ON_WM_VSCROLL()
	ON_WM_CLOSE()
	ON_COMMAND(ID_MENU_CONN, &CDMoniterDlg::OnMenuConn)
	ON_UPDATE_COMMAND_UI(ID_MENU_CONN, &CDMoniterDlg::OnUpdateMenuConn)
	ON_COMMAND(ID_MENU_DISCONN, &CDMoniterDlg::OnMenuDisconn)
	ON_UPDATE_COMMAND_UI(ID_MENU_DISCONN, &CDMoniterDlg::OnUpdateMenuDisconn)
	ON_COMMAND(ID_MENU_MEASUREDOWN, &CDMoniterDlg::OnMenuMeasuredown)
	ON_UPDATE_COMMAND_UI(ID_MENU_MEASUREDOWN, &CDMoniterDlg::OnUpdateMenuMeasuredown)
	ON_COMMAND(ID_MENU_MEASUREUP, &CDMoniterDlg::OnMenuMeasureup)
	ON_UPDATE_COMMAND_UI(ID_MENU_MEASUREUP, &CDMoniterDlg::OnUpdateMenuMeasureup)
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_MENU_INSTRUMENT, &CDMoniterDlg::OnMenuInstrument)
	ON_COMMAND(ID_MENU_JOB, &CDMoniterDlg::OnMenuJob)
	ON_COMMAND(ID_MENU_TOOL, &CDMoniterDlg::OnMenuTool)
	ON_COMMAND(ID_MENU_NEWJOB, &CDMoniterDlg::OnMenuNewjob)
	ON_COMMAND(ID_MENU_JOBLOAD, &CDMoniterDlg::OnMenuJobload)
	ON_COMMAND(ID_TEST_MODE1, &CDMoniterDlg::OnTestMode1)
END_MESSAGE_MAP()


// CDMoniterDlg ��Ϣ�������

BOOL CDMoniterDlg::OnInitDialog()
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
	m_hMenu = LoadMenu(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));//������Դ,�����˵�
	::SetMenu(this->GetSafeHwnd(),m_hMenu);//��ӵ��Ի���
	OnInitWidget();
	ParseTestData();
    RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST, 0);
	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	ShowWindow(SW_SHOWMAXIMIZED);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}
void CDMoniterDlg::OnInitWidget()
{
	value01.LoadString(IDS_DEEPTH);
	cs01.SetWindowText(value01);
	value02.LoadString(IDS_SPEED);
	cs02.SetWindowText(value02);
	value03.LoadString(IDS_TENSION);
	cs03.SetWindowText(value03);
	value04.LoadString(IDS_STATE);
	cs04.SetWindowText(value04);

	//btSetup.SetWindowText(theApp.GetResString(IDS_SETUP));	listView.InsertColumn( 0, theApp.GetResString(IDS_WHY_CURVE), LVCFMT_LEFT, 60 ); 
    listView.InsertColumn( 1, theApp.GetResString(IDS_WHY_VALUE), LVCFMT_LEFT, 60 ); 
	listView.InsertColumn( 2, theApp.GetResString(IDS_WHY_UNIT), LVCFMT_LEFT, 60 ); 
	listView.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP ); 
}
void CDMoniterDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
void CDMoniterDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (MessageBox(theApp.GetResString(IDS_EXIT_APP_QUESTION),_T("��ʾ"),MB_ICONQUESTION|MB_OKCANCEL)==IDOK)
	{
		theApp.commLayer.m_SerialPort.ClosePort();//�رմ���
		Sleep(500);
		ClearList();
	
		CDialogEx::OnClose();
	}
}
// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�
BOOL CDMoniterDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
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

void CDMoniterDlg::OnPaint()
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
		CPaintDC dc(this);
		GetClientRect(rectMain);
		GetRectParam(rectMain);
		if(mScrollV)
		{
			mScrollV.SetWindowPos(0,rectScrollV.left,rectScrollV.top,rectScrollV.Width(),rectScrollV.Height(),SWP_NOZORDER);
		}
		//CBrush brush(crViewBackground);
		//CBrush *oBrush = dc.SelectObject(&brush);
		dc.FillSolidRect(rectView,crViewBackground); 
		//dc.SelectObject(oBrush);

		CPen pen(PS_SOLID, 1, RGB(0,0,0));
		CPen *oPen = dc.SelectObject(&pen);
		
		dc.Rectangle(rectView.left-1,rectView.top-1,rectView.right+1,rectView.bottom+1);
		dc.SelectObject(oPen);
		///////////////////////
		//��������Ļ��ʾ���ݵ��ڴ���ʾ�豸����ʱ�����ܻ�ͼ����Ϊû�еط���
		MemDC.CreateCompatibleDC(NULL);  
		//����һ������Ļ��ʾ���ݵ�λͼ������λͼ�Ĵ�С�������ô��ڵĴ�С  
		MemBitmap.CreateCompatibleBitmap(&dc,rectView.Width(),rectView.Height());  
		CBitmap *pOldBit=MemDC.SelectObject(&MemBitmap);
		MemDC.FillSolidRect(0,0,rectView.Width(),rectView.Height(),crViewBackground);
		m_clientRect = rectView;
		m_clientRect.right -= m_clientRect.left; 
		m_clientRect.left = 0;
		m_clientRect.bottom -= m_clientRect.top; 
		m_clientRect.top = 0;

		DrawData(&MemDC);
		//���ڴ��е�ͼ��������Ļ�Ͻ�����ʾ  
		dc.BitBlt(rectView.left,rectView.top,rectView.Width(),rectView.Height(),&MemDC,0,0,SRCCOPY);  
		//��ͼ��ɺ������  
		MemBitmap.DeleteObject();  
		MemDC.DeleteDC(); 
		//////////////////////		
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CDMoniterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CDMoniterDlg::InitColorVariable()
{
	crViewBackground = RGB(255,255,255);//����
	crViewCoordinate = RGB(0,0,0);
	crViewPlot = RGB(127,127,127);//������
	crViewGraduation  = RGB(0,0,0);//�̶�
	crViewGridColor = RGB(0,0,0);//RGB(127,127,127);

	colorRed = RGB(255,0,0);
	colorBlue = RGB(0,0,255);
	colorGreen = RGB(0,255,0);
	colorBlack = RGB(0,0,0);
	colorCyan = RGB(2,141,153);
	colorArray = NULL;
}
void CDMoniterDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
}


void CDMoniterDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}


void CDMoniterDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	lpMMI-> ptMinTrackSize.x = minMainDialogWidth ; //�� 
	lpMMI-> ptMinTrackSize.y = min(minMainDialogHeight,m_SM_CYFULLSCREEN) ; //�� 
	CDialogEx::OnGetMinMaxInfo(lpMMI);
}


void CDMoniterDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	GetClientRect(rectMain);
	GetRectParam(rectMain);
	int mWidth1 = 40;
	int mWidth2 = rectPanel.Width()-20-mWidth1;
	int temp = (rectPanel.Height())/15;
	int mHight = temp*3/4;
	int gap = temp - mHight;
	if(cs01.m_hWnd != NULL)
	{
		cs01.SetWindowPos(NULL,rectPanel.left,rectPanel.top,mWidth1,mHight,0);
	}
	if(Edit01.m_hWnd != NULL)
	{
		Edit01.SetWindowPos(NULL,rectPanel.left+50,rectPanel.top,mWidth2,mHight,0);
	}

	if(cs02.m_hWnd != NULL)
	{
		cs02.SetWindowPos(NULL,rectPanel.left,rectPanel.top + (mHight + gap)*1,mWidth1,mHight,0);
	}
	if(Edit02.m_hWnd != NULL)
	{
		Edit02.SetWindowPos(NULL,rectPanel.left+50,rectPanel.top + (mHight + gap)*1,mWidth2,mHight,0);
	}

	if(cs03.m_hWnd != NULL)
	{
		cs03.SetWindowPos(NULL,rectPanel.left,rectPanel.top + (mHight + gap)*2,mWidth1,mHight,0);
	}
	if(Edit03.m_hWnd != NULL)
	{
		Edit03.SetWindowPos(NULL,rectPanel.left+50,rectPanel.top + (mHight + gap)*2,mWidth2,mHight,0);
	}

	if(cs04.m_hWnd != NULL)
	{
		cs04.SetWindowPos(NULL,rectPanel.left,rectPanel.top + (mHight + gap)*3,mWidth1,mHight,0);
	}
	if(Edit04.m_hWnd != NULL)
	{
		Edit04.SetWindowPos(NULL,rectPanel.left+50,rectPanel.top + (mHight + gap)*3,mWidth2,mHight,0);
	}
	if(listView.m_hWnd != NULL)
	{
		listView.SetWindowPos(NULL,rectPanel.left,rectPanel.top + (mHight + gap)*4,rectPanel.Width()-10,rectPanel.bottom - (mHight + gap)*5 ,0);
	}
	if(mScrollV)
	{
		mScrollV.SetWindowPos(0,rectScrollV.left,rectScrollV.top,rectScrollV.Width(),rectScrollV.Height(),SWP_NOZORDER);
	}
	InvalidateRect(rectMain);
	// TODO: �ڴ˴������Ϣ����������
}

void CDMoniterDlg::GetRectParam(CRect rectMain)
{
	rectMain.left += 2;
	rectMain.top += 25;
	rectMain.right -= 2;
	rectMain.bottom -= 2;

	rectPanel = rectMain;
	rectPanel.right = rectPanel.left+200;

	rectView = rectMain;
	rectView.left = rectPanel.right + 1;
	rectView.right = rectMain.right -20;
	rectView.bottom = rectMain.bottom - 200;

	rectScrollV = rectMain;
	rectScrollV.left = rectView.right + 1;
	rectScrollV.bottom = rectView.bottom;

	rectScale = rectMain;
	rectScale.left = rectPanel.right + 1;
	rectScale.top = rectView.bottom+1;
}


void CDMoniterDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	
	// TODO: �ڴ˴������Ϣ����������
	DestroyMenu(m_hMenu);
}


void CDMoniterDlg::OnFileOpen()
{
	// TODO: �ڴ���������������
	bool bReturn=false;
	processType = FILEDATA_PROCESSING;
	TCHAR   *pFileBuffer = new TCHAR[MAX_PATH];//�������300���ļ�
	CFileDialog dlg (TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_EXPLORER , _T("dmor�ļ�(*.dmor)|*.dmor||"),this);
	if (dlg.DoModal () == IDOK)
	{
		CString csFileName = dlg.GetPathName();
		CFile fAddressImport;
		CFileException eFileException;
		if (!fAddressImport.Open (csFileName, CFile::modeRead | CFile::shareDenyWrite, &eFileException))
		{
			return;
		}
		DWORD dwFileLength = fAddressImport.GetLength ();
		pData = new BYTE[dwFileLength];
		memset(pData,0,dwFileLength);
		fAddressImport.SeekToBegin ();
		fAddressImport.Read (pData,dwFileLength);
		petroList.RemoveAll();
		ParseData(pData,dwFileLength);
		StopTimer();
		StartTimer();
	}
}


void CDMoniterDlg::OnUpdateFileOpen(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
}

void CDMoniterDlg::ParseData(BYTE* tmp, WPARAM wParam) 
{
	std::string str,strTitle,strData;
	CPetroData* pPData = NULL;
	BYTE buf0 = 0;
	for(int i = 0; i< wParam; i++)
	{
		if(i == wParam-1)
		{
			if(pPData != NULL)
			{
				pushToQueue(pPData);
				//petroList.AddTail(pPData);
			}
		}
		if(tmp[i] == '$')//��ʼ��ʾ
		{
			str.clear();
			//TRACE(_T("Find Char '$' \r\n"));
		}
		else if(tmp[i] == ',' || tmp[i] == '*')//���ݽ���
		{
			if(str == "DEPT" || str == "TEMP" || str == "RM" 
				|| str == "GM" || str == "MAGX" || str == "CCL" )
			{
				strTitle = str;
				if(str == "DEPT")
				{
					//TRACE(_T("Find Char 'DEPT' \r\n"));
					if(pPData != NULL)
					{
						petroList.AddTail(pPData);
					}
					pPData = new CPetroData();
				}
				str.clear();
			}
			else
			{
				int result = CheckString(str);
				//if(result == 0)//����
				if(str != "LQRD" )
				{
					double num;
					num=atof(str.c_str());
					if(pPData != NULL)
					{
						if(strTitle == "DEPT")
						{
							//TRACE(_T("strTitle == 'DEPT' \r\n"));
							pPData->dept.iData = num;
							pPData->dept.strData = str.c_str();
							pPData->dept.bAssign = true;
							pPData->dept.strTag = strTitle.c_str();
							pPData->dept.strUnit = _T("m");
						}
						else if(strTitle == "TEMP")
						{
							//TRACE(_T("strTitle == 'TEMP' \r\n"));
							pPData->temp.iData = num;
							pPData->temp.strData = str.c_str();
							pPData->temp.bAssign = true;
							pPData->temp.strTag = strTitle.c_str();
							pPData->temp.strUnit = _T("��");
						}
						else if(strTitle == "RM")
						{
							//TRACE(_T("strTitle == 'RM' \r\n"));
							pPData->rm.iData = num;
							pPData->rm.strData = str.c_str();
							pPData->rm.bAssign = true;
							pPData->rm.strTag = strTitle.c_str();
							pPData->rm.strUnit = _T("��");
						}
						else if(strTitle == "GM")
						{
							//TRACE(_T("strTitle == 'GM' \r\n"));
							pPData->gr.iData = num;
							pPData->gr.strData = str.c_str();
							pPData->gr.bAssign = true;
							pPData->gr.strTag = strTitle.c_str();
							pPData->gr.strUnit = _T("��");
						}
						else if(strTitle == "MAGX")
						{
							//TRACE(_T("strTitle == 'MAGX' \r\n"));
							if(pPData->mag[0].iData == 0)
							{
								pPData->mag[0].iData = num;
								pPData->mag[0].strData = str.c_str();
								pPData->mag[0].bAssign = true;
								pPData->mag[0].strTag = strTitle.c_str();
								pPData->mag[0].strUnit = _T("��");
							}
							else if(pPData->mag[1].iData == 0)
							{
								pPData->mag[1].iData = num;
								pPData->mag[1].strData = str.c_str();
								pPData->mag[1].bAssign = true;
								pPData->mag[1].strTag = strTitle.c_str();
								pPData->mag[1].strUnit = _T("��");
							}
							else if(pPData->mag[2].iData == 0)
							{
								pPData->mag[2].iData = num;
								pPData->mag[2].strData = str.c_str();
								pPData->mag[2].bAssign = true;
								pPData->mag[2].strTag = strTitle.c_str();
								pPData->mag[2].strUnit = _T("��");
							}
							else
							{
							}
						}
						else if(strTitle == "CCL")
						{
							//TRACE(_T("strTitle == 'CCL' \r\n"));
							pPData->ccl.iData = num;
							pPData->ccl.strData = str.c_str();
							pPData->ccl.bAssign = true;
							pPData->ccl.strTag = strTitle.c_str();
							pPData->ccl.strUnit = _T("��");
						}
					}
				}
				str.clear();
				strTitle.empty();
			}
			if( tmp[i] == '*')
			{
				i+=2;
			}
		}
		else if(tmp[i] == 0x0D || tmp[i] == 0x0A)
		{
			continue;
		}
		else
		{
			str += tmp[i];
		}
	}
}

void CDMoniterDlg::pushToQueue(CPetroData* pPData)
{
	int nSize = petroList.GetSize();
	CPetroData* pCurrent = NULL;
	if(nSize >= maxqueue)
	{
		pCurrent = petroList.RemoveHead();
		delete pCurrent;
		pCurrent=NULL;
	}
	petroList.AddTail(pPData);
}
int CDMoniterDlg::CheckString( std::string str )
{
	bool bIsDigit = true;
	bool bIsPoint = false;
	int nCount = str.length(); // ����ַ�����
	for ( int i = 0; i < nCount; i ++ )
	{
		if ( 0 == isdigit( str[i] ) ) // �������־��ñ�־λ
		{
			if(str[i] == '.')//��С����
			{
				bIsPoint = true;
			}
			else
			{
				bIsDigit = false;
				break;// �˳�
			}
		}
	}
	if(!bIsDigit)
	{
		return 2;//������
	}
	else if(bIsPoint)
	{
		return 1;//��С����
	}
	else
	{
		return 0;//����
	}
}

void CDMoniterDlg::ClearPetroData()
{
	CPetroData* pCurrent = NULL;
	while(petroList.IsEmpty()==false)
	{
		pCurrent = petroList.RemoveHead();
		delete pCurrent;
		pCurrent=NULL;
	}
}
void CDMoniterDlg::InitArrayData()
{
	for(int i = 0;i<2;i++)
	{
		tempLimitArray[i] = 0;//0-min;1-max;
		gmLimitArray[i] = 0;
		rmLimitArray[i] = 0;
		cclLimitArray[i] = 0;
		magLimitArray[0][i] = 0;
		magLimitArray[1][i] = 0;
		magLimitArray[2][i] = 0;
	}
}
void CDMoniterDlg::InitOldArrayData()
{
	olddeptArray.dx = 0;
	olddeptArray.dy = 0;
	olddeptArray.bAssign = false;
	olddeptArray.strDx = _T("");

	oldtempArray.dx = 0;
	oldtempArray.dy = 0;
	oldtempArray.bAssign = false;
	oldtempArray.strDx = _T("");

	oldgmArray.dx = 0;
	oldgmArray.dy = 0;
	oldgmArray.bAssign = false;
	oldgmArray.strDx = _T("");

	oldrmArray.dx = 0;
	oldrmArray.dy = 0;
	oldrmArray.bAssign = false;
	oldrmArray.strDx = _T("");

	oldcclArray.dx = 0;
	oldcclArray.dy = 0;
	oldcclArray.bAssign = false;
	oldcclArray.strDx = _T("");

	oldmagArray[0].dx = 0;
	oldmagArray[0].dy = 0;
	oldmagArray[0].bAssign = false;
	oldmagArray[0].strDx = _T("");

	oldmagArray[1].dx = 0;
	oldmagArray[1].dy = 0;
	oldmagArray[1].bAssign = false;
	oldmagArray[1].strDx = _T("");

	oldmagArray[2].dx = 0;
	oldmagArray[2].dy = 0;
	oldmagArray[2].bAssign = false;
	oldmagArray[2].strDx = _T("");
}
void CDMoniterDlg::PrepareDraw()
{
	base = 0;
	bias = 0;
	counter = 0;
	m_iterator = 1;
	m_drawCount = 0;
	bScroll = false;
	m_step = 5;//ÿ���ƶ�����Ϊ5��
	pPData = NULL;
	pOldPData = NULL;
	pos = NULL;//��ǰ��¼λ��
	bTimer = true;
	InitOldArrayData();
	InitArrayData();
	if(petroList.IsEmpty())
	{
		return;//û�����ݲ����л���
	}
	if(m_bDirectDown)
	{
		pos = petroList.GetHeadPosition();
	}
	else
	{
		pos = petroList.GetTailPosition();
	}

	if(processType == REALTIME_PROCESSING)
	{
		minDepth = 0;
		maxDepth = 0;
		minDepthLimit = 0;
		maxDepthLimit = 0;
		bScroll = false;
		//AddPanelListView();
		//SetTimer(TIMER_CMD_DRAW,TIME_REFRESH_REALTIME,NULL);
	}
	else// if(theApp.processType == FILEDATA_PROCESSING)
	{
		minDepth = 0;
		maxDepth = 0;
		minDepthLimit = 0;
		maxDepthLimit = 0;
		bScroll = false;
		m_drawCount = TIME_REFRESH_FILE/20;
		GetMaxMinData();//�ڻ�ͼǰ����һ�μ���Ĳ���
		//AddPanelListView();
		if(m_bDirectDown)
		{
			minDepth = (int)minDepthLimit;
		}
		else
		{
			maxDepth = (int)maxDepthLimit+1;
		}
		SetTimer(TIMER_CMD_DRAW,TIME_REFRESH_FILE,NULL);
	}
}
void CDMoniterDlg::SetRealtimeDataLimit()
{
	tempLimitArray[0] = 0;
	tempLimitArray[1] = 60000;

	gmLimitArray[0] = 0;
	gmLimitArray[1] = 60000;

	rmLimitArray[0] = 0;
	rmLimitArray[1] = 60000;

	cclLimitArray[0] = 0;
	cclLimitArray[1] = 60000;

	magLimitArray[0][0] = 0;
	magLimitArray[0][1] = 60000;

	magLimitArray[1][0] = 0;
	magLimitArray[1][1] = 60000;

	magLimitArray[2][0] = 0;
	magLimitArray[2][1] = 60000;
}
void CDMoniterDlg::GetMaxMinData()
{
	if(!petroList.IsEmpty())
	{
		minDepthLimit = petroList.GetHead()->dept.iData;
		maxDepthLimit = minDepthLimit;
	}
	POSITION nPos = petroList.GetHeadPosition();
	while(nPos)
	{
		CPetroData* pData = petroList.GetNext(nPos);

		//���ҵ�ǰ��С���ֵ
		if(minDepthLimit > pData->dept.iData)
		{
			minDepthLimit = pData->dept.iData;
		}
		if(maxDepthLimit < pData->dept.iData)
		{
			maxDepthLimit = pData->dept.iData;
		}

		unsigned long m = 0;
		
		tempLimitArray[0] = GetMinData(pData->temp,tempLimitArray[0]);
		tempLimitArray[1] = max(tempLimitArray[1], pData->temp.iData/10*10+10);

		gmLimitArray[0] = GetMinData(pData->gr,gmLimitArray[0]);
		gmLimitArray[1] = max(gmLimitArray[1], pData->gr.iData/10*10+10);

		rmLimitArray[0] = GetMinData(pData->rm,rmLimitArray[0]);
		rmLimitArray[1] = max(rmLimitArray[1], pData->rm.iData/10*10+10);

		cclLimitArray[0] = GetMinData(pData->ccl,cclLimitArray[0]);
		cclLimitArray[1] = max(cclLimitArray[1], pData->ccl.iData/10*10+10);

		magLimitArray[0][0] = GetMinData(pData->mag[0],magLimitArray[0][0]);
		magLimitArray[0][1] = max(magLimitArray[0][1], pData->mag[0].iData/10*10+10);

		magLimitArray[1][0] = GetMinData(pData->mag[1],magLimitArray[1][0]);
		magLimitArray[1][1] = max(magLimitArray[1][1], pData->mag[1].iData/10*10+10);

		magLimitArray[2][0] = GetMinData(pData->mag[2],magLimitArray[2][0]);
		magLimitArray[2][1] = max(magLimitArray[2][1], pData->mag[2].iData/10*10+10);
	}
}

void CDMoniterDlg::DrawData(CDC* pDC)
{
	if(processType == REALTIME_PROCESSING)
	{
		DrawRealtimeBasic(pDC);//��ȡ�ڴ�ӳ����������С
		DrawRealtimeGrid(pDC);
		DrawRealtimePlot(pDC);
		DrawRealtimeCurve(pDC,minDepth,maxDepth);
	}
	else if(processType == FILEDATA_PROCESSING)
	{
		DrawFileDataBasic(pDC);//��ȡ�ڴ�ӳ����������С
		DrawFileDataGrid(pDC);
		DrawFileDataPlot(pDC);
		DrawFileDataCurve(pDC,minDepth,maxDepth);
	}
	else
	{
		DrawBasic(pDC);//��ȡ�ڴ�ӳ����������С
		DrawGrid(pDC);
		DrawPlot(pDC);
	}
}

void CDMoniterDlg::DrawCurve(CDC* pDC , double upDepth, double DownDepth)
{
	int iDrawType = PS_SOLID;
	long pre_iy=0,cur_iy=0,pre_dy=0,cur_dy=0;
	long pre_ix=0,cur_ix=0,pre_dx=0,cur_dx=0;
	
	CPen pen(iDrawType, 1,colorRed); //����
	CPen pen2(iDrawType, 1,colorBlue); //����2
	CPen pen3(iDrawType, 1,colorGreen); //����3
	CPen pen4(iDrawType, 1,colorCyan); //����4
	CPen pen5(iDrawType, 1,colorBlack); //����5
	CPen* pOldPen = pDC->SelectObject(&pen);//���ʺͻ���������
	int mCounter = 0;
	if(processType == REALTIME_PROCESSING)
	{
		if(m_bDirectDown)
		{
			pos = petroList.GetTailPosition();
			while(pos)
			{
				pPData = petroList.GetPrev(pos);
				if(pPData->dept.bAssign == true)
				{
					if(bScroll)
					{
						if(pPData->dept.iData > minDepth)
						{
							DrawDeptData(pDC,pPData,&pen);//���
							DrawTempData(pDC,pPData,&pen);//draw tepm
							DrawRmData(pDC,pPData,&pen2);//rm
							DrawGmData(pDC,pPData,&pen3);//gm
							DrawCclData(pDC,pPData,&pen4);//ccl
							DrawMagxData(pDC,pPData,&pen5);//magx
						}
						else
						{
							break;
						}
					}
					else
					{
						DrawDeptData(pDC,pPData,&pen);//���
						DrawTempData(pDC,pPData,&pen);//draw tepm
						DrawRmData(pDC,pPData,&pen2);//rm
						DrawGmData(pDC,pPData,&pen3);//gm
						DrawCclData(pDC,pPData,&pen4);//ccl
						DrawMagxData(pDC,pPData,&pen5);//magx
					}
				}
		
			}
			pPData = petroList.GetTail();
			//UpdatePanelListView(pPData);
		}
		else
		{
			pos = petroList.GetTailPosition();
			while(pos)
			{
				pPData = petroList.GetPrev(pos);
				if(pPData->dept.bAssign == true)
				{
					if(bScroll)
					{
						if(pPData->dept.iData < maxDepth)
						{
							DrawDeptData(pDC,pPData,&pen);//���
							DrawTempData(pDC,pPData,&pen);//draw tepm
							DrawRmData(pDC,pPData,&pen2);//rm
							DrawGmData(pDC,pPData,&pen3);//gm
							DrawCclData(pDC,pPData,&pen4);//ccl
							DrawMagxData(pDC,pPData,&pen5);//magx
						}
						else
						{
							break;
						}
					}
					else
					{
						DrawDeptData(pDC,pPData,&pen);//���
						DrawTempData(pDC,pPData,&pen);//draw tepm
						DrawRmData(pDC,pPData,&pen2);//rm
						DrawGmData(pDC,pPData,&pen3);//gm
						DrawCclData(pDC,pPData,&pen4);//ccl
						DrawMagxData(pDC,pPData,&pen5);//magx
					}
				}
		
			}
			//UpdatePanelListView(pPData);
		}	
	}
	else if(processType == FILEDATA_PROCESSING)
	{
		if(m_bDirectDown)
		{
			int TempPos = mScrollV.GetScrollPos();
			int pageMeter = m_clientRect.Height()/unitPixel;
			minDepth = (int)TempPos+minDepthLimit;
			maxDepth = (int)minDepth+pageMeter;

			pos = petroList.GetHeadPosition();
			while(pos)
			{
				pPData = petroList.GetNext(pos);
				if(pPData->dept.bAssign == true)
				{
					if(pPData->dept.iData < minDepth)
					{
						continue;
					}
					else
					{
						if(pPData->dept.iData < maxDepth)
						{
							if(pPData->dept.bAssign)
							{
								DrawDeptData(pDC,pPData,&pen);//���
								DrawTempData(pDC,pPData,&pen);//draw tepm
								DrawRmData(pDC,pPData,&pen2);//rm
								DrawGmData(pDC,pPData,&pen3);//gm
								DrawCclData(pDC,pPData,&pen4);//ccl
								DrawMagxData(pDC,pPData,&pen5);//magx
							}
						}
						else
						{
							break;
						}
					}
			
				}
		
			}
			pDC->SelectObject(pOldPen);
		}
		else
		{
			pos = petroList.GetTailPosition();
			while(pos)
			{
				pPData = petroList.GetPrev(pos);
				if(pPData->dept.bAssign == true)
				{
					if(pPData->dept.iData > maxDepth)
					{
						continue;
					}
					else
					{
						if(pPData->dept.iData > minDepth && (!bScroll))
						{
							if(mCounter < m_drawCount*m_iterator)
							{
								if(pPData->dept.bAssign)
								{
									DrawDeptData(pDC,pPData,&pen);//���
									DrawTempData(pDC,pPData,&pen);//draw tepm
									DrawRmData(pDC,pPData,&pen2);//rm
									DrawGmData(pDC,pPData,&pen3);//gm
									DrawCclData(pDC,pPData,&pen4);//ccl
									DrawMagxData(pDC,pPData,&pen5);//magx
								}
								mCounter++;
							}
							else
							{
								m_iterator++;
								pDC->SelectObject(pOldPen);
								break;
							}
						}
						else if(pPData->dept.iData > minDepth && bScroll)
						{
							if(pPData->dept.bAssign)
							{
								DrawDeptData(pDC,pPData,&pen);//���
								DrawTempData(pDC,pPData,&pen);//draw tepm
								DrawRmData(pDC,pPData,&pen2);//rm
								DrawGmData(pDC,pPData,&pen3);//gm
								DrawCclData(pDC,pPData,&pen4);//ccl
								DrawMagxData(pDC,pPData,&pen5);//magx
							}
						}
						else
						{
							if(!bScroll)
							{
								bScroll = true;
							}
							maxDepth =(int) (maxDepth - m_step);
							m_iterator = 1;
							mCounter = 0;
							break;
						}
					}
			
				}
		
			}
			if(pos == NULL)
			{
				KillTimer(TIMER_CMD_DRAW);//
			}
			else
			{
				//UpdatePanelListView(pPData);
				if(bTimer)
				{
					SetTimer(TIMER_CMD_DRAW,TIME_REFRESH_FILE,NULL);
				}
			}
		}	
	}
	else
	{

	}
}
void CDMoniterDlg::DrawRealtimeCurve(CDC* pDC , double upDepth, double DownDepth)
{
	int iDrawType = PS_SOLID;
	long pre_iy=0,cur_iy=0,pre_dy=0,cur_dy=0;
	long pre_ix=0,cur_ix=0,pre_dx=0,cur_dx=0;

	CPen pen(iDrawType, 1,colorRed); //����
	CPen pen2(iDrawType, 1,colorBlue); //����2
	CPen pen3(iDrawType, 1,colorGreen); //����3
	CPen pen4(iDrawType, 1,colorCyan); //����4
	CPen pen5(iDrawType, 1,colorBlack); //����5
	CPen* pOldPen = pDC->SelectObject(&pen);//���ʺͻ���������
	int mCounter = 0;
	if(processType == REALTIME_PROCESSING)
	{
		if(m_bDirectDown)
		{
			pos = petroList.GetTailPosition();
			while(pos)
			{
				pPData = petroList.GetPrev(pos);
				if(pPData->dept.bAssign == true)
				{
					if(pPData->dept.iData > minDepth)
					{
						DrawDeptData(pDC,pPData,&pen);//���
						DrawTempData(pDC,pPData,&pen);//draw tepm
						DrawRmData(pDC,pPData,&pen2);//rm
						DrawGmData(pDC,pPData,&pen3);//gm
						DrawCclData(pDC,pPData,&pen4);//ccl
						DrawMagxData(pDC,pPData,&pen5);//magx
					}
					else
					{
						break;
					}
				}
		
			}
			pPData = petroList.GetTail();
			UpdatePanelListView(pPData);
		}
		else
		{
			pos = petroList.GetTailPosition();
			while(pos)
			{
				pPData = petroList.GetPrev(pos);
				if(pPData->dept.bAssign == true)
				{
					if(pPData->dept.iData < maxDepth)
					{
						DrawDeptData(pDC,pPData,&pen);//���
						DrawTempData(pDC,pPData,&pen);//draw tepm
						DrawRmData(pDC,pPData,&pen2);//rm
						DrawGmData(pDC,pPData,&pen3);//gm
						DrawCclData(pDC,pPData,&pen4);//ccl
						DrawMagxData(pDC,pPData,&pen5);//magx
					}
					else
					{
						break;
					}
				}
		
			}
			UpdatePanelListView(pPData);
		}	
	}
	else if(processType == FILEDATA_PROCESSING)
	{
		if(m_bDirectDown)
		{
			int TempPos = mScrollV.GetScrollPos();
			int pageMeter = m_clientRect.Height()/unitPixel;
			minDepth = (int)TempPos+minDepthLimit;
			maxDepth = (int)minDepth+pageMeter;

			pos = petroList.GetHeadPosition();
			while(pos)
			{
				pPData = petroList.GetNext(pos);
				if(pPData->dept.bAssign == true)
				{
					if(pPData->dept.iData < minDepth)
					{
						continue;
					}
					else
					{
						if(pPData->dept.iData < maxDepth)
						{
							if(pPData->dept.bAssign)
							{
								DrawDeptData(pDC,pPData,&pen);//���
								DrawTempData(pDC,pPData,&pen);//draw tepm
								DrawRmData(pDC,pPData,&pen2);//rm
								DrawGmData(pDC,pPData,&pen3);//gm
								DrawCclData(pDC,pPData,&pen4);//ccl
								DrawMagxData(pDC,pPData,&pen5);//magx
							}
						}
						else
						{
							break;
						}
					}
			
				}
		
			}
			pDC->SelectObject(pOldPen);
		}
		else
		{
			pos = petroList.GetTailPosition();
			while(pos)
			{
				pPData = petroList.GetPrev(pos);
				if(pPData->dept.bAssign == true)
				{
					if(pPData->dept.iData > maxDepth)
					{
						continue;
					}
					else
					{
						if(pPData->dept.iData > minDepth && (!bScroll))
						{
							if(mCounter < m_drawCount*m_iterator)
							{
								if(pPData->dept.bAssign)
								{
									DrawDeptData(pDC,pPData,&pen);//���
									DrawTempData(pDC,pPData,&pen);//draw tepm
									DrawRmData(pDC,pPData,&pen2);//rm
									DrawGmData(pDC,pPData,&pen3);//gm
									DrawCclData(pDC,pPData,&pen4);//ccl
									DrawMagxData(pDC,pPData,&pen5);//magx
								}
								mCounter++;
							}
							else
							{
								m_iterator++;
								pDC->SelectObject(pOldPen);
								break;
							}
						}
						else if(pPData->dept.iData > minDepth && bScroll)
						{
							if(pPData->dept.bAssign)
							{
								DrawDeptData(pDC,pPData,&pen);//���
								DrawTempData(pDC,pPData,&pen);//draw tepm
								DrawRmData(pDC,pPData,&pen2);//rm
								DrawGmData(pDC,pPData,&pen3);//gm
								DrawCclData(pDC,pPData,&pen4);//ccl
								DrawMagxData(pDC,pPData,&pen5);//magx
							}
						}
						else
						{
							if(!bScroll)
							{
								bScroll = true;
							}
							maxDepth =(int) (maxDepth - m_step);
							m_iterator = 1;
							mCounter = 0;
							break;
						}
					}
			
				}
		
			}
			if(pos == NULL)
			{
				KillTimer(TIMER_CMD_DRAW);//
			}
			else
			{
				//UpdatePanelListView(pPData);
				if(bTimer)
				{
					SetTimer(TIMER_CMD_DRAW,TIME_REFRESH_FILE,NULL);
				}
			}
		}	
	}
	else
	{

	}
}
void CDMoniterDlg::DrawFileDataCurve(CDC* pDC , double upDepth, double DownDepth)
{
	int iDrawType = PS_SOLID;
	long pre_iy=0,cur_iy=0,pre_dy=0,cur_dy=0;
	long pre_ix=0,cur_ix=0,pre_dx=0,cur_dx=0;
	
	CPen pen(iDrawType, 1,colorRed); //����
	CPen pen2(iDrawType, 1,colorBlue); //����2
	CPen pen3(iDrawType, 1,colorGreen); //����3
	CPen pen4(iDrawType, 1,colorCyan); //����4
	CPen pen5(iDrawType, 1,colorBlack); //����5
	CPen* pOldPen = pDC->SelectObject(&pen);//���ʺͻ���������
	int mCounter = 0;
	if(processType == FILEDATA_PROCESSING)
	{
		if(m_bDirectDown)
		{
			int TempPos = mScrollV.GetScrollPos();
			int pageMeter = m_clientRect.Height()/unitPixel;
			minDepth = (int)TempPos+minDepthLimit;
			maxDepth = (int)minDepth+pageMeter;

			pos = petroList.GetHeadPosition();
			while(pos)
			{
				pPData = petroList.GetNext(pos);
				if(pPData->dept.bAssign == true)
				{
					if(pPData->dept.iData < minDepth)
					{
						continue;
					}
					else
					{
						if(pPData->dept.iData < maxDepth)
						{
							if(pPData->dept.bAssign)
							{
								DrawDeptData(pDC,pPData,&pen);//���
								DrawTempData(pDC,pPData,&pen);//draw tepm
								DrawRmData(pDC,pPData,&pen2);//rm
								DrawGmData(pDC,pPData,&pen3);//gm
								DrawCclData(pDC,pPData,&pen4);//ccl
								DrawMagxData(pDC,pPData,&pen5);//magx
							}
						}
						else
						{
							break;
						}
					}
			
				}
			}
			UpdatePanelListView(pPData);
			pDC->SelectObject(pOldPen);
		}
		else
		{
			int TempPos = mScrollV.GetScrollPos();
			int pageMeter = m_clientRect.Height()/unitPixel;
			minDepth = (int)TempPos+minDepthLimit;
			maxDepth = (int)minDepth+pageMeter;
			pos = petroList.GetTailPosition();
			while(pos)
			{
				pPData = petroList.GetPrev(pos);
				if(pPData->dept.bAssign == true)
				{
					if(pPData->dept.iData > maxDepth)
					{
						continue;
					}
					else
					{
						if(pPData->dept.iData > minDepth)
						{

								if(pPData->dept.bAssign)
								{
									DrawDeptData(pDC,pPData,&pen);//���
									DrawTempData(pDC,pPData,&pen);//draw tepm
									DrawRmData(pDC,pPData,&pen2);//rm
									DrawGmData(pDC,pPData,&pen3);//gm
									DrawCclData(pDC,pPData,&pen4);//ccl
									DrawMagxData(pDC,pPData,&pen5);//magx
								}
						}
						else
						{
							break;
						}
					}
			
				}
		
			}
			UpdatePanelListView(pPData);
			pDC->SelectObject(pOldPen);
		}	
	}
}
void CDMoniterDlg::DrawDeptData(CDC* pDC ,CPetroData* pPData,CPen* pPpen)
{
	if(olddeptArray.bAssign)
	{
							
	}
	else
	{
						
	}
}
void CDMoniterDlg::DrawTempData(CDC* pDC ,CPetroData* pPData,CPen* pPpen)
{
	long pre_iy=0,cur_iy=0;
	long pre_ix=0,cur_ix=0;
	if(pPData->temp.bAssign)
	{
		if(oldtempArray.bAssign)
		{
			pDC->SelectObject(pPpen);
			long tempRange = tempLimitArray[1]-tempLimitArray[0];
			int i_dx = (int)((oldtempArray.dx - (int)oldtempArray.dx)*10);
			int d_dx = (int)(oldtempArray.dx);
			pre_ix = (int)((d_dx-tempLimitArray[0])*m_plot1Rect.Width()/tempRange
				+i_dx*m_plot1Rect.Width()/(10*tempRange));
			double mid = (pPData->temp.iData-tempLimitArray[0])*m_plot1Rect.Width()/tempRange;
			cur_ix = (int)mid;
			pre_iy = (oldtempArray.dy - minDepth)*unitPixel;
			cur_iy = (pPData->dept.iData - minDepth)*unitPixel;
			pDC->MoveTo(pre_ix,pre_iy);
			pDC->LineTo(cur_ix,cur_iy);
		}
		//else
		{
			oldtempArray.bAssign = pPData->temp.bAssign;
			oldtempArray.dx = pPData->temp.iData;
			oldtempArray.dy = pPData->dept.iData;
			oldtempArray.strDx = pPData->temp.strData;
		}
	}
}
void CDMoniterDlg::DrawRmData(CDC* pDC ,CPetroData* pPData,CPen* pPpen)
{
	long pre_iy=0,cur_iy=0;
	long pre_ix=0,cur_ix=0;
	if(pPData->rm.bAssign)
	{
		if(oldtempArray.bAssign)
		{
			pDC->SelectObject(pPpen);
			long rmRange = rmLimitArray[1]-rmLimitArray[0];
			int i_dx = (int)((oldrmArray.dx - (int)oldrmArray.dx)*10);
			int d_dx = (int)(oldrmArray.dx);
			pre_ix = (int)((d_dx-rmLimitArray[0])*m_plot1Rect.Width()/rmRange
				+i_dx*m_plot1Rect.Width()/(10*rmRange));
			double mid = (pPData->rm.iData-rmLimitArray[0])*m_plot1Rect.Width()/rmRange;
			cur_ix = (int)mid;
			pre_iy = (oldrmArray.dy - minDepth)*unitPixel;
			cur_iy = (pPData->dept.iData - minDepth)*unitPixel;
			pDC->MoveTo(pre_ix,pre_iy);
			pDC->LineTo(cur_ix,cur_iy);
		}
		//else
		{
			oldrmArray.bAssign = pPData->rm.bAssign;
			oldrmArray.dx = pPData->rm.iData;
			oldrmArray.dy = pPData->dept.iData;
			oldrmArray.strDx = pPData->rm.strData;
		}
	}
}
//���ƴ�����������
void CDMoniterDlg::DrawMagxData(CDC* pDC ,CPetroData* pPData,CPen* pPpen)
{
	int iDrawType = PS_SOLID;
	COLORREF color1 = RGB(255,0,255);
	COLORREF color2 = RGB(0,255,255);

	CPen penmag0(iDrawType, 1,color1); //����
	CPen penmag1(iDrawType, 1,color1); //����2
	CPen penmag2(iDrawType, 1,color2); //����3
	
	long pre_iy=0,cur_iy=0;
	long pre_ix=0,cur_ix=0;
	//0
	if(pPData->mag[0].bAssign)
	{
		if(oldmagArray[0].bAssign)
		{
			pDC->SelectObject(pPpen);
			long magRange = magLimitArray[0][1]-magLimitArray[0][0];
			int i_dx = (int)((oldmagArray[0].dx - (int)oldmagArray[0].dx)*10);
			int d_dx = (int)(oldmagArray[0].dx);
			pre_ix = (int)((d_dx-magLimitArray[0][0])*m_plot1Rect.Width()/magRange
				+i_dx*m_plot1Rect.Width()/(10*magRange));
			double mid = (pPData->mag[0].iData-magLimitArray[0][0])*m_plot1Rect.Width()/magRange;
			cur_ix = (int)mid;
			pre_iy = (oldmagArray[0].dy - minDepth)*unitPixel;
			cur_iy = (pPData->dept.iData - minDepth)*unitPixel;
			pDC->MoveTo(pre_ix,pre_iy);
			pDC->LineTo(cur_ix,cur_iy);
		}
		//else
		{
			oldmagArray[0].bAssign = pPData->mag[0].bAssign;
			oldmagArray[0].dx = pPData->mag[0].iData;
			oldmagArray[0].dy = pPData->dept.iData;
			oldmagArray[0].strDx = pPData->mag[0].strData;
		}
	}
	//1
	if(pPData->mag[1].bAssign)
	{
		if(oldmagArray[1].bAssign)
		{
			pDC->SelectObject(&penmag1);
			long magRange = magLimitArray[1][1]-magLimitArray[1][0];
			int i_dx = (int)((oldmagArray[1].dx - (int)oldmagArray[1].dx)*10);
			int d_dx = (int)(oldmagArray[1].dx);
			pre_ix = (int)((d_dx-magLimitArray[1][0])*m_plot1Rect.Width()/magRange
				+i_dx*m_plot1Rect.Width()/(10*magRange));
			double mid = (pPData->mag[1].iData-magLimitArray[1][0])*m_plot1Rect.Width()/magRange;
			cur_ix = (int)mid;
			pre_iy = (oldmagArray[1].dy - minDepth)*unitPixel;
			cur_iy = (pPData->dept.iData - minDepth)*unitPixel;
			pDC->MoveTo(pre_ix,pre_iy);
			pDC->LineTo(cur_ix,cur_iy);
		}
		//else
		{
			oldmagArray[1].bAssign = pPData->mag[1].bAssign;
			oldmagArray[1].dx = pPData->mag[1].iData;
			oldmagArray[1].dy = pPData->dept.iData;
			oldmagArray[1].strDx = pPData->mag[1].strData;
		}
	}
	//2
	if(pPData->mag[2].bAssign)
	{
		if(oldmagArray[2].bAssign)
		{
			pDC->SelectObject(&penmag2);
			long magRange = magLimitArray[2][1]-magLimitArray[2][0];
			int i_dx = (int)((oldmagArray[2].dx - (int)oldmagArray[2].dx)*10);
			int d_dx = (int)(oldmagArray[2].dx);
			pre_ix = (int)((d_dx-magLimitArray[2][0])*m_plot1Rect.Width()/magRange
				+i_dx*m_plot1Rect.Width()/(10*magRange));
			double mid = (pPData->mag[2].iData-magLimitArray[2][0])*m_plot1Rect.Width()/magRange;
			cur_ix = (int)mid;
			pre_iy = (oldmagArray[2].dy - minDepth)*unitPixel;
			cur_iy = (pPData->dept.iData - minDepth)*unitPixel;
			pDC->MoveTo(pre_ix,pre_iy);
			pDC->LineTo(cur_ix,cur_iy);
		}
		//else
		{
			oldmagArray[2].bAssign = pPData->mag[2].bAssign;
			oldmagArray[2].dx = pPData->mag[2].iData;
			oldmagArray[2].dy = pPData->dept.iData;
			oldmagArray[2].strDx = pPData->mag[2].strData;
		}
	}
}
//����Gm����
void CDMoniterDlg::DrawGmData(CDC* pDC ,CPetroData* pPData,CPen* pPpen)
{
	long pre_iy=0,cur_iy=0;
	long pre_ix=0,cur_ix=0;
	if(pPData->gr.bAssign)
	{
		if(oldgmArray.bAssign)
		{
			pDC->SelectObject(pPpen);
			long gmRange = gmLimitArray[1]-gmLimitArray[0];
			int i_dx = (int)((oldgmArray.dx - (int)oldgmArray.dx)*10);
			int d_dx = (int)(oldgmArray.dx);
			pre_ix = (int)((d_dx-gmLimitArray[0])*m_plot1Rect.Width()/gmRange
				+i_dx*m_plot1Rect.Width()/(10*gmRange));
			double mid = (pPData->gr.iData-gmLimitArray[0])*m_plot1Rect.Width()/gmRange;
			cur_ix = (int)mid;
			pre_iy = (oldgmArray.dy - minDepth)*unitPixel;
			cur_iy = (pPData->dept.iData - minDepth)*unitPixel;
			pDC->MoveTo(pre_ix,pre_iy);
			pDC->LineTo(cur_ix,cur_iy);
		}
		//else
		{
			oldgmArray.bAssign = pPData->gr.bAssign;
			oldgmArray.dx = pPData->gr.iData;
			oldgmArray.dy = pPData->dept.iData;
			oldgmArray.strDx = pPData->gr.strData;
		}
	}
}
//����Ccl����
void CDMoniterDlg::DrawCclData(CDC* pDC ,CPetroData* pPData,CPen* pPpen)
{
	long pre_iy=0,cur_iy=0;
	long pre_ix=0,cur_ix=0;
	if(pPData->ccl.bAssign)
	{
		if(oldcclArray.bAssign)
		{
			pDC->SelectObject(pPpen);
			long cclRange = cclLimitArray[1]-cclLimitArray[0];
			int i_dx = (int)((oldcclArray.dx - (int)oldcclArray.dx)*10);
			int d_dx = (int)(oldcclArray.dx);
			pre_ix = (int)((d_dx-cclLimitArray[0])*m_plot1Rect.Width()/cclRange
				+i_dx*m_plot1Rect.Width()/(10*cclRange));
			double mid = (pPData->ccl.iData-cclLimitArray[0])*m_plot1Rect.Width()/cclRange;
			cur_ix = (int)mid;
			pre_iy = (oldcclArray.dy - minDepth)*unitPixel;
			cur_iy = (pPData->dept.iData - minDepth)*unitPixel;
			pDC->MoveTo(pre_ix,pre_iy);
			pDC->LineTo(cur_ix,cur_iy);
		}
		//else
		{
			oldcclArray.bAssign = pPData->ccl.bAssign;
			oldcclArray.dx = pPData->ccl.iData;
			oldcclArray.dy = pPData->dept.iData;
			oldcclArray.strDx = pPData->ccl.strData;
		}
	}
}

unsigned long CDMoniterDlg::GetMinData(DATA_PART tmp,unsigned long m)
{
	unsigned long t = 0;
	if(tmp.bAssign == false)
	{
		return m;
	}
	if(tmp.iData > 0)
	{
		if(m == 0)
		{
			t = tmp.iData/10*10;
		}
		else
		{
			t = min(m,tmp.iData/10*10);
		}
		return t;
	}
	else
	{
		return m;
	}
}
void CDMoniterDlg::DrawBasic(CDC * pDC)
{
	m_totalRect = rectView;
	maxPreDepth = maxDepth;
	int depPixel = 0;

	if(processType == NO_PROCESSING)
	{
		m_totalRect.bottom = m_totalRect.top + max(depPixel,m_totalRect.Height());
		mScrollV.SetScrollRange(0,m_totalRect.bottom-m_totalRect.top);
		mScrollV.SetScrollPos(0);
	
	}
}
void CDMoniterDlg::DrawRealtimeBasic(CDC * pDC)
{
	m_totalRect = rectView;
	maxPreDepth = maxDepth;
	int depPixel = 0;
	if(processType == REALTIME_PROCESSING)
	{
		CPetroData * pHeadData = petroList.GetHead();
		CPetroData * pTailData = petroList.GetTail();
		
		if(m_bDirectDown)
		{
			minDepthLimit = pHeadData->dept.iData;
			maxDepthLimit = pTailData->dept.iData;
			if(bScroll)
			{
				maxDepth = maxDepthLimit;
				minDepth = maxDepth - m_clientRect.Height()/unitPixel;
				//depPixel = (int)((maxDepth - minDepthLimit)*10/10) * unitPixel;
			}
			else
			{
				//minDepth = minDepthLimit;
				maxDepth = maxDepthLimit;//minDepth + m_clientRect.Height()/unitPixel;
				minDepth = maxDepth - m_clientRect.Height()/unitPixel;
				//depPixel = (int)((maxDepth - minDepthLimit)*10/10) * unitPixel;
			}
		}
		else
		{
			maxDepthLimit = pHeadData->dept.iData;
			minDepthLimit = pTailData->dept.iData;
			if(bScroll)
			{
				minDepth = minDepthLimit;
				maxDepth = minDepth + m_clientRect.Height()/unitPixel;
				//depPixel = (int)((maxDepth - minDepthLimit)*10/10) * unitPixel;
			}
			else
			{
				//maxDepth = maxDepthLimit;
				//minDepth = maxDepth - m_clientRect.Height()/unitPixel;
				minDepth = minDepthLimit;
				maxDepth = minDepth + m_clientRect.Height()/unitPixel;
				//depPixel = (int)((maxDepth - minDepthLimit)*10/10) * unitPixel;
				
			}
		}
		//m_totalRect.bottom = m_totalRect.top + max(depPixel,m_totalRect.Height());
	
		mScrollV.SetScrollRange(0,0);
		mScrollV.SetScrollPos(0);
	}
}
void CDMoniterDlg::DrawFileDataBasic(CDC * pDC)
{
	m_totalRect = rectView;
	maxPreDepth = maxDepth;
	int depPixel = 0;
	if(processType == FILEDATA_PROCESSING)
	{

	}
}
void CDMoniterDlg::DrawGrid(CDC * pDC)
{
	m_plot1Rect = m_clientRect;
	m_plot1Rect.right = m_plot1Rect.left + 400;

	m_plot2Rect = m_clientRect;
	m_plot2Rect.left = m_plot1Rect.right;
	m_plot2Rect.right = m_plot2Rect.left + 150;

	m_plot3Rect = m_clientRect;
	m_plot3Rect.left = m_plot2Rect.right;
	m_plot3Rect.right = m_plot3Rect.left + 400;


	CPen *old, pen(PS_SOLID, 1, crViewGridColor), pen2(PS_DOT, 1,crViewGridColor); //����;
	CPen stick(PS_SOLID,0,RGB(0,0,0));
	CPen mline(PS_SOLID,0,RGB(192,192,192));
	old = pDC->SelectObject(&pen);
	//��������ϵ
	int iCount=0;

	for (int i=m_plot1Rect.left,iCount= 0;i<=m_plot1Rect.right;i+=unitPixel*unitRatio,iCount++)
	{
		if(iCount != 0 && iCount%5 == 0)
		{
			pDC->SelectObject(&pen);
			pDC->MoveTo(i,m_plot1Rect.top); //
			pDC->LineTo(i,m_plot1Rect.bottom);
		}
		else
		{
			pDC->SelectObject(&pen2);
			pDC->MoveTo(i,m_plot1Rect.top); // [0,0]
			pDC->LineTo(i,m_plot1Rect.bottom);
		}
	}
	for (int i=m_plot3Rect.left,iCount = 0;i<=m_plot3Rect.right;i+=unitPixel*unitRatio,iCount++)
	{
		
		if(iCount%5 == 0)
		{
			pDC->SelectObject(&pen);
			pDC->MoveTo(i,m_plot3Rect.top);
			pDC->LineTo(i,m_plot3Rect.bottom);
		}
		else
		{
			pDC->SelectObject(&pen2);
			pDC->MoveTo(i,m_plot3Rect.top);
			pDC->LineTo(i,m_plot3Rect.bottom);
		}
	}
	for (int j=m_plot1Rect.top,iCount = 0;j<=m_plot3Rect.bottom;j+=unitPixel,iCount++)
	{
		
		if(iCount != 0 && (iCount)%10 == 0)
		{
			pDC->SelectObject(&pen);
			pDC->MoveTo(m_plot1Rect.left,j); 
			pDC->LineTo(m_plot1Rect.right,j);
		}
		else if(iCount != 0 && (iCount)%5 == 0)
		{
			pDC->SelectObject(&pen2);
			pDC->MoveTo(m_plot1Rect.left,j); 
			pDC->LineTo(m_plot1Rect.right,j);
		}
	}
	for (int j=m_plot3Rect.top,iCount = 0;j<=m_plot3Rect.bottom;j+=unitPixel,iCount++)
	{
		if(iCount != 0 && (iCount)%10 == 0)
		{
			pDC->SelectObject(&pen);
			pDC->MoveTo(m_plot3Rect.left,j);
			pDC->LineTo(m_plot3Rect.right,j);
		}
		else if(iCount != 0 && (iCount)%5 == 0)
		{
			pDC->SelectObject(&pen2);
			pDC->MoveTo(m_plot3Rect.left,j); 
			pDC->LineTo(m_plot3Rect.right,j);
		}
	}
	//����ˢ��
	pDC->SelectObject(old);
}
void CDMoniterDlg::DrawRealtimeGrid(CDC * pDC)
{
	m_plot1Rect = m_clientRect;
	m_plot1Rect.right = m_plot1Rect.left + 400;

	m_plot2Rect = m_clientRect;
	m_plot2Rect.left = m_plot1Rect.right;
	m_plot2Rect.right = m_plot2Rect.left + 150;

	m_plot3Rect = m_clientRect;
	m_plot3Rect.left = m_plot2Rect.right;
	m_plot3Rect.right = m_plot3Rect.left + 400;

	CPen *old, pen(PS_SOLID, 1, crViewGridColor), pen2(PS_DOT, 1,crViewGridColor); //����;
	CPen stick(PS_SOLID,0,RGB(0,0,0));
	CPen mline(PS_SOLID,0,RGB(192,192,192));
	old = pDC->SelectObject(&pen);
	//��������ϵ
	int iCount=0;

	for (int i=m_plot1Rect.left,iCount= 0;i<=m_plot1Rect.right;i+=unitPixel*unitRatio,iCount++)
	{
		if(iCount != 0 && iCount%5 == 0)
		{
			pDC->SelectObject(&pen);
			pDC->MoveTo(i,m_plot1Rect.top); //
			pDC->LineTo(i,m_plot1Rect.bottom);
		}
		else
		{
			pDC->SelectObject(&pen2);
			pDC->MoveTo(i,m_plot1Rect.top); // [0,0]
			pDC->LineTo(i,m_plot1Rect.bottom);
		}
	}
	for (int i=m_plot3Rect.left,iCount = 0;i<=m_plot3Rect.right;i+=unitPixel*unitRatio,iCount++)
	{
		
		if(iCount%5 == 0)
		{
			pDC->SelectObject(&pen);
			pDC->MoveTo(i,m_plot3Rect.top);
			pDC->LineTo(i,m_plot3Rect.bottom);
		}
		else
		{
			pDC->SelectObject(&pen2);
			pDC->MoveTo(i,m_plot3Rect.top);
			pDC->LineTo(i,m_plot3Rect.bottom);
		}
	}
	for (int j=m_plot1Rect.top,iCount = 0;j<=m_plot3Rect.bottom;j+=unitPixel,iCount++)
	{
		
		if(iCount != 0 && (iCount)%10 == 0)
		{
			pDC->SelectObject(&pen);
			pDC->MoveTo(m_plot1Rect.left,j); 
			pDC->LineTo(m_plot1Rect.right,j);
		}
		else if(iCount != 0 && (iCount)%5 == 0)
		{
			pDC->SelectObject(&pen2);
			pDC->MoveTo(m_plot1Rect.left,j); 
			pDC->LineTo(m_plot1Rect.right,j);
		}
	}
	for (int j=m_plot3Rect.top,iCount = 0;j<=m_plot3Rect.bottom;j+=unitPixel,iCount++)
	{
		if(iCount != 0 && (iCount)%10 == 0)
		{
			pDC->SelectObject(&pen);
			pDC->MoveTo(m_plot3Rect.left,j);
			pDC->LineTo(m_plot3Rect.right,j);
		}
		else if(iCount != 0 && (iCount)%5 == 0)
		{
			pDC->SelectObject(&pen2);
			pDC->MoveTo(m_plot3Rect.left,j); 
			pDC->LineTo(m_plot3Rect.right,j);
		}
	}
	//����ˢ��
	pDC->SelectObject(old);
}
void CDMoniterDlg::DrawFileDataGrid(CDC * pDC)
{
	m_plot1Rect = m_clientRect;
	m_plot1Rect.right = m_plot1Rect.left + 400;

	m_plot2Rect = m_clientRect;
	m_plot2Rect.left = m_plot1Rect.right;
	m_plot2Rect.right = m_plot2Rect.left + 150;

	m_plot3Rect = m_clientRect;
	m_plot3Rect.left = m_plot2Rect.right;
	m_plot3Rect.right = m_plot3Rect.left + 400;

	CPen *old, pen(PS_SOLID, 1, crViewGridColor), pen2(PS_DOT, 1,crViewGridColor); //����;
	CPen stick(PS_SOLID,0,RGB(0,0,0));
	CPen mline(PS_SOLID,0,RGB(192,192,192));
	old = pDC->SelectObject(&pen);
	//��������ϵ
	int iCount=0;

	for (int i=m_plot1Rect.left,iCount= 0;i<=m_plot1Rect.right;i+=unitPixel*unitRatio,iCount++)
	{
		if(iCount != 0 && iCount%5 == 0)
		{
			pDC->SelectObject(&pen);
			pDC->MoveTo(i,m_plot1Rect.top); //
			pDC->LineTo(i,m_plot1Rect.bottom);
		}
		else
		{
			pDC->SelectObject(&pen2);
			pDC->MoveTo(i,m_plot1Rect.top); // [0,0]
			pDC->LineTo(i,m_plot1Rect.bottom);
		}
	}
	for (int i=m_plot3Rect.left,iCount = 0;i<=m_plot3Rect.right;i+=unitPixel*unitRatio,iCount++)
	{
		
		if(iCount%5 == 0)
		{
			pDC->SelectObject(&pen);
			pDC->MoveTo(i,m_plot3Rect.top);
			pDC->LineTo(i,m_plot3Rect.bottom);
		}
		else
		{
			pDC->SelectObject(&pen2);
			pDC->MoveTo(i,m_plot3Rect.top);
			pDC->LineTo(i,m_plot3Rect.bottom);
		}
	}
	for (int j=m_plot1Rect.top,iCount = 0;j<=m_plot3Rect.bottom;j+=unitPixel,iCount++)
	{
		
		if(iCount != 0 && (iCount)%10 == 0)
		{
			pDC->SelectObject(&pen);
			pDC->MoveTo(m_plot1Rect.left,j); 
			pDC->LineTo(m_plot1Rect.right,j);
		}
		else if(iCount != 0 && (iCount)%5 == 0)
		{
			pDC->SelectObject(&pen2);
			pDC->MoveTo(m_plot1Rect.left,j); 
			pDC->LineTo(m_plot1Rect.right,j);
		}
	}
	for (int j=m_plot3Rect.top,iCount = 0;j<=m_plot3Rect.bottom;j+=unitPixel,iCount++)
	{
		if(iCount != 0 && (iCount)%10 == 0)
		{
			pDC->SelectObject(&pen);
			pDC->MoveTo(m_plot3Rect.left,j);
			pDC->LineTo(m_plot3Rect.right,j);
		}
		else if(iCount != 0 && (iCount)%5 == 0)
		{
			pDC->SelectObject(&pen2);
			pDC->MoveTo(m_plot3Rect.left,j); 
			pDC->LineTo(m_plot3Rect.right,j);
		}
	}
	//����ˢ��
	pDC->SelectObject(old);
}
void CDMoniterDlg::DrawPlot(CDC* pDC)
{
	m_zoomFont.lfHeight			= -13;
	m_zoomFont.lfWidth			= 0;
	m_zoomFont.lfEscapement		= 0;
	m_zoomFont.lfOrientation		= 0;
	m_zoomFont.lfWeight			= 400;
	m_zoomFont.lfItalic			= FALSE;
	m_zoomFont.lfUnderline		= FALSE;
	m_zoomFont.lfStrikeOut		= FALSE;
	m_zoomFont.lfCharSet			= ANSI_CHARSET;
	m_zoomFont.lfOutPrecision	= OUT_DEFAULT_PRECIS;
	m_zoomFont.lfClipPrecision	= CLIP_DEFAULT_PRECIS;
	m_zoomFont.lfQuality			= PROOF_QUALITY;
	m_zoomFont.lfPitchAndFamily	= DEFAULT_PITCH;
	wcscpy(m_zoomFont.lfFaceName,_T("Ariel"));
	
	m_font.Detach();
	m_font.CreateFontIndirect(&m_zoomFont);
	CFont *oFont = pDC->SelectObject(&m_font);
	CRect textRect;
	CSize z=pDC->GetTextExtent(CString("A"));
	textRect = m_plot2Rect;
	textRect.left += 2;
	textRect.right -= 2;
	CString str;
	int TempPos = mScrollV.GetScrollPos();
	int pageMeter = m_clientRect.Height()/unitPixel;
	minDepth = (int)TempPos+minDepthLimit;
	maxDepth = (int)minDepth+pageMeter;
	for (int i=m_plot2Rect.top,iCount = 0;i<=m_plot2Rect.bottom;i+=unitPixel*10,iCount++)
	{
		textRect.top = m_plot2Rect.top + i-z.cy/2;
		textRect.bottom = textRect.top + z.cy;
		double tmp = i/unitPixel+minDepth;
		str.Format(_T("%.1f"),tmp);
		pDC->DrawText(str,textRect,DT_LEFT|DT_VCENTER);

	}
	pDC->SelectObject(oFont);
}
void CDMoniterDlg::DrawRealtimePlot(CDC* pDC)
{
	m_zoomFont.lfHeight			= -13;
	m_zoomFont.lfWidth			= 0;
	m_zoomFont.lfEscapement		= 0;
	m_zoomFont.lfOrientation		= 0;
	m_zoomFont.lfWeight			= 400;
	m_zoomFont.lfItalic			= FALSE;
	m_zoomFont.lfUnderline		= FALSE;
	m_zoomFont.lfStrikeOut		= FALSE;
	m_zoomFont.lfCharSet			= ANSI_CHARSET;
	m_zoomFont.lfOutPrecision	= OUT_DEFAULT_PRECIS;
	m_zoomFont.lfClipPrecision	= CLIP_DEFAULT_PRECIS;
	m_zoomFont.lfQuality			= PROOF_QUALITY;
	m_zoomFont.lfPitchAndFamily	= DEFAULT_PITCH;
	wcscpy(m_zoomFont.lfFaceName,_T("Ariel"));
	
	m_font.Detach();
	m_font.CreateFontIndirect(&m_zoomFont);
	CFont *oFont = pDC->SelectObject(&m_font);
	CRect textRect;
	CSize z=pDC->GetTextExtent(CString("A"));
	textRect = m_plot2Rect;
	textRect.left += 2;
	textRect.right -= 2;
	CString str;
	int TempPos = mScrollV.GetScrollPos();
	int pageMeter = m_clientRect.Height()/unitPixel;
	//minDepth = (int)TempPos+minDepthLimit;
	//maxDepth = (int)minDepth+pageMeter;
	for (int i=m_plot2Rect.top,iCount = 0;i<=m_plot2Rect.bottom;i+=unitPixel*10,iCount++)
	{
		textRect.top = m_plot2Rect.top + i-z.cy/2;
		textRect.bottom = textRect.top + z.cy;
		double tmp = i/unitPixel+minDepth;
		str.Format(_T("%.1f"),tmp);
		pDC->DrawText(str,textRect,DT_LEFT|DT_VCENTER);

	}
	pDC->SelectObject(oFont);
}
void CDMoniterDlg::DrawFileDataPlot(CDC* pDC)
{
	m_zoomFont.lfHeight			= -13;
	m_zoomFont.lfWidth			= 0;
	m_zoomFont.lfEscapement		= 0;
	m_zoomFont.lfOrientation		= 0;
	m_zoomFont.lfWeight			= 400;
	m_zoomFont.lfItalic			= FALSE;
	m_zoomFont.lfUnderline		= FALSE;
	m_zoomFont.lfStrikeOut		= FALSE;
	m_zoomFont.lfCharSet			= ANSI_CHARSET;
	m_zoomFont.lfOutPrecision	= OUT_DEFAULT_PRECIS;
	m_zoomFont.lfClipPrecision	= CLIP_DEFAULT_PRECIS;
	m_zoomFont.lfQuality			= PROOF_QUALITY;
	m_zoomFont.lfPitchAndFamily	= DEFAULT_PITCH;
	wcscpy(m_zoomFont.lfFaceName,_T("Ariel"));
	
	m_font.Detach();
	m_font.CreateFontIndirect(&m_zoomFont);
	CFont *oFont = pDC->SelectObject(&m_font);
	CRect textRect;
	CSize z=pDC->GetTextExtent(CString("A"));
	textRect = m_plot2Rect;
	textRect.left += 2;
	textRect.right -= 2;
	CString str;
	int TempPos = mScrollV.GetScrollPos();
	int pageMeter = m_clientRect.Height()/unitPixel;
	minDepth = (int)TempPos+minDepthLimit;
	maxDepth = (int)minDepth+pageMeter;
	for (int i=m_plot2Rect.top,iCount = 0;i<=m_plot2Rect.bottom;i+=unitPixel*10,iCount++)
	{
		textRect.top = m_plot2Rect.top + i-z.cy/2;
		textRect.bottom = textRect.top + z.cy;
		double tmp = i/unitPixel+minDepth;
		str.Format(_T("%.1f"),tmp);
		pDC->DrawText(str,textRect,DT_LEFT|DT_VCENTER);

	}
	pDC->SelectObject(oFont);
}
void CDMoniterDlg::CalculateParam()
{
	CPetroData * pHeadData = petroList.GetHead();
	CPetroData * pTailData = petroList.GetTail();
	if(pHeadData->dept.iData < pTailData->dept.iData)
	{
		m_bDirectDown = true;
	}
	else
	{
		m_bDirectDown = false;
	}
	
}

void CDMoniterDlg::StartTimer()
{
	base = 0;
	bias = 0;
	counter = 0;
	m_iterator = 1;
	m_drawCount = 0;
	bScroll = false;
	m_step = 5;//ÿ���ƶ�����Ϊ5��
	pPData = NULL;
	pOldPData = NULL;
	pos = NULL;//��ǰ��¼λ��
	bTimer = true;
	InitOldArrayData();
	InitArrayData();

	if(processType == REALTIME_PROCESSING)
	{
		minDepth = 0;
		maxDepth = 0;
		minDepthLimit = 0;
		maxDepthLimit = 0;
		bScroll = false;
		SetRealtimeDataLimit();
		//AddPanelListView();
		SetTimer(TIMER_CMD_DRAW,TIME_REFRESH_REALTIME,NULL);
	}
	else if(processType == FILEDATA_PROCESSING)
	{
		if(petroList.IsEmpty())
		{
			return;//û�����ݲ����л���
		}
		if(m_bDirectDown)
		{
			pos = petroList.GetHeadPosition();
		}
		else
		{
			pos = petroList.GetTailPosition();
		}
		minDepth = 0;
		maxDepth = 0;
		minDepthLimit = 0;
		maxDepthLimit = 0;
		bScroll = false;
		m_drawCount = TIME_REFRESH_FILE/20;
		GetMaxMinData();//�ڻ�ͼǰ����һ�μ���Ĳ���
		AddPanelListView();
		int pageMeter = m_clientRect.Height()/unitPixel;
		if(m_bDirectDown)
		{
			mScrollV.SetScrollRange(0,(maxDepthLimit - minDepthLimit - pageMeter));
			mScrollV.SetScrollPos(maxDepthLimit - minDepthLimit - pageMeter);
		}
		else
		{
			mScrollV.SetScrollRange(0,(maxDepthLimit - minDepthLimit - pageMeter));
			mScrollV.SetScrollPos(0);
		}
		SetTimer(TIMER_CMD_DRAW,TIME_REFRESH_FILE,NULL);
	}
}
void CDMoniterDlg::StopTimer()
{
	bTimer = false;
	KillTimer(TIMER_CMD_DRAW);
}

void CDMoniterDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch(nIDEvent)
	{
	case TIMER_CMD_DRAW:
		{
			if(processType == FILEDATA_PROCESSING)
			{				
				Invalidate();
				KillTimer(TIMER_CMD_DRAW);
				return;
			}
			else if(processType == REALTIME_PROCESSING)
			{
				if(!petroList.IsEmpty())
				{
					//CalculateParam();
					InvalidateRect(rectView,false);
				}
				SetTimer(TIMER_CMD_DRAW,TIME_REFRESH_REALTIME,NULL);
			}
			else if(processType == NO_PROCESSING)
			{
				
			}
		}
		break;
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CDMoniterDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	int TempPos = pScrollBar->GetScrollPos();
	int pageMeter = m_clientRect.Height()/unitPixel;
	switch(nSBCode)
	{
	case SB_THUMBPOSITION://�϶�����
		pScrollBar->SetScrollPos(nPos);
		if(processType == FILEDATA_PROCESSING)
		{
			TempPos = pScrollBar->GetScrollPos();
			if(TempPos<pageMeter)
			{
				TempPos = 0;
			}
			else if(TempPos>maxDepthLimit - minDepthLimit - pageMeter)
			{
				TempPos = maxDepthLimit - minDepthLimit - pageMeter;
			}
			else
			{
				TempPos = pScrollBar->GetScrollPos();
			}
			minDepth = (int)TempPos+minDepthLimit;
			maxDepth = (int)minDepth+pageMeter;
		}
		InvalidateRect(rectView,false);
		break;
	case SB_LINEUP://����ϱߵļ�ͷ
		if(processType == FILEDATA_PROCESSING)
		{
			if(TempPos<pageMeter)
			{
				TempPos = 0;
			}
			else
			{
				TempPos -= pageMeter;
			}
			minDepth = (int)TempPos+minDepthLimit;
			maxDepth = (int)minDepth+pageMeter;
			pScrollBar->SetScrollPos(TempPos);
		}
		InvalidateRect(rectView,false);
		break;
	case SB_LINEDOWN://����±ߵļ�ͷ
		if(processType == FILEDATA_PROCESSING)
		{
			if(TempPos>maxDepthLimit - minDepthLimit - pageMeter)
			{
				TempPos = maxDepthLimit - minDepthLimit - pageMeter;
			}
			else
			{
				TempPos += pageMeter;
			}
			minDepth = (int)TempPos+minDepthLimit;
			maxDepth = (int)minDepth+pageMeter;
			pScrollBar->SetScrollPos(TempPos);
		}
		InvalidateRect(rectView,false);
		break;
	case SB_PAGEUP://���Ϸ�ҳ
		if(processType == FILEDATA_PROCESSING)
		{
			if(TempPos<pageMeter)
			{
				TempPos = 0;
			}
			else
			{
				TempPos -= pageMeter;
			}
			minDepth = (int)TempPos+minDepthLimit;
			maxDepth = (int)minDepth+pageMeter;
			pScrollBar->SetScrollPos(TempPos);
		}
		InvalidateRect(rectView,false);
		break;
	case SB_PAGEDOWN://���·�ҳ
		if(processType == FILEDATA_PROCESSING)
		{
			if(TempPos>maxDepthLimit - minDepthLimit - pageMeter)
			{
				TempPos = maxDepthLimit - minDepthLimit - pageMeter;
			}
			else
			{
				TempPos += pageMeter;
			}
			minDepth = (int)TempPos+minDepthLimit;
			maxDepth = (int)minDepth+pageMeter;
			pScrollBar->SetScrollPos(TempPos);
		}
		InvalidateRect(rectView,false);
		break;
	} 
	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}





void CDMoniterDlg::OnMenuConn()
{
	// TODO: �ڴ���������������
	CMenu* pSubMenu = NULL;
	CMenu* pMainMenu = AfxGetMainWnd()->GetMenu();
	{
		pSubMenu = pMainMenu->GetSubMenu(1);
		if (pSubMenu && pSubMenu->GetMenuItemID(0) == ID_MENU_CONN)
		{
			pSubMenu->CheckMenuItem(ID_MENU_CONN,MF_CHECKED); 
		}
		if (pSubMenu && pSubMenu->GetMenuItemID(1) == ID_MENU_DISCONN)
		{
			pSubMenu->CheckMenuItem(ID_MENU_DISCONN,MF_UNCHECKED); 
		}
	}
	if(bConnect == true)
	{
		return;
	}
	bConnect = true;
	theApp.commLayer.SetConnectType(TYPE_NONE);
	theApp.commLayer.fatherHwnd = (AfxGetMainWnd()->GetSafeHwnd());//��ȡHWND����ֵ��ͨ�Ų������Ϣ����
	
	CString strTime;
	//��ȡϵͳʱ��
	CTime tm;
	tm=CTime::GetCurrentTime();
	strTime=tm.Format(_T("(%Y-%m-%d-%H-%M-%S)"));
	fileName = theApp.DataPath + _T("export ")+strTime;
	CFileDialog dlg (FALSE, _T("dmor"), fileName, OFN_HIDEREADONLY | OFN_EXPLORER | OFN_OVERWRITEPROMPT, NULL);
	if (dlg.DoModal () == IDOK)
	{
		sGetFileName = dlg.GetPathName ();
		openDataFile(sGetFileName);
	}
	totalReceiveByte = 0;
	fileNum = 0;
	theApp.m_CommResault=theApp.commLayer.CreatConnect();
	if(theApp.m_CommResault==COMM_SUCCESS)
	{
		processType = REALTIME_PROCESSING;
		StartTimer();
	}
	if(theApp.m_CommResault==COMM_ERROE_HARDWARE_CONNECT_FAIL)
	{
		//TRACE(_T("Communication Fail!\n"));
	}
}


void CDMoniterDlg::OnUpdateMenuConn(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
}


void CDMoniterDlg::OnMenuDisconn()
{
	// TODO: �ڴ���������������
	CMenu* pSubMenu = NULL;
	CMenu* pMainMenu = AfxGetMainWnd()->GetMenu();
	{
		pSubMenu = pMainMenu->GetSubMenu(1);
		if (pSubMenu && pSubMenu->GetMenuItemID(0) == ID_MENU_CONN)
		{
			pSubMenu->CheckMenuItem(ID_MENU_CONN,MF_UNCHECKED); 
		}
		if (pSubMenu && pSubMenu->GetMenuItemID(1) == ID_MENU_DISCONN)
		{
			pSubMenu->CheckMenuItem(ID_MENU_DISCONN,MF_CHECKED); 
		}
	}
	if(bConnect == false)
	{
		return;
	}
	bConnect = false;
	theApp.commLayer.m_SerialPort.ClosePort();//�رմ���
	//bRunning = false;
	Sleep(500);
	closeDataFile(sGetFileName);
}


void CDMoniterDlg::OnUpdateMenuDisconn(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
}

void CDMoniterDlg::closeDataFile(CString strFile)  
{    
	if(fp != NULL)
	{
		fclose(fp);   
	}
} 
void CDMoniterDlg::openDataFile(CString strFile)  
{   
	if(fp != NULL)
	{
		fclose(fp);  
	}
	fp = _wfopen(strFile, _T("w")); 
} 
LRESULT CDMoniterDlg::OnCommReceive(WPARAM wParam, LPARAM lParam)
{
#ifdef _DEBUG
#if 0
    //TRACE(_T("Communication Receive!\n"));
	TRACE0("RX = ");
	//TRACE(_T(" %02X\n"),wParam);

    for(WORD cont = 0; cont < wParam ; cont++)
    {
        //TRACE(_T(" %02X"),theApp.commLayer.m_ReceiveBuff[cont]);
    }
	TRACE0("\n");
#endif
#endif
	CString str;
	totalReceiveByte += wParam;
	if(totalReceiveByte>fileLimit)
	{
		fileNum++;
		closeDataFile(sGetFileName);
		sGetFilePreName = sGetFileName;
		int pos = sGetFilePreName.ReverseFind(_T('.'));
		int length = sGetFilePreName.GetLength();
		CString strPre = sGetFilePreName.Left(pos);
		CString strSub = sGetFilePreName.Right(length - pos);
		CString strAdd;
		strAdd.Format(_T("_%d"),fileNum);
		sGetFileName = strPre + strAdd + strSub;
		openDataFile(sGetFileName);
	}
#if 0
	if(totalReceiveByte < 1024)
	{
		str.Format(_T("�ѽ�������: %d B"),totalReceiveByte);
	}
	else if(totalReceiveByte < 1024*1024)
	{
		str.Format(_T("�ѽ�������:%2f KB"),(float)totalReceiveByte/1024);
	}
	else
	{
		str.Format(_T("�ѽ�������:%2f MB"),(float)totalReceiveByte/(1024*1024));
	}
#endif
	writeDataFile(&theApp.commLayer.m_ReceiveBuff[0],wParam);
	ParseData(&theApp.commLayer.m_ReceiveBuff[0],wParam);
    return 0;
}
void CDMoniterDlg::writeDataFile(BYTE* tmp, WPARAM wParam)  
{     
    if(fp != NULL)
	{ 
		fseek(fp,0L,2);
		{
			unsigned int nSize = fwrite( &tmp[0], wParam, 1, fp);  
		}
    }   
}  

void CDMoniterDlg::AddPanelListView( )
{
	listView.SetRedraw(FALSE);
	listView.DeleteAllItems();
	listView.InsertItem(0,_T("DEPT"));
	listView.InsertItem(1,_T("TEMP"));
	listView.InsertItem(2,_T("RM"));
	listView.InsertItem(3,_T("GM"));
	listView.InsertItem(4,_T("MAGX"));
	listView.InsertItem(5,_T("CCL"));
	listView.SetRedraw(TRUE);
	listView.Invalidate();
	listView.UpdateWindow();
}
void CDMoniterDlg::UpdatePanelListView(CPetroData* pPData)
{
	Edit01.SetWindowText(pPData->dept.strData);

	listView.SetRedraw(FALSE);
	listView.SetItemText(0,1,pPData->dept.strData);
	listView.SetItemText(0,2,pPData->dept.strUnit);

	listView.SetItemText(1,1,oldtempArray.strDx);
	listView.SetItemText(1,2,pPData->temp.strUnit);

	listView.SetItemText(2,1,oldrmArray.strDx);
	listView.SetItemText(2,2,pPData->rm.strUnit);

	listView.SetItemText(3,1,oldgmArray.strDx);
	listView.SetItemText(3,2,pPData->gr.strUnit);

	CString str;
	str += oldmagArray[0].strDx;
	str += _T(",");
	str += oldmagArray[1].strDx;
	str += _T(",");
	str += oldmagArray[2].strDx;
	listView.SetItemText(4,1,str);
	listView.SetItemText(4,2,pPData->mag[0].strUnit);

	listView.SetItemText(5,1,oldcclArray.strDx);
	listView.SetItemText(5,2,pPData->ccl.strUnit);

	listView.SetRedraw(TRUE);
	listView.Invalidate();
	listView.UpdateWindow();
}

void CDMoniterDlg::OnMenuMeasuredown()
{
	// TODO: �ڴ���������������
	m_bDirectDown = true;
	CMenu* pSubMenu = NULL;
	CMenu* pMainMenu = AfxGetMainWnd()->GetMenu();
	{
		pSubMenu = pMainMenu->GetSubMenu(3);
		if (pSubMenu && pSubMenu->GetMenuItemID(0) == ID_MENU_MEASUREUP)
		{
			pSubMenu->CheckMenuItem(ID_MENU_MEASUREUP,m_bDirectDown?MF_UNCHECKED:MF_CHECKED); 
		}
		if (pSubMenu && pSubMenu->GetMenuItemID(1) == ID_MENU_MEASUREDOWN)
		{
			pSubMenu->CheckMenuItem(ID_MENU_MEASUREDOWN,m_bDirectDown?MF_CHECKED:MF_UNCHECKED); 
		}
	}
}


void CDMoniterDlg::OnUpdateMenuMeasuredown(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
}


void CDMoniterDlg::OnMenuMeasureup()
{
	// TODO: �ڴ���������������
	m_bDirectDown = false;
	CMenu* pSubMenu = NULL;
	CMenu* pMainMenu = AfxGetMainWnd()->GetMenu();
	{
		pSubMenu = pMainMenu->GetSubMenu(3);
		if (pSubMenu && pSubMenu->GetMenuItemID(0) == ID_MENU_MEASUREUP)
		{
			pSubMenu->CheckMenuItem(ID_MENU_MEASUREUP,m_bDirectDown?MF_UNCHECKED:MF_CHECKED); 
		}
		if (pSubMenu && pSubMenu->GetMenuItemID(1) == ID_MENU_MEASUREDOWN)
		{
			pSubMenu->CheckMenuItem(ID_MENU_MEASUREDOWN,m_bDirectDown?MF_CHECKED:MF_UNCHECKED); 
		}
	}
}


void CDMoniterDlg::OnUpdateMenuMeasureup(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
}


BOOL CDMoniterDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	return CDialogEx::OnEraseBkgnd(pDC);
}


void CDMoniterDlg::OnMenuInstrument()
{
	// TODO: �ڴ���������������
	
}


void CDMoniterDlg::OnMenuJob()
{
	// TODO: �ڴ���������������
	parameterFlag=0;
	OnMyMenuJob(parameterFlag);
}

void CDMoniterDlg::OnMyMenuJob(int myparameterFlag)
{
	// TODO: �ڴ���������������
	CJobDlg m_jDlg;
	m_jDlg.m_Path=theApp.JobPath;
	m_jDlg.receiveFlag = myparameterFlag;
	m_jDlg.m_Title=_T("������ҵ����");
	m_jDlg.m_TreeTitle=_T("��ҵ�б�");
	m_jDlg.DoModal();
}

void CDMoniterDlg::OnMenuTool()
{
	// TODO: �ڴ���������������
	// TODO: �ڴ���������������
	CJobDlg m_jDlg;
	m_jDlg.m_Path=theApp.ToolPath;
	m_jDlg.m_Title=_T("������������");
	m_jDlg.m_TreeTitle=_T("�����б�");
	m_jDlg.DoModal();
}


void CDMoniterDlg::OnMenuNewjob()
{
	// TODO: �ڴ���������������
	CFileDialog dlg(FALSE,_T(""),_T(""),OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("tools files(*.tools)|*.tools|tool files(*.mdb)|*.mdb|All files(*.*)|*.*|"));//���챣���ļ��Ի���
	dlg.m_ofn.lpstrInitialDir=theApp.JobPath;
    if(dlg.DoModal()==IDOK)//��ʾ�����ļ��Ի���
	{
		CJobEditDlg m_jeDlg;
		m_jeDlg.m_jobName=dlg.GetPathName();//��ȡ�ļ�·������
		m_jeDlg.m_Open=FALSE;
		if(m_jeDlg.DoModal()==IDOK){
		m_treeCtrl.ModifyStyle(0,TVS_HASBUTTONS   |   TVS_LINESATROOT   |   TVS_HASLINES);	
		m_treeCtrl.DisplayTree (theApp.JobPath,TRUE);
		}
	}
}


void CDMoniterDlg::OnMenuJobload()
{
	// TODO: �ڴ���������������
	// TODO: �ڴ���������������
	CJobDlg m_jDlg;
	m_jDlg.m_Path=theApp.JobPath;
	m_jDlg.m_Title=_T("������ҵ����");
	m_jDlg.DoModal();
}


void CDMoniterDlg::OnTestMode1()
{
	// TODO: �ڴ���������������
	//testDlg.DoModal();
	if (testDlg.DoModal () == IDOK)
	{
		CreateCurveFile(theApp.strCurveFile);
	}
}

void CDMoniterDlg::ClearList()
{
	CWorkInfo* pCurrent = NULL;
	while(theApp.workInfoList.IsEmpty()==false)
	{
		pCurrent=theApp.workInfoList.RemoveHead();
		delete pCurrent;
		pCurrent=NULL;
	}
}
int CDMoniterDlg::CreateCurveFile(CString strFile)
{

	TiXmlDocument doc;
	TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "UTF-8", "yes" );
	doc.LinkEndChild( decl ); 
	TiXmlElement * element = new TiXmlElement( "Works" );
	POSITION pos = theApp.workInfoList.GetHeadPosition();
	while(pos)
	{
		CWorkInfo* plist = theApp.workInfoList.GetNext(pos);
		LinkElementeFuns(element,plist);	
	}
	doc.LinkEndChild( element );
	////
	char * pFileName = theApp.FromUNICODEToANSI(strFile);
	doc.SaveFile(pFileName);

	delete []pFileName;
	pFileName = NULL;
	return 0;
}

void CDMoniterDlg::LinkElementeFuns(TiXmlElement * element,CWorkInfo* plist)
{
	TiXmlElement* msgs = new TiXmlElement( "WorkInfo" ); 
	element->LinkEndChild( msgs );
	TiXmlElement* msg = new TiXmlElement( "Signal" ); 
	CString str = plist->strSignal;
	////********/////transfer//////********//////
	const int maxBufferSize = 64;
	unsigned short UnicodeStr[maxBufferSize];
	unsigned short utf8StrLen;
	DWORD dwLength;//ת�����UTF��8����ĳ���in BYTEs
	char utf8Str[64];
	memset(utf8Str,0,maxBufferSize*sizeof(char));
	memset(UnicodeStr,0,maxBufferSize*sizeof(unsigned short));
	short UnicodeStrLen=2*str.GetLength();
	memcpy(UnicodeStr,str,UnicodeStrLen);
	utf8StrLen=6*UnicodeStrLen;
	memset(utf8Str,0,maxBufferSize*sizeof(char));
	dwLength=theApp.FromUnicodeToUTF8( utf8Str, utf8StrLen, UnicodeStr, UnicodeStrLen);//Unicode to UTF-8
	//////////********///////////
	msg->LinkEndChild( new TiXmlText( utf8Str )); 
	msgs->LinkEndChild( msg ); 

	msg = new TiXmlElement( "Title" ); 
	str = plist->strTitle;
	////********/////transfer//////********//////

	memset(utf8Str,0,maxBufferSize*sizeof(char));
	memset(UnicodeStr,0,maxBufferSize*sizeof(unsigned short));
	UnicodeStrLen=2*str.GetLength();
	memcpy(UnicodeStr,str,UnicodeStrLen);
	utf8StrLen=6*UnicodeStrLen;
	memset(utf8Str,0,maxBufferSize*sizeof(char));
	dwLength=theApp.FromUnicodeToUTF8( utf8Str, utf8StrLen, UnicodeStr, UnicodeStrLen);//Unicode to UTF-8
	//////////********///////////
	msg->LinkEndChild( new TiXmlText( utf8Str )); 
	msgs->LinkEndChild( msg ); 

	msg = new TiXmlElement( "Unit" ); 
	str = plist->strUnit;
	////********/////transfer//////********//////

	memset(utf8Str,0,maxBufferSize*sizeof(char));
	memset(UnicodeStr,0,maxBufferSize*sizeof(unsigned short));
	UnicodeStrLen=2*str.GetLength();
	memcpy(UnicodeStr,str,UnicodeStrLen);
	utf8StrLen=6*UnicodeStrLen;
	memset(utf8Str,0,maxBufferSize*sizeof(char));
	dwLength=theApp.FromUnicodeToUTF8( utf8Str, utf8StrLen, UnicodeStr, UnicodeStrLen);//Unicode to UTF-8
	//////////********///////////
	msg->LinkEndChild( new TiXmlText( utf8Str )); 
	msgs->LinkEndChild( msg ); 

	msg = new TiXmlElement( "LeftLimit" ); 
	str.Format(_T("%d"),plist->leftLimit);
	////********/////transfer//////********//////
	memset(utf8Str,0,maxBufferSize*sizeof(char));
	memset(UnicodeStr,0,maxBufferSize*sizeof(unsigned short));
	UnicodeStrLen=2*str.GetLength();
	memcpy(UnicodeStr,str,UnicodeStrLen);
	utf8StrLen=6*UnicodeStrLen;
	memset(utf8Str,0,maxBufferSize*sizeof(char));
	dwLength=theApp.FromUnicodeToUTF8( utf8Str, utf8StrLen, UnicodeStr, UnicodeStrLen);//Unicode to UTF-8
	//////////********///////////
	msg->LinkEndChild( new TiXmlText( utf8Str )); 
	msgs->LinkEndChild( msg ); 

	msg = new TiXmlElement( "RightLimit" ); 
	str.Format(_T("%d"),plist->rightLimit);
	////********/////transfer//////********//////
	memset(utf8Str,0,maxBufferSize*sizeof(char));
	memset(UnicodeStr,0,maxBufferSize*sizeof(unsigned short));
	UnicodeStrLen=2*str.GetLength();
	memcpy(UnicodeStr,str,UnicodeStrLen);
	utf8StrLen=6*UnicodeStrLen;
	memset(utf8Str,0,maxBufferSize*sizeof(char));
	dwLength=theApp.FromUnicodeToUTF8( utf8Str, utf8StrLen, UnicodeStr, UnicodeStrLen);//Unicode to UTF-8
	//////////********///////////
	msg->LinkEndChild( new TiXmlText( utf8Str )); 
	msgs->LinkEndChild( msg ); 

	msg = new TiXmlElement( "Color" ); 
	str.Format(_T("%d"),plist->curveColor);
	////********/////transfer//////********//////
	memset(utf8Str,0,maxBufferSize*sizeof(char));
	memset(UnicodeStr,0,maxBufferSize*sizeof(unsigned short));
	UnicodeStrLen=2*str.GetLength();
	memcpy(UnicodeStr,str,UnicodeStrLen);
	utf8StrLen=6*UnicodeStrLen;
	memset(utf8Str,0,maxBufferSize*sizeof(char));
	dwLength=theApp.FromUnicodeToUTF8( utf8Str, utf8StrLen, UnicodeStr, UnicodeStrLen);//Unicode to UTF-8
	//////////********///////////
	msg->LinkEndChild( new TiXmlText( utf8Str )); 
	msgs->LinkEndChild( msg ); 

}

void CDMoniterDlg::ParseTestData()
{
	WCHAR pwBuffer[100];
	CString str;
	ClearList();
	string fileXml = theApp.IniFilePath + "workinfo.xml";
	TiXmlDocument doc(fileXml.c_str());
	bool loadOkay = doc.LoadFile();
	if (loadOkay)
	{
		if ( !doc.Parse( fileXml.c_str() ) )
		{
			cout << doc.ErrorDesc() << endl;
		}
		//step 1 : Get the RootElement
		TiXmlElement *root = doc.RootElement();
		for ( TiXmlNode *child = root->FirstChild(); child; child = child->NextSibling() )
		{
			if ( ( child->Type() == TiXmlNode::TINYXML_ELEMENT ) && ( !strcmp( child->Value(), "WorkInfo" ) ) )
			{
				CWorkInfo* plist = new CWorkInfo();
				for ( TiXmlNode *subchild = child->FirstChild(); subchild; subchild = subchild->NextSibling() )
				{
					if ( ( subchild->Type() == TiXmlNode::TINYXML_ELEMENT ) && ( !strcmp( subchild->Value(), "Signal" ) ) )
					{
						TiXmlNode* subsubchild = subchild->FirstChild();
						memset(pwBuffer,0,sizeof(pwBuffer));
						const char* strValue = subsubchild->Value();
						int len = strlen(strValue);
						theApp.FromUTF8ToUnicode((BYTE*)strValue,  len, (WORD*)pwBuffer, sizeof(pwBuffer));
						str = pwBuffer;
						plist->strSignal = str;
					}
					else if ( ( subchild->Type() == TiXmlNode::TINYXML_ELEMENT ) && ( !strcmp( subchild->Value(), "Title" ) ) )
					{
						TiXmlNode* subsubchild = subchild->FirstChild();
						memset(pwBuffer,0,sizeof(pwBuffer));
						const char* strValue = subsubchild->Value();
						int len = strlen(strValue);
						theApp.FromUTF8ToUnicode((BYTE*)strValue,  len, (WORD*)pwBuffer, sizeof(pwBuffer));
						str = pwBuffer;
						plist->strTitle = str;
					}
					else if ( ( subchild->Type() == TiXmlNode::TINYXML_ELEMENT ) && ( !strcmp( subchild->Value(), "Unit" ) ) )
					{
						TiXmlNode* subsubchild = subchild->FirstChild();
						memset(pwBuffer,0,sizeof(pwBuffer));
						const char* strValue = subsubchild->Value();
						int len = strlen(strValue);
						theApp.FromUTF8ToUnicode((BYTE*)strValue,  len, (WORD*)pwBuffer, sizeof(pwBuffer));
						str = pwBuffer;
						plist->strUnit = str;
					}
					else if ( ( subchild->Type() == TiXmlNode::TINYXML_ELEMENT ) && ( !strcmp( subchild->Value(), "LeftLimit" ) ) )
					{
						TiXmlNode* subsubchild = subchild->FirstChild();
						memset(pwBuffer,0,sizeof(pwBuffer));
						const char* strValue = subsubchild->Value();
						int len = strlen(strValue);
						theApp.FromUTF8ToUnicode((BYTE*)strValue,  len, (WORD*)pwBuffer, sizeof(pwBuffer));
						str = pwBuffer;
						plist->leftLimit = _tstoi(pwBuffer);;
					}
					else if ( ( subchild->Type() == TiXmlNode::TINYXML_ELEMENT ) && ( !strcmp( subchild->Value(), "RightLimit" ) ) )
					{
						TiXmlNode* subsubchild = subchild->FirstChild();
						memset(pwBuffer,0,sizeof(pwBuffer));
						const char* strValue = subsubchild->Value();
						int len = strlen(strValue);
						theApp.FromUTF8ToUnicode((BYTE*)strValue,  len, (WORD*)pwBuffer, sizeof(pwBuffer));
						str = pwBuffer;
						plist->rightLimit = _tstoi(pwBuffer);;
					}
					else if ( ( subchild->Type() == TiXmlNode::TINYXML_ELEMENT ) && ( !strcmp( subchild->Value(), "Color" ) ) )
					{
						TiXmlNode* subsubchild = subchild->FirstChild();
						memset(pwBuffer,0,sizeof(pwBuffer));
						const char* strValue = subsubchild->Value();
						int len = strlen(strValue);
						theApp.FromUTF8ToUnicode((BYTE*)strValue,  len, (WORD*)pwBuffer, sizeof(pwBuffer));
						str = pwBuffer;
						plist->curveColor = _tstoi(pwBuffer);;
					}
				}
				theApp.workInfoList.AddTail(plist);
			}
		}
	}
}
