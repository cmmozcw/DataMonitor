// JobDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DMonitor.h"
#include "JobDlg.h"
#include "afxdialogex.h"


// CJobDlg �Ի���

IMPLEMENT_DYNAMIC(CJobDlg, CDialog)

CJobDlg::CJobDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CJobDlg::IDD, pParent)
{

}

CJobDlg::~CJobDlg()
{
}

void CJobDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_NEWJOB, btnNewJob);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, btnClose);
}


BEGIN_MESSAGE_MAP(CJobDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CJobDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CJobDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_NEWJOB, &CJobDlg::OnBnClickedButtonNewjob)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CJobDlg::OnBnClickedButtonClose)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CJobDlg ��Ϣ�������
BOOL CJobDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetWindowText(m_Title); 
	CRect rectMain;
	GetClientRect(rectMain);
	if(btnClose.m_hWnd != NULL)
	{
		btnClose.SetWindowPos(NULL,rectMain.right-105 ,rectMain.bottom -50 ,100,40 ,0);
	}
	if(btnNewJob.m_hWnd != NULL)
	{
		btnNewJob.SetWindowPos(NULL,rectMain.right-225 ,rectMain.bottom -50 ,100,40 ,0);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CJobDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialog::OnOK();
}


void CJobDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialog::OnCancel();
}


BOOL CJobDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		{
			switch (pMsg->wParam)
			{
			case VK_ESCAPE:
				return TRUE;
			case VK_UP://�������·����
			case VK_DOWN:	
			case VK_SPACE:
			case VK_RETURN://���λس�
				return TRUE;
			default:
				break;
			}
		}
		break;
	default:
		break;
	}
	return CDialog::PreTranslateMessage(pMsg);
}





void CJobDlg::OnBnClickedButtonNewjob()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CJobDlg::OnBnClickedButtonClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CJobDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	CRect rectMain;
	GetClientRect(rectMain);
	if(btnClose.m_hWnd != NULL)
	{
		btnClose.SetWindowPos(NULL,rectMain.right-105 ,rectMain.bottom -50 ,100,40 ,0);
	}
	if(btnNewJob.m_hWnd != NULL)
	{
		btnNewJob.SetWindowPos(NULL,rectMain.right-225 ,rectMain.bottom -50 ,100,40 ,0);
	}
}
