
#include <iphlpapi.h>
#pragma comment(lib,"Iphlpapi.lib")

#pragma once
class lanCardInfo
{
public:
	BOOL gettingLanCardInfo();
public:
	PIP_ADAPTER_INFO pAdapterInfo, pAdapt;
};

