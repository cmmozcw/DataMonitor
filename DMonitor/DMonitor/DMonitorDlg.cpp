
// DMoniterDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DMonitor.h"
#include "DMonitorDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


COLORREF crDlgbackground = RGB(255,255,255);

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CDMoniterDlg 对话框




CDMoniterDlg::CDMoniterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDMoniterDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//设置主界面大小
	m_SM_CXFULLSCREEN = GetSystemMetrics (SM_CXFULLSCREEN);
	m_SM_CYFULLSCREEN = GetSystemMetrics (SM_CYFULLSCREEN);
	minMainDialogWidth = 960;
	minMainDialogHeight = minMainDialogWidth * 9 / 16;
	processType = NO_PROCESSING;

	ClearPetroData();
	pData = NULL;

	unitPixel = 5;//20 pixel = 1 m
	unitRatio = 4;
	m_bDirectDown = true;

	base = 0;
	bias = 0;
	
	minDepth = 0;
	maxDepth = 0;
	maxPreDepth = 0;
	minDepthLimit = 0;
	maxDepthLimit = 0;
	
	m_drawCount = 0;
	bScroll = false;
	m_step = 5;//每次移动步长为5米
	pPData = NULL;
	pOldPData = NULL;

	bTimer = false;//计时器是否开启
	pos = NULL;//当前记录位置

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
}

BEGIN_MESSAGE_MAP(CDMoniterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CDMoniterDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDMoniterDlg::OnBnClickedCancel)
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_FILE_OPEN, &CDMoniterDlg::OnFileOpen)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, &CDMoniterDlg::OnUpdateFileOpen)
	ON_WM_TIMER()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


// CDMoniterDlg 消息处理程序

BOOL CDMoniterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	m_hMenu = LoadMenu(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));//导入资源,创建菜单
	::SetMenu(this->GetSafeHwnd(),m_hMenu);//添加到对话框

	// TODO: 在此添加额外的初始化代码
	ShowWindow(SW_SHOWMAXIMIZED);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。
BOOL CDMoniterDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		{
			switch (pMsg->wParam)
			{
			case VK_ESCAPE:
				return TRUE;
			case VK_UP://屏蔽上下方向键
			case VK_DOWN:	
			case VK_SPACE:
			case VK_RETURN://屏蔽回车
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
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
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
		CBrush brush(crDlgbackground);
		CBrush *oBrush = dc.SelectObject(&brush);
		dc.FillSolidRect(rectView,crDlgbackground); 
		dc.SelectObject(oBrush);

		CPen pen(PS_SOLID, 1, RGB(0,0,0));
		CPen *oPen = dc.SelectObject(&pen);
		
		dc.Rectangle(rectView.left-1,rectView.top-1,rectView.right+1,rectView.bottom+1);
		dc.SelectObject(oPen);
		///////////////////////
		//建立与屏幕显示兼容的内存显示设备，这时还不能绘图，因为没有地方画
		MemDC.CreateCompatibleDC(NULL);  
		//建立一个与屏幕显示兼容的位图，至于位图的大小，可以用窗口的大小  
		MemBitmap.CreateCompatibleBitmap(&dc,rectView.Width(),rectView.Height());  
		CBitmap *pOldBit=MemDC.SelectObject(&MemBitmap);
		MemDC.FillSolidRect(0,0,rectView.Width(),rectView.Height(),RGB(255,255,255));
		m_clientRect = rectView;
		m_clientRect.right -= m_clientRect.left; 
		m_clientRect.left = 0;

		DrawData(&MemDC);
		//将内存中的图拷贝到屏幕上进行显示  
		dc.BitBlt(rectView.left,rectView.top,rectView.Width(),rectView.Height(),&MemDC,0,0,SRCCOPY);  
		//绘图完成后的清理  
		MemBitmap.DeleteObject();  
		MemDC.DeleteDC();  
		//////////////////////		
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDMoniterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDMoniterDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


void CDMoniterDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CDMoniterDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	lpMMI-> ptMinTrackSize.x = minMainDialogWidth ; //宽 
	lpMMI-> ptMinTrackSize.y = min(minMainDialogHeight,m_SM_CYFULLSCREEN) ; //高 
	CDialogEx::OnGetMinMaxInfo(lpMMI);
}


void CDMoniterDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	GetClientRect(rectMain);
	GetRectParam(rectMain);
	if(mScrollV)
	{
		mScrollV.SetWindowPos(0,rectScrollV.left,rectScrollV.top,rectScrollV.Width(),rectScrollV.Height(),SWP_NOZORDER);
	}
	InvalidateRect(rectMain);
	// TODO: 在此处添加消息处理程序代码
}

