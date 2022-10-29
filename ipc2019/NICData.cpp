#include "stdAfx.h"
#include "NICData.h"
#include "Packet32.h"
#include "pch.h"
#include <algorithm>
#include <string>
#define _CRT_SEQURE_NO_WARNINGS

#define OID_802_3_CURRENT_ADDRESS 0x01010102

NICData::NICData(void)	// 컴퓨터의 네트워크 장치 정보를 얻어옴
{
	ULONG length = 0;
	PacketGetAdapterNames(NULL, &length);
	char* adapters = new char[++length];
	if (PacketGetAdapterNames(adapters, &length) != FALSE)
	{
		std::replace(adapters, adapters + length, '\0', '|');

		char* name = strtok(adapters, "|");
		while (name)
		{
			LPADAPTER adapter = PacketOpenAdapter(name);
			if (adapter && adapter->hFile != INVALID_HANDLE_VALUE)
			{
				PACKET_OID_DATA* oid_data = (PACKET_OID_DATA*)malloc(6 + sizeof(PACKET_OID_DATA));
				oid_data->Oid = OID_802_3_CURRENT_ADDRESS;
				oid_data->Length = 6;
				memset(&(oid_data->Data), 0, 6);

				if (PacketRequest(adapter, FALSE, oid_data))
				{
					char mac_addr[31];
					sprintf(mac_addr, "%.2X:%.2X:%.2X:%.2X:%.2X:%.2X",
						(char*)(oid_data->Data)[0],
						(char*)(oid_data->Data)[1],
						(char*)(oid_data->Data)[2],
						(char*)(oid_data->Data)[3],
						(char*)(oid_data->Data)[4],
						(char*)(oid_data->Data)[5]);

					SrcData* data = new SrcData;
					data->name = name;
					//data->ip = ;
					data->mac = mac_addr;
					src_list_.push_back(data);
				}

				PacketCloseAdapter(adapter);
			}
			name = strtok(NULL, "|");
		}
	}
	delete[] adapters;
}

NICData::~NICData(void)		// 저장된 장치 정보를 지움
{
	std::vector<SrcData*>::iterator itr = src_list_.begin();
	std::vector<SrcData*>::iterator end = src_list_.end();
	for (; itr != end; ++itr)
		delete* itr;
	src_list_.clear();
}

NICData& NICData::GetNICData(void)	// singleton pattern
{
	static NICData data;
	return data;
}

bool NICData::IsMyAddress(LPCTSTR address)	// 입력된 주소가 내 주소 또는 broadcast 주소인지 확인
{
	if (!src_list_.empty())
	{
		std::vector<SrcData*>::iterator itr = src_list_.begin();
		std::vector<SrcData*>::iterator end = src_list_.end();
		for (; itr != end; ++itr)
		{
			if (_stricmp((*itr)->mac.c_str(), address) == 0 ||
				_stricmp("ff:ff:ff:ff:ff:ff", address) == 0)
				return true;
		}
	}
	return false;
}

LPCTSTR NICData::GetMacAddress(int index)	// 저장된 리스트에서 해당하는 인덱스의 mac address를 받음
{
	return (src_list_.size() > index ? src_list_[index]->mac.c_str() : NULL);
}

int NICData::GetAddressCount(void) const	// 장치 개수를 얻음
{
	return src_list_.size();
}

LPCSTR NICData::GetDeviceNameFromMAC(LPCTSTR mac_address)	// mac address에 해당하는 장치 이름을 얻음
{
	if (!src_list_.empty())
	{
		std::vector<SrcData*>::iterator itr = src_list_.begin();
		std::vector<SrcData*>::iterator end = src_list_.end();
		for (; itr != end; ++itr)
		{
			if (_stricmp((*itr)->mac.c_str(), mac_address) == 0)
				return (*itr)->name.c_str();
		}
	}
	return NULL;
}

LPCSTR NICData::GetSrcAddress(void) const	// 소스 주소를 얻음
{
	return src_.c_str();
}

void NICData::SetSrcAddress(LPCSTR address)	// 소스 주소를 저장
{
	src_ = (address ? address : "");
}

LPCSTR NICData::GetDstAddress(void) const	// 목적지 주소를 얻음
{
	return dst_.c_str();
}

void NICData::SetDstAddress(LPCSTR address)	// 목적지 주소를 저장
{
	dst_ = (address ? address : "");
}
