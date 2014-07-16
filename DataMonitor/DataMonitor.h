
// DataMonitor.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "CommLayer.h"

// CMonitorApp:
// 有关此类的实现，请参阅 DataMonitor.cpp
//

class CMonitorApp : public CWinApp
{
public:
	HINSTANCE exe_hInstance;
	HANDLE hSem;//判断软件市立是否已存在
	CString strSemaphore;
    CString strSemaphoreExist;
    CString strSemaphoreCreatFail;
	CString strUserSetCommPort;//用户选择的串口号
	BYTE m_DeviceType;//设备类型
	CCommLayer commLayer;
	int m_CommResault;//记录串口连接结果

	//存储路径信息
	CString szExePath;//可执行文件所在位置绝对路径
	CString ModuleFilePath;
	CString TempPath;//临时文件夹绝对路径
	CString LogPath;//log日志所在位置绝对路径
	CString IniFilePath;//ini文件夹绝对路径
	CString strIniFilePath;//ini文件所在位置绝对路径
	//发送给串口设备的数据buffer
	UINT8 sendCmd[COMM_BUFFER_BASESIZE];//通信层发送数据 buffer
	WORD sendDataSize;
	CWnd* activeWnd;//加载child窗口
public:
	CMonitorApp();
	CString GetResString(UINT uStringID);
	bool isConnected();
private:
	void GetModulePath();
// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CMonitorApp theApp;