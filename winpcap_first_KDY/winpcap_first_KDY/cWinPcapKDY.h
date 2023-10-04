#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include "DlgMonitoring.h"
#include <WinSock2.h>
#include <pcap.h>
#pragma comment(lib,"wpcap.lib")
#pragma comment(lib,"ws2_32.lib")

#include <mutex>
#include <fstream>

#include <vector>
#include <queue>

//2023.02.16 vector_sessionInfo -> map_sessionInfo로 변경하기 위한 map 헤더
#include <map>

#include <algorithm>

#include <Packet32.h>
#include <ntddndis.h>

#include "fileInputOutput.h"



struct INTERFACE_ID {
	CString InterfaceId;
	CString interfaceName;
	CString InterfaceDescription;
};

/* 4 bytes IP address */
typedef struct ip_address
{
	u_char byte1;
	u_char byte2;
	u_char byte3;
	u_char byte4;
}ip_address;

typedef struct ipv6_address
{
	u_short byte1;
	u_short byte2;
	u_short byte3;
	u_short byte4;
	u_short byte5;
	u_short byte6;
	u_short byte7;
	u_short byte8;
}IPV6_ADDRESS;

struct DESTINATION_INFO {
	ip_address address;
	//.... ..X. .... .... .... .... -> X is LG bit
	//.... ...X .... .... .... .... -> X is IG bit
};

struct SOURCE_INFO {
	ip_address address;
	//.... ..X. .... .... .... .... -> X is LG bit
	//.... ...X .... .... .... .... -> X is IG bit
};

struct ETHERNAT_INFO {
	DESTINATION_INFO destination;
	SOURCE_INFO source;
	unsigned short type;
};

struct INTERNET_PROTOCOL_VERSION {
	BYTE versionAndHeaderLength;
	BYTE differentiatedServiceField;
	//XXXX XX.. -> X is Differentiated Services Codepoint
	//.... ..XX -> X is Explicit Congestion Notification
	unsigned short totalLength;
	unsigned short identification;
	unsigned short flags;
	//X... .... -> X is Reserved bit | 0 is "not set"
	//.X.. .... -> X is Don't fragment | 0 is "not set"
	//..X. .... -> X is More  fragment | 0 is "not set"
	//XXXX XX.. -> X is Reserved bit | 0 is "not set"
	//...X XXXX XXXX XXXX -> X is Fragment Offset
	BYTE timeToLive;
	BYTE protocol;
	unsigned short headerCheckSum;
	unsigned int sourceAddress;
	unsigned int destinationAddress;
};

struct TRANSMISSION_CONTROL_PROTOCOL {
	unsigned short sourcePort;
	unsigned short destinationPort;
	unsigned int sequenceNumber;
	unsigned int acknowledgmentNumber;
	BYTE headerLength;
	//XXXX .... -> X is Header length
	unsigned short flags;
	//XXX. .... ....-> X is Reserved | 0 is "not set"
	//...X .... ....-> X is Nonce | 0 is "not set"
	//.... X... ....-> X is Congestion Window Reduced | 0 is "not set"
	//.... .X.. ....-> X is ECN-Echo  | 0 is "not set"
	//.... ..X. ....-> X is Urgent | 0 is "not set"
	//.... ...X ....-> X is Acknowledgment | 0 is "not set"
	//.... .... X...-> X is Push | 0 is "not set"
	//.... .... .X..-> X is Reset | 0 is "not set"
	//.... .... ..X.-> X is Syn | 0 is "not set"
	//.... .... ...X-> X is Fin | 0 is "not set"
	unsigned short window;
	unsigned short checksum;
	unsigned short urgentPointer;
	unsigned char options[12];
};

struct TCP_FRAME {
	INTERFACE_ID interface_id;
	CString Encapsulation_type;
	struct timeval ts;
	int frameNumber;
	int frameLength;
	int captureLength;
	BOOL frameIsMarked;
	BOOL frameIsIgnored;
	CString protocolInFrame;
	CString coloringRuleName;
	CString coloringRuleString;
};

struct PACKET_TYPE {
	UINT type;
};

struct RAW_PACKET {
//	long sec, usec;
	int th;
	struct timeval tv;
	u_char* pkt_data;
	int pkt_len;	//capture length
	int len;
	BOOL checkPacketReaded = FALSE;

	//2023.02.27 PostgreSQL Authentication request status 저장
	BOOL authenticationRequestStatus = FALSE;	//TRUE -> status 존재, FALSE -> status 존재 안함
	std::list<u_char*> authenticationRequestDataList;


};

#define IPV4_TYPE	0x0800
#define IPV6_TYPE	0x86DD
#define ARP_TYPE	0x0806

#define VLAN_TYPE 0x8100		//VLAN 802.1Q

#define HOPOPT_PROTOCOL	0x00	//IPv6
#define ICMP_PROTOCOL	0x01
#define IGMP_PROTOCOL	0x02
#define GGP_PROTOCOL	0x03
#define IPV4_PROTOCOL	0x04
#define ST_PROTOCOL		0x05
#define TCP_PROTOCOL	0x06
#define CBT_PROTOCOL	0x07
#define EGP_PROTOCOL	0x08
#define IGP_PROTOCOL	0x09
#define BBN__RCC__MON_PROTOCOL	0x0A
#define NVP__II_PROTOCOL	0x0B
#define PUP_PROTOCOL	0x0C
#define ARGUS_PROTOCOL	0x0D
#define EMCON_PROTOCOL	0x0E
#define XNET_PROTOCOL	0x0F
#define CHAOS_PROTOCOL	0x10

#define UDP_PROTOCOL	0x11

#define MUX_PROTOCOL	0x12
#define DCN__MEAS_PROTOCOL	0x13
#define HMP_PROTOCOL	0x14
#define PRM_PROTOCOL	0x15
#define XNS__IDP_PROTOCOL	0x16

//#define ARP_TYPE		0x0806


struct PACKET_KDY {
	long sec, usec;
	u_char* pkt_data;
	int caplen;
};

extern struct PACKET_INFO;

//session 별 패킷 수집을 위한..
struct struct_session {
	CString client_ip;
	CString client_port;
	CString server_ip;
	CString server_port;

	//session check
	//2023.02.14 TCP 연결 시작 -> 종료까지의 과정을 알수 있도록 해당 과정을 정의해야 한다.

	//2023.02.15 TCP 연결 종료 구분은 4개이다.
	// 1. 정상 종료, 2. 반 종료, 3. 동시 종료, 4. 강제 종료
	// 대체적으로 2. 반 종료가 많이 이루어 진다고 한다.
	// -> 0 | default : 아무것도 없는 기존 값
	// -> 1 | SYN : 시작
	// -> 2 | SYN, ACK : Server 측 응답
	// -> 3 | ACK : Client 측 응답
	// -> 5 | PUSH 상태
	// -> 6 | FIN : Client 종료 송신
	// -> 7 | FIN, ACK : Server가 Client 종료 수신
	// -> 8 | FIN : Server 종료 송신
	// -> 9 | FIN, ACK : Client가 Server 종료 수신 -> 완전한 종료
	// -> 10 | 

	//2023.03.03 멀티 패킷 처리를 위한 변수.
	// 만약 잘린 패킷이 들어온다는 상황(쿼리 등의 긴 문자열은 모든 헤더 + 데이터 크기인 1514를 넘어가면 안되기에..)
	// 인 경우엔 초과된 데이터 크기만큼 다음 해당 세션 패킷에서 데이터를 가져와야 한다.
	// 나누어진 파트를 "조각"이란 단어를 사용하여 기술하낟. 분석 방법은 다음과 같다.
	// 1. 원본 데이터의 크기(Length)를 저장한다.
	// 2. 첫번째 조각에서 데이터를 크기를 빼준다.
	// 3. 다음번째 조각들의 데이터 크기를 빼준뒤 해당 원본 데이터 크기의 length 연산이 0이 된다면.
	// 4. 해당 데이터들은 하나로 본다.
	// 코드 내 처리
	// 1. 패킷이 들어온다.
	// 2. capture length에서 모든 헤더의 길이를 빼준 값을 checkMultiPacketValue에 저장한다.
	// 3. 데이터 부에서 길이가 저장된 값을 총 데이터 크기를 저장하는 변수(totalLengthOfData)에 저장한다.
	// 4. totalLengthOfData - checkMultiPacketValue의 값이 0 이 넘어가면 multiPacketState를 지금 패킷의 이전 패킷의 multiPacketState에 + 1을 연산한다.
	// 5. 이전 패킷의 multiPacketState의 값이 0이 아니면 totalLengthOfData -= (totalLengthOfData - checkMultiPacketValue)를 한다.
	// 6. 반복하여 totalLengthOfData값이 0이 될때까지 간다음 0이 되면 해당 패킷들을 가져와서 데이터 부를 합쳐서 하나의 packet을 만든다. 
	// 7. 그것을 main queue에 저장한다.

	UINT seqNum = 0;
	UINT ackNum = 0;

	BOOL checkPshAck = FALSE;
	BOOL checkAck = FALSE;

	UINT multiPacketState = 0;
	/*
	UINT checkMultiPacketValue = 0;
	UINT totalLengthOfData = 0;
	*/
	//2023.03.02 세션별 queue를 만들어야 한다.
	std::queue<PACKET_INFO*> session_queue;
};

#pragma pack(push,1)
/* IPv4 header */
typedef struct ipv4_header
{
	u_char	ver_ihl;		// Version (4 bits) + Internet header length (4 bits)
	u_char	tos;			// Type of service 
	u_short tlen;			// Total length 
	u_short identification; // Identification
	u_short flags_fo;		// Flags (3 bits) + Fragment offset (13 bits)
	u_char	ttl;			// Time to live
	u_char	proto;			// Protocol
	u_short crc;			// Header checksum
	ip_address	saddr;		// Source address
	ip_address	daddr;		// Destination address
	BOOL	op_pad = FALSE;			// if Option + Padding exist => TRUE 
	u_char* option;
}IPV4_HEADER;

/* IPv6 header */
typedef struct ipv6_header
{
	u_char	ver_ihl;		// Version (4 bits) + Internet header length (4 bits)
	u_short	trafClass;		// Traffic Class (8 bits)
	u_char flowLabel[5];	// Flow Label (20 bits) 
	u_int payloadLen;		// Payload Length (16 bits)
	u_short nextHeader;		// Next Header (8 bits) | same of IPv4's protocol number
	u_short	hopLimit;		// Hop Libit (8 bits) | same of IPv4's TTL
	ipv6_address saddr;		// Source address
	ipv6_address daddr;		// Destination address
}IPV6_HEADER;

/* ARP header */
typedef struct arp_header
{
	u_short	hwType;			// H/W Type (16 bits)
	u_short	protoType;		// Protocol Type (16 bits)
	u_char hwLen;			// H/W Length (8 bits) 
	u_char protLen;			// Prot. Length (8 bits)
	u_short op;				// Operation (16 bits)
	u_char sHwAddr[6];		// Source H/W Address (48 bits)
	ip_address saddr;		// Source Protocol Address
	u_char tHwAddr[6];		// Target H/W Address (48 bits)
	ip_address taddr;		// Target Protocol Address
}ARP_HEADER;

/* Ether header */
typedef struct ether_header
{
	//	u_char	preamble[7];	// Version (4 bits) + Internet header length (4 bits)
	//	u_char	SFD;			// Type of service 
	u_char daddr[6];		// Total length 
	u_char saddr[6];		// Total length 
	u_short type;
	// 0x600 이상 : Type(DIX 2.0), 0x600 이하 : Length(802.3)|length 범위(3 ~ 1500 bytes)
//	ip_header* ip_header_ether;
//	u_char* data;			// 46 ~ 1500 byte
//	IP_HEADER ip_header;
//	void* data;
//	u_int FCS;				// Frame Check Sequence
}ETHER_HEADER;

