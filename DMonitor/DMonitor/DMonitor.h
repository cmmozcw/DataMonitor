
// DMonitor.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "CommLayer.h"

// CDMoniterApp:
// �йش����ʵ�֣������ DMonitor.cpp
//

class CDMoniterApp : public CWinApp
{
public:
	CDMoniterApp();
	CString GetResString(UINT uStringID);
	char *FromUNICODEToANSI(CString str);
	short FromUnicodeToUTF8 (LPSTR utf8Str, short utf8StrLen, WORD * unStr, unsigned short unMaxLen);
	void GetModulePath();
	//int CreateDmFile(CString strFile);
	int CreateIniFile(CString strFile);

	//void LinkElementeFuns(TiXmlElement * element, CString str);

	CCommLayer commLayer;//����ͨ�Ź���
	HINSTANCE exe_hInstance;
	HANDLE hSem;//�ж���������Ƿ��Ѵ���

	// MFC�ٽ��������
	CCriticalSection g_cs;

	CString strSemaphore;
    CString strSemaphoreExist;
    CString strSemaphoreCreatFail;
	CString strUserSetCommPort;//�û�ѡ��Ĵ��ں�
	BYTE m_DeviceType;//�豸����
	int m_CommResault;//��¼�������ӽ��
//�洢·����Ϣ
	CString szExePath;//��ִ���ļ�����λ�þ���·��
	CString ModuleFilePath;
	CString TempPath;//��ʱ�ļ��о���·��
	CString LogPath;//log��־����λ�þ���·��
	CString IniFilePath;//ini�ļ��о���·��
	CString strIniFilePath;//ini�ļ�����λ�þ���·��
	CString DataPath;//��ʱ�ļ��о���·��
	CString JobPath;//��ҵ�ļ��еľ���·��
	CString ToolPath;//�����ļ��о���·��
	CString strUnitsFile;//��¼������λ�������ļ�
	CString strCurveFile;//��¼��ͼģ��������ļ�
	CString strSettingFile;//��¼�û����õĳ�����ֵ�����ļ�
	//���͸������豸������buffer
	bool bCurveFileExist;
	UINT8 sendCmd[COMM_BUFFER_BASESIZE];//ͨ�Ų㷢������ buffer
	WORD sendDataSize;
// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDMoniterApp theApp;