void CDMoniterDlg::GetRectParam(CRect rectMain)
{
	rectMain.left += 2;
	rectMain.top += 2;
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
	rectScale.top = rectView.bottom+1;
}


void CDMoniterDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	
	// TODO: 在此处添加消息处理程序代码
	DestroyMenu(m_hMenu);
}


void CDMoniterDlg::OnFileOpen()
{
	// TODO: 在此添加命令处理程序代码
	bool bReturn=false;
	processType = FILEDATA_PROCESSING;
	TCHAR   *pFileBuffer = new TCHAR[MAX_PATH];//最多允许300个文件
	CFileDialog dlg (TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_EXPLORER , _T("dmor文件(*.dmor)|*.dmor||"),this);
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
	// TODO: 在此添加命令更新用户界面处理程序代码
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
				petroList.AddTail(pPData);
			}
		}
		if(tmp[i] == '$')//起始标示
		{
			str.clear();
			TRACE(_T("Find Char '$' \r\n"));
		}
		else if(tmp[i] == ',' || tmp[i] == '*')//数据结束
		{
			if(str == "DEPT" || str == "TEMP" || str == "RM" 
				|| str == "GM" || str == "MAGX" || str == "CCL" )
			{
				strTitle = str;
				if(str == "DEPT")
				{
					TRACE(_T("Find Char 'DEPT' \r\n"));
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
				//if(result == 0)//整数
				if(str != "LQRD" )
				{
					double num;
					num=atof(str.c_str());
					if(pPData != NULL)
					{
						if(strTitle == "DEPT")
						{
							TRACE(_T("strTitle == 'DEPT' \r\n"));
							pPData->dept.iData = num;
							pPData->dept.strData = str.c_str();
							pPData->dept.bAssign = true;
							pPData->dept.strTag = strTitle.c_str();
							pPData->dept.strUnit = _T("m");
						}
						else if(strTitle == "TEMP")
						{
							TRACE(_T("strTitle == 'TEMP' \r\n"));
							pPData->temp.iData = num;
							pPData->temp.strData = str.c_str();
							pPData->temp.bAssign = true;
							pPData->temp.strTag = strTitle.c_str();
							pPData->temp.strUnit = _T("℃");
						}
						else if(strTitle == "RM")
						{
							TRACE(_T("strTitle == 'RM' \r\n"));
							pPData->rm.iData = num;
							pPData->rm.strData = str.c_str();
							pPData->rm.bAssign = true;
							pPData->rm.strTag = strTitle.c_str();
							pPData->rm.strUnit = _T("？");
						}
						else if(strTitle == "GM")
						{
							TRACE(_T("strTitle == 'GM' \r\n"));
							pPData->gr.iData = num;
							pPData->gr.strData = str.c_str();
							pPData->gr.bAssign = true;
							pPData->gr.strTag = strTitle.c_str();
							pPData->gr.strUnit = _T("？");
						}
						else if(strTitle == "MAGX")
						{
							TRACE(_T("strTitle == 'MAGX' \r\n"));
							if(pPData->mag[0].iData == 0)
							{
								pPData->mag[0].iData = num;
								pPData->mag[0].strData = str.c_str();
								pPData->mag[0].bAssign = true;
								pPData->mag[0].strTag = strTitle.c_str();
								pPData->mag[0].strUnit = _T("？");
							}
							else if(pPData->mag[1].iData == 0)
							{
								pPData->mag[1].iData = num;
								pPData->mag[1].strData = str.c_str();
								pPData->mag[1].bAssign = true;
								pPData->mag[1].strTag = strTitle.c_str();
								pPData->mag[1].strUnit = _T("？");
							}
							else if(pPData->mag[2].iData == 0)
							{
								pPData->mag[2].iData = num;
								pPData->mag[2].strData = str.c_str();
								pPData->mag[2].bAssign = true;
								pPData->mag[2].strTag = strTitle.c_str();
								pPData->mag[2].strUnit = _T("？");
							}
							else
							{
							}
						}
						else if(strTitle == "CCL")
						{
							TRACE(_T("strTitle == 'CCL' \r\n"));
							pPData->ccl.iData = num;
							pPData->ccl.strData = str.c_str();
							pPData->ccl.bAssign = true;
							pPData->ccl.strTag = strTitle.c_str();
							pPData->ccl.strUnit = _T("？");
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

int CDMoniterDlg::CheckString( std::string str )
{
	bool bIsDigit = true;
	bool bIsPoint = false;
	int nCount = str.length(); // 获得字符个数
	for ( int i = 0; i < nCount; i ++ )
	{
		if ( 0 == isdigit( str[i] ) ) // 不是数字就置标志位
		{
			if(str[i] == '.')//带小数点
			{
				bIsPoint = true;
			}
			else
			{
				bIsDigit = false;
				break;// 退出
			}
		}
	}
	if(!bIsDigit)
	{
		return 2;//非数字
	}
	else if(bIsPoint)
	{
		return 1;//有小数点
	}
	else
	{
		return 0;//整数
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
	m_step = 5;//每次移动步长为5米
	pPData = NULL;
	pOldPData = NULL;
	pos = NULL;//当前记录位置
	bTimer = true;
	InitOldArrayData();
	InitArrayData();
	if(petroList.IsEmpty())
	{
		return;//没有数据不进行绘制
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
		GetMaxMinData();//在绘图前进行一次计算的操作
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

		//查找当前最小深度值
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
		DrawBasic(pDC);//获取内存映射绘制区域大小
		DrawGrid(pDC);
		DrawPlot(pDC);
		//DrawCurve(pDC,minDepth,maxDepth);
	}
	else if(processType == FILEDATA_PROCESSING)
	{
		DrawBasic(pDC);//获取内存映射绘制区域大小
		DrawGrid(pDC);
		DrawPlot(pDC);
		DrawCurve(pDC,minDepth,maxDepth);
	}
	else
	{
		DrawBasic(pDC);//获取内存映射绘制区域大小
		DrawGrid(pDC);
		DrawPlot(pDC);
	}
}

void CDMoniterDlg::DrawCurve(CDC* pDC , double upDepth, double DownDepth)
{
	int iDrawType = PS_SOLID;
	long pre_iy=0,cur_iy=0,pre_dy=0,cur_dy=0;
	long pre_ix=0,cur_ix=0,pre_dx=0,cur_dx=0;
	COLORREF colorRed = RGB(255,0,0);
	COLORREF colorBlue = RGB(0,0,255);
	COLORREF colorGreen = RGB(0,255,0);
	COLORREF colorBlack = RGB(0,0,0);
	COLORREF color1 = RGB(2,141,153);
	CPen pen(iDrawType, 1,colorRed); //画笔
	CPen pen2(iDrawType, 1,colorBlue); //画笔2
	CPen pen3(iDrawType, 1,colorGreen); //画笔3
	CPen pen4(iDrawType, 1,color1); //画笔4
	CPen pen5(iDrawType, 1,colorBlack); //画笔5
	CPen* pOldPen = pDC->SelectObject(&pen);//画笔和画线区连接
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
							DrawDeptData(pDC,pPData,&pen);//深度
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
						DrawDeptData(pDC,pPData,&pen);//深度
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
							DrawDeptData(pDC,pPData,&pen);//深度
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
						DrawDeptData(pDC,pPData,&pen);//深度
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
								DrawDeptData(pDC,pPData,&pen);//深度
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
									DrawDeptData(pDC,pPData,&pen);//深度
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
								DrawDeptData(pDC,pPData,&pen);//深度
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
//绘制磁三分量曲线
void CDMoniterDlg::DrawMagxData(CDC* pDC ,CPetroData* pPData,CPen* pPpen)
{
	int iDrawType = PS_SOLID;
	COLORREF color1 = RGB(255,0,255);
	COLORREF color2 = RGB(0,255,255);

	CPen penmag0(iDrawType, 1,color1); //画笔
	CPen penmag1(iDrawType, 1,color1); //画笔2
	CPen penmag2(iDrawType, 1,color2); //画笔3
	
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
//绘制Gm曲线
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
//绘制Ccl曲线
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
				depPixel = (int)((maxDepth - minDepthLimit)*10/10) * unitPixel;
			}
			else
			{
				minDepth = minDepthLimit;
				maxDepth = minDepth + m_clientRect.Height()/unitPixel;
				depPixel = (int)((maxDepth - minDepthLimit)*10/10) * unitPixel;
				if(maxDepthLimit >= maxDepth)
				{
					bScroll = true;
				}
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
				depPixel = (int)((maxDepth - minDepthLimit)*10/10) * unitPixel;
			}
			else
			{
				maxDepth = maxDepthLimit;
				minDepth = maxDepth - m_clientRect.Height()/unitPixel;
				depPixel = (int)((maxDepth - minDepthLimit)*10/10) * unitPixel;
				if(minDepthLimit <= minDepth)
				{
					bScroll = true;
				}
			}
		}
		m_totalRect.bottom = m_totalRect.top + max(depPixel,m_totalRect.Height());
	
		mScrollV.SetScrollRange(0,m_totalRect.bottom);
		mScrollV.SetScrollPos(m_totalRect.bottom);
	}
	else if(processType == FILEDATA_PROCESSING)
	{
#if 0
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
				depPixel = (int)((maxDepth - minDepthLimit)*10/10) * unitPixel;
			}
			else
			{
				minDepth = minDepthLimit;
				maxDepth = minDepth + m_clientRect.Height()/unitPixel;
				depPixel = (int)((maxDepth - minDepthLimit)*10/10) * unitPixel;
				if(maxDepthLimit >= maxDepth)
				{
					bScroll = true;
				}
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
				depPixel = (int)((maxDepth - minDepthLimit)*10/10) * unitPixel;
			}
			else
			{
				maxDepth = maxDepthLimit;
				minDepth = maxDepth - m_clientRect.Height()/unitPixel;
				depPixel = (int)((maxDepth - minDepthLimit)*10/10) * unitPixel;
				if(minDepthLimit <= minDepth)
				{
					bScroll = true;
				}
			}
		}
#endif
	}
	else
	{
		m_totalRect.bottom = m_totalRect.top + max(depPixel,m_totalRect.Height());
		mScrollV.SetScrollRange(0,m_totalRect.bottom);
		mScrollV.SetScrollPos(m_totalRect.bottom);
	
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

	m_gridColor = RGB(127,127,127);

	CPen *old, pen(PS_SOLID, 1, m_gridColor), pen2(PS_DOT, 1,m_gridColor); //画笔;
	CPen stick(PS_SOLID,0,RGB(0,0,0));
	CPen mline(PS_SOLID,0,RGB(192,192,192));
	old = pDC->SelectObject(&pen);
	//绘制坐标系
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
	//画笔刷新
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
	m_step = 5;//每次移动步长为5米
	pPData = NULL;
	pOldPData = NULL;
	pos = NULL;//当前记录位置
	bTimer = true;
	InitOldArrayData();
	InitArrayData();
	if(petroList.IsEmpty())
	{
		return;//没有数据不进行绘制
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
		SetTimer(TIMER_CMD_DRAW,TIME_REFRESH_REALTIME,NULL);
	}
	else if(processType == FILEDATA_PROCESSING)
	{
		minDepth = 0;
		maxDepth = 0;
		minDepthLimit = 0;
		maxDepthLimit = 0;
		bScroll = false;
		m_drawCount = TIME_REFRESH_FILE/20;
		GetMaxMinData();//在绘图前进行一次计算的操作
		//AddPanelListView();
		int pageMeter = m_clientRect.Height()/unitPixel;
		if(m_bDirectDown)
		{
			//minDepth = (int)minDepthLimit;
			//maxDepth = (int)maxDepthLimit+1;
			mScrollV.SetScrollRange(0,(maxDepthLimit - minDepthLimit - pageMeter));
			mScrollV.SetScrollPos(maxDepthLimit - minDepthLimit - pageMeter);
		}
		else
		{
			//minDepth = (int)minDepthLimit;
			//maxDepth = (int)maxDepthLimit+1;
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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
					CalculateParam();
					InvalidateRect(rectView);
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int TempPos = pScrollBar->GetScrollPos();
	int pageMeter = m_clientRect.Height()/unitPixel;
	switch(nSBCode)
	{
	case SB_THUMBPOSITION://拖动滑块
		pScrollBar->SetScrollPos(nPos);
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
		InvalidateRect(rectView);
		break;
	case SB_LINEUP://点击上边的箭头
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
		InvalidateRect(rectView);
		break;
	case SB_LINEDOWN://点击下边的箭头
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
		InvalidateRect(rectView);
		break;
	case SB_PAGEUP://向上翻页
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
		InvalidateRect(rectView);
		break;
	case SB_PAGEDOWN://向下翻页
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
		InvalidateRect(rectView);
		break;
	} 
	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}
