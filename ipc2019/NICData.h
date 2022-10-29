#pragma once

#include <vector>
#include <string>

#include <pcap.h>
#include <Packet32.h>


class NICData
{
	struct SrcData {
		std::string name;	// device name
		std::string ip;
		std::string mac;	// mac address
	};

private:
	NICData(void);	// singleton pattern을 위해

public:
	~NICData(void);

	static NICData& GetNICData(void);

	bool IsMyAddress(LPCTSTR address);
	LPCTSTR GetMacAddress(int index);
	int GetAddressCount(void) const;

	LPCSTR GetDeviceNameFromMAC(LPCTSTR mac_address);

public:
	LPCSTR GetSrcAddress(void) const;
	void SetSrcAddress(LPCSTR address);
	LPCSTR GetDstAddress(void) const;
	void SetDstAddress(LPCSTR address);

private:
	std::vector<SrcData*> src_list_;	// 장치 리스트

	std::string src_;	// 소스 주소
	std::string dst_;	// 목적지 주소
};