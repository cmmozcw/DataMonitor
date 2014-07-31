
// DMonitor.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "DataMonitor.h"
#include "MainFrm.h"

#include "DataMonitorDoc.h"
#include "DataMonitorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDataMonitorApp

BEGIN_MESSAGE_MAP(CDataMonitorApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CDataMonitorApp::OnAppAbout)
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// ��׼��ӡ��������
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// CDataMonitorApp ����
static HINSTANCE _hLangDLL = NULL;
CDataMonitorApp::CDataMonitorApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// ���Ӧ�ó��������ù�����������ʱ֧��(/clr)�����ģ���:
	//     1) �����д˸������ã�������������������֧�ֲ�������������
	//     2) ��������Ŀ�У������밴������˳���� System.Windows.Forms ������á�
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: ������Ӧ�ó��� ID �ַ����滻ΪΨһ�� ID �ַ�����������ַ�����ʽ
	//Ϊ CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("DMonitor.AppID.NoVersion"));

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CDataMonitorApp ����

CDataMonitorApp theApp;


// CDataMonitorApp ��ʼ��

BOOL CDataMonitorApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// ʹ�� RichEdit �ؼ���Ҫ  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("DataMonitor"));
	/*wuhaoyong add CMutex*/
    //Creat CMutex
    hSem=NULL;
	strSemaphore.LoadString( IDS_SEMAPHORE );
    hSem=CreateSemaphore(NULL,1,1,strSemaphore);
    if(hSem)  //Creat CMutex success
    {
        //the CMutex exist
        if(ERROR_ALREADY_EXISTS==GetLastError())
        {
			strSemaphoreExist.LoadString( IDS_SEMAPHORE_ALREADY_EXISTS );
            AfxMessageBox(strSemaphoreExist);
            CloseHandle(hSem);      //Close CMutex
            return FALSE;           //Exit App
        }
    }
    else
    {
		strSemaphoreCreatFail.LoadString(IDS_SEMAPHORE_CREATE_FAIL);
        AfxMessageBox(strSemaphoreCreatFail);
        return FALSE;
    }
	exe_hInstance = GetModuleHandle(NULL);//��ȡhInstance wuhaoyong add
	if(0 == exe_hInstance)
	{
		return FALSE;
	}
	GetModulePath();//��Ҫ�ļ��г�ʼ��
	LoadStdProfileSettings(1);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)


	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CDataMonitorDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ��ܴ���
		RUNTIME_CLASS(CDataMonitorView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// ������׼ shell ���DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// ���á�DDE ִ�С�
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);


	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand ֮����
	// ������/��
	m_pMainWnd->DragAcceptFiles();
	return TRUE;
}

//����_hLangDLL�Ƿ��������dll���ж�ʹ�����Կ���Դ����Ĭ����Դ��Ĭ��Ϊ���ļ���
CString CDataMonitorApp::GetResString(UINT uStringID)
{
	CString resString;
	if (_hLangDLL)
		resString.LoadString(_hLangDLL, uStringID);
	if (resString.IsEmpty())
		resString.LoadString(theApp.exe_hInstance, uStringID);
	return resString;
}

void CDataMonitorApp::GetModulePath()
{
	DWORD dwAttr = 0;
	/******
	TCHAR szPath[MAX_PATH];
	if(SUCCEEDED(SHGetFolderPath(NULL, 
									CSIDL_APPDATA, 
									NULL, 
									0, 
									szPath))) 
	{
		//��ʱ�ļ���·��
		TempPath = szPath;
		CString backupDir = _T("\\Temp\\");
		TempPath += backupDir;
		dwAttr=GetFileAttributes(TempPath);
		//���ļ��в����ڣ������ļ���
		if(dwAttr==0xFFFFFFFF)
		{
			CreateDirectory(TempPath,NULL);
		}
	}
	******/
	TCHAR szModPath[_MAX_PATH];
	GetModuleFileName(m_hInstance, szModPath, _MAX_PATH);
    szExePath = szModPath;
    ModuleFilePath = szExePath.Left(szExePath.ReverseFind('\\'));
	LogPath = ModuleFilePath + _T("\\Log\\");
	dwAttr=GetFileAttributes(LogPath);
	//���ļ��в����ڣ������ļ���
	if(dwAttr==0xFFFFFFFF)
	{
		CreateDirectory(LogPath,NULL);
	}

	TempPath = ModuleFilePath + _T("\\Temp\\");
	//���ļ��в����ڣ������ļ���
	dwAttr=GetFileAttributes(TempPath);
	if(dwAttr==0xFFFFFFFF)
	{
		CreateDirectory(TempPath,NULL);
	}

	DataPath = ModuleFilePath + _T("\\Data\\");
	//���ļ��в����ڣ������ļ���
	dwAttr=GetFileAttributes(DataPath);
	if(dwAttr==0xFFFFFFFF)
	{
		CreateDirectory(DataPath,NULL);
	}

	JobPath = ModuleFilePath + _T("\\Jobs\\");
	//���ļ��в����ڣ������ļ���
	dwAttr=GetFileAttributes(JobPath);
	if(dwAttr==0xFFFFFFFF)
	{
		CreateDirectory(JobPath,NULL);
	}

	ToolPath = ModuleFilePath + _T("\\Tools\\");
	//���ļ��в����ڣ������ļ���
	dwAttr=GetFileAttributes(ToolPath);
	if(dwAttr==0xFFFFFFFF)
	{
		CreateDirectory(ToolPath,NULL);
	}

	IniFilePath = ModuleFilePath + _T("\\Config\\");
	//���ļ��в����ڣ������ļ���
	dwAttr=GetFileAttributes(IniFilePath);
	if(dwAttr==0xFFFFFFFF)
	{
		CreateDirectory(IniFilePath,NULL);
	}
	strIniFilePath = IniFilePath + _T("config.ini");
#ifdef FEATURE_LOG
	LogFileName = LogPath + _T("log.txt");
#endif
	return;
}

int CDataMonitorApp::ExitInstance()
{
	//TODO: �����������ӵĸ�����Դ
	AfxOleTerm(FALSE);

	return CWinApp::ExitInstance();
}

// CDataMonitorApp ��Ϣ�������


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

// �������жԻ����Ӧ�ó�������
void CDataMonitorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CDataMonitorApp ��Ϣ�������



