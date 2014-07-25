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
	//�ر����ݿ�
	if (m_DataBase.IsOpen())
	m_DataBase.Close();
}

void CJobEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ZCW_JOBEDIT_JOBNAME, m_jobName);
	DDX_Control(pDX, IDC_ZCW_JOBEDIT_TREE, m_jeTree);
}

BEGIN_MESSAGE_MAP(CJobEditDlg, CDialog)
	ON_NOTIFY(TVN_SELCHANGED, IDC_ZCW_JOBEDIT_TREE, &CJobEditDlg::OnSelchangedJobeditTree)
	ON_BN_CLICKED(IDOK, &CJobEditDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_ZCW_JOBEDIT_ADD, &CJobEditDlg::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_ZCW_JOBEDIT_DELETE, &CJobEditDlg::OnBnClickedDelete)
	ON_BN_CLICKED(IDC_ZCW_JOBEDIT_UPDATE, &CJobEditDlg::OnBnClickedUpdate)
END_MESSAGE_MAP()


// CJobEditDlg ��Ϣ�������

BOOL CJobEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	if(m_Open==TRUE){//����Ǵ���ҵ�������������ݿ����ݳ�ʼ������
		m_DataBase.Open(m_jobName);	
		ToolInit();
	}else{
		JobInit();		
	}

	DisplayTree();
	CRect rect;  
	GetDlgItem(IDC_ZCW_STATIC_RECT)-> GetWindowRect(&rect);  
	ScreenToClient(&rect); 
	//Ĭ����ʾ��о��ʾ���	
	m_Tool.Create(IDD_JOBEDIT_TOOL,this);  
	m_Tool.MoveWindow(rect);  
	m_Tool.ShowWindow(SW_SHOW); 

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CJobEditDlg::DisplayTree(){
	m_jeTree.DeleteAllItems();
	//���οؼ���ʼ��
	m_jeTree.ModifyStyle(0,TVS_HASBUTTONS|TVS_LINESATROOT|TVS_HASLINES);
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
	 try{						   	  			   
				CDaoRecordset rs(&m_DataBase);
				COleVariant OleVariant ;
				//���Tool���ӽڵ�
				rs.Open(dbOpenDynaset, L"SELECT * FROM TOOL" );		
				while(!rs.IsEOF()){
					rs.GetFieldValue (0,OleVariant);
					m_jeTree.InsertItem(OleVariant.bstrVal,0,1,root1,TVI_LAST);
					rs.MoveNext();
				}
				rs.Close();     
				//���Curve���ӽڵ�
				rs.Open(dbOpenDynaset, L"SELECT * FROM Curve" );		
				while(!rs.IsEOF()){
					rs.GetFieldValue (0,OleVariant);
					m_jeTree.InsertItem(OleVariant.bstrVal,0,1,root2,TVI_LAST);
					rs.MoveNext();
				}
				rs.Close();     

				//���Origin���ӽڵ�
				rs.Open(dbOpenDynaset, L"SELECT * FROM Origin" );		
				while(!rs.IsEOF()){
					rs.GetFieldValue (0,OleVariant);
					m_jeTree.InsertItem(OleVariant.bstrVal,0,1,root3,TVI_LAST);
					rs.MoveNext();
				}
				rs.Close();  
				
	 } catch(CDaoException *e){		
			MessageBox(e->m_pErrorInfo->m_strDescription);
			e->Delete();
     }
}

void CJobEditDlg::OnSelchangedJobeditTree(NMHDR *pNMHDR, LRESULT *pResult)
{	
	
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	 HTREEITEM hSel=m_jeTree.GetSelectedItem();//��ǰ�ڵ�
	 HTREEITEM hPar=m_jeTree.GetParentItem(hSel);//���ڵ�
	 DWORD idp;//���ڵ�ID
	 if(hPar){
	 idp = m_jeTree.GetItemData(hPar);//�����ڵ�
	 }else{
	 idp=m_jeTree.GetItemData(hSel);//һ���ڵ�
	 }
	 //CString par;	
     //par.Format(_T("%d"), idp);�˴�����������ʾѡ�еĸ��ڵ��
	 //MessageBox(par,MB_OK);
	 CRect rect;  
	 GetDlgItem(IDC_ZCW_STATIC_RECT)-> GetWindowRect(&rect);  
	 ScreenToClient(&rect);  	
	 if(m_Tool.GetSafeHwnd()!=NULL)
			m_Tool.DestroyWindow();//�����Ϣ��������Ѿ��򿪹�������������������ظ���
	 if(m_Curve.GetSafeHwnd()!=NULL)
			m_Curve.DestroyWindow();//���������Ϣ����Ѿ��򿪹�������������������ظ���
	 if(m_Origin.GetSafeHwnd()!=NULL)
			m_Origin.DestroyWindow();//���ԭʼ�ź�����Ѿ��򿪹�������������������ظ���	
	 switch(idp){
	 case 1:
		    if(hPar){			
			ToolInit(m_jeTree.GetItemText(hSel));			
			}else{
			ToolInit();			
			}			
			m_Tool.Create(IDD_JOBEDIT_TOOL, this);  
			m_Tool.MoveWindow(rect);  
			m_Tool.ShowWindow( SW_SHOW ); 				
			m_editTable=1;		
			GetDlgItem(IDC_ZCW_JOBEDIT_ADD)->ShowWindow(TRUE);
			GetDlgItem(IDC_ZCW_JOBEDIT_DELETE)->ShowWindow(TRUE);
			GetDlgItem(IDC_ZCW_JOBEDIT_UPDATE)->ShowWindow(TRUE);
		break;
	 case 2:
		    if(hPar){			
			CurveInit(m_jeTree.GetItemText(hSel));				
			}else{
			CurveInit();			
			}		
		    m_Curve.Create(IDD_JOBEDIT_CURVE, this);  
			m_Curve.MoveWindow(rect);  			
			m_Curve.ShowWindow( SW_SHOW ); 		   		
			m_editTable=2;	
			GetDlgItem(IDC_ZCW_JOBEDIT_ADD)->ShowWindow(TRUE);
			GetDlgItem(IDC_ZCW_JOBEDIT_DELETE)->ShowWindow(TRUE);
			GetDlgItem(IDC_ZCW_JOBEDIT_UPDATE)->ShowWindow(TRUE);
		break;
	 case 3:
		    if(hPar){			
			OriginInit(m_jeTree.GetItemText(hSel));				
			}else{
			OriginInit();				
			}		
		    m_Origin.Create(IDD_JOBEDIT_ORIGIN, this);
			m_Origin.MoveWindow(rect);  			
			m_Origin.ShowWindow( SW_SHOW ); 			
			m_editTable=3;		
			GetDlgItem(IDC_ZCW_JOBEDIT_ADD)->ShowWindow(TRUE);
			GetDlgItem(IDC_ZCW_JOBEDIT_DELETE)->ShowWindow(TRUE);
			GetDlgItem(IDC_ZCW_JOBEDIT_UPDATE)->ShowWindow(TRUE);
		break;
	 case 4:
			GetDlgItem(IDC_ZCW_JOBEDIT_ADD)->ShowWindow(FALSE);
			GetDlgItem(IDC_ZCW_JOBEDIT_DELETE)->ShowWindow(FALSE);
			GetDlgItem(IDC_ZCW_JOBEDIT_UPDATE)->ShowWindow(FALSE);
			m_editTable=4;
		break;
	 case 5:
			GetDlgItem(IDC_ZCW_JOBEDIT_ADD)->ShowWindow(FALSE);
			GetDlgItem(IDC_ZCW_JOBEDIT_DELETE)->ShowWindow(FALSE);
			GetDlgItem(IDC_ZCW_JOBEDIT_UPDATE)->ShowWindow(FALSE);
			m_editTable=5;
		break;
	 }
	*pResult = 0;
}

void CJobEditDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������	
	 if(m_Open==TRUE){//�����ҵ�Ѿ��������޸�		
	   }else{//���򴴽�����ҵ	        
	 }	
	CDialog::OnOK();
}

