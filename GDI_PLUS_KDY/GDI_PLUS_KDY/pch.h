// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#define GDIPLUS_NO_PRIVATE_GDIPLUSHEADER
#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "framework.h"

#include <FreeImage.h>
#pragma comment(lib,"FreeImage")


#include <gdiplus.h>
#pragma comment(lib,"gdiplus")
using namespace Gdiplus;
#endif //PCH_H