/* UDP header*/
typedef struct udp_header
{
	u_short sport;			// Source port
	u_short dport;			// Destination port
	u_short len;			// Datagram length
	u_short crc;			// Checksum
}UDP_HEADER;

typedef struct tcp_header
{
	u_short sport;		//16 bits
	u_short dport;		//16 bits
	u_int seqnum;		//32 bits
	u_int acknum;		//32 bits
	u_short flag;		//16 bits *
		// Data Offset	:	4 bits
		// Reserved		:	6 bits
		// Control Bits	:	6 bits (from left to right)
			// URG
			// ACK
			// PSH
			// RST
			// SYN
			// FIN
	u_short winsize;	//16 bits
	u_short crc;		//16 bits
	u_short urgentPoint;//16 bits
	BOOL optionExist = FALSE;
	u_char* options;	//variable
	// case 1 : A single octet of option-kind
	// case 2 : An octet of option-kind, an octet of option-length, and the actual option-data octets
	//정리 : TCP 헤더는 기본 20byte이다. TCP options을 포함하면 TCP 헤더의 크기는 최대 60bytes이다.
	//고로, TCP header length의 크기 20 이상이라면 options 체크해야함.
}TCP_HEADER;

struct TCP_FLAG {
	BOOL URG = FALSE;
	BOOL ACK = FALSE;
	BOOL PSH = FALSE;
	BOOL RST = FALSE;
	BOOL SYN = FALSE;
	BOOL FIN = FALSE;
};

struct TCP_PACKET : PACKET_TYPE {
	//	TCP_FRAME frame;
	ETHERNAT_INFO ethernet;
	INTERNET_PROTOCOL_VERSION internetProtocolVersion;
	TRANSMISSION_CONTROL_PROTOCOL transmissionControlProtocol;
};

struct PACKET_HEADER_LENGTH {
	UINT ether = 0;
	UINT ip = 0;
	UINT protocol = 0;
};

struct PACKET_INFO {
//	long sec, usec;
	struct timeval tv;
	u_char* pkt_data;

	//2023.04.11 추가
	PACKET_HEADER_LENGTH headerLength;
	UINT dataLength = 0;

	int startDataPos;
	int endDataPos;
	int etherHeaderPos;
	int ipHeaderPos;
	int protocolPos;
	int caplen;
	u_short protocol;
	u_short type;
	CString sourceIP;
	CString destinationIP;
	CString info_string;

	//2023.02.09 session 별 모니터링 기능을 위한 포트 정보 추가
	CString sourcePort;
	CString destinationPort;

	//2023.02.13 protocol type별로 해당 데이터를 가르키는 void 포인터
	BOOL exist_protocol_data = FALSE;
	void* protocol_data;

	//2023.02.22 IP header length + Protocol length = ipTotalLength
	int ipTotalLength = 0;

	UINT trailer_size = 0;
	UINT padding_size = 0;
	
};

//multi packet 처리를 위하여 이 구조체의 realData가 완성되야 UI에 현시하든, 사용하든 해야한다.
struct REAL_PACKET {
	BOOL check_ShowToUI = TRUE;
	BOOL check_Use_realPacket = FALSE;
	UINT totalLength = 0;
	UINT currentLength = 0;
	UINT seqNum = 0;
	UINT ackNum = 0;
	PACKET_INFO* realPacket;
	struct_session session;
	u_char* realData;
};

typedef struct pcat_hdr_s {
	u_int magic_number;
	u_short major_ver;
	u_short minor_ver;
	u_int thiszone;
	u_int sigfigs;
	u_int snapLen;
	u_int linkType;
}PCAP_HDR_S;

typedef struct pcaprec_hdr_s
{
	u_int ts_sec;
	u_int ts_usec;
	u_int incl_len;
	u_int orig_len;
}PCAPREC_HDR_S;

#pragma pack(pop)

struct START_PACKET_TIME {
	time_t timer;
	tm *iTime;
	struct timeval tv;
};

typedef struct MULTI_PACKET {
	BYTE startAndEnd = 0;
	UINT totalLength = 0;

	UINT ackNum = 0;

	std::queue<u_char*> packet_data_queue;
}multi_packet;

struct auth_request_data {
	
};

struct oid_struct {
	CString name = L"";
	CString oid = L"";
	CString typcategory = L"";
	CString typarray = L"";
	int typlen = 0;
};

// Type OID

//Numeric Types
#define INT8OID			20	//8
#define INT2OID			21	//2
#define INT4OID			23	//4
#define REGPROCOID		24	//4
#define OIDOID			26	//4
#define FLOAT4OID		700 //4
#define FLOAT8OID		701 //8
#define MONEYOID		790	//8
#define NUMERICOID		1700 //-1

#define REGPROCEDUREOID	2202 //4
#define REGOPEROID		2203 //4
#define REGOPERATOROID	2204 //4
#define REGCLASSOID		2205 //4
#define REGTYPEOID		2206 //4
#define REGCONFIGOID	3734 //4
#define REGDICTIONARYOID	3769 //4
#define REGNAMESPACEOID	4089 //4
#define REGROLEOID		4096 //4
#define CARDINAL_NUMBEROID		12730 //4

//Boolean Type
#define BOOLOID			16 //1 //-

//String Types
#define CHAROID			18 //1
#define NAMEOID			19 //64
#define TEXTOID			25 //-1

#define PG_NODE_TREEOID	194 //-1
#define BPCHAROID		1042
#define VARCHAROID		1043
#define PG_NDISTINCTOID	3361 //?
#define PG_DEPENDENCIESOID	3402 //?
#define CHARACTER_DATAOID		12733 //?
#define SQL_IDENTIFIEROID		12735 //?
#define YES_OR_NOOID			12742 //?