void CJobEditDlg::ToolInit(CString Label){
	//��ʼ��������Ϣ����
	 COleVariant OleVariant ;
		try{					
				CDaoRecordset rs(&m_DataBase);
				rs.Open(dbOpenDynaset,_T("SELECT * FROM TOOL where LABEL='")+Label+_T("'"));
					rs.MoveFirst () ;				
					rs.GetFieldValue (0,OleVariant);
					m_Tool.m_Label=OleVariant.bstrVal;

					rs.GetFieldValue (1,OleVariant);
					m_Tool.m_Type=OleVariant.bstrVal;

					rs.GetFieldValue (2,OleVariant);
					m_Tool.m_SN=OleVariant.bstrVal;	

					rs.GetFieldValue (3,OleVariant);
					m_Tool.m_Length=OleVariant.fltVal;	

					rs.GetFieldValue (4,OleVariant);
					m_Tool.m_Weight=OleVariant.fltVal;

					rs.GetFieldValue (5,OleVariant);
					m_Tool.m_OuteRdiam=OleVariant.fltVal;	

					rs.GetFieldValue (6,OleVariant);
					m_Tool.m_Speed=OleVariant.bstrVal;							
				rs.Close();				
            }
            catch(CDaoException *e){
			MessageBox(e->m_pErrorInfo->m_strDescription);
			e->Delete();
            }
}

void CJobEditDlg::CurveInit(CString Label){
	//��ʼ��������Ϣ����	
	 COleVariant OleVariant ;
		try{				
				CDaoRecordset rs(&m_DataBase);
				rs.Open(dbOpenDynaset,_T("SELECT * FROM Curve where LABEL='")+Label+_T("'"));
					rs.MoveFirst () ;				
					rs.GetFieldValue (0,OleVariant);
					m_Curve.m_Label=OleVariant.bstrVal;

					rs.GetFieldValue (1,OleVariant);
					m_Curve.m_unit=OleVariant.iVal;						

					rs.GetFieldValue (2,OleVariant);
					m_Curve.m_filter=OleVariant.iVal;		

				rs.Close();				
            }
            catch(CDaoException *e){
			MessageBox(e->m_pErrorInfo->m_strDescription);
			e->Delete();
            }
}

void CJobEditDlg::OriginInit(CString Label){
	//��ʼ��ԭʼ�źŴ���	
	 COleVariant OleVariant ;
		try{						
				CDaoRecordset rs(&m_DataBase);
				rs.Open(dbOpenDynaset,_T("SELECT * FROM Origin where LABEL='")+Label+_T("'"));
					rs.MoveFirst () ;				
					rs.GetFieldValue (0,OleVariant);
					m_Origin.m_Label=OleVariant.bstrVal;

					rs.GetFieldValue (1,OleVariant);
					m_Origin.m_unit=OleVariant.iVal;						

					rs.GetFieldValue (2,OleVariant);
					m_Origin.m_filter=OleVariant.iVal;		

				rs.Close();				
            }
            catch(CDaoException *e){
			MessageBox(e->m_pErrorInfo->m_strDescription);
			e->Delete();
            }
}

void CJobEditDlg::ToolDel(CString Label){
	//ɾ��ѡ�е�������Ϣ	
	 COleVariant OleVariant ;
		try{					
				CDaoRecordset rs(&m_DataBase);
				rs.Open(dbOpenDynaset,_T("SELECT * FROM TOOL where LABEL='")+Label+_T("'"));
				rs.Delete();					
				rs.Close();				
            }
            catch(CDaoException *e){
			MessageBox(e->m_pErrorInfo->m_strDescription);
			e->Delete();
            }
}

void CJobEditDlg::CurveDel(CString Label){
	//ɾ��ѡ�е�������Ϣ	
	 COleVariant OleVariant ;
		try{					
				CDaoRecordset rs(&m_DataBase);
				rs.Open(dbOpenDynaset,_T("SELECT * FROM Curve where LABEL='")+Label+_T("'"));
				rs.Delete();					
				rs.Close();				
            }
            catch(CDaoException *e){
			MessageBox(e->m_pErrorInfo->m_strDescription);
			e->Delete();
            }
}

void CJobEditDlg::OriginDel(CString Label){
	//ɾ��ѡ�е�ԭʼ�ź�	
	 COleVariant OleVariant ;
		try{					
				CDaoRecordset rs(&m_DataBase);
				rs.Open(dbOpenDynaset,_T("SELECT * FROM Origin where LABEL='")+Label+_T("'"));
				rs.Delete();					
				rs.Close();				
            }
            catch(CDaoException *e){
			MessageBox(e->m_pErrorInfo->m_strDescription);
			e->Delete();
            }
}

