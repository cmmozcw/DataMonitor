#pragma once
class CWorkInfo
{
public:
	CWorkInfo(void);
	~CWorkInfo(void);
	void init();
public:
	CString strSignal;//�ź�����
	CString strTitle;//�ź���д,����������
	CString strUnit;//��λ
	int leftLimit;//��߽�
	int rightLimit;//�ұ߽�
	int trackNum;//�����
	COLORREF curveColor; //��ɫ
};

