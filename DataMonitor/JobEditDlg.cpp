// JobEditDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataMonitor.h"
#include "JobEditDlg.h"
#include "afxdialogex.h"


// CJobEditDlg �Ի���

IMPLEMENT_DYNAMIC(CJobEditDlg, CDialog)

CJobEditDlg::CJobEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CJobEditDlg::IDD, pParent)
	, m_jobName(_T(""))
	, m_Open(false)
{
	
}

CJobEditDlg::~CJobEditDlg()
{
}

void CJobEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_JOBNAME, m_jobName);
	DDX_Control(pDX, IDC_JOBEDIT_TREE, m_jeTree);
}


BEGIN_MESSAGE_MAP(CJobEditDlg, CDialog)
	ON_NOTIFY(TVN_SELCHANGED, IDC_JOBEDIT_TREE, &CJobEditDlg::OnSelchangedJobeditTree)
	ON_BN_CLICKED(IDOK, &CJobEditDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_ADD, &CJobEditDlg::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_DELETE, &CJobEditDlg::OnBnClickedDelete)
END_MESSAGE_MAP()


// CJobEditDlg ��Ϣ�������

BOOL CJobEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	if(m_Open==TRUE){//����Ǵ���ҵ�������������ݿ����ݳ�ʼ������
		CableInit();
	}else{
		JobInit();		
	}

	DisplayTree();
	CRect rect;  
	GetDlgItem(IDC_STATIC_RECT)-> GetWindowRect(&rect);  
	ScreenToClient(&rect); 
	//Ĭ����ʾ������Ϣ���
	m_Cable.Create(IDD_JOBEDIT_CABLE,this);  
	m_Cable.MoveWindow(rect);  
	m_Cable.ShowWindow(SW_SHOW); 

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CJobEditDlg::DisplayTree(){
	m_jeTree.DeleteAllItems();
	//���οؼ���ʼ��
	m_jeTree.ModifyStyle(0,TVS_HASBUTTONS|TVS_LINESATROOT|TVS_HASLINES);
	HTREEITEM root0=m_jeTree.InsertItem(_T("��о��ʾ"),0,1,TVI_ROOT,TVI_LAST);
	m_jeTree.SetItemData(root0,(DWORD)0);
	HTREEITEM root1=m_jeTree.InsertItem(_T("������Ϣ"),0,1,TVI_ROOT,TVI_LAST);
	m_jeTree.SetItemData(root1,(DWORD)1);
	HTREEITEM root2=m_jeTree.InsertItem(_T("������Ϣ"),0,1,TVI_ROOT,TVI_LAST);
	m_jeTree.SetItemData(root2,(DWORD)2);
	HTREEITEM root3=m_jeTree.InsertItem(_T("ԭʼ�ź�"),0,1,TVI_ROOT,TVI_LAST);
	m_jeTree.SetItemData(root3,(DWORD)3);
	HTREEITEM root4=m_jeTree.InsertItem(_T("������Ϣ"),0,1,TVI_ROOT,TVI_LAST);
	m_jeTree.SetItemData(root4,(DWORD)4);
	HTREEITEM root5=m_jeTree.InsertItem(_T("�ӵ����"),0,1,TVI_ROOT,TVI_LAST);
	m_jeTree.SetItemData(root5,(DWORD)5);

	//һ���ӽڵ�
	 CDaoDatabase m_DataBase;
	 try{
				m_DataBase.Open(m_jobName);			   	  			   
				CDaoRecordset rs(&m_DataBase);
				 COleVariant OleVariant ;
				rs.Open(dbOpenDynaset, L"SELECT * FROM TOOL" );		
				while(!rs.IsEOF()){
					rs.GetFieldValue (0,OleVariant);
					m_jeTree.InsertItem(OleVariant.bstrVal,0,1,root1,TVI_LAST);
					rs.MoveNext();
				}
				rs.Close();              
				//�ر����ݿ�
				if (m_DataBase.IsOpen())
				m_DataBase.Close();
	 } catch(CDaoException *e){		
			MessageBox(e->m_pErrorInfo->m_strDescription);
			e->Delete();
     }
}

