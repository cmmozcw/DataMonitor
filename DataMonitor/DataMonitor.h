
// DataMonitor.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "CommLayer.h"

// CMonitorApp:
// �йش����ʵ�֣������ DataMonitor.cpp
//

class CMonitorApp : public CWinApp
{
public:
	HINSTANCE exe_hInstance;
	HANDLE hSem;//�ж���������Ƿ��Ѵ���
	CString strSemaphore;
    CString strSemaphoreExist;
    CString strSemaphoreCreatFail;
	CString strUserSetCommPort;//�û�ѡ��Ĵ��ں�
	BYTE m_DeviceType;//�豸����
	CCommLayer commLayer;
	int m_CommResault;//��¼�������ӽ��

	//�洢·����Ϣ
	CString szExePath;//��ִ���ļ�����λ�þ���·��
	CString ModuleFilePath;
	CString TempPath;//��ʱ�ļ��о���·��
	CString LogPath;//log��־����λ�þ���·��
	CString IniFilePath;//ini�ļ��о���·��
	CString strIniFilePath;//ini�ļ�����λ�þ���·��
	//���͸������豸������buffer
	UINT8 sendCmd[COMM_BUFFER_BASESIZE];//ͨ�Ų㷢������ buffer
	WORD sendDataSize;
	CWnd* activeWnd;//����child����
public:
	CMonitorApp();
	CString GetResString(UINT uStringID);
	bool isConnected();
private:
	void GetModulePath();
// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMonitorApp theApp;