// PanelView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataMonitor.h"
#include "PanelView.h"


// CPanelView

IMPLEMENT_DYNCREATE(CPanelView, CFormView)

CPanelView::CPanelView()
	: CFormView(CPanelView::IDD)
{

}

CPanelView::~CPanelView()
{
}

void CPanelView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPanelView, CFormView)
END_MESSAGE_MAP()


// CPanelView ���

#ifdef _DEBUG
void CPanelView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CPanelView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPanelView ��Ϣ�������
