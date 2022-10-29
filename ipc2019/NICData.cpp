#include "stdAfx.h"
#include "NICData.h"
#include "Packet32.h"
#include "pch.h"
#include <algorithm>
#include <string>
#define _CRT_SEQURE_NO_WARNINGS

#define OID_802_3_CURRENT_ADDRESS 0x01010102

NICData::NICData(void)	// ��ǻ���� ��Ʈ��ũ ��ġ ������ ����
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

NICData::~NICData(void)		// ����� ��ġ ������ ����
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

bool NICData::IsMyAddress(LPCTSTR address)	// �Էµ� �ּҰ� �� �ּ� �Ǵ� broadcast �ּ����� Ȯ��
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

LPCTSTR NICData::GetMacAddress(int index)	// ����� ����Ʈ���� �ش��ϴ� �ε����� mac address�� ����
{
	return (src_list_.size() > index ? src_list_[index]->mac.c_str() : NULL);
}

int NICData::GetAddressCount(void) const	// ��ġ ������ ����
{
	return src_list_.size();
}

LPCSTR NICData::GetDeviceNameFromMAC(LPCTSTR mac_address)	// mac address�� �ش��ϴ� ��ġ �̸��� ����
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

LPCSTR NICData::GetSrcAddress(void) const	// �ҽ� �ּҸ� ����
{
	return src_.c_str();
}

void NICData::SetSrcAddress(LPCSTR address)	// �ҽ� �ּҸ� ����
{
	src_ = (address ? address : "");
}

LPCSTR NICData::GetDstAddress(void) const	// ������ �ּҸ� ����
{
	return dst_.c_str();
}

void NICData::SetDstAddress(LPCSTR address)	// ������ �ּҸ� ����
{
	dst_ = (address ? address : "");
}
