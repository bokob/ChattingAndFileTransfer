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
	: CBaseLayer(pName),
	mp_Dlg(NULL)
{
	ResetHeader();
}

CChatAppLayer::~CChatAppLayer()
{
}

void CChatAppLayer::ResetHeader()
{
	m_sHeader.capp_totlen = 0x0000;
	m_sHeader.capp_type = 0x00;

	memset(m_sHeader.capp_data, 0, MAX_APP_DATA);
}

BOOL CChatAppLayer::Send(unsigned char* ppayload, int nlength)
{
	m_sHeader.capp_totlen = (unsigned short)nlength;

	BOOL bSuccess = FALSE;

	int len = 0;

	if (nlength > MAX_APP_DATA) // 메시지 길이가 1496 초과인 경우
	{
		while (1)
		{
			if (len == 0)	// 단편화 했을 때, 시작 조각인 경우
			{
				m_sHeader.capp_type = DATA_TYPE_BEGIN;
				memcpy(m_sHeader.capp_data, ppayload, nlength > MAX_APP_DATA ? MAX_APP_DATA : nlength);
				((CEthernetLayer*)mp_UnderLayer)->Send((unsigned char*)&m_sHeader, MAX_APP_DATA, DATA_TYPE_BEGIN);
				len += MAX_APP_DATA;
			}
			else
			{
				if (nlength - len < MAX_APP_DATA) // 단편화 했을 때, 마지막 조각인 경우
				{
					m_sHeader.capp_type = DATA_TYPE_END;
					memcpy(m_sHeader.capp_data, ppayload, nlength - len);
					((CEthernetLayer*)mp_UnderLayer)->Send((unsigned char*)&m_sHeader, nlength - len + CHAT_APP_HEADER, DATA_TYPE_END);
					break;
				}
				// 단편화 했을 때, 중간 조각인 경우
				m_sHeader.capp_type = DATA_TYPE_CONT;
				memcpy(m_sHeader.capp_data, ppayload, nlength - len > MAX_APP_DATA ? MAX_APP_DATA : nlength);
				((CEthernetLayer*)mp_UnderLayer)->Send((unsigned char*)&m_sHeader, MAX_APP_DATA, DATA_TYPE_CONT);
				len += MAX_APP_DATA;
			}
		}
	}
	else // 메시지 길이가 1496 이하인 경우
	{
		memcpy(m_sHeader.capp_data, ppayload, nlength);
		((CEthernetLayer*)mp_UnderLayer)->Send((unsigned char*)&m_sHeader, nlength + CHAT_APP_HEADER, CHAT_TYPE);
	}
	bSuccess = TRUE;

	return bSuccess;
}

BOOL CChatAppLayer::Receive(unsigned char* ppayload)
{
	PCHAT_APP_HEADER capp_hdr = (PCHAT_APP_HEADER)ppayload;
	static unsigned char* GetBuff;

	if (capp_hdr->capp_totlen <= MAX_APP_DATA)	// 메시지 길이가 1496 이하인 경우
	{
		GetBuff = (unsigned char*)malloc(capp_hdr->capp_totlen);
		memset(GetBuff, 0, capp_hdr->capp_totlen);
		memcpy(GetBuff, capp_hdr->capp_data, capp_hdr->capp_totlen);
		GetBuff[capp_hdr->capp_totlen] = '\0';

		mp_aUpperLayer[0]->Receive((unsigned char*)GetBuff);
		return TRUE;
	}
	if (capp_hdr->capp_type == DATA_TYPE_BEGIN)	// 단편화 했을 때, 시작 조각인 경우
	{
		GetBuff = (unsigned char*)malloc(capp_hdr->capp_totlen);
		memset(GetBuff, 0, capp_hdr->capp_totlen);
	}
	else if (capp_hdr->capp_type == DATA_TYPE_CONT)	// 단편화 했을 때, 중간 조각인 경우
	{
		strncat((char*)GetBuff, (char*)capp_hdr->capp_data, strlen((char*)capp_hdr->capp_data));
		GetBuff[strlen((char*)GetBuff)] = '\0';
	}
	else if (capp_hdr->capp_type == DATA_TYPE_END)	// 단편화 했을 때, 마지막 조각인 경우
	{
		memcpy(GetBuff, GetBuff, capp_hdr->capp_totlen);
		GetBuff[capp_hdr->capp_totlen] = '\0';

		mp_aUpperLayer[0]->Receive((unsigned char*)GetBuff);
		free(GetBuff);
	}
	else
		return FALSE;

	return TRUE;
}


/*
UINT CChatAppLayer::ChatThread(LPVOID pParam)
{
	BOOL bSuccess = FALSE;
	CChatAppLayer* pChat = (CChatAppLayer*)pParam;
	int data_length = APP_DATA_SIZE;
	int seq_tot_num;
	int data_index;
	int temp = 0;


	if (pChat->m_length < APP_DATA_SIZE)
		seq_tot_num = 1;
	else
		seq_tot_num = (pChat->m_length / APP_DATA_SIZE) + 1;

	for (int i = 0; i <= seq_tot_num + 1; i++)
	{

		if (seq_tot_num == 1) {
			data_length = pChat->m_length;
		}
		else {
			if (i == seq_tot_num)
				data_length = pChat->m_length % APP_DATA_SIZE;
			else
				data_length = APP_DATA_SIZE;
		}

		memset(pChat->m_sHeader.capp_data, 0, data_length);

		if (i == 0)
		{
			pChat->m_sHeader.capp_totlen = pChat->m_length;
			pChat->m_sHeader.capp_type = DATA_TYPE_BEGIN;
			memset(pChat->m_sHeader.capp_data, 0, data_length);
			data_length = 0;
		}
		else if (i != 0 && i <= seq_tot_num)
		{
			data_index = data_length;
			pChat->m_sHeader.capp_type = DATA_TYPE_CONT;
			pChat->m_sHeader.capp_seq_num = i - 1;

			CString str = pChat->m_ppayload;
			str = str.Mid(temp, temp + data_index);

			memcpy(pChat->m_sHeader.capp_data, str, data_length);
			temp += data_index;
		}
		else
		{
			pChat->m_sHeader.capp_type = DATA_TYPE_END;
			memset(pChat->m_ppayload, 0, data_length);
			data_length = 0;
		}
		bSuccess = pChat->mp_UnderLayer->Send((unsigned char*)&pChat->m_sHeader, data_length + APP_HEADER_SIZE);
	}

	return bSuccess;
}
*/

void CChatAppLayer::make_frame(unsigned char* ppayload, unsigned short nlength, unsigned char type, int seq) {
	unsigned short length = nlength;
	if (type == 0x00 || type == 0x03)length++;
	m_sHeader.capp_type = type;
	m_sHeader.capp_totlen = length;
	//m_sHeader.capp_seq_num = (unsigned char)seq;
	//memset(m_sChatApp.capp_data, 0, length > CHAR_DATA_MAX_SIZE ? CHAR_DATA_MAX_SIZE : length);
	memcpy(m_sHeader.capp_data, ppayload + (seq * MAX_APP_DATA), length > MAX_APP_DATA ? MAX_APP_DATA : length);
	((CEthernetLayer*)(this->GetUnderLayer()))->Send((unsigned char*)&m_sHeader, CHAT_APP_HEADER + (length > CHAT_APP_HEADER ? MAX_APP_DATA : length), CHAT_TYPE);
}
