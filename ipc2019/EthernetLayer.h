// EthernetLayer.h: interface for the CEthernetLayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ETHERNETLAYER_H__7857C9C2_B459_4DC8_B9B3_4E6C8B587B29__INCLUDED_)
#define AFX_ETHERNETLAYER_H__7857C9C2_B459_4DC8_B9B3_4E6C8B587B29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseLayer.h"
#include "pch.h"

class CEthernetLayer
	: public CBaseLayer
{
private:
	inline void		ResetHeader();								// ��� �ʱ�ȭ

public:
	unsigned char*	Receive();									// CNI���� ����
	BOOL			Send(unsigned char* ppayload, int nlength);	// CNI���� �۽�
	void			SetDestinAddress(unsigned char* pAddress);	// �Ѱܹ��� source �ּҸ� Ethernet source�ּҷ� ����
	void			SetSourceAddress(unsigned char* pAddress);	// �Ѱܹ��� ������ �ּҸ� Ethernet destination �ּҷ� ����
	unsigned char* GetDestinAddress();							// ����� ����ִ� �����ּҸ� ��ȯ�Ѵ�.
	unsigned char* GetSourceAddress();							// ����� ����ִ� �����ּҸ� ��ȯ�Ѵ�.

	CEthernetLayer(char* pName);	// ������
	virtual ~CEthernetLayer();		// �Ҹ���

	typedef struct _ETHERNET_HEADER {

		unsigned char	enet_dstaddr[6];		// destination address of ethernet layer
		unsigned char	enet_srcaddr[6];		// source address of ethernet layer
		unsigned short	enet_type;		// type of ethernet layer
		unsigned char	enet_data[ETHER_MAX_DATA_SIZE]; // frame data

	} ETHERNET_HEADER, * PETHERNET_HEADER;

protected:
	ETHERNET_HEADER	m_sHeader;
};

#endif // !defined(AFX_ETHERNETLAYER_H__7857C9C2_B459_4DC8_B9B3_4E6C8B587B29__INCLUDED_)
