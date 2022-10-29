
// ipc2019Dlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "ipc2019.h"
#include "ipc2019Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);	// 대화 상자 데이터 교환
}


/*
클래스의 멤버 함수를 정의하는 구현(.cpp) 파일에서
매크로를 BEGIN_MESSAGE_MAP 사용하여
메시지 맵을 시작한 다음 각 메시지 처리기 함수에 대한 매크로 항목을 추가하고
매크로를 사용하여 메시지 맵을 END_MESSAGE_MAP 완료합니다.
*/

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Cipc2019Dlg 대화 상자


Cipc2019Dlg::Cipc2019Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_IPC2019_DIALOG, pParent)
	, CBaseLayer("ChatDlg")
	, m_bSendReady(FALSE)
	, m_nAckReady(-1)

	, m_unSrcAddr(_T(""))
	, m_unDstAddr(_T(""))
	, m_stMessage(_T(""))
	, m_fileAdd(_T(""))
{
	//대화상자 멤버 변수 초기화
	//  m_unDstAddr = 0;
	//  unSrcAddr = 0;
	//  m_stMessage = _T("");
	//대화 상자 멤버 초기화 완료

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//Protocol Layer Setting
	m_LayerMgr.AddLayer(new CChatAppLayer("ChatApp"));
	//m_LayerMgr.AddLayer(new CChatAppLayer("FileApp"));	FileApp 계층
	m_LayerMgr.AddLayer(new CEthernetLayer("Ethernet"));
	m_LayerMgr.AddLayer(new CNILayer("NI"));
	m_LayerMgr.AddLayer(this);

	// 레이어를 연결한다. (레이어 생성)
	m_LayerMgr.ConnectLayers("NI ( *Ethernet ( *ChatApp ( *ChatDlg ) ) )");

	m_NI = (CNILayer*)m_LayerMgr.GetLayer("NI");
	m_Ether = (CEthernetLayer*)m_LayerMgr.GetLayer("Ethernet");
	m_ChatApp = (CChatAppLayer*)m_LayerMgr.GetLayer("ChatApp");
	//Protocol Layer Setting
}

void Cipc2019Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST_CHAT, m_ListChat);	// 메시지들이 출력되는 곳
	DDX_Control(pDX, IDC_COMBO1, m_Adapter);		// 어댑터(Mac 주소 나오는 곳)
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);	// 파일 전송 게이지 표시하는 곳

	DDX_Text(pDX, IDC_EDIT_SRC, m_unSrcAddr);		// source 주소 뜨는 곳
	DDX_Text(pDX, IDC_EDIT_DST, m_unDstAddr);		// destination 주소 입력하는 곳
	DDX_Text(pDX, IDC_EDIT_MSG, m_stMessage);		// 메시지 입력하는 곳
	DDX_Text(pDX, IDC_EDIT_FILE_ADD, m_fileAdd);	// 파일 추가했을 때 경로 뜨는 곳
}

// 레지스트리에 등록하기 위한 변수
UINT nRegSendMsg;
UINT nRegAckMsg;
// 레지스트리에 등록하기 위한 변수


