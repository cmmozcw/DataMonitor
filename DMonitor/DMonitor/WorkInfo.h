#pragma once
class CWorkInfo
{
public:
	CWorkInfo(void);
	~CWorkInfo(void);
public:
	CString strSignal;//�ź�����
	CString strTitie;//�ź���д,����������
	CString strUnit;//��λ
	int leftLimit;//��߽�
	int rightLimit;//�ұ߽�
	COLORREF curveColor; //��ɫ
};

