#pragma once

ULONG CharSet(const char* str, int bytelen);
void UTF8ToANSI(const char* pszCode, CString* str, int* existing_pos);

template <class T>
void endswap(T* objp)
{
    unsigned char* memp = reinterpret_cast<unsigned char*>(objp);
    std::reverse(memp, memp + sizeof(T));
}

template <class T>
T* endswap_t(T* objp)
{
    unsigned char* memp = reinterpret_cast<unsigned char*>(objp);
    std::reverse(memp, memp + sizeof(T));
    return objp;
}

BOOL is_network_order(unsigned char* a) {
    if (a == 0)
        return FALSE;
    unsigned char p = *a;
    return (p == 0);
}

unsigned short get_byteToShort(unsigned char* byte)
{
    USHORT data = 0;
    memcpy(&data, &byte[0], sizeof(USHORT));
    if (byte == 0)
        return 0;
    if (is_network_order(byte)) {
        endswap(&data);
        return data;
    }
    else {
        //host order
        return data;
    }
}

unsigned int get_byteToInt(unsigned char* byte)
{
    int data = 0;
    memcpy(&data, &byte[0], sizeof(UINT));
    if (byte == 0)
        return 0;
    if (is_network_order(byte)){
        //network order
//        data = ntohl(*reinterpret_cast<int*>(byte));
        //return *(unsigned int*)byte;
        endswap(&data);
        return data;
    }
    else {
        //host order
        return data;
    }
}

int32_t shift_and_return(u_char* value, size_t bytes) {
    if (bytes == 1) {
        uint8_t p = *(uint8_t*)value;
        return (int32_t)(p << 8);
    }
    else if (bytes == 2) {
        uint16_t p = *(uint16_t*)value;
        return (int32_t)(p << 16);
    }
    else if (bytes == 4) {
        uint32_t p = *(uint32_t*)value;
        return (int32_t)(p);
    }
    else if (bytes == 8) {
        uint64_t p = *(uint64_t*)value;
        return (int32_t)(p >> 32);
    }
    // 처리할 수 없는 크기의 값이 들어온 경우 -1을 리턴합니다.
    return -1;
}


#define STRING_ENG 0
#define STRING_ANSI 1
#define STRING_UTF8 2

void ANSIToUTF8(const char* pszCode, CString* str, int bytelen);
CString setCharacterSet(u_char* tempPkt, int bytelen) {
    int i = 0;
    CString str = L"";
    int tempPktPos = i;
    CString temp_String = L"";
    u_char ansiChar[2] = { 0, };
    char* tempAnsiChar = 0;

    ULONG checkCharSet = CharSet((char*)&tempPkt[i], bytelen);
    switch (checkCharSet)
    {
    case STRING_ENG:
    {
        while (tempPktPos < bytelen) {
            str += tempPkt[tempPktPos];
            tempPktPos++;
        }
    }
    break;
    case STRING_ANSI:
    {
        USES_CONVERSION;
        while (tempPktPos < bytelen) {
            if (tempPkt[tempPktPos] < 128) {
                str += tempPkt[tempPktPos];
                tempPktPos++;
            }
            else {
                ansiChar[0] = *&tempPkt[tempPktPos];
                ansiChar[1] = *&tempPkt[tempPktPos + 1];
                temp_String = A2W((char*)ansiChar);
                temp_String = temp_String.Left(1);
                tempPktPos += 2;
                str += temp_String;
            }
        }
    }
    break;
    case STRING_UTF8:
    {
        //UTF8ToANSI((char*)&tempPkt[tempPktPos], &str, &tempPktPos);
        ANSIToUTF8((char*)&tempPkt[0], &str,bytelen);
    }
    break;
    default:
        return FALSE;
        break;
    }
    return str;
}

ULONG CharSet(const char* str, int bytelen)
{
    ULONG ret = STRING_ENG;
    if (!str || !bytelen || (bytelen > 64000)) {
        return STRING_ENG;
    }

    bool isUtf8(true);
    int index = 0;

    while (index < bytelen && isUtf8) {
        char achar = str[index];
        int supplemental_bytes = 0;
        if ((achar & 0x80) == 0) {
            // 0xxxxxxx
            ++index;
        }
        else if ((achar & 0xF8) == 0xF0) {
            // 11110uuu 10uuzzzz 10yyyyyy 10xxxxxx
            ++index;
            supplemental_bytes = 3;
            ret |= STRING_UTF8;
        }
        else if ((achar & 0xE0) == 0xE0) {
            // 1110zzzz 10yyyyyy 10xxxxxx
            ++index;
            supplemental_bytes = 2;
            ret |= STRING_UTF8;
        }
        else if ((achar & 0xE0) == 0xC0) {
            // 110yyyyy 10xxxxxx
            ++index;
            supplemental_bytes = 1;
            ret |= STRING_UTF8;
        }
        else {
            isUtf8 = false;
            ret |= STRING_ANSI;
        }

        while (isUtf8 && supplemental_bytes--) {
            if (index >= bytelen) {
                isUtf8 = false;
                ret |= STRING_ANSI;
            }
            else if ((str[index++] & 0xC0) != 0x80) {
                // 10uuzzzz
                isUtf8 = false;
                ret |= STRING_ANSI;
            }
        }
    }

    if (ret == STRING_ENG) { return STRING_ENG; }
    if (ret & STRING_ANSI) { return STRING_ANSI; }

    return STRING_UTF8;
}


