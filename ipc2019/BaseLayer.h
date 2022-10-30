#pragma once
// BaseLayer.h: interface for the CBaseLayer class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"
#include"stdafx.h"

#if !defined(AFX_BASELAYER_H__041C5A07_23A9_4CBC_970B_8743460A7DA9__INCLUDED_)
#define AFX_BASELAYER_H__041C5A07_23A9_4CBC_970B_8743460A7DA9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBaseLayer
{
public:
	char* GetLayerName();											// ������ �̸� ��ȯ

	CBaseLayer* GetUnderLayer();									// ���� ������ ��ȯ�Ѵ�.
	CBaseLayer* GetUpperLayer(int nindex);							// �ε����� �ش��ϴ� ���� ������ ��ȯ�Ѵ�.
	void			SetUnderUpperLayer(CBaseLayer* pUULayer = NULL);// ���ڷ� ���� ������ ���� ������ Under��, ���� ������ ���ڷ� ���� ������ Upper�� ����
	void			SetUpperUnderLayer(CBaseLayer* pUULayer = NULL);// ���ڷ� ���� ������ ���� ������ Upper��, ���� ������ ���ڷ� ���� ������ Under�� ����
	void			SetUnderLayer(CBaseLayer* pUnderLayer = NULL);  // ���ڷ� ���� ������ �������� Under�� ����
	void			SetUpperLayer(CBaseLayer* pUpperLayer = NULL);  // ���ڷ� ���� ������ �������� Upper�� ����

	CBaseLayer(char* pName = NULL);		// ������, �⺻���� NULL�̴�.
	virtual ~CBaseLayer();				// �Ҹ���

	// param : unsigned char*	- the data of the upperlayer
	//         int				- the length of data
	virtual	BOOL	Send(unsigned char*, int) { return FALSE; }	// ����� �������� ���̴� �Լ�, �ٸ� �������� ����
	// param : unsigned char*	- the data of the underlayer	
	virtual	BOOL	Receive(unsigned char* ppayload) { return FALSE; }	// Dlg Layer���� ���̴� �Լ�, �ٸ� �������� ����
	virtual	BOOL	Receive() { return FALSE; }	// ����� �������� ���̴� �Լ�, �ٸ� �������� ����
	
protected:
	char* m_pLayerName;		// ���� �̸�
	CBaseLayer* mp_UnderLayer;							// UnderLayer pointer
	CBaseLayer* mp_aUpperLayer[MAX_LAYER_NUMBER];		// UpperLayer pointer
	int				m_nUpperLayerCount;						// UpperLayer Count
};

#endif // !defined(AFX_BASELAYER_H__041C5A07_23A9_4CBC_970B_8743460A7DA9__INCLUDED_)
