// PetroData.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataMonitor.h"
#include "PetroData.h"


// CPetroData

CPetroData::CPetroData()
{
	dept.integer = 0;//���
	dept.decimal = 0;
	dept.bAssign = false;

	temp.integer = 0;//����
	temp.decimal = 0;
	temp.bAssign = false;

	rm.integer = 0;//������
	rm.decimal = 0;
	rm.bAssign = false;

	gr.integer = 0;//������
	gr.decimal = 0;
	gr.bAssign = false;

	mag[0].integer = 0;//��������
	mag[0].decimal = 0;
	mag[0].bAssign = false;

	mag[1].integer = 0;//��������
	mag[1].decimal = 0;
	mag[1].bAssign = false;

	mag[2].integer = 0;//��������
	mag[2].decimal = 0;
	mag[2].bAssign = false;

	ccl.integer = 0;//�Ŷ�λ��
	ccl.decimal = 0;
	ccl.bAssign = false;
}

CPetroData::~CPetroData()
{
	dept.integer = 0;//���
	dept.decimal = 0;
	dept.bAssign = false;

	temp.integer = 0;//����
	temp.decimal = 0;
	temp.bAssign = false;

	rm.integer = 0;//������
	rm.decimal = 0;
	rm.bAssign = false;

	gr.integer = 0;//������
	gr.decimal = 0;
	gr.bAssign = false;

	mag[0].integer = 0;//��������
	mag[0].decimal = 0;
	mag[0].bAssign = false;

	mag[1].integer = 0;//��������
	mag[1].decimal = 0;
	mag[1].bAssign = false;

	mag[2].integer = 0;//��������
	mag[2].decimal = 0;
	mag[2].bAssign = false;

	ccl.integer = 0;//�Ŷ�λ��
	ccl.decimal = 0;
	ccl.bAssign = false;
}


// CPetroData ��Ա����
