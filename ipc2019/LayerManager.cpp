#pragma once
// LayerManager.cpp: implementation of the CLayerManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "pch.h"
#include "LayerManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLayerManager::CLayerManager()
	: m_nLayerCount(0),	// Layer ����
	mp_sListHead(NULL),	// Layer �պκ�
	mp_sListTail(NULL),	// Layer �޺κ�
	m_nTop(-1)
{

}

CLayerManager::~CLayerManager()	// �Ҹ���
{

}

void CLayerManager::AddLayer(CBaseLayer* pLayer)	// ���� �߰�
{
	mp_aLayers[m_nLayerCount++] = pLayer;	
}

CBaseLayer* CLayerManager::GetLayer(int nindex)	// �ε����� �ش��ϴ� ���� ��ȯ
{
	return mp_aLayers[nindex];
}

CBaseLayer* CLayerManager::GetLayer(char* pName)	// ���ڷ� ���� �̸��� ���� ���� ��ȯ
{
	for (int i = 0; i < m_nLayerCount; i++)
	{
		if (!strcmp(pName, mp_aLayers[i]->GetLayerName()))
			return mp_aLayers[i];
	}

	return NULL;
}

void CLayerManager::ConnectLayers(char* pcList)	// ���� ����
{
	MakeList(pcList);			// �Է¹��� ���ڸ� ���Ḯ��Ʈ�� �����.
	LinkLayer(mp_sListHead);
	int arr;
	arr = 3;
}

void CLayerManager::MakeList(char* pcList)	// ǥ������ ���Ḯ��Ʈ�� ������ش�.
{
	while (pcList != (char*)0x01)
	{
		char sBuff[100];
		sscanf_s(pcList, "%s", sBuff, sizeof(sBuff));
		pcList = strchr(pcList, ' ') + 1;

		PNODE pNode = AllocNode(sBuff);
		AddNode(pNode);
	}
}

CLayerManager::PNODE CLayerManager::AllocNode(char* pcName)	// �������� ���Ḯ��Ʈ�� ��� ����
{
	PNODE node = new NODE;
	ASSERT(node);

	strcpy_s(node->token, pcName);
	node->next = NULL;

	return node;
}

void CLayerManager::AddNode(PNODE pNode)	// �������� ���Ḯ��Ʈ �ڿ� ��� �߰�
{
	if (!mp_sListHead)
	{
		mp_sListHead = mp_sListTail = pNode;
	}
	else
	{
		mp_sListTail->next = pNode;
		mp_sListTail = pNode;
	}
}


void CLayerManager::Push(CBaseLayer* pLayer) // ���ÿ� ���� push
{
	if (m_nTop >= MAX_LAYER_NUMBER)
	{
#ifdef _DEBUG
		TRACE("The Stack is full.. so cannot run the push operation.. \n");
#endif
		return;
	}

	mp_Stack[++m_nTop] = pLayer;
}

CBaseLayer* CLayerManager::Pop()	// ������ pop
{
	if (m_nTop < 0)
	{
#ifdef _DEBUG
		TRACE("The Stack is empty.. so cannot run the pop operation.. \n");
#endif
		return NULL;
	}

	CBaseLayer* pLayer = mp_Stack[m_nTop];
	mp_Stack[m_nTop] = NULL;
	m_nTop--;

	return pLayer;
}

CBaseLayer* CLayerManager::Top()	// ������ top Ȯ��
{
	if (m_nTop < 0)
	{
#ifdef _DEBUG
		TRACE("The Stack is empty.. so cannot run the top operation.. \n");
#endif
		return NULL;
	}

	return mp_Stack[m_nTop];
}

void CLayerManager::LinkLayer(PNODE pNode)
// ���Ḯ��Ʈ�� ���� �������� �����Ѵ�. -> ǥ������ ������������ �ٲ� �Ŀ� �������� ���Ʒ��� �Ǻ��ؼ� �����Ѵ�.
{
	CBaseLayer* pLayer = NULL;

	while (pNode)
	{
		if (!pLayer)
			pLayer = GetLayer(pNode->token);
		else
		{
			if (*pNode->token == '(')
				Push(pLayer);
			else if (*pNode->token == ')')
				Pop();
			else
			{
				char cMode = *pNode->token;
				char* pcName = pNode->token + 1;

				pLayer = GetLayer(pcName);

				switch (cMode)
				{
				case '*': Top()->SetUpperUnderLayer(pLayer); break;
				case '+': Top()->SetUpperLayer(pLayer); break;
				case '-': Top()->SetUnderLayer(pLayer); break;
				}
			}
		}

		pNode = pNode->next;
	}
}

void CLayerManager::DeAllocLayer()	// �������� ���Ḯ��Ʈ ��� ����
{
	for (int i = 0; i < this->m_nLayerCount; i++)
		delete this->mp_aLayers[i];
}