void CJobEditDlg::OnSelchangedJobeditTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	HTREEITEM hSel=m_jeTree.GetSelectedItem();
	 DWORD ids;
	 ids = m_jeTree.GetItemData(hSel);
	 CRect rect;  
	 GetDlgItem(IDC_STATIC_RECT)-> GetWindowRect(&rect);  
	 ScreenToClient(&rect);  
	 if(m_Cable.GetSafeHwnd()!=NULL)
	 		m_Cable.DestroyWindow();//�����о��ʾ����Ѿ��򿪹�������������������ظ���
	 if(m_Tool.GetSafeHwnd()!=NULL)
			m_Tool.DestroyWindow();//�����Ϣ��������Ѿ��򿪹�������������������ظ���
	 switch(ids){
	 case 0:		    
			m_Cable.Create(IDD_JOBEDIT_CABLE, this);  
			m_Cable.MoveWindow(rect);  
			m_Cable.ShowWindow( SW_SHOW ); 
			GetDlgItem(IDC_ADD)->ShowWindow(FALSE);
			GetDlgItem(IDC_DELETE)->ShowWindow(FALSE);
			m_editTable=0;
		 break;
	 case 1:		  
			m_Tool.Create(IDD_JOBEDIT_TOOL, this);  
			m_Tool.MoveWindow(rect);  
			m_Tool.ShowWindow( SW_SHOW ); 	
			GetDlgItem(IDC_ADD)->ShowWindow(TRUE);
			GetDlgItem(IDC_DELETE)->ShowWindow(TRUE);
			m_editTable=1;
		break;
	 case 2:		  		
		    GetDlgItem(IDC_ADD)->ShowWindow(TRUE);
			GetDlgItem(IDC_DELETE)->ShowWindow(TRUE);
			m_editTable=2;		
		break;
	 case 3:
			GetDlgItem(IDC_ADD)->ShowWindow(TRUE);
			GetDlgItem(IDC_DELETE)->ShowWindow(TRUE);
			m_editTable=3;
		break;
	 case 4:		  			
			GetDlgItem(IDC_ADD)->ShowWindow(FALSE);
			GetDlgItem(IDC_DELETE)->ShowWindow(FALSE);
			m_editTable=4;
		break;
	 case 5:		  			
			GetDlgItem(IDC_ADD)->ShowWindow(FALSE);
			GetDlgItem(IDC_DELETE)->ShowWindow(FALSE);
			m_editTable=5;
		break;
	 }
	*pResult = 0;
}

void CJobEditDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������	
	 if(m_Open==TRUE){//�����ҵ�Ѿ��������޸�
		CableUpdate();
	   }else{//���򴴽�����ҵ
		CableAdd();         
	 }
	CDialog::OnOK();
}