void CJobEditDlg::ToolInit(){
	//���tool����	
					m_Tool.m_Label=_T("");					
					m_Tool.m_SN=_T("001");					
					m_Tool.m_Speed=_T("1000");						
					m_Tool.m_Type=_T("001");						
					m_Tool.m_Length=0;						
					m_Tool.m_Weight=0;						
					m_Tool.m_OuteRdiam=0;			
}

void CJobEditDlg::CurveInit(){
	//���tool����	
					m_Curve.m_Label=_T("");					
					m_Curve.m_unit=0;
					m_Curve.m_filter=0;
}

void CJobEditDlg::OriginInit(){
	//���tool����	
					m_Origin.m_Label=_T("");					
					m_Origin.m_unit=0;
					m_Origin.m_filter=0;
}

void CJobEditDlg::ToolAdd()
{	
	 try{				
			   //�½����ݿ��¼			  			   
				CDaoRecordset rs(&m_DataBase);				
				rs.Open(dbOpenDynaset,_T("SELECT * FROM TOOL where LABEL='")+m_Tool.m_Label+_T("'"));
				if(rs.IsBOF()){
			    rs.AddNew();
				rs.SetFieldValue(0,COleVariant(m_Tool.m_Label));
				rs.SetFieldValue(1,COleVariant(m_Tool.m_Type));	
				rs.SetFieldValue(2,COleVariant(m_Tool.m_SN));
				rs.SetFieldValue(3,COleVariant(m_Tool.m_Length));
				rs.SetFieldValue(4,COleVariant(m_Tool.m_Weight));	
				rs.SetFieldValue(5,COleVariant(m_Tool.m_OuteRdiam));
				rs.SetFieldValue(6,COleVariant(m_Tool.m_Speed));
				rs.Update();
				MessageBox(_T("��ӳɹ�,�����������Ϣ�������"));
				}else{
				MessageBox(_T("������Ϣ")+m_Tool.m_Label+_T("�Ѿ�����"),MB_OK);
				}
				rs.Close();		

	 } catch(CDaoException *e){
			MessageBox(e->m_pErrorInfo->m_strDescription);
			e->Delete();
     }
}

void CJobEditDlg::CurveAdd()
{	
	 try{				
			   //�½����ݿ��¼			  			   
				CDaoRecordset rs(&m_DataBase);				
				rs.Open(dbOpenDynaset,_T("SELECT * FROM Curve where LABEL='")+m_Curve.m_Label+_T("'"));
				if(rs.IsBOF()){
			    rs.AddNew();
				rs.SetFieldValue(0,COleVariant(m_Curve.m_Label));
				rs.SetFieldValue(1,COleVariant(long(m_Curve.m_unitbox.GetCurSel())));	
				rs.SetFieldValue(2,COleVariant(long(m_Curve.m_filterbox.GetCurSel())));				
				rs.Update();
				MessageBox(_T("��ӳɹ�,�����������Ϣ�������"));
				}else{
				MessageBox(_T("������Ϣ")+m_Curve.m_Label+_T("�Ѿ�����"),MB_OK);
				}
				rs.Close();			
	 } catch(CDaoException *e){
			MessageBox(e->m_pErrorInfo->m_strDescription);
			e->Delete();
     }
}

void CJobEditDlg::OriginAdd()
{	
	 try{				
			   //�½����ݿ��¼			  			   
				CDaoRecordset rs(&m_DataBase);				
				rs.Open(dbOpenDynaset,_T("SELECT * FROM Origin where LABEL='")+m_Origin.m_Label+_T("'"));
				if(rs.IsBOF()){
			    rs.AddNew();
				rs.SetFieldValue(0,COleVariant(m_Origin.m_Label));
				rs.SetFieldValue(1,COleVariant(long(m_Origin.m_unitbox.GetCurSel())));	
				rs.SetFieldValue(2,COleVariant(long(m_Origin.m_filterbox.GetCurSel())));				
				rs.Update();
				MessageBox(_T("��ӳɹ�,�����ԭʼ�źż������"));
				}else{
				MessageBox(_T("ԭʼ�ź�")+m_Origin.m_Label+_T("�Ѿ�����"),MB_OK);
				}
				rs.Close();	

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
		if(m_Tool.m_Label.Trim().IsEmpty()){
		MessageBox(_T("���Ʋ���Ϊ��"),MB_OK);
		}else{
		ToolAdd();				
		}
		break;
	case 2:
		if(m_Curve.m_Label.Trim().IsEmpty()){
		MessageBox(_T("���Ʋ���Ϊ��"),MB_OK);
		}else{
		CurveAdd();			
		}		
		break;
	case 3:
		if(m_Origin.m_Label.Trim().IsEmpty()){
		MessageBox(_T("���Ʋ���Ϊ��"),MB_OK);
		}else{
		OriginAdd();			
		}		
		break;
	case 4:
		MessageBox(_T("��ӿ�����Ϣ"),MB_OK);
		break;
	case 5:
		MessageBox(_T("��Ӽҵ����"),MB_OK);
		break;
	}
	DisplayTree();	
}

void CJobEditDlg::JobInit(){
	try{
			m_DataBase.Create(m_jobName);//����½���ҵ�򴴽����ݿ�
				CDaoTableDef td(&m_DataBase);        //��������						
				
				//����TOOL��
				td.Create(_T("TOOL"));
				td.CreateField(_T("LABEL"),dbText,50,0L);			
				td.CreateField(_T("TYPE"),dbText,50,0L);
				td.CreateField(_T("SN"),dbText,50,0L);					
				td.CreateField(_T("LENGTH"),dbSingle,0L);
				td.CreateField(_T("Weight"),dbSingle,0L);
			    td.CreateField(_T("OUTERDIAMATER"),dbSingle,0L);			
				td.CreateField(_T("SPEED"),dbText,50,0L);
			    td.Append();
				td.Close();

				//����Curve��
				td.Create(_T("CURVE"));
				td.CreateField(_T("LABEL"),dbText,50,0L);
				td.CreateField(_T("UNIT"),dbInteger,0L);		
				td.CreateField(_T("FILTER"),dbInteger,0L);		
			    td.Append();
				td.Close();

				//����Origin��
				td.Create(_T("ORIGIN"));
				td.CreateField(_T("LABEL"),dbText,50,0L);
				td.CreateField(_T("UNIT"),dbInteger,0L);		
				td.CreateField(_T("FILTER"),dbInteger,0L);		
			    td.Append();
				td.Close();				    

		}catch(CDaoException *e){
			MessageBox(e->m_pErrorInfo->m_strDescription);
			e->Delete();
        }
}

void CJobEditDlg::OnBnClickedDelete()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	HTREEITEM hSel=m_jeTree.GetSelectedItem();//��ǰ�ڵ�
	HTREEITEM hPar=m_jeTree.GetParentItem(hSel);//���ڵ�		
	switch(m_editTable){
	case 1:				
			if(hPar){//��һ���ڵ�
				if(MessageBox(_T("ȷ��ɾ��������Ϣ")+m_jeTree.GetItemText(hSel)+_T("��"),_T("����"),MB_OKCANCEL)==IDOK){
					ToolDel(m_jeTree.GetItemText(hSel));					
				}			
			}			
		break;
	case 2:
		   if(hPar){//��һ���ڵ�
				if(MessageBox(_T("ȷ��ɾ��������Ϣ")+m_jeTree.GetItemText(hSel)+_T("��"),_T("����"),MB_OKCANCEL)==IDOK){
					CurveDel(m_jeTree.GetItemText(hSel));					
				}			
			}	
		break;
	case 3:
		if(hPar){//��һ���ڵ�
				if(MessageBox(_T("ȷ��ɾ��ԭʼ�ź�")+m_jeTree.GetItemText(hSel)+_T("��"),_T("����"),MB_OKCANCEL)==IDOK){
					OriginDel(m_jeTree.GetItemText(hSel));					
				}			
			}	
		break;
	case 4:
		MessageBox(_T("ɾ��������Ϣ"),MB_OK);
		break;
	case 5:
		MessageBox(_T("ɾ���ҵ����"),MB_OK);
		break;
	}
	DisplayTree();	
}