//Date, Time Types
#define ABSTIMEOID		702 //?
#define DATEOID			1082
#define TIMEOID			1083
#define TIMESTAMPOID	1114
#define TIMESTAMPTZOID	1184
#define TIMETZOID		1266 //?
#define TIME_STAMPOID			12740 //?

//User defined Types
#define BYTEAOID		17 //-
#define TIDOID			27
#define XIDOID			28
#define CIDOID			29
#define JSONOID			114
#define XMLOID			142
#define SMGROID			210 //?
#define MACADDR8OID		774
#define MACADDROID		829 //?
#define ACLITEMOID		1033 //?
#define REFCURSOROID	1790 //?
#define UUIDOID			2950 //?
#define TXID_SNAPSHOTOID	2970 //?
#define PG_LSNOID		3220 //?
#define TSVECTOROID		3614 //?
#define TSQUERYOID		3615 //?
#define GTSVECTOROID	3642 //?
#define JSONBOID		3802 //?

//Pseudo-types
#define PG_DDL_COMMANDOID	32
#define INDEX_AM_HANDLER 325 //?
#define RECORDOID		2249 //?
#define CSTRINGOID		2275 //?
#define ANYOID			2276 //?
#define ANYARRAYOID		2277 //?
#define VOIDOID			2278 //?
#define TRIGGEROID		2279 //?
#define LANGUAGE_HANDLEROID	2280 //?
#define INTERNALOID		2281 //?
#define OPAQUEOID		2282 //?
#define ANYELEMENTOID	2283 //?
#define _RECORD			2287 //?
#define ANYNONARRAY		2776 //?
#define FDW_HANDLEROID	3115 //?
#define TSM_HANDLEROID	3310 //?
#define ANYENUMOID		3500 //?
#define ANYRANGEOID		3831 //?
#define EVENT_TRIGGEROID	3838 //?

//Enum Types

//Array Types
#define INT2VECTOROID	22
#define OIDVECTOROID	30
#define _XMLOID			143	//?
#define _JSONOID		199 //?
#define _LINEOID		629 //?
#define _CIDROID		651 //?
#define _CIRCLEOID		719 //?
#define _MACADDR8OID	775
#define _MONEYOID		791 //?
#define _BOOLOID		1000 //?
#define _BYTEAOID		1001 //?
#define _CHAROID		1002 //? 
#define _NAMEOID		1003 //?
#define _INT2OID		1005 //?
#define _INT2VECTOROID	1006 //?
#define _INT4OID		1007 //?
#define _REGPROCOID		1008 //?
#define _TEXTOID		1009 //?
#define _TIDOID			1010 //?
#define _XIDOID			1011 //?
#define _CIDOID			1012 //?
#define _OIDVECTOROID	1013 //?
#define _BPCHAROID		1014 //?
#define _VARCHAROID		1015 //?
#define _INT8OID		1016 //?
#define _POINTOID		1017 //?
#define _LSEGOID		1018 //?
#define _PATHOID		1019 //?
#define _BOXOID			1020 //?
#define _FLOAT4ARRAYOID	1021 //?
#define _FLOAT8ARRAYOID	1022 //?
#define _ABSTIMEOID		1023 //?
#define _RELTIMEOID		1024 //?
#define _TINTERVALOID	1025 //?
#define _POLYGONOID		1027 //?
#define _OIDOID			1028 //?
#define _ACLITEMOID		1034 //?
#define _MACADDROID		1040 //?
#define _INETOID		1041 //?
#define _TIMESTAMPOID	1115 //?
#define _DATEOID		1182 //?
#define _TIMEOID		1183 //?
#define _TIMESTAMPTZOID	1185 //?
#define _INTERVALOID	1187 //?
#define _NUMERICOID		1231 //?
#define _CSTRINGOID		1263 //?
#define _TIMETZOID		1270 //?
#define _BITOID			1561 //?
#define _VARBITOID		1563 //?
#define _REFCURSOROID	2201 //?
#define _REGPROCEDUREOID	2207 //?
#define _REGOPEROID		2208 //?
#define _REGOPERATOROID	2209 //?
#define _REGCLASSOID	2210 //?
#define _REGTYPEOID		2211 //?
#define _TXID_SANPSHOT	2949 //?
#define _UUIDOID		2951 //?
#define _PG_LSNOID		3221 //?
#define _TSVECTOROID	3643 //?
#define _GTSVECTOROID	3644 //?
#define _TSQUERYOID		3645 //?
#define _REGCONFIGOID	3735 //?
#define _REGDICTIONARYOID	3770 //?
#define _JSONBOID		3807 //?
#define _INT4RANGEOID	3905 //?
#define _NUMRANGEOID	3907 //?
#define _TSRANGEOID		3909 //?
#define _TSTZRANGEOID	3911 //?
#define _DATERANGEOID	3913 //?
#define _INT8RANGEOID	3927 //?
#define _REGNAMESPACEOID	4090 //?
#define _REGROLEOID		4097 //?
#define _CARDINAL_NUMBEROID		12729 //?
#define _CHARACTER_DATAOID		12732 //?
#define _SQL_IDENTIFIEROID		12734 //?
#define _TIME_STAMPOID			12739 //?
#define _YES_OR_NOOID			12741 //?

//(V)Bit-string Types
#define BITOID			1560
#define VARBITOID		1562

//Geometric Types
#define POINTOID		600
#define LSEGOID			601
#define PATHOID			602
#define BOXOID			603
#define POLYGONOID		604
#define LINEOID			628
#define CIRCLEOID		718

//Network Address Types
#define CIDROID			650
#define INETOID			869 //?