void CJobEditDlg::CableInit(){
	//�������ݿ��ʼ�����ڿؼ�����
	 CDaoDatabase m_DataBase;
	 COleVariant OleVariant ;
		try{
			m_DataBase.Open(m_jobName);				
				CDaoRecordset rs(&m_DataBase);
				rs.Open(dbOpenDynaset, L"SELECT * FROM CABLE" );
					rs.MoveFirst () ;				
					rs.GetFieldValue (2,OleVariant);
					m_Cable.m_Wire1=OleVariant.bstrVal;
					rs.GetFieldValue (3,OleVariant);
					m_Cable.m_Panel1=OleVariant.bstrVal;

					rs.MoveNext();
					rs.GetFieldValue (2,OleVariant);
					m_Cable.m_Wire2=OleVariant.bstrVal;
					rs.GetFieldValue (3,OleVariant);
					m_Cable.m_Panel2=OleVariant.bstrVal;

					rs.MoveNext();
					rs.GetFieldValue (2,OleVariant);
					m_Cable.m_Wire3=OleVariant.bstrVal;
					rs.GetFieldValue (3,OleVariant);
					m_Cable.m_Panel3=OleVariant.bstrVal;

					rs.MoveNext();
					rs.GetFieldValue (2,OleVariant);
					m_Cable.m_Wire4=OleVariant.bstrVal;
					rs.GetFieldValue (3,OleVariant);
					m_Cable.m_Panel4=OleVariant.bstrVal;

					rs.MoveNext();
					rs.GetFieldValue (2,OleVariant);
					m_Cable.m_Wire5=OleVariant.bstrVal;
					rs.GetFieldValue (3,OleVariant);
					m_Cable.m_Panel5=OleVariant.bstrVal;

					rs.MoveNext();
					rs.GetFieldValue (2,OleVariant);
					m_Cable.m_Wire6=OleVariant.bstrVal;
					rs.GetFieldValue (3,OleVariant);
					m_Cable.m_Panel6=OleVariant.bstrVal;

					rs.MoveNext();
					rs.GetFieldValue (2,OleVariant);
					m_Cable.m_Wire7=OleVariant.bstrVal;
					rs.GetFieldValue (3,OleVariant);
					m_Cable.m_Panel7=OleVariant.bstrVal;

					rs.MoveNext();
					rs.GetFieldValue (2,OleVariant);
					m_Cable.m_Wire8=OleVariant.bstrVal;
					rs.GetFieldValue (3,OleVariant);
					m_Cable.m_Panel8=OleVariant.bstrVal;

					rs.MoveNext();
					rs.GetFieldValue (2,OleVariant);
					m_Cable.m_Wire9=OleVariant.bstrVal;
					rs.GetFieldValue (3,OleVariant);
					m_Cable.m_Panel9=OleVariant.bstrVal;
					
					rs.MoveNext();
					rs.GetFieldValue (2,OleVariant);
					m_Cable.m_Wire10=OleVariant.bstrVal;
					rs.GetFieldValue (3,OleVariant);
					m_Cable.m_Panel10=OleVariant.bstrVal;
				rs.Close();
				//�ر����ݿ�
				if (m_DataBase.IsOpen())
					m_DataBase.Close();
            }
            catch(CDaoException *e){
			MessageBox(e->m_pErrorInfo->m_strDescription);
			e->Delete();
            }
}

void CJobEditDlg::CableAdd()
{
	 CDaoDatabase m_DataBase;
	 try{
				m_DataBase.Open(m_jobName);
			   //�½����ݿ��¼
			    CDaoTableDef td(&m_DataBase);        //��������
			   
				CDaoRecordset rs(&m_DataBase);
				rs.Open(dbOpenDynaset, L"SELECT * FROM CABLE" );

				rs.AddNew();
				rs.SetFieldValue(0,_T("1#"));
				rs.SetFieldValue(2,COleVariant(m_Cable.m_Wire1));	
				rs.SetFieldValue(3,COleVariant(m_Cable.m_Panel1));
				rs.Update();

				rs.AddNew();
				rs.SetFieldValue(0,_T("2#"));
				rs.SetFieldValue(2,COleVariant(m_Cable.m_Wire2));	
				rs.SetFieldValue(3,COleVariant(m_Cable.m_Panel2));
				rs.Update();

				rs.AddNew();
				rs.SetFieldValue(0,_T("3#"));
				rs.SetFieldValue(2,COleVariant(m_Cable.m_Wire3));	
				rs.SetFieldValue(3,COleVariant(m_Cable.m_Panel3));
				rs.Update();

				rs.AddNew();
				rs.SetFieldValue(0,_T("4#"));
				rs.SetFieldValue(2,COleVariant(m_Cable.m_Wire4));	
				rs.SetFieldValue(3,COleVariant(m_Cable.m_Panel4));
				rs.Update();

				rs.AddNew();
				rs.SetFieldValue(0,_T("5#"));
				rs.SetFieldValue(2,COleVariant(m_Cable.m_Wire5));	
				rs.SetFieldValue(3,COleVariant(m_Cable.m_Panel5));
				rs.Update();

				rs.AddNew();
				rs.SetFieldValue(0,_T("6#"));
				rs.SetFieldValue(2,COleVariant(m_Cable.m_Wire6));	
				rs.SetFieldValue(3,COleVariant(m_Cable.m_Panel6));
				rs.Update();

				rs.AddNew();
				rs.SetFieldValue(0,_T("7#"));
				rs.SetFieldValue(2,COleVariant(m_Cable.m_Wire7));	
				rs.SetFieldValue(3,COleVariant(m_Cable.m_Panel7));
				rs.Update();

				rs.AddNew();
				rs.SetFieldValue(0,_T("CT#"));
				rs.SetFieldValue(2,COleVariant(m_Cable.m_Wire8));	
				rs.SetFieldValue(3,COleVariant(m_Cable.m_Panel8));
				rs.Update();

				rs.AddNew();
				rs.SetFieldValue(0,_T("Ground#"));
				rs.SetFieldValue(2,COleVariant(m_Cable.m_Wire9));	
				rs.SetFieldValue(3,COleVariant(m_Cable.m_Panel9));
				rs.Update();

				rs.AddNew();
				rs.SetFieldValue(0,_T("SWITCHCODE#"));
				rs.SetFieldValue(2,COleVariant(m_Cable.m_Wire10));	
				rs.SetFieldValue(3,COleVariant(m_Cable.m_Panel10));
				rs.Update();

				rs.Close();
                td.Close();
				//�ر����ݿ�
				if (m_DataBase.IsOpen())
				m_DataBase.Close();
	 } catch(CDaoException *e){
			MessageBox(e->m_pErrorInfo->m_strDescription);
			e->Delete();
            }
}

