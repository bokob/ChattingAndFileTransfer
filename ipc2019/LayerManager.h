// LayerManager.h: interface for the CLayerManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LAYERMANAGER_H__D9F8CF34_8A6D_425A_BDB9_47A4874FF902__INCLUDED_)
#define AFX_LAYERMANAGER_H__D9F8CF34_8A6D_425A_BDB9_47A4874FF902__INCLUDED_

#include "pch.h"
#include "BaseLayer.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLayerManager
{
private:
	typedef struct _NODE {

		char			token[50];		// ǥ���� ���� �迭
		struct _NODE* next;	

	} NODE, * PNODE;

public:
	void			DeAllocLayer();					// �������� ���Ḯ��Ʈ ��� ����

	void			ConnectLayers(char* pcList);	// ���� ����
	CBaseLayer* GetLayer(char* pName);				// ���ڷ� ���� �̸��� ���� ���� ��ȯ
	CBaseLayer* GetLayer(int nindex);				// �ε����� �ش��ϴ� ���� ��ȯ		
	void			AddLayer(CBaseLayer* pLayer);	// ���� �߰�

	CLayerManager();			// ������
	virtual ~CLayerManager();	// �Ҹ���

private:
	// about stack...
	int				m_nTop;					// ������ top�� ��Ÿ�� ����
	CBaseLayer* mp_Stack[MAX_LAYER_NUMBER];	// ����

	CBaseLayer* Top();	// ������ top Ȯ��
	CBaseLayer* Pop();	// ������ pop
	void			Push(CBaseLayer* pLayer);

	PNODE			mp_sListHead;	// Layer �պκ�
	PNODE			mp_sListTail;	// Layer �޺κ�

	// about Link Layer...
	void			LinkLayer(PNODE pNode);		
	// ���Ḯ��Ʈ�� ���� �������� �����Ѵ�. -> ǥ������ ������������ �ٲ� �Ŀ� �������� ���Ʒ��� �Ǻ��ؼ� �����Ѵ�.

	inline void		AddNode(PNODE pNode);		// �������� ���Ḯ��Ʈ �ڿ� ��� �߰�
	inline PNODE	AllocNode(char* pcName);	// �������� ���Ḯ��Ʈ�� ��� ����
	void			MakeList(char* pcList);		// ǥ������ ���Ḯ��Ʈ�� ������ش�.

	int				m_nLayerCount;				// ���� ����
	CBaseLayer* mp_aLayers[MAX_LAYER_NUMBER];	// �������� �迭

};

#endif // !defined(AFX_LAYERMANAGER_H__D9F8CF34_8A6D_425A_BDB9_47A4874FF902__INCLUDED_)
