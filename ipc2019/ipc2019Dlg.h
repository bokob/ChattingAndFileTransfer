
// ipc2019Dlg.h: 헤더 파일
//

#pragma once

#include "LayerManager.h"	// Added by ClassView
#include "ChatAppLayer.h"	// Added by ClassView
#include "EthernetLayer.h"	// Added by ClassView
#include "FileAppLayer.h"	// Added by ClassView
#include "NILayer.h"
// Cipc2019Dlg 대화 상자
class Cipc2019Dlg : public CDialogEx, public CBaseLayer
{
// 생성입니다.
public:
	Cipc2019Dlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.



// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IPC2019_DIALOG };
#endif

	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
//	UINT m_unDstAddr;
//	UINT unSrcAddr;
//	CString m_stMessage;
//	CListBox m_ListChat;
	
	afx_msg void OnTimer(UINT nIDEvent);


public:
	BOOL			Receive(unsigned char* ppayload);
	inline void		SendData();		// ChatAppLayer로 메시지 전송
	unsigned char* MacAddrToHexInt(CString ether);

private:
	CLayerManager	m_LayerMgr;
	int				m_nAckReady;

	enum {
		IPC_INITIALIZING,
		IPC_READYTOSEND,
		IPC_WAITFORACK,
		IPC_ERROR,
		IPC_BROADCASTMODE,
		IPC_UNICASTMODE,
		IPC_ADDR_SET,
		IPC_ADDR_RESET,
		IPC_SET_FileOn,
		IPC_SET_FileOff
	};

	void			SetDlgState(int state);
	inline void		EndofProcess();
	inline void		SetRegstryMessage();
	LRESULT			OnRegSendMsg(WPARAM wParam, LPARAM lParam);
	LRESULT			OnRegAckMsg(WPARAM wParam, LPARAM lParam);

	BOOL			m_bSendReady;

	unsigned char* m_srcMacAddress;		// 출발 Mac 주소
	unsigned char  m_dstMacAddress[6];	// 목적지 Mac 주소
 
	// Object App
	CChatAppLayer*	m_ChatApp;
	CFileAppLayer*	m_FileApp;
	CEthernetLayer*	m_Ether;
	CNILayer*		m_NI;

	// Implementation
	UINT			m_wParam;
	DWORD			m_lParam;
public:

	BOOL		FileSended;

	CWinThread* m_RecvThread;
	static UINT	ReceiveThread(LPVOID pParam);

	afx_msg void OnBnClickedButtonAddr();	// 설정 버튼 눌렀을 때 일어나는 이벤트
	afx_msg void OnBnClickedButtonSend();	// send 버튼 눌렀을 때 일어나는 이벤트
	CString m_stMessage;
	CString m_unDstAddr;
	CListBox m_ListChat;
	afx_msg void OnBnClickedCheckToall();	// 클릭 못하게 조절
	CComboBox m_Adapter;
	CString m_unSrcAddr;
	CString m_fileAdd;
	CProgressCtrl m_progress;
	CEdit		  m_EditDstAddr;
	afx_msg void OnBnClickedButtonFileAdd();
	afx_msg void OnBnClickedButtonFileTransfer();
	afx_msg void OnCbnSelchangeCombo1();
};
