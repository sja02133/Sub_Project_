#include "pch.h"
#include "lanCardInfo.h"



BOOL lanCardInfo::gettingLanCardInfo() {
	PIP_ADAPTER_INFO info;
	DWORD size;
	int result;
	CString str = L"";

	ZeroMemory(&info, sizeof(PIP_ADAPTER_INFO));
	size = sizeof(PIP_ADAPTER_INFO);
	result = GetAdaptersInfo(info, &size);

	if (result == ERROR_BUFFER_OVERFLOW) {
		info = (PIP_ADAPTER_INFO)malloc(size);
		GetAdaptersInfo(info, &size);
	}

	while (info != NULL) {
		switch (info->Type) {
		case MIB_IF_TYPE_OTHER:
			break;
		case MIB_IF_TYPE_ETHERNET:
			str = L"이더넷";
			break;
		case IF_TYPE_ISO88025_TOKENRING:
			break;
		case IF_TYPE_IEEE80211:
			str = L"무선 네트워크";
			break;
		}
		info = info->Next;
	}

	return TRUE;
}