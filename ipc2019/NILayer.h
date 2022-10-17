#if !defined(AFX_FILELAYER_H__D67222B3_1B00_4C77_84A4_CEF6D572E181__INCLUDED_)
#define AFX_FILELAYER_H__D67222B3_1B00_4C77_84A4_CEF6D572E181__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseLayer.h"
#include <pcap.h>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <Packet32.h>
#pragma comment(lib, "packet.lib")

class CNILayer
	: public CBaseLayer	// CBaseLayer 惑加
{

public:
	unsigned char*	Receive();
	BOOL			Send(unsigned char* ppayload, int nlength);	
	void			SetNICList();
	void			setAdapterNum(int index);
	int				getAdapterNum();
	char*			getAdapterName(int index);
	void			setMacAddress();
	unsigned char*	getMacAddress();

	CNILayer(char* pName, pcap_t* pAdapterObject = NULL, int idxAdapter = 0);	// 积己磊
	virtual ~CNILayer();		// 家戈磊

private:
	unsigned char m_MacAddress[6];
	char		  m_adapterName[1024];
	int			  m_selectedNum;
	int			  m_iNumAdapter;
	pcap_t*		  m_pAdapterObjects;
	pcap_if_t*    m_allDevs;
};

#endif // !defined(AFX_FILELAYER_H__D67222B3_1B00_4C77_84A4_CEF6D572E181__INCLUDED_)
