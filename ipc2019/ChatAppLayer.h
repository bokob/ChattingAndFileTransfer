#pragma once
// ChatAppLayer.h: interface for the CChatAppLayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHATAPPLAYER_H__E78615DE_0F23_41A9_B814_34E2B3697EF2__INCLUDED_)
#define AFX_CHATAPPLAYER_H__E78615DE_0F23_41A9_B814_34E2B3697EF2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseLayer.h"
#include "pch.h"
class CChatAppLayer
	: public CBaseLayer	// CBaseLayer 상속
{
private:
	inline void		ResetHeader();	// 헤더 초기화
	CObject* mp_Dlg;				// MFC가리키는 것

public:
	unsigned char*			Receive(int ThreadType);				// CEthernet계층에서 수신
	BOOL			Send(unsigned char* ppayload, int nlength, unsigned char type);		// CEthernet계층으로 전송

	unsigned int	GetDestinAddress();								// 헤더에 들어있는 도착주소를 반환한다.
	unsigned int	GetSourceAddress();								// 헤더에 들어있는 시작주소를 반환한다.
	void			SetDestinAddress(unsigned int dst_addr);		// 헤더의 도착하는 주소 설정
	void			SetSourceAddress(unsigned int src_addr);		// 헤더의 시작하는 주소 설정

	CChatAppLayer(char* pName);		// 생성자
	virtual ~CChatAppLayer();		// 소멸자

	typedef struct _CHAT_APP_HEADER {

		unsigned short	app_length;				// total length of the data
		unsigned char	app_unused;				// 우선 사용안함
		/*
		unsigned int	app_dstaddr;			// destination address of application layer
		unsigned int	app_srcaddr;			// source address of application layer
		*/
		unsigned char	app_type;				// type of application data
		unsigned char	app_data[APP_DATA_SIZE];// application data

	} CHAT_APP_HEADER, * PCHAT_APP_HEADER;

protected:
	CHAT_APP_HEADER		m_sHeader;

	enum {
		DATA_TYPE_CONT = 0x01,
		DATA_TYPE_END = 0x02
	};
};

#endif // !defined(AFX_CHATAPPLAYER_H__E78615DE_0F23_41A9_B814_34E2B3697EF2__INCLUDED_)










