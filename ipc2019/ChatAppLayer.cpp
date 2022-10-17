// ChatAppLayer.cpp: implementation of the CChatAppLayer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "pch.h"
#include "ChatAppLayer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChatAppLayer::CChatAppLayer(char* pName)
	: CBaseLayer(pName),	// CBaseLayer���� ��ӹ޴´�. �Է¹��� ���ڷ� CBaseLayer�� ������ ����					
	mp_Dlg(NULL)			// Dlg�� NULL�� �ʱ�ȭ
{
	ResetHeader();	// ��� �ʱ�ȭ
}

CChatAppLayer::~CChatAppLayer()	// �Ҹ���
{

}

/*
void CChatAppLayer::SetSourceAddress(unsigned int src_addr)
// ����� �����ϴ� �ּ� ����
{
	m_sHeader.app_srcaddr = src_addr; // ����� �����ּҸ� �ִ´�.
}

void CChatAppLayer::SetDestinAddress(unsigned int dst_addr)
// ����� �����ϴ� �ּ� ����
{
	m_sHeader.app_dstaddr = dst_addr;	// ����� �����ּҸ� �ִ´�.
}

unsigned int CChatAppLayer::GetSourceAddress()
// ����� ����ִ� �����ּҸ� ��ȯ�Ѵ�.
{
	return m_sHeader.app_srcaddr;
}

unsigned int CChatAppLayer::GetDestinAddress()
// ����� ����ִ� �����ּҸ� ��ȯ�Ѵ�.
{
	return m_sHeader.app_dstaddr;
}
*/

void CChatAppLayer::ResetHeader()	// ��� �ʱ�ȭ
{
	/*
	m_sHeader.app_srcaddr = 0x00000000;
	m_sHeader.app_dstaddr = 0x00000000;
	*/

	m_sHeader.app_length = 0x0000;
	m_sHeader.app_type = 0x00;
	memset(m_sHeader.app_data, 0, APP_DATA_SIZE);
}


BOOL CChatAppLayer::Send(unsigned char* ppayload, int nlength, unsigned char type)
{
	m_sHeader.app_length = (unsigned short)nlength;

	BOOL bSuccess = FALSE;
	m_sHeader.app_type = type;
	//////////////////////// fill the blank ///////////////////////////////
		// �޸� ����� �����͸� header�� ����
		// ChatApp ���̾��� ����� �����Ϳ� �� ���̸� �����Ѵ�.
	memcpy(m_sHeader.app_data, ppayload, nlength > APP_DATA_SIZE ? APP_DATA_SIZE : nlength);
	// ChatApp ���̾��� �ؿ� ���̾��� Ethertnet ���̾ �����͸� �Ѱ��ش�.
	// �޸� ������ ChatApp��(��� + ������)�� (������ ����+�������)��
	// ���� ������ data�� �Ѱ��ش�.
	bSuccess = mp_UnderLayer->Send((unsigned char*)&m_sHeader, nlength + APP_HEADER_SIZE);
	///////////////////////////////////////////////////////////////////////
	return bSuccess;
}

unsigned char* CChatAppLayer::Receive(int ThreadType)
{
	unsigned char* ppayload = mp_UnderLayer->Receive(ThreadType);

	if (ppayload != NULL)
	{
		// ppayload�� ChatApp ��� ����ü�� �ִ´�.
		PCHAT_APP_HEADER app_hdr = (PCHAT_APP_HEADER)ppayload;
		int length = app_hdr->app_length;

		//////////////////////// fill the blank ///////////////////////////////
				// �� �������� �Ѱܹ��� ppayload�� �м��Ͽ� ChatDlg �������� �Ѱ��ش�.
		unsigned char GetBuff[APP_DATA_SIZE]; // 32��Ʈ ũ���� App Data Size��ŭ�� GetBuff�� �����Ѵ�.
		memset(GetBuff, '\0', APP_DATA_SIZE);  // GetBuff�� �ʱ�ȭ���ش�.

		// ���� �������� App Header�� �м��Ͽ�, GetBuff�� data ���̿� APP_DATA_SIZE ���̿� ���Ͽ� ���� ���̸�ŭ
		// data�� �����Ѵ�.
		memcpy(GetBuff, app_hdr->app_data, app_hdr->app_length > APP_DATA_SIZE ? APP_DATA_SIZE : app_hdr->app_length);

		if (app_hdr->app_type = CHAT_REC)
		{
			GetBuff[APP_DATA_SIZE + 1] = 1;
			return (unsigned char*)GetBuff;
		}
		else if (app_hdr->app_type == CHAT_NOTREC)
		{
			GetBuff[APP_DATA_SIZE + 1] = 0;
			return (unsigned char*)GetBuff;
		}
	}
	return 0;


	/*

	// ppayload�� ChatApp ��� ����ü�� �ִ´�.
	PCHAT_APP_HEADER app_hdr = (PCHAT_APP_HEADER)ppayload;

	// ������ �� �ּҿ� �޴� ���� �ּҰ� ��ġ�� ��� �޽����� ������.
	if (app_hdr->app_dstaddr == m_sHeader.app_srcaddr ||
		(app_hdr->app_srcaddr != m_sHeader.app_srcaddr &&		   // �ϴ��� or ��ε�ĳ����
			app_hdr->app_dstaddr == (unsigned int)0xff))
	{
		//////////////////////// fill the blank ///////////////////////////////
				// �� �������� �Ѱܹ��� ppayload�� �м��Ͽ� ChatDlg �������� �Ѱ��ش�.
		unsigned char GetBuff[APP_DATA_SIZE]; // 32��Ʈ ũ���� App Data Size��ŭ�� GetBuff�� �����Ѵ�.
		memset(GetBuff, '\0', APP_DATA_SIZE);  // GetBuff�� �ʱ�ȭ���ش�.

		// ���� �������� App Header�� �м��Ͽ�, GetBuff�� data ���̿� APP_DATA_SIZE ���̿� ���Ͽ� ���� ���̸�ŭ
		// data�� �����Ѵ�.
		memcpy(GetBuff, app_hdr->app_data, app_hdr->app_length > APP_DATA_SIZE ? APP_DATA_SIZE : app_hdr->app_length);

		CString Msg;
		// App Header�� �м��Ͽ�, ����Ʈ â�� �ѷ��� ������ �޽����� �����Ѵ�.
		// ������ �� �Ǵ� �޴� �ʰ� GetBuff�� ����� �޽��� ������ ��ģ��.
		if (app_hdr->app_dstaddr == (unsigned)0xff)
			Msg.Format(_T("[%d:BROADCAST] %s"), app_hdr->app_srcaddr, (char*)GetBuff);
		else
			Msg.Format(_T("[%d:%d] %s"), app_hdr->app_srcaddr, app_hdr->app_dstaddr, (char*)GetBuff);

		// ������ ������� �޽��� ������ ChatDlg�� �Ѱ��ش�.
		mp_aUpperLayer[0]->Receive((unsigned char*)Msg.GetBuffer(0));
		///////////////////////////////////////////////////////////////////////
		return TRUE;
	}
	else    // �޽��� �Ⱥ�����.
		return FALSE;

	*/
}