BEGIN_MESSAGE_MAP(Cipc2019Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_ADDR, &Cipc2019Dlg::OnBnClickedButtonAddr)	// 설정 버튼
	ON_BN_CLICKED(IDC_BUTTON_SEND, &Cipc2019Dlg::OnBnClickedButtonSend)	// 메시지 전송 버튼
	ON_BN_CLICKED(IDC_BUTTON_FILE_ADD, &Cipc2019Dlg::OnBnClickedButtonFileAdd)	// 파일 추가 버튼
	ON_BN_CLICKED(IDC_BUTTON_FILE_ADD, &Cipc2019Dlg::OnBnClickedButtonFileTransfer)	// 파일 전송 버튼
	ON_WM_TIMER()

	/*
	ON_REGISTERED_MESSAGE(nRegSendMsg, OnRegSendMsg)	  //Send 레지스터 등록
	//////////////////////// fill the blank ///////////////////////////////
		// Ack 레지스터 등록
	ON_REGISTERED_MESSAGE(nRegAckMsg, OnRegAckMsg)
	///////////////////////////////////////////////////////////////////////
	*/

	ON_BN_CLICKED(IDC_BUTTON_FILE_ADD, &Cipc2019Dlg::OnBnClickedButtonFileAdd)
	ON_BN_CLICKED(IDC_BUTTON_FILE_TRANSFER, &Cipc2019Dlg::OnBnClickedButtonFileTransfer)
	ON_CBN_SELCHANGE(IDC_COMBO1, &Cipc2019Dlg::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// Cipc2019Dlg 메시지 처리기

BOOL Cipc2019Dlg::OnInitDialog()	// 로그인 다이얼로그 생성
{
	CDialogEx::OnInitDialog();	// 로그인 다이얼롤그 생성

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	SetRegstryMessage();
	SetDlgState(IPC_INITIALIZING);


	NICData& data = NICData::GetNICData();
	for (int index = 0; index < data.GetAddressCount(); ++index)
		m_Adapter.InsertString(-1, data.GetMacAddress(index));
	m_Adapter.SetCurSel(0);

	m_progress.SetRange(0, 1000);

	/*
	// 어댑터(Mac 주소) 목록에 내용 채우기
	for (int i = 0; i < m_NI->getAdapterNum(); i++)
		m_Adapter.AddString(m_NI->getAdapterName(i));
	*/

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void Cipc2019Dlg::OnSysCommand(UINT nID, LPARAM lParam) // 메뉴창 조절 함수
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)	// 메뉴창 종료
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);		// 메뉴창 마우스로 이동
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void Cipc2019Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR Cipc2019Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void Cipc2019Dlg::OnBnClickedButtonSend()	// 메시지 전송 버튼 눌렀을 때 일어나는 이벤트(전송)
{
	UpdateData(TRUE);

	if (!m_stMessage.IsEmpty())
	{
		/*
		SetTimer(1, 2000, NULL);
		m_nAckReady = 0;
		*/

		SendData();
		m_stMessage = "";

		(CEdit*)GetDlgItem(IDC_EDIT3)->SetFocus();
	}

	UpdateData(FALSE);
}

void Cipc2019Dlg::SetRegstryMessage()
{
	nRegSendMsg = RegisterWindowMessage(_T("Send IPC Message"));
	nRegAckMsg = RegisterWindowMessage(_T("Ack IPC Message"));

}

void Cipc2019Dlg::SendData()	// ChatAppLayer로 메시지 전송
{
	CString MsgHeader;
	/*
	if (m_unDstAddr == (unsigned int)0xff)
		MsgHeader.Format(_T("[%d:BROADCAST] "), m_unSrcAddr);
	*/

	MsgHeader.Format(_T("[%s:%s] "), m_unSrcAddr, m_unDstAddr);

	m_ListChat.AddString(MsgHeader + m_stMessage);

	int nlength = m_stMessage.GetLength();

	int len = 0;

	//////////////////////////////segemantation APP_DATA
	if (m_stMessage.GetLength() > APP_DATA_SIZE) // 메시지 길아가 1496 초과인 경우
	{
		while (1)
		{
			if (len == 0)
			{
				this->mp_UnderLayer->Send((unsigned char*)(LPCTSTR)m_stMessage.Mid(len, APP_DATA_SIZE), APP_DATA_SIZE, CHAT_TYPE);
				len += APP_DATA_SIZE;
			}
			else
			{
				if (m_stMessage.GetLength() - len < APP_DATA_SIZE)	// 단편화 했을 때, 마지막 조각인 경우 
				{
					this->mp_UnderLayer->Send((unsigned char*)(LPCTSTR)m_stMessage.Mid(len, m_stMessage.GetLength() - len), m_stMessage.GetLength() - len, CHAT_TYPE);
					break;
				}
				this->mp_UnderLayer->Send((unsigned char*)(LPCTSTR)m_stMessage.Mid(len, APP_DATA_SIZE), APP_DATA_SIZE, CHAT_TYPE);
				len += APP_DATA_SIZE;
			}
		}
	}
	else   // 메시지 길이가 1496 이하인 경우 
	{
		this->mp_UnderLayer->Send((unsigned char*)(LPCTSTR)m_stMessage, m_stMessage.GetLength(), CHAT_TYPE);
	}



	/*
	if (nlength > 1496) // 메시지 길이가 1496보다 클 경우
	{

	}
	else    // 메시지 길이가 1496 이하일 경우
	{
		unsigned char* ppayload = new unsigned char[nlength + 1];
		memcpy(ppayload, (unsigned char*)(LPCTSTR)m_stMessage, nlength);
		ppayload[nlength] = '\0';


		// 보낼 data와 메시지 길이를 Send함수로 넘겨준다.
		m_ChatApp->Send(ppayload, nlength);
	}
	*/


	/*
	unsigned char* ppayload = new unsigned char[nlength + 1];
	memcpy(ppayload, (unsigned char*)(LPCTSTR)m_stMessage, nlength);
	ppayload[nlength] = '\0';


	// 보낼 data와 메시지 길이를 Send함수로 넘겨준다.
	m_ChatApp->Send(ppayload, nlength);
	*/
}

BOOL Cipc2019Dlg::Receive(unsigned char* ppayload) // ChatAppLayer로부터 메시지 수신
{
	CString message;
	message.Format("%s", ppayload);

	if (ppayload[APP_DATA_SIZE + 1] == 1)	// Chatting인 경우
	{
		message.Format(_T("[%s:%s] "), m_unSrcAddr, m_unDstAddr);
		int index = m_ListChat.AddString(message);
		m_ListChat.SetCurSel(index);
	}

	message.Format("%s", ppayload);

	int len = 0;

	if (message.GetLength() > 30)	// 30글자보다 길면 30줄마다 프린트
	{
		while (1)
		{
			int index = m_ListChat.AddString(message.Mid(len, 30));
			m_ListChat.SetCurSel(index);
			len += 30;
			if (message.GetLength() - len < 30)
			{
				index = m_ListChat.AddString(message.Mid(len, message.GetLength() - len));
				m_ListChat.SetCurSel(index);
				break;
			}
		}
	}
	else // 30 글자 이하면
	{
		int index = m_ListChat.AddString(message);
		m_ListChat.SetCurSel(index);
	}

	return TRUE;

	/*
	if (m_nAckReady == -1)
	{
	}

	m_ListChat.AddString((LPCTSTR)ppayload);
	return TRUE;
	*/
}

BOOL Cipc2019Dlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			if (::GetDlgCtrlID(::GetFocus()) == IDC_EDIT3)
				OnBnClickedButtonSend();
			return FALSE;
		case VK_ESCAPE: return FALSE;
		}
		break;
	}

	return CDialog::PreTranslateMessage(pMsg);
}


void Cipc2019Dlg::SetDlgState(int state)	// 영역별 들어갈 내용
{
	UpdateData(TRUE);

	CButton* pChkButton = (CButton*)GetDlgItem(IDC_CHECK1);

	CButton* pSendButton = (CButton*)GetDlgItem(bt_send);
	CButton* pSetAddrButton = (CButton*)GetDlgItem(bt_setting);
	CEdit* pMsgEdit = (CEdit*)GetDlgItem(IDC_EDIT3);
	CEdit* pSrcEdit = (CEdit*)GetDlgItem(IDC_EDIT1);
	CEdit* pDstEdit = (CEdit*)GetDlgItem(IDC_EDIT2);
	CEdit* pFAEdit = (CEdit*)GetDlgItem(IDC_EDIT_FILE_ADD);
	CEdit* pFAButton = (CEdit*)GetDlgItem(IDC_BUTTON_FILE_ADD);
	CEdit* pFTButton = (CEdit*)GetDlgItem(IDC_BUTTON_FILE_TRANSFER);

	switch (state)
	{
	case IPC_INITIALIZING:
		pSendButton->EnableWindow(FALSE);
		pMsgEdit->EnableWindow(FALSE);
		m_ListChat.EnableWindow(FALSE);
		pFAButton->EnableWindow(FALSE);
		pFAEdit->EnableWindow(FALSE);
		pFTButton->EnableWindow(FALSE);
		break;
	case IPC_READYTOSEND:
		pSendButton->EnableWindow(TRUE);
		pMsgEdit->EnableWindow(TRUE);
		m_ListChat.EnableWindow(TRUE);
		break;
	case IPC_WAITFORACK:	break;
	case IPC_ERROR:		break;
		/*
		case IPC_UNICASTMODE:
			m_unDstAddr = 0x0;
			pDstEdit->EnableWindow(TRUE);
			break;
		case IPC_BROADCASTMODE:
			m_unDstAddr = 0xff;
			pDstEdit->EnableWindow(FALSE);
			break;
		*/
	case IPC_ADDR_SET:
		pSetAddrButton->SetWindowText(_T("재설정(&R)"));
		pSrcEdit->EnableWindow(FALSE);
		pDstEdit->EnableWindow(FALSE);
		pSendButton->EnableWindow(TRUE);
		m_ListChat.EnableWindow(TRUE);
		pMsgEdit->EnableWindow(TRUE);
		pFAButton->EnableWindow(TRUE);
		pFAEdit->EnableWindow(TRUE);
		pFTButton->EnableWindow(TRUE);
		//pChkButton->EnableWindow(FALSE);
		break;
	case IPC_ADDR_RESET:
		pSetAddrButton->SetWindowText(_T("설정(&O)"));
		pSrcEdit->EnableWindow(FALSE);
		/*
		if (!pChkButton->GetCheck())
			pDstEdit->EnableWindow(TRUE);
		pChkButton->EnableWindow(TRUE);
		*/
		pDstEdit->EnableWindow(TRUE);
		m_ListChat.EnableWindow(FALSE);
		pSendButton->EnableWindow(FALSE);
		pMsgEdit->EnableWindow(FALSE);
		pFAButton->EnableWindow(FALSE);
		pFAEdit->EnableWindow(FALSE);
		pFTButton->EnableWindow(FALSE);
		m_unSrcAddr = "";
		m_unDstAddr = "";
		/*
		m_Ether->SetSourceAddress((unsigned char*)"00000000");
		m_Ether->SetDestinAddress((unsigned char*)"00000000");
		*/
		break;
	case IPC_SET_FileOn://button text change
		pFTButton->SetWindowText("취소");
		break;
	case IPC_SET_FileOff://button text change
		pFTButton->SetWindowText("전송");
		break;


	}

	UpdateData(FALSE);
}


void Cipc2019Dlg::EndofProcess()
{
	m_LayerMgr.DeAllocLayer();
}

// Send메시지 레지스트리가 켜졌을 때
LRESULT Cipc2019Dlg::OnRegSendMsg(WPARAM wParam, LPARAM lParam)
{
	//////////////////////// fill the blank ///////////////////////////////
	if (m_nAckReady) {
		// File 레이어에서 상대방이 전송한 메시지가 담긴 파일을 가져옴
		if (m_LayerMgr.GetLayer("File")->Receive())
		{
			// 메시지를 받았다면 Ack 신호를 브로드캐스트로 날린다.
			::SendMessage(HWND_BROADCAST, nRegAckMsg, 0, 0);
		}
	}
	///////////////////////////////////////////////////////////////////////
	return 0;
}

LRESULT Cipc2019Dlg::OnRegAckMsg(WPARAM wParam, LPARAM lParam)
{
	if (!m_nAckReady) { // Ack 신호를 받으면 타이머를 멈춘다.
		m_nAckReady = -1;
		KillTimer(1);
	}

	return 0;
}

void Cipc2019Dlg::OnTimer(UINT nIDEvent)	// 타이머
{
	// TODO: Add your message handler code here and/or call default
	m_ListChat.AddString(_T(">> The last message was time-out.."));
	m_nAckReady = -1;
	KillTimer(1);

	CDialog::OnTimer(nIDEvent);
}