void CJobEditDlg::CableUpdate(){
	    CDaoDatabase m_DataBase;
		try{
			m_DataBase.Open(m_jobName);
				//�޸����ݿ��¼
			CDaoRecordset rs(&m_DataBase);				
				rs.Open(dbOpenDynaset, L"SELECT * FROM CABLE" );					
					rs.MoveFirst () ;
					rs.Edit();
					rs.SetFieldValue(2,COleVariant(m_Cable.m_Wire1));	
					rs.SetFieldValue(3,COleVariant(m_Cable.m_Panel1));
					rs.Update();

					rs.MoveNext();
					rs.Edit();
					rs.SetFieldValue(2,COleVariant(m_Cable.m_Wire2));	
					rs.SetFieldValue(3,COleVariant(m_Cable.m_Panel2));
					rs.Update();

					rs.MoveNext();
					rs.Edit();
					rs.SetFieldValue(2,COleVariant(m_Cable.m_Wire3));	
					rs.SetFieldValue(3,COleVariant(m_Cable.m_Panel3));
					rs.Update();

					rs.MoveNext();
					rs.Edit();
					rs.SetFieldValue(2,COleVariant(m_Cable.m_Wire4));	
					rs.SetFieldValue(3,COleVariant(m_Cable.m_Panel4));
					rs.Update();

					rs.MoveNext();
					rs.Edit();
					rs.SetFieldValue(2,COleVariant(m_Cable.m_Wire5));	
					rs.SetFieldValue(3,COleVariant(m_Cable.m_Panel5));
					rs.Update();

					rs.MoveNext();
					rs.Edit();
					rs.SetFieldValue(2,COleVariant(m_Cable.m_Wire6));	
					rs.SetFieldValue(3,COleVariant(m_Cable.m_Panel6));
					rs.Update();

					rs.MoveNext();
					rs.Edit();
					rs.SetFieldValue(2,COleVariant(m_Cable.m_Wire7));	
					rs.SetFieldValue(3,COleVariant(m_Cable.m_Panel7));
					rs.Update();

					rs.MoveNext();
					rs.Edit();
					rs.SetFieldValue(2,COleVariant(m_Cable.m_Wire8));	
					rs.SetFieldValue(3,COleVariant(m_Cable.m_Panel8));
					rs.Update();

					rs.MoveNext();
					rs.Edit();
					rs.SetFieldValue(2,COleVariant(m_Cable.m_Wire9));	
					rs.SetFieldValue(3,COleVariant(m_Cable.m_Panel9));
					rs.Update();
					
					rs.MoveNext();
					rs.Edit();
					rs.SetFieldValue(2,COleVariant(m_Cable.m_Wire10));	
					rs.SetFieldValue(3,COleVariant(m_Cable.m_Panel10));
					rs.Update();
				rs.Close();
				//�ر����ݿ�
				if (m_DataBase.IsOpen())
					m_DataBase.Close();
            }
            catch(CDaoException *e){
			MessageBox(e->m_pErrorInfo->m_strDescription);
			e->Delete();
            }
}

