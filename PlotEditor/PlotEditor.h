
// PlotEditor.h : PlotEditor Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CPlotEditorApp:
// �йش����ʵ�֣������ PlotEditor.cpp
//

class CPlotEditorApp : public CWinApp
{
public:
	CPlotEditorApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CPlotEditorApp theApp;