void Cipc2019Dlg::OnBnClickedButtonAddr()	// 설정 버튼 눌렀을 때 일어나는 이벤트
{
	UpdateData(TRUE);

	if (!m_bSendReady)
	{
		m_bSendReady = TRUE;		// 아직 준비가 안되었으면 True로 한다.
		SetDlgState(IPC_ADDR_SET);

		m_NI->setAdapterNum(m_Adapter.GetCurSel());
		m_NI->setMacAddress();
		m_srcMacAddress = m_NI->getMacAddress();
		m_unSrcAddr.Format("%.2X-%.2X-%.2X-%.2X-%.2X-%.2X",
			m_srcMacAddress[0], m_srcMacAddress[1], m_srcMacAddress[2],
			m_srcMacAddress[3], m_srcMacAddress[4], m_srcMacAddress[5]);

		//m_unDstAddr.MakeLower();
		ConvertHex(m_unDstAddr, m_dstMacAddress);

		/*
		m_unDstAddr.Format("%.2X-%.2X-%.2X-%.2X-%.2X-%.2X",
			m_dstMacAddress[0], m_dstMacAddress[1], m_dstMacAddress[2],
			m_dstMacAddress[3], m_dstMacAddress[4], m_dstMacAddress[5]);
		*/

		m_Ether->SetSourceAddress(m_srcMacAddress);
		m_Ether->SetSourceAddress(m_dstMacAddress);

		m_RecvThread = ::AfxBeginThread(Cipc2019Dlg::ReceiveThread, this);
	}
	else
	{
		m_bSendReady = FALSE;
		SetDlgState(IPC_ADDR_RESET);
	}
	UpdateData(FALSE);





	/*
	if (!m_unDstAddr ||
		!m_unSrcAddr)
	{
		AfxMessageBox(_T("주소를 설정 오류발생",
			"경고"),
			MB_OK | MB_ICONSTOP);

		return;
	}

	if (m_bSendReady) {
		SetDlgState(IPC_ADDR_RESET);
		SetDlgState(IPC_INITIALIZING);
	}
	else {
		m_ChatApp->SetSourceAddress(m_unSrcAddr);
		m_ChatApp->SetDestinAddress(m_unDstAddr);

		SetDlgState(IPC_ADDR_SET);
		SetDlgState(IPC_READYTOSEND);
	}

	m_bSendReady = !m_bSendReady;
	*/
}

BOOL Cipc2019Dlg::ConvertHex(CString cs, unsigned char* hex)//change string to hex
{
	int i;
	char* srcStr = cs.GetBuffer(0);

	for (i = 0; i < 12; i++) {
		// error
		if (srcStr[i] < '0' || (srcStr[i] > '9' && srcStr[i] < 'a') || srcStr[i] > 'f')
			return FALSE;
	}
	for (i = 0; i < 12; i = i + 2) {
		hex[i / 2] = (((srcStr[i] > '9') ? (srcStr[i] - 87) : (srcStr[i] - '0')) << 4 |
			((srcStr[i + 1] > '9') ? (srcStr[i + 1] - 87) : (srcStr[i + 1] - '0')));
	}
	return TRUE;
}

UINT Cipc2019Dlg::ReceiveThread(LPVOID pParam)
{
	Cipc2019Dlg* obj = (Cipc2019Dlg*)pParam;

	//if m_bSendReady is on, start Thread
	while (obj->m_bSendReady == TRUE)
	{
		CBaseLayer* bLayer;
		bLayer = obj->m_LayerMgr.GetLayer("ChatApp");
		unsigned char* ppayload = bLayer->Receive(0);
		if (ppayload != NULL)
			obj->Receive(ppayload);
	}
	return 0;
}


void Cipc2019Dlg::OnBnClickedCheckToall()	// 클릭 못하게 조절
{
	CButton* pChkButton = (CButton*)GetDlgItem(IDC_CHECK_TOALL);

	if (pChkButton->GetCheck()) {
		SetDlgState(IPC_BROADCASTMODE);
	}
	else {
		SetDlgState(IPC_UNICASTMODE);
	}
}


void Cipc2019Dlg::OnBnClickedButtonFileAdd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog fd(TRUE, NULL, NULL, OFN_HIDEREADONLY, "All Files(*.*)|*.*||");

	if (IDOK == fd.DoModal()) { //fd.DoModal을 이용하여 파일선택 dialog를 팝업
		m_fileAdd = fd.GetPathName(); //파일의 경로명을 받아와서 저장
	}
	UpdateData(FALSE); //변수(m_strFile)에 저장된 값을 화면에 출력
}


void Cipc2019Dlg::OnBnClickedButtonFileTransfer()
{
	/*
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if (!m_fileAdd.IsEmpty())
		if (!FileSended) {
			FileSended = TRUE;//file Transfering
			CWinThread* m_FileThread = ::AfxBeginThread(Cipc2019Dlg::FileThread, this);
			SetDlgState(IPC_SET_FileOn);

		}
		else {
			SetDlgState(IPC_SET_FileOff);
			FileSended = FALSE;//init
		}
	else
		AfxMessageBox("Select a file to send first!");

	UpdateData(FALSE);
	*/
}


void Cipc2019Dlg::OnCbnSelchangeCombo1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
