#pragma once

// CPetroData ����Ŀ��

using namespace std;

#include"PublicInterface.h"
#include <queue>

class CPetroData : public CObject
{
public:
	CPetroData();
	virtual ~CPetroData();
#if 0
	void CPetroData::InitData();
	DATA_PART dept;//���			DEPT			m			һ�飬С��
	DATA_PART temp;//����		    TEMP			C 			һ�飬С��
	DATA_PART rm;//������			RM				rm			һ�飬С��
	DATA_PART gr;//������			GR				GAPI       һ�飬С��
	DATA_PART mag[3];//��������		MAG			mag			���飬С��
	DATA_PART ccl;//�Ŷ�λ��		CCL				CCL			һ�飬����	
#endif
	std::vector<DATA_PART> pData;
};


