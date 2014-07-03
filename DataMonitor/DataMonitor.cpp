
// DataMonitor.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "DataMonitor.h"
#include "MonitorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMonitorApp

BEGIN_MESSAGE_MAP(CMonitorApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

static HINSTANCE _hLangDLL = NULL;

// CMonitorApp ����

CMonitorApp::CMonitorApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CMonitorApp ����

CMonitorApp theApp;


// CMonitorApp ��ʼ��

BOOL CMonitorApp::InitInstance()
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


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

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
	CMonitorDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

//����_hLangDLL�Ƿ��������dll���ж�ʹ�����Կ���Դ����Ĭ����Դ��Ĭ��Ϊ���ļ���
CString CMonitorApp::GetResString(UINT uStringID)
{
	CString resString;
	if (_hLangDLL)
		resString.LoadString(_hLangDLL, uStringID);
	if (resString.IsEmpty())
		resString.LoadString(theApp.exe_hInstance, uStringID);
	return resString;
}

