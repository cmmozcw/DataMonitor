// JobEditDlgTool.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataMonitor.h"
#include "JobEditDlgTool.h"
#include "afxdialogex.h"


// CJobEditDlgTool �Ի���

IMPLEMENT_DYNAMIC(CJobEditDlgTool, CDialog)

CJobEditDlgTool::CJobEditDlgTool(CWnd* pParent /*=NULL*/)
	: CDialog(CJobEditDlgTool::IDD, pParent)
	, m_Label(_T(""))
	, m_Type(_T(""))
	, m_SN(_T(""))
	, m_Length(0)
	, m_Weight(0)
	, m_OuteRdiam(0)
	, m_Speed(_T(""))
{

}

CJobEditDlgTool::~CJobEditDlgTool()
{
}

void CJobEditDlgTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LABEL, m_Label);
	DDX_Text(pDX, IDC_TYPE, m_Type);
	DDX_Text(pDX, IDC_SN, m_SN);
	DDX_Text(pDX, IDC_LENGTH, m_Length);
	DDX_Text(pDX, IDC_WEIGHT, m_Weight);
	DDX_Text(pDX, IDC_OUTERDIAM, m_OuteRdiam);
	DDX_Text(pDX, IDC_SPEED5, m_Speed);
}


BEGIN_MESSAGE_MAP(CJobEditDlgTool, CDialog)
	ON_EN_CHANGE(IDC_LABEL, &CJobEditDlgTool::OnEnChangeLabel)
	ON_EN_CHANGE(IDC_TYPE, &CJobEditDlgTool::OnEnChangeType)
	ON_EN_CHANGE(IDC_LENGTH, &CJobEditDlgTool::OnEnChangeLength)
	ON_EN_CHANGE(IDC_SN, &CJobEditDlgTool::OnEnChangeSn)
	ON_EN_CHANGE(IDC_WEIGHT, &CJobEditDlgTool::OnEnChangeWeight)
	ON_EN_CHANGE(IDC_OUTERDIAM, &CJobEditDlgTool::OnEnChangeOuterdiam)
	ON_EN_CHANGE(IDC_SPEED, &CJobEditDlgTool::OnEnChangeSpeed)
END_MESSAGE_MAP()


// CJobEditDlgTool ��Ϣ�������


void CJobEditDlgTool::OnEnChangeLabel()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
}


void CJobEditDlgTool::OnEnChangeType()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
}


void CJobEditDlgTool::OnEnChangeLength()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
}


void CJobEditDlgTool::OnEnChangeSn()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
}


void CJobEditDlgTool::OnEnChangeWeight()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
}


void CJobEditDlgTool::OnEnChangeOuterdiam()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
}


void CJobEditDlgTool::OnEnChangeSpeed()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
}