void CJobEditDlg::ToolUpdate(CString Label)
{	
	 COleVariant OleVariant ;
		try{						
				CDaoRecordset rs(&m_DataBase);
				rs.Open(dbOpenDynaset,_T("SELECT * FROM TOOL where LABEL='")+Label+_T("'"));
				rs.Edit();
				if(!rs.IsBOF()){			   
				rs.SetFieldValue(0,COleVariant(m_Tool.m_Label));
				rs.SetFieldValue(1,COleVariant(m_Tool.m_Type));	
				rs.SetFieldValue(2,COleVariant(m_Tool.m_SN));
				rs.SetFieldValue(3,COleVariant(m_Tool.m_Length));
				rs.SetFieldValue(4,COleVariant(m_Tool.m_Weight));	
				rs.SetFieldValue(5,COleVariant(m_Tool.m_OuteRdiam));
				rs.SetFieldValue(6,COleVariant(m_Tool.m_Speed));
				rs.Update();
				MessageBox(_T("�޸ĳɹ�,�����������Ϣ�鿴"));
				}else{
				MessageBox(_T("������Ϣ")+m_Tool.m_Label+_T("������"),MB_OK);
				}				
				rs.Close();				
            }
            catch(CDaoException *e){
			MessageBox(e->m_pErrorInfo->m_strDescription);
			e->Delete();
            }
}

void CJobEditDlg::CurveUpdate(CString Label)
{	
	 COleVariant OleVariant ;
		try{						
				CDaoRecordset rs(&m_DataBase);
				rs.Open(dbOpenDynaset,_T("SELECT * FROM CURVE where LABEL='")+Label+_T("'"));
				rs.Edit();
				if(!rs.IsBOF()){			   
				rs.SetFieldValue(0,COleVariant(m_Curve.m_Label));
				rs.SetFieldValue(1,COleVariant(long(m_Curve.m_unitbox.GetCurSel())));	
				rs.SetFieldValue(2,COleVariant(long(m_Curve.m_filterbox.GetCurSel())));		
				rs.Update();
				MessageBox(_T("�޸ĳɹ�,�����������Ϣ�鿴"));
				}else{
				MessageBox(_T("������Ϣ")+m_Curve.m_Label+_T("������"),MB_OK);
				}				
				rs.Close();				
            }
            catch(CDaoException *e){
			MessageBox(e->m_pErrorInfo->m_strDescription);
			e->Delete();
            }
}

void CJobEditDlg::OriginUpdate(CString Label)
{	
	 COleVariant OleVariant ;
		try{					
				CDaoRecordset rs(&m_DataBase);
				rs.Open(dbOpenDynaset,_T("SELECT * FROM Origin where LABEL='")+Label+_T("'"));
				rs.Edit();
				if(!rs.IsBOF()){			   
				rs.SetFieldValue(0,COleVariant(m_Origin.m_Label));
				rs.SetFieldValue(1,COleVariant(long(m_Origin.m_unitbox.GetCurSel())));	
				rs.SetFieldValue(2,COleVariant(long(m_Origin.m_filterbox.GetCurSel())));		
				rs.Update();
				MessageBox(_T("�޸ĳɹ�,�����ԭʼ�źŲ鿴"));
				}else{
				MessageBox(_T("ԭʼ�ź�")+m_Origin.m_Label+_T("������"),MB_OK);
				}				
				rs.Close();				
            }
            catch(CDaoException *e){
			MessageBox(e->m_pErrorInfo->m_strDescription);
			e->Delete();
            }
}

void CJobEditDlg::OnBnClickedUpdate()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	HTREEITEM hSel=m_jeTree.GetSelectedItem();//��ǰ�ڵ�
	HTREEITEM hPar=m_jeTree.GetParentItem(hSel);//���ڵ�		
	switch(m_editTable){
	case 1:				
			if(hPar){//��һ���ڵ�				
				ToolUpdate(m_jeTree.GetItemText(hSel));						
			}			
		break;
	case 2:
		if(hPar){//��һ���ڵ�				
				CurveUpdate(m_jeTree.GetItemText(hSel));							
			}	
		break;
	case 3:
		if(hPar){//��һ���ڵ�				
				OriginUpdate(m_jeTree.GetItemText(hSel));							
			}	
		break;
	case 4:
		MessageBox(_T("ɾ��������Ϣ"),MB_OK);
		break;
	case 5:
		MessageBox(_T("ɾ���ҵ����"),MB_OK);
		break;
	}
	DisplayTree();	
}