void CJobEditDlg::ToolAdd()
{
	 CDaoDatabase m_DataBase;
	 try{
				m_DataBase.Open(m_jobName);
			   //�½����ݿ��¼			  			   
				CDaoRecordset rs(&m_DataBase);				
				rs.Open(dbOpenDynaset,_T("SELECT * FROM TOOL where LABEL='")+m_Tool.m_Label+_T("'"));
				if(rs.IsBOF()){
			    rs.AddNew();
				rs.SetFieldValue(0,COleVariant(m_Tool.m_Label));
				rs.SetFieldValue(1,COleVariant(m_Tool.m_SN));	
				rs.SetFieldValue(2,COleVariant(m_Tool.m_Speed));
				rs.SetFieldValue(3,COleVariant(m_Tool.m_Type));
				rs.SetFieldValue(4,COleVariant(m_Tool.m_Length));	
				rs.SetFieldValue(5,COleVariant(m_Tool.m_Weight));
				rs.SetFieldValue(6,COleVariant(m_Tool.m_OuteRdiam));
				rs.Update();
				}else{
				MessageBox(_T("����")+m_Tool.m_Label+_T("�Ѿ�����"),MB_OK);
				}
				rs.Close();			
				//�ر����ݿ�
				if (m_DataBase.IsOpen())
				m_DataBase.Close();				

	 } catch(CDaoException *e){
			MessageBox(e->m_pErrorInfo->m_strDescription);
			e->Delete();
     }
}

void CJobEditDlg::OnBnClickedAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	switch(m_editTable){
	case 1:
		//MessageBox(_T("������Ϣ"),MB_OK);
		if(m_Tool.m_Label.Trim().IsEmpty()){
		MessageBox(_T("���Ʋ���Ϊ��"),MB_OK);
		}else{
		ToolAdd();		
		DisplayTree();	
		}
		break;
	case 2:
		MessageBox(_T("���������Ϣ"),MB_OK);
		break;
	case 3:
		MessageBox(_T("���ԭʼ�ź�"),MB_OK);
		break;
	case 4:
		MessageBox(_T("��ӿ�����Ϣ"),MB_OK);
		break;
	case 5:
		MessageBox(_T("��Ӽҵ����"),MB_OK);
		break;
	}

}

