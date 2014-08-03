// TargetDepth.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataMonitor.h"
#include "TargetDepth.h"
#include "afxdialogex.h"


// CTargetDepth �Ի���

IMPLEMENT_DYNAMIC(CTargetDepth, CDialog)

CTargetDepth::CTargetDepth(CWnd* pParent /*=NULL*/)
	: CDialog(CTargetDepth::IDD, pParent)
{
	strEditOne = _T("0");
	strEditTwo = _T("0");
}

CTargetDepth::~CTargetDepth()
{
}

void CTargetDepth::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_DEPTHTIP, staticTip);
	DDX_Control(pDX, IDC_STATIC_DEPTHONE, staticOne);
	DDX_Control(pDX, IDC_STATIC_DEPTHTWO, staticTwo);
	DDX_Control(pDX, IDC_EDIT1, edit1);
	DDX_Control(pDX, IDC_EDIT2, edit2);
}


BEGIN_MESSAGE_MAP(CTargetDepth, CDialog)
	ON_BN_CLICKED(IDOK, &CTargetDepth::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CTargetDepth::OnBnClickedCancel)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CTargetDepth ��Ϣ�������


void CTargetDepth::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SaveParameter();
	CDialog::OnOK();
}


void CTargetDepth::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialog::OnCancel();
}


BOOL CTargetDepth::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetWindowText(theApp.GetResString(IDS_SET_TARGETDEPTH));
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}




void CTargetDepth::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: �ڴ˴������Ϣ����������
	if(staticTip.m_hWnd != NULL)
	{
		staticTip.SetWindowText(theApp.GetResString(IDS_TARGETDEPTH_TIP));
	}
	if(staticOne.m_hWnd != NULL)
	{
		staticOne.SetWindowText(theApp.GetResString(IDS_TARGETDEPTH_ONE));
	}
	if(staticTwo.m_hWnd != NULL)
	{
		staticTwo.SetWindowText(theApp.GetResString(IDS_TARGETDEPTH_TWO));
	}
	if(edit1.m_hWnd != NULL)
	{
		edit1.SetWindowText(strEditOne);
	}
	if(edit2.m_hWnd != NULL)
	{
		edit2.SetWindowText(strEditTwo);
	}
}

void CTargetDepth::SaveParameter()
{

}