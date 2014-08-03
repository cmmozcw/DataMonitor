// JobDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataMonitor.h"
#include "JobDlg.h"
#include "afxdialogex.h"
#include "JobCopyDlg.h"

// CJobDlg �Ի���

IMPLEMENT_DYNAMIC(CJobDlg, CDialog)

CJobDlg::CJobDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CJobDlg::IDD, pParent)
{
	receiveFlag = 0;
}

CJobDlg::~CJobDlg()
{
}

void CJobDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ZCW_JOB_TREE, m_treeCtrl);
	DDX_Control(pDX, IDC_ZCW_JOB_TREE, m_treeCtrl);
}


BEGIN_MESSAGE_MAP(CJobDlg, CDialog)
	ON_COMMAND(ID_JOB_NEW, &CJobDlg::OnJobNew)
	ON_COMMAND(ID_JOB_OPEN, &CJobDlg::OnJobOpen)
	ON_COMMAND(ID_JOB_DEL, &CJobDlg::OnJobDel)
	ON_COMMAND(ID_JOB_COPY, &CJobDlg::OnJobCopy)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CJobDlg ��Ϣ�������


void CJobDlg::OnJobNew()
{
	// TODO: �ڴ���������������
	CFileDialog dlg(FALSE,_T(""),_T(""),OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("tools files(*.tools)|*.tools|tool files(*.mdb)|*.mdb|All files(*.*)|*.*|"));//���챣���ļ��Ի���
	dlg.m_ofn.lpstrInitialDir=m_Path;
    if(dlg.DoModal()==IDOK)//��ʾ�����ļ��Ի���
	{
		CJobEditDlg m_jeDlg;
		m_jeDlg.m_jobName=dlg.GetPathName();//��ȡ�ļ�·������
		m_jeDlg.m_Open=FALSE;
		if(m_jeDlg.DoModal()==IDOK){
		m_treeCtrl.ModifyStyle(0,TVS_HASBUTTONS   |   TVS_LINESATROOT   |   TVS_HASLINES);	
		m_treeCtrl.DisplayTree (m_Path,TRUE);
		}
	}
}


void CJobDlg::OnJobOpen()
{
	// TODO: �ڴ���������������
		CJobEditDlg m_jeDlg;
		HTREEITEM sel_htem=m_treeCtrl.GetSelectedItem(); 
		 CString strPath=m_treeCtrl.GetFullPath(sel_htem);//��ȡ�ļ�·������
		if(!m_treeCtrl.FindSubDir(strPath)){//���ܱ༭�ǿ���Ŀ¼
		m_jeDlg.m_jobName=strPath;//��ȡ�ļ�·������
		m_jeDlg.m_Open=TRUE;
		m_jeDlg.DoModal();
		}
}


BOOL CJobDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetWindowText(m_Title); 	
	GetDlgItem(IDC_ZCW_JOB_TREE_TITLE)->SetWindowText(m_TreeTitle);
	m_treeCtrl.ModifyStyle(0,TVS_HASBUTTONS   |   TVS_LINESATROOT   |   TVS_HASLINES);	
	m_treeCtrl.DisplayTree (m_Path,TRUE);


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CJobDlg::OnJobDel()
{
	// TODO: �ڴ���������������
	HTREEITEM sel_htem=m_treeCtrl.GetSelectedItem(); 
    CString strPath=m_treeCtrl.GetFullPath(sel_htem);//��ȡ�ļ�·������
	if(!m_treeCtrl.FindSubDir(strPath)){//����ɾ���ǿ���Ŀ¼
		if(MessageBox(_T("��ȷ��Ҫɾ����ҵ")+strPath+_T("��?"),_T("����"), MB_OKCANCEL )==IDOK)
		{
		  DeleteFile(strPath);
		  m_treeCtrl.DeleteItem(sel_htem);//ɾ���ڵ�
		} 
	}
}


void CJobDlg::OnJobCopy()
{
	// TODO: �ڴ���������������
	CJobCopyDlg m_jcDlg;
	HTREEITEM sel_htem=m_treeCtrl.GetSelectedItem(); //��ǰ�ڵ�
	HTREEITEM par_htem=m_treeCtrl.GetParentItem(sel_htem); //���ڵ�
	CString strPath=m_treeCtrl.GetFullPath(sel_htem);	
	if(!m_treeCtrl.FindSubDir(strPath)){//���ܸ���Ŀ¼
    CString stroldName=m_treeCtrl.GetItemText(sel_htem);
	m_jcDlg.m_oldName=stroldName.Left(stroldName.Find(_T(".")));//��ȡ�ļ���
	if(m_jcDlg.DoModal()==IDOK){
		CString strNewPath=m_treeCtrl.GetFullPath(par_htem)+_T("\\")+m_jcDlg.m_newName+_T(".tools");//����ҵ·��
		if(GetFileAttributes(strNewPath)==-1L){
		CopyFile(strPath,strNewPath,TRUE);
		m_treeCtrl.AddItem(par_htem,strNewPath);//��ӽڵ�
		}else{
			MessageBox(strNewPath+_T("�Ѵ���"),MB_OK);
		}
	}
	}
}

void CJobDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: �ڴ˴������Ϣ����������
	if(receiveFlag == 1)
	{
		OnJobNew();
	}
}
