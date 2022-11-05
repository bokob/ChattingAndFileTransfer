// EthernetLayer.cpp: implementation of the CEthernetLayer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "pch.h"
#include "EthernetLayer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEthernetLayer::CEthernetLayer(char* pName)
	: CBaseLayer(pName)
{
	ResetHeader();
}

CEthernetLayer::~CEthernetLayer()
{
}

void CEthernetLayer::ResetHeader()
{
	memset(m_sHeader.enet_dstaddr.addrs, 0, 6);
	memset(m_sHeader.enet_srcaddr.addrs, 0, 6);
	memset(m_sHeader.enet_data, 0, MAX_ETHERNET_DATA);
	m_sHeader.enet_type = 0x3412; // 0x0800
}

unsigned char* CEthernetLayer::GetEnetDstAddress()
{
	return m_sHeader.enet_srcaddr.addrs;
}

unsigned char* CEthernetLayer::GetEnetSrcAddress()
{
	return m_sHeader.enet_dstaddr.addrs;
}

void CEthernetLayer::SetEnetSrcAddress(unsigned char* pAddress)
{
	memcpy(m_sHeader.enet_srcaddr.addrs, pAddress, 6);
}

void CEthernetLayer::SetEnetDstAddress(unsigned char* pAddress)
{
	memcpy(m_sHeader.enet_dstaddr.addrs, pAddress, 6);
}

BOOL CEthernetLayer::Send(unsigned char* ppayload, int nlength, unsigned short type)
{
	BOOL bSuccess = FALSE;

	memcpy(m_sHeader.enet_data, ppayload, nlength);

	m_sHeader.enet_type = type;

	bSuccess = mp_UnderLayer->Send((unsigned char*)&m_sHeader, nlength + ETHER_HEADER_SIZE);

	return bSuccess;
}

BOOL CEthernetLayer::Receive(unsigned char* ppayload)
{
	LPETHERNET pFrame = (LPETHERNET)ppayload;

	BOOL bSuccess = FALSE;

	if ((memcmp((char*)pFrame->enet_dstaddr.S_un.s_ether_addr, (char*)m_sHeader.enet_srcaddr.S_un.s_ether_addr, 6) == 0 &&
		memcmp((char*)pFrame->enet_srcaddr.S_un.s_ether_addr, (char*)m_sHeader.enet_srcaddr.S_un.s_ether_addr, 6) != 0))
	{
		if (ntohs(pFrame->enet_type) == CHAT_TYPE || ntohs(pFrame->enet_type) == DATA_TYPE_BEGIN || 
			ntohs(pFrame->enet_type) == DATA_TYPE_CONT || ntohs(pFrame->enet_type) == DATA_TYPE_END)  // Ethernet Frametype °Ë»ç
			bSuccess = mp_aUpperLayer[0]->Receive((unsigned char*)pFrame->enet_data);
		else if (ntohs(pFrame->enet_type) == FILE_TYPE)
			bSuccess = mp_aUpperLayer[1]->Receive((unsigned char*)pFrame->enet_data);
	}
	return bSuccess;
}