//Composite Types
#define PG_TYPEOID		71 //?
#define PG_ATTRIBUTEOID	75 //?
#define PG_PROCOID		81 //?
#define PG_CLASSOID		83 //?
#define PG_DATABASEOID	1248 //?
#define PG_AUTHIDOID	2842 //?
#define PG_AUTH_MEMBERS	2843 //?
#define PG_SHSECLABELOID	4066 //?
#define PG_SUBSCRIPTIONOID	6101 //?
#define PG_ATTRDEFOID	10000 //?
#define PG_CONSTRAINTOID	10001 //?
#define PG_INHERITSOID	10002 //?
#define PG_INDEXOID		10003 //?
#define PG_OPERATOROID	10004 //?
#define PG_OPFAMILYOID	10005 //?
#define PG_OPCLASS		10006 //?
#define PG_AMOID		10131 //?
#define PG_AMOPOID		10132 //?
#define PG_AMPROCOID	10855 //?
#define PG_LANGUAGEOID	11314 //?
#define PG_LARGEOBJECT_METADATAOID	11315 //?
#define PG_LARGEOBJECTOID	11316 //?
#define PG_AGGREGATEOID	11317 //?
#define PG_STATISTIC_EXTOID	11318 //?
#define PG_STATISTICOID	11319 //?
#define PG_REWRITEOID	11320 //?
#define PG_TRIGGEROID	11321 //?
#define PG_EVENT_TRIGGEROID	11322 //?
#define PG_DESCRIPTIONOID	11323 //?
#define PG_CASTOID		11324 //?
#define PG_ENUMOID		11551 //?
#define PG_NAMESPACEOID	11552 //?
#define PG_CONVERSIONOID	11553 //?
#define PG_DEPENDOID	11554 //?
#define PG_DB_ROLE_SETTINGOID	11555 //?
#define PG_TABLESPACEOID	11556 //?
#define PG_PLTEMPLATEOID	11557 //?
#define PG_SHDEPENDOID		11558 //?
#define PG_SHDESCRIPTIONOID	11559 //?
#define PG_TS_CONFIGOID		11560 //?
#define PG_TS_CONFIG_MAPOID	11561 //?
#define PG_TS_DICTOID		11562 //?
#define PG_TS_PARSEROID		11563 //?
#define PG_TS_TEMPLATEOID	11564 //?
#define PG_EXTENSIONOID		11565 //?
#define PG_FOREIGN_DATA_WRAPPEROID	11566 //?
#define PG_FOREIGN_SERVEROID	11567 //?
#define PG_USER_MAPPINGOID	11568 //?
#define PG_FOREIGN_TABLEOID	11569 //?
#define PG_POLICYOID	11570 //?
#define PG_REPLICATION_ORIGINOID 11571 //?
#define PG_DEFAULT_ACLOID	11572 //?
#define PG_INIT_PRIVSOID	11573 //?
#define PG_SECLABELOID		11574 //?
#define PG_COLLATIONOID		11575 //?
#define PG_PARTITIONED_TABLE	11576 //?
#define PG_RANGEOID		11577 //?
#define PG_TRANSFORMOID	11578 //?
#define PG_SEQUENCEOID	11579 //?
#define PG_PUBLICATIONOID	11580 //?
#define PG_PUBLICATION_RELOID	11581 //?
#define PG_SUBSCRIPTION_RELOID	11582 //?
#define PG_TOAST_2604OID		11583 //?
#define PG_TOAST_2606OID		11584 //?
#define PG_TOAST_2609OID		11585 //?
#define PG_TOAST_1255OID		11586 //?
#define PG_TOAST_2618OID		11587 //?
#define PG_TOAST_3596OID		11588 //?
#define PG_TOAST_2619OID		11589 //?
#define PG_TOAST_3381OID		11590 //?
#define PG_TOAST_2620OID		11591 //?
#define PG_TOAST_2396OID		11592 //?
#define PG_TOAST_2964OID		11593 //?
#define PG_TOAST_3592OID		11594 //?
#define PG_ROLESOID		11596 //?
#define PG_SHADOWOID	11600 //?
#define PG_GROUPOID		11604 //?
#define PG_USEROID		11607 //?
#define PG_POLICIESOID	11610 //?
#define PG_RULESOID		11614 //?
#define PG_VIEWSOID		11618 //?
#define PG_TABLESOID	11622 //?
#define PG_MATVIEWSOID	11626 //?
#define PG_INDEXESOID	11630 //?
#define PG_SEQUENCESOID	11634 //?
#define PG_STATSOID		11638 //?
#define PG_PUBLICATION_TABLESOID	11642 //?
#define PG_LOCKSOID		11646 //?
#define PG_CURSORSOID	11649 //?
#define PG_AVAILABLE_EXTENSIONSOID	11652 //?
#define PG_AVAILABLE_EXTENSION_VERSIONSOID	11655 //?
#define PG_PREPARED_XACTSOID	11658 //?
#define PG_PREPARED_STATEMENTSOID	11662 //?
#define PG_SECLABELSOID		11665 //?
#define PG_SETTINGSOID		11674 //?
#define PG_HBA_FILE_RULESOID	11677 //?
#define PG_TIMEZONE_ABBREVSOID	11680 //?
#define PG_TIMEZONE_NAMESOID	11683 //?
#define PG_CONFIGOID			11686 //?
#define PG_STAT_ALL_TABLESOID	11689 //?
#define PG_STAT_XACT_ALL_TABLESOID	11693 //?
#define PG_STAT_SYS_TABLESOID	11697 //?
#define PG_STAT_XACT_SYS_TABLESOID	11701 //?
#define PG_STAT_USER_TABLESOID		11704 //?
#define PG_STAT_XACT_USER_TABLESOID	11708 //?
#define PG_STATIO_ALL_TABLESOID		11711 //?
#define PG_STATIO_SYS_TABLESOID		11715 //?
#define PG_STATIO_USER_TABLESOID	11718 //?
#define PG_STAT_ALL_INDEXSESOID		11721 //?
#define PG_STAT_SYS_INDEXESOID		11725 //?
#define PG_STAT_USER_INDEXESOID		11728 //?
#define PG_STATIO_ALL_INDEXESOID	11731 //?
#define PG_STATIO_SYS_INDEXESOID	11735 //?
#define PG_STATIO_USER_INDEXESOID	11738 //?
#define PG_STATIO_ALL_SEQUENCESOID	11741 //?
#define PG_STATIO_SYS_SEQUENCESOID	11745 //?
#define PG_STATIO_USER_SEQUENCESOID	11748 //?
#define PG_STAT_ACTIVITYOID			11751 //?
#define PG_STAT_REPLICATIONOID		11755 //?
#define PG_STAT_WAL_RECEIVEROID		11759 //?
#define PG_STAT_SUBSCRIPTIONOID		11762 //?
#define PG_STAT_SSLOID				11765 //?
#define PG_REPLICATION_SLOTSOID		11768 //?
#define PG_STAT_DATABASEOID			11772 //?
#define PG_STAT_DATABASE_CONFLICTSOID	11775 //?
#define PG_STAT_USER_FUNCTIONSOID	11778 //?
#define PG_STAT_XACT_USER_FUNCTIONSOID	11782 //?
#define PG_STAT_ARCHIVEROID		11786 //?
#define PG_STAT_BGWRITEROID		11789 //?
#define PG_STAT_PROGRESS_VACUUMOID	11792 //?
#define PG_USER_MAPPINGSOID		11796 //?
#define PG_REPLICATION_ORIGIN_STATUSOID 11800 //?
#define INFORMATION_SCHEMA_CATALOG_NAMEOID	12737 //?
#define APPLICABLE_ROLESOID		12745 //?
#define ADMINISTRABLE_ROLE_AUTHORIZATIONSOID	12749 //?
#define ATTRIBUTESOID		12752 //?
#define CHARACTER_SETSOID	12756 //?
#define CHECK_CONSTRAINT_ROUTINE_USAGEOID	12760 //?
#define CHECK_CONSTRAINTSOID	12764 //?
#define COLLATIONSOID		12768 //?
#define COLLATION_CHARACTER_SET_APPLICABILITYOID	12772 //?
#define COLUMN_DOMAIN_USAGEOID	12776 //?
#define COLUMN_PRIVILEGESOID	12780 //?
#define COLUMN_UDT_USAGEOID		12784 //?
#define COLUMNSOID				12788 //?
#define CONSTRAINT_COLUMN_USAGEOID	12792 //?
#define CONSTRAINT_TABLE_USAGEOID	12796 //?
#define DOMAIN_CONSTRAINTSOID	12800 //?
#define DOMAIN_UDT_USAGEOID		12804 //?
#define DOMAINSOID				12808 //?
#define ENABLED_ROLESOID		12812 //?
#define KEY_COLUMN_USAGEOID		12815 //?
#define PARAMETERSOID			12819 //?
#define REFERENTIAL_CONSTRAINTSOID	12823 //?
#define ROLE_COLUMN_GRANTSOID	12827 //?
#define ROUTINE_PRIVILEGESOID	12830 //?
#define ROLE_ROUTINE_GRANTSOID	12834 //?
#define ROTINESOID				12837 //?
#define SCHEMATAOID				12841 //?
#define SEQUENCESOID			12844 //?
#define SQL_FEATURESOID			12848 //?
#define PG_TOAST_12847OID		12850 //?
#define SQL_IMPLEMENTATION_INFOOID	12853 //?
#define PG_TOAST_12852OID		12855 //?
#define SQL_LANGUAGESOID		12858 //?
#define PG_TOAST_12857OID		12860 //?
#define SQL_PACKAGESOID			12863 //?
#define PG_TOAST_12862OID		12865 //?
#define SQL_PARTSOID			12868 //?
#define PG_TOAST_12867OID		12870 //?
#define SQL_SIZEINGOID			12873 //?
#define PG_TOAST_12872OID		12875 //?
#define SQL_SIZING_PROFILESOID	12878 //?
#define PG_TOAST_12877OID		12880 //?
#define TABLE_CONSTRAINTSOID	12883 //?
#define TABLE_PRIVILEGESOID		12887 //?
#define ROLE_TABLE_GRANTSOID	12891 //?
#define TABLESOID				12894 //?
#define TRANSFORMSOID			12898 //?
#define TRIGGERED_UPDATE_COLUMNOID	12902 //?
#define TRIGGERSOID				12906 //?
#define UDT_PRIVILEGESOID		12910 //?
#define ROLE_UDT_GRANTSOID		12914 //?
#define USAGE_PRIVILEGESOID		12917 //?
#define ROLE_USAGE_GRANTSOID	12921 //?
#define USER_DEFINED_TYPESOID	12924 //?
#define VIEW_COLUMN_USAGEOID	12928 //?
#define VIEW_ROUTINE_USAGEOID	12932 //?
#define VIEW_TABLE_USAGEOID		12936 //?
#define VIEWSOID				12940 //?
#define DATA_TYPE_PRIVILEGESOID	12944 //?
#define ELEMENT_TYPESOID		12948 //?
#define _PG_FOREIGN_TABLE_COLUMNSOID	12952 //?
#define COLUMN_OPTIONSOID		12956 //?
#define _PG_FOREIGN_DATA_WRAPPERSOID	12959 //?
#define FOREIGN_DATA_WRAPPER_OPTIONSOID	12962 //?
#define FOREIGN_DATA_WRAPPERSOID		12965 //?
#define _PG_FOREIGN_SERVERSOID			12968 //?
#define FOREIGN_SERVER_OPTIONSOID		12972 //?
#define FOREIGN_SERVERSOID				12975 //?
#define _PG_FOREIGN_TABLESOID			12978 //?
#define FOREIGN_TABLE_OPTIONSOID		12982 //?
#define FOREIGN_TABLESOID				12985 //?
#define _PG_USER_MAPPINGSOID			12988 //?
#define USER_MAPPING_OPTIONSOID			12992 //?
#define USER_MAPPINGSOID				12996 //?

//Range Types
#define INT4RANGEOID	3904 //?
#define NUMRANGEOID		3906 //?
#define TSRANGEOID		3908 //?
#define TSTZRANGEOID	3910 //?
#define DATERANGEOID	3912 //?
#define INT8RANGEOID	3926 //?

//Timespan Types
#define RELTIMEOID		703 //?
#define TINTERVALOID	704 //?
#define INTERVALOID		1186

//UNKNOWN Types
#define UNKNOWNOID		705 //?

//(Z) Internal-use Types


struct POSTGRESQL_TYPE_LIST
{
	u_char type = 0;
	UINT current_data_length = 0;	//type + length 총 5바이트를 제외한 순수한 데이터의 length
	UINT length = 0;
	u_char* data;
	BOOL done = FALSE;
};

#define RECV_CHECK_END		0
#define RECV_CHECK_START	1
#define RECV_CHECK_STOP		2

class cWinPcapKDY : public DlgMonitoring
{
public:
	BOOL coreMode = FALSE;	//if coreMode is TRUE => core, FALSE => UI mode
	void main_start();


	BOOL check_stateOfProgram();


	//멀티 패킷 처리부...
//	int getDataLength(PACKET_INFO info);
	BOOL checkMultiPacket(PACKET_INFO* info);

	std::map<CString, std::list<POSTGRESQL_TYPE_LIST*>> type_list_map;
	std::list<POSTGRESQL_TYPE_LIST*> make_postgreSQL_type_list(u_char* pkt_data,UINT dataLength, CString key);
	BOOL check_postgreSQL_type_list_done(std::list<POSTGRESQL_TYPE_LIST*> list);
	
