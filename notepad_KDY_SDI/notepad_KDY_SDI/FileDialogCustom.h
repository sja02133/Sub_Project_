#pragma once

#ifndef _WINDOWS_
#   include <Windows.h>
#endif

enum class eBOM {
    eUTF8 = 0x01,   // EF BB BF
    eUTF16BE,       // FE FF
    eUTF16LE,       // FF FE
    eUTF32BE,       // 00 00 FE FF
    eUTF32LE,       // FF FE 00 00
    eNoneBom = 0xFF // Ansi
};

BOOL GetBomType(const TCHAR* filepath, eBOM& bom);
HRESULT BasicFileOpen(OUT TCHAR* filepath, BOOL bOpen, IN OUT eBOM& bom);