char* UTF8ToANSI(const char* pszCode)
{
    BSTR    bstrWide;
    char* pszAnsi;
    int     nLength;

    nLength = MultiByteToWideChar(CP_UTF8, 0, pszCode, strlen(pszCode) + 1, NULL, NULL);
    bstrWide = SysAllocStringLen(NULL, nLength);

    MultiByteToWideChar(CP_UTF8, 0, pszCode, strlen(pszCode) + 1, bstrWide, nLength);

    nLength = WideCharToMultiByte(CP_ACP, 0, bstrWide, -1, NULL, 0, NULL, NULL);
    pszAnsi = new char[nLength];

    WideCharToMultiByte(CP_ACP, 0, bstrWide, -1, pszAnsi, nLength, NULL, NULL);
    SysFreeString(bstrWide);

    return pszAnsi;
}


void a(u_char* tempPkt) {
    //tempPkt data is "ec 95 88 eb 85 95"
    CString str = L"";
    int bytelen = 6;
    ANSIToUTF8((char*)&tempPkt[0], &str, bytelen);
}

//test
void ANSIToUTF8(const char* pszCode, CString* str,int bytelen)
{
    BSTR    bstrWide;
    u_char* pszAnsi;
    int     nLength;

    nLength = MultiByteToWideChar(CP_UTF8, 0, pszCode, bytelen, NULL, NULL);
    bstrWide = SysAllocStringLen(NULL, nLength);

    MultiByteToWideChar(CP_UTF8, 0, pszCode, bytelen + 1, bstrWide, nLength);

    nLength = WideCharToMultiByte(CP_ACP, 0, bstrWide, -1, NULL, 0, NULL, NULL);
    pszAnsi = new u_char[nLength];

    WideCharToMultiByte(CP_ACP, 0, bstrWide, -1, (char*)pszAnsi, nLength, NULL, NULL);

    SysFreeString(bstrWide);

    u_char ansiChar[2] = { 0, };
    CString temp_String = L"";
    USES_CONVERSION;
    for (int i = 0; i < nLength - 1;) {
        //        *str += pszAnsi[i];
        if (pszAnsi[i] != 0 && pszAnsi[i] < 128) {
            *str += pszAnsi[i];
            i++;
        }
        else {
            ansiChar[0] = *&pszAnsi[i];
            ansiChar[1] = *&pszAnsi[i + 1];
            temp_String = A2W((char*)ansiChar);
            temp_String = temp_String.Left(1);
            i += 2;
            *str += temp_String;
        }
    }
 //   *existing_pos += nLength;

    delete pszAnsi;
    //   return pszAnsi;
}


void UTF8ToANSI(const char* pszCode,CString* str,int* existing_pos)
{
    BSTR    bstrWide;
    u_char* pszAnsi;
    int     nLength;

    nLength = MultiByteToWideChar(CP_UTF8, 0, pszCode, strlen(pszCode) + 1, NULL, NULL);
    bstrWide = SysAllocStringLen(NULL, nLength);

    MultiByteToWideChar(CP_UTF8, 0, pszCode, strlen(pszCode) + 1, bstrWide, nLength);

    nLength = WideCharToMultiByte(CP_ACP, 0, bstrWide, -1, NULL, 0, NULL, NULL);
    pszAnsi = new u_char[nLength];

    WideCharToMultiByte(CP_ACP, 0, bstrWide, -1, (char*)pszAnsi, nLength, NULL, NULL);
    
    SysFreeString(bstrWide);

    u_char ansiChar[2] = { 0, };
    CString temp_String = L"";
    USES_CONVERSION;
    for (int i = 0; i < nLength-1;) {
//        *str += pszAnsi[i];
        if (pszAnsi[i] < 128) {
            *str += pszAnsi[i];
            i++;
        }
        else {
            ansiChar[0] = *&pszAnsi[i];
            ansiChar[1] = *&pszAnsi[i + 1];
            temp_String = A2W((char*)ansiChar);
            temp_String = temp_String.Left(1);
            i += 2;
            *str += temp_String;
        }
    }
    *existing_pos += nLength;

    delete pszAnsi;
 //   return pszAnsi;
}