	//-
	std::list<POSTGRESQL_TYPE_LIST*> make_combine_list(std::list<POSTGRESQL_TYPE_LIST*> list1, std::list<POSTGRESQL_TYPE_LIST*> list2);
	
	u_char* make_oneData(REAL_PACKET* real,std::list<POSTGRESQL_TYPE_LIST*> list);



	//패킷이 캡처되면 IP, PORT를 검사하여 TRUE면 realPacketMap에 푸시.
	std::map<CString, std::queue<REAL_PACKET*>> realPacketMap;
	BOOL erase_map_key_queue(CString key, std::map<CString, std::queue<REAL_PACKET*>> map);

	BOOL delete_struct_REAL_PACKET(REAL_PACKET* real);
	BOOL delete_struct_PACKET_INFO(PACKET_INFO* info);
	BOOL delete_queue_struct_REAL_PACKET(std::queue<REAL_PACKET*> queue);

	//realPacketMap에서 완성된 패킷이 나오면 이곳에 push한다.
	//UI에서는 FOR_UI 큐가 쌓이면 가져와 현시한다.
	std::queue<REAL_PACKET*> FOR_UI;



	BOOL check_postgreSQL_packet_multi_end(REAL_PACKET* realPacket, CString key);
	

	BOOL set_postgreSQL_real_packet(PACKET_INFO* info, REAL_PACKET* realPacket);
	BOOL set_postgreSQL_real_packet_totalLength(REAL_PACKET* realPacket);

	BOOL make_intact_realData(REAL_PACKET* returnReal,CString key);

	void gettingInfo(std::vector<CString> deviceName, CString filter);
	std::vector<pcap_if_t> getNetworkDevice();
	CString filter = L"";
//	std::queue<PACKET_TYPE> queue_packet;
	//모든 캡처된 패킷을 저장한다.
	std::queue<RAW_PACKET*> queue_packet;
	void get_packet(CString deviceName, CString filter);
	CString settingFilter(CString* filter);
	int checkProtocolType(CString* filter);
	BOOL checkTcpFlag(u_char* tcp,struct TCP_FLAG& flag);


	//2023.02.13 리스트 컬럼 중 "INFO" 컬럼의 내용을 채우기 위하여 추가..
	CString settingInfoString(u_short protocol, void* protocol_data);
	
//	BOOL settingEtherFrame(const u_char* pkt_data, ETHER_HEADER* ether_header, int pkt_length, PACKET_INFO* info);
	BOOL settingIpv4Header(const u_char* pkt_data, IPV4_HEADER* ip_header, int& position);
	BOOL settingIpv6Header(const u_char* pkt_data, IPV6_HEADER* ip_header, int& position);
	BOOL settingArpHeader(const u_char* pkt_data, ARP_HEADER* ip_header, int& position);
	BOOL settingTcpPacket(const u_char* pkt_data, TCP_HEADER* tcp_packet_info, int& position);
	BOOL settingUdpPacket(const u_char* pkt_data, UDP_HEADER* udp_packet_info,int& position);
	BOOL settingPacketInfo(const u_char* pkt_data, int pkt_length, PACKET_INFO* info);
	
	START_PACKET_TIME firstCapturePacketTime;
//	CString getProtocolText(u_char protocol);
	CString getProtocolText(u_short protocol);
	void makeEmptyQueue(std::queue<RAW_PACKET*>* queue);
	void stopRecvPacket();
	void startRecvPacket();

	//2023.02.16 데이터가 존재할 때만 세션에 저장한다..
	BOOL check_packet_data(int startDataPos, int endDataPos);

	//2023.02.16 flag에 FIN이 들어오면 세션에서 지워준다.
	BOOL check_TCP_flag_FIN(void* protocol_data);
	

	void savePcapFile(CString filePath,CListCtrl* m_list);

	void trace_tcp_packet(PACKET_INFO* info, const u_char* pkt_data);

