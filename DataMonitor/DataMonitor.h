
// DataMonitor.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMonitorApp:
// �йش����ʵ�֣������ DataMonitor.cpp
//

class CMonitorApp : public CWinApp
{
public:
	HINSTANCE exe_hInstance;
	HANDLE hSem;//�ж����������Ƿ��Ѵ���
	CString strSemaphore;
    CString strSemaphoreExist;
    CString strSemaphoreCreatFail;
public:
	CMonitorApp();
	CString GetResString(UINT uStringID);
// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMonitorApp theApp;