void CJobEditDlg::JobInit(){
	CDaoDatabase m_DataBase;	
	try{
			m_DataBase.Create(m_jobName);//����½���ҵ�򴴽����ݿ�
				CDaoTableDef td_C(&m_DataBase);        //��������
				CDaoTableDef td_T(&m_DataBase);        //��������
				//����Cable��
				td_C.Create(_T("CABLE"));
				CDaoFieldInfo Cf1;
				Cf1.m_strName = _T("LABEL");
				Cf1.m_nType = dbText;
				Cf1.m_lSize = 50;
				Cf1.m_lAttributes = dbVariableField;//dbVariableFieldΪ�ɱ䳤���ֶΣ������ı��� dbFixedField�ֶγ����ǹ̶���(�����ֶε�Ĭ��ֵ��
				Cf1.m_bRequired = FALSE;
				Cf1.m_bAllowZeroLength = TRUE;				
				td_C.CreateField(Cf1);

				CDaoFieldInfo Cf2;
				Cf2.m_strName = _T("LONGNAME");
				Cf2.m_nType = dbText;
				Cf2.m_lSize = 50;
				Cf2.m_lAttributes = dbVariableField;//dbVariableFieldΪ�ɱ䳤���ֶΣ������ı��� dbFixedField�ֶγ����ǹ̶���(�����ֶε�Ĭ��ֵ��
				Cf2.m_bRequired = FALSE;
				Cf2.m_bAllowZeroLength = TRUE;				
				td_C.CreateField(Cf2);

				CDaoFieldInfo Cf3;
				Cf3.m_strName = _T("WIRE");
				Cf3.m_nType = dbText;
				Cf3.m_lSize = 50;
				Cf3.m_lAttributes = dbVariableField;//dbVariableFieldΪ�ɱ䳤���ֶΣ������ı��� dbFixedField�ֶγ����ǹ̶���(�����ֶε�Ĭ��ֵ��
				Cf3.m_bRequired = FALSE;
				Cf3.m_bAllowZeroLength = TRUE;				
				td_C.CreateField(Cf3);

				CDaoFieldInfo Cf4;
				Cf4.m_strName = _T("PANEL");
				Cf4.m_nType = dbText;
				Cf4.m_lSize = 50;
				Cf4.m_lAttributes = dbVariableField;//dbVariableFieldΪ�ɱ䳤���ֶΣ������ı��� dbFixedField�ֶγ����ǹ̶���(�����ֶε�Ĭ��ֵ��
				Cf4.m_bRequired = FALSE;
				Cf4.m_bAllowZeroLength = TRUE;				
				td_C.CreateField(Cf4);

				CDaoFieldInfo Cf5;
				Cf5.m_strName = _T("REMARK");
				Cf5.m_nType = dbText;
				Cf5.m_lSize = 50;
				Cf5.m_lAttributes = dbVariableField;//dbVariableFieldΪ�ɱ䳤���ֶΣ������ı��� dbFixedField�ֶγ����ǹ̶���(�����ֶε�Ĭ��ֵ��
				Cf5.m_bRequired = FALSE;
				Cf5.m_bAllowZeroLength = TRUE;			
				td_C.CreateField(Cf5);
			    td_C.Append();
				td_C.Close();

				//����TOOL��
				td_T.Create(_T("TOOL"));
				CDaoFieldInfo Tf1;
				Tf1.m_strName = _T("LABEL");
				Tf1.m_nType = dbText;
				Tf1.m_lSize = 50;
				Tf1.m_lAttributes = dbVariableField;//dbVariableFieldΪ�ɱ䳤���ֶΣ������ı��� dbFixedField�ֶγ����ǹ̶���(�����ֶε�Ĭ��ֵ��
				Tf1.m_bRequired = FALSE;
				Tf1.m_bAllowZeroLength = TRUE;							
				td_T.CreateField(Tf1);
			
				CDaoFieldInfo Tf2;
				Tf2.m_strName = _T("TYPE");
				Tf2.m_nType = dbText;
				Tf2.m_lSize = 50;
				Tf2.m_lAttributes = dbVariableField;//dbVariableFieldΪ�ɱ䳤���ֶΣ������ı��� dbFixedField�ֶγ����ǹ̶���(�����ֶε�Ĭ��ֵ��
				Tf2.m_bRequired = FALSE;
				Tf2.m_bAllowZeroLength = TRUE;			
				td_T.CreateField(Tf2);

				CDaoFieldInfo Tf3;
				Tf3.m_strName = _T("SN");
				Tf3.m_nType = dbText;
				Tf3.m_lSize = 50;
				Tf3.m_lAttributes = dbVariableField;//dbVariableFieldΪ�ɱ䳤���ֶΣ������ı��� dbFixedField�ֶγ����ǹ̶���(�����ֶε�Ĭ��ֵ��
				Tf3.m_bRequired = FALSE;
				Tf3.m_bAllowZeroLength = TRUE;						
				td_T.CreateField(Tf3);				
				
				td_T.CreateField(_T("LENGTH"),dbSingle,0L);
				td_T.CreateField(_T("Weight"),dbSingle,0L);
			    td_T.CreateField(_T("OUTERDIAMATER"),dbSingle,0L);			

				CDaoFieldInfo Tf7;
				Tf7.m_strName = _T("SPEED");
				Tf7.m_nType = dbText;
				Tf7.m_lSize = 50;
				Tf7.m_lAttributes = dbVariableField;//dbVariableFieldΪ�ɱ䳤���ֶΣ������ı��� dbFixedField�ֶγ����ǹ̶���(�����ֶε�Ĭ��ֵ��
				Tf7.m_bRequired = FALSE;
				Tf7.m_bAllowZeroLength = TRUE;							
				td_T.CreateField(Tf7);
			    td_T.Append();
				td_T.Close();

		}catch(CDaoException *e){
			MessageBox(e->m_pErrorInfo->m_strDescription);
			e->Delete();
        }
}

void CJobEditDlg::OnBnClickedDelete()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	switch(m_editTable){
	case 1:
		//MessageBox(_T("������Ϣ"),MB_OK);
		MessageBox(_T("ɾ��������Ϣ"),MB_OK);
		break;
	case 2:
		MessageBox(_T("ɾ��������Ϣ"),MB_OK);
		break;
	case 3:
		MessageBox(_T("ɾ��ԭʼ�ź�"),MB_OK);
		break;
	case 4:
		MessageBox(_T("ɾ��������Ϣ"),MB_OK);
		break;
	case 5:
		MessageBox(_T("ɾ���ҵ����"),MB_OK);
		break;
	}
}
