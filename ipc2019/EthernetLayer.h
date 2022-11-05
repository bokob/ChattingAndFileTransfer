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
	inline void		ResetHeader();

public:
	unsigned short	tcp_port;
	void			StartThread();

	BOOL			Receive(unsigned char* ppayload);
	BOOL			Send(unsigned char* ppayload, int nlength, unsigned short type);

	void			SetEnetDstAddress(unsigned char* pAddress);
	void			SetEnetSrcAddress(unsigned char* pAddress);
	unsigned char* GetEnetDstAddress();
	unsigned char* GetEnetSrcAddress();

	CEthernetLayer(char* pName);
	virtual ~CEthernetLayer();

	typedef struct _ETHERNET {

		ETHERNET_ADDR	enet_dstaddr;		// destination address of ethernet layer
		ETHERNET_ADDR	enet_srcaddr;		// source address of ethernet layer
		unsigned short	enet_type;			// type of ethernet layer
		unsigned char	enet_data[MAX_ETHERNET_DATA]; // frame data

	} ETHERNET, * LPETHERNET;

protected:
	ETHERNET	m_sHeader;
};

#endif // !defined(AFX_ETHERNETLAYER_H__7857C9C2_B459_4DC8_B9B3_4E6C8B587B29__INCLUDED_)