	//postgreSQL
	CString pgsql_checkMessage();
	void postgreSQL_packet(PACKET_INFO* info, const u_char* pkt_data, CString* postgreSql_str);
	void postgreSQL_packet(PACKET_INFO* info, RAW_PACKET* packet, CString* postgreSql_str);
	void postgreSQL_packet(REAL_PACKET* packet, CString* postgreSql_str);

	
	BOOL check_usePacket(PACKET_INFO* info, u_char* pkt_data);
	BOOL set_startUpMessage(CString* info_str,u_char* data, int dataSize);
	BOOL set_SimpleQueryMessage(CString* info_str, u_char* data, int dataSize);
	BOOL set_ErrorMessage(CString* info_str, u_char* data, int dataSize);
	BOOL set_errorCodeForInfoString(CString* errorCode, CString* info_str);
	BOOL set_AuthenticationRequest(CString* info_str, u_char* data, int dataSize);
	BOOL set_AuthenticationRequestPassword(PACKET_INFO* info, CString* info_str, int type);
	BOOL set_AuthenticationRequestLoginSuccess(PACKET_INFO* info, u_char* tempPkt,int dataSize, RAW_PACKET* packet);
	BOOL set_ExtendedQuery(CString* info_str,PACKET_INFO* info, u_char* tempPkt, int dataSize);
	BOOL set_ExtendedQuery(CString* info_str, u_char* tempPkt, int dataSize);
	BOOL set_parseComletion(CString* info_str, u_char* data, int dataSize);



	BOOL set_parse(CString* info_str, u_char* tempPkt, int* Pkt_pos, u_char** oid_list);
	BOOL set_bind(CString* info_str, u_char* tempPkt, int* Pkt_pos,u_char** oid_list);
	BOOL set_bind(CString* info_str, u_char* data, int dataSize, std::list<UINT> oid_list);
	
	int set_columnLengthOfParameter(int th,u_char* oid_list);
	CString get_bind_value(CString* info_str, u_char* data, int dataLength, u_char** typeOid, int th);
	CString get_value_Of_OID_NumericTypes(struct oid_struct oid_info, u_char* data, int dataLength);
	CString get_value_Of_OID_BooleanType(struct oid_struct oid_info, u_char* data);
	CString get_value_Of_OID_StringType(struct oid_struct oid_info, u_char* data, int dataLength);
	CString get_value_Of_OID_DateTimeType(struct oid_struct oid_info, u_char* data, int dataLength);
	CString get_value_Of_OID_UserDefinedType(struct oid_struct oid_info, u_char* data, int dataLength);
	CString get_value_Of_OID_PseudoTypes(struct oid_struct oid_info, u_char* data, int dataLength);
	CString get_value_Of_OID_ArrayTypes(struct oid_struct oid_info, u_char* data, int dataLength);
	CString get_value_Of_OID_BitStringTypes(struct oid_struct oid_info, u_char* data, int dataLength);
	CString get_value_Of_OID_GeometricTypes(struct oid_struct oid_info, u_char* data, int dataLength);
	CString get_value_Of_OID_NetworkAddressTypes(struct oid_struct oid_info, u_char* data, int dataLength);
	CString getget_value_Of_OID_CompositeTypes(struct oid_struct oid_info, u_char* data, int dataLength);
	CString get_value_Of_OID_RangeTypes(struct oid_struct oid_info, u_char* data, int dataLength);
	CString get_value_Of_OID_TimespanTypes(struct oid_struct oid_info, u_char* data, int dataLength);
	CString get_value_Of_OID_UnknownTypes(struct oid_struct oid_info, u_char* data, int dataLength);
	CString get_value_Of_OID_findAndDestroy(struct oid_struct oid_info, u_char* data, int dataLength);
	CString get_hexStringAndUtf8String(u_char* data, int dataLength);
	std::map<UINT, oid_struct> oid_map;
	BOOL set_OidTypeMap(CString filePath);


	CString get_bind_value_binary(CString* info_str, u_char* data, int dataLength, u_char** typeOid, int th);

	std::vector<CString> parse_bind_params(u_char* message, int* before_pos);


	BOOL set_PasswordMessage(CString* info_str, u_char* data, int dataSize);


	//npcap
	std::vector<pcap_if_t> npcap_getNetworkDevice();

	//2023.02.13 Server, Client의 IP, Port 정보를 TRACE()에서 보기 편하도록 CString 형식으로 반환함
	CString print_serverAndClientIpPort(CString s_ip, CString s_port, CString c_ip, CString c_port);	


	int check_count = 0;
	USHORT checkRecvPacket = 1;
	//0 => end
	//1 => start(resume)
	//2 => suspend
	std::vector<std::thread> thread_PacketRecv;
	std::vector<std::thread> thread_check_clearMemoryAlloc;


//	std::vector<struct_session> vector_sessionInfo;
	std::map<CString,struct_session> map_sessionInfo;
	CString make_mapSessionKey(CString s_ip, CString s_port, CString c_ip, CString c_port);
	CString make_mapSessionKey(struct_session session);
	struct_session get_mapSessionValue(CString key);
	BOOL add_mapSessionValue(CString key, struct_session session);
	BOOL check_mapSessionValue(CString key);
	BOOL delete_mapSessionKeyValue(CString key);
	BOOL set_structSession(PACKET_INFO* info,struct_session* session);
	PACKET_INFO* copy_PACKET_INFO_data(PACKET_INFO* info);

	std::map<CString, multi_packet> multi_map;

	UINT recv_packet_mode = 0;
	// 0 -> all
	// 1 -> session
	// 2 -> skip	--------------여기부터는 안씀
	BOOL check_sessionPacket(PACKET_INFO* info_data);
	BOOL set_sessionState(u_short protocol, void* protocol_data, CString ServerIp, CString ServerPort, CString ClientIp, CString ClientPort);
};