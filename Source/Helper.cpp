/*
* (C) 2020 see Authors.txt
*
* This file is part of MPC-BE.
*
* MPC-BE is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* MPC-BE is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*/

#include "stdafx.h"
#include "../Include/Version.h"

#ifndef __AVISYNTH_7_H__
#include "../Include/avisynth.h"
#endif
#ifndef VSSCRIPT_H
#include "../Include/VSScript.h"
#endif

#include "Helper.h"



std::wstring GetVersionStr()
{
	std::wstring version = _CRT_WIDE(MPCSS_VERSION_STR);
#if MPCIS_RELEASE != 1
	version += fmt::format(L" (git-{}-{})",
		_CRT_WIDE(_CRT_STRINGIZE(MPCSS_REV_DATE)),
		_CRT_WIDE(_CRT_STRINGIZE(MPCSS_REV_HASH))
	);
#endif
#ifdef _WIN64
	version.append(L" x64");
#endif
#ifdef _DEBUG
	version.append(L" DEBUG");
#endif
	return version;
}

LPCWSTR GetNameAndVersion()
{
	static std::wstring version = L"MPC Script Source " + GetVersionStr();

	return version.c_str();
}

static const FmtParams_t s_FormatTable[] = {
	// fourcc                   |   subtype                | ASformat                | VSformat     | str    |Packsize|buffCoeff|CDepth|planes|bitCount
	{DWORD(-1),                  GUID_NULL,                 0,                        0,             nullptr,        0, 0,       0,     0,     0},
	// YUV packed
	{FCC('YUY2'),                MEDIASUBTYPE_YUY2,         VideoInfo::CS_YUY2,       pfCompatYUY2,  "YUY2",         2, 2,       8,     1,     16},
	// YUV planar
	{FCC('YV12'),                MEDIASUBTYPE_YV12,         VideoInfo::CS_I420,       0,             "I420",         1, 3,       8,     3,     12}, // for tests
	{FCC('YV12'),                MEDIASUBTYPE_YV12,         VideoInfo::CS_YV12,       pfYUV420P8,    "YV12",         1, 3,       8,     3,     12},
	{FCC('YV16'),                MEDIASUBTYPE_YV16,         VideoInfo::CS_YV16,       pfYUV422P8,    "YV16",         1, 4,       8,     3,     16},
	{FCC('YV24'),                MEDIASUBTYPE_YV24,         VideoInfo::CS_YV24,       pfYUV444P8,    "YV24",         1, 6,       8,     3,     24},
	{MAKEFOURCC('Y','3',11,10),  MEDIASUBTYPE_LAV_RAWVIDEO, VideoInfo::CS_YUV420P10,  pfYUV420P10,   "YUV420P10",    2, 3,       10,    3,     24},
	{MAKEFOURCC('Y','3',11,16),  MEDIASUBTYPE_LAV_RAWVIDEO, VideoInfo::CS_YUV420P16,  pfYUV420P16,   "YUV420P16",    2, 3,       16,    3,     24},
	{MAKEFOURCC('Y','3',10,10),  MEDIASUBTYPE_LAV_RAWVIDEO, VideoInfo::CS_YUV422P10,  pfYUV422P10,   "YUV422P10",    2, 4,       10,    3,     32},
	{MAKEFOURCC('Y','3',10,16),  MEDIASUBTYPE_LAV_RAWVIDEO, VideoInfo::CS_YUV422P16,  pfYUV422P16,   "YUV422P16",    2, 4,       16,    3,     32},
	{MAKEFOURCC('Y','3',0,10),   MEDIASUBTYPE_LAV_RAWVIDEO, VideoInfo::CS_YUV444P10,  pfYUV444P10,   "YUV444P10",    2, 6,       10,    3,     48},
	{MAKEFOURCC('Y','3',0,16),   MEDIASUBTYPE_LAV_RAWVIDEO, VideoInfo::CS_YUV444P16,  pfYUV444P16,   "YUV444P16",    2, 6,       16,    3,     48},
	// YUV planar whith alpha
	{MAKEFOURCC('Y','4',0,8),    MEDIASUBTYPE_LAV_RAWVIDEO, VideoInfo::CS_YUVA444,    0,             "YUVA444P8",    1, 8,       8,     4,     32},
	{MAKEFOURCC('Y','4',0,10),   MEDIASUBTYPE_LAV_RAWVIDEO, VideoInfo::CS_YUVA444P10, 0,             "YUVA444P10",   2, 8,       10,    4,     64},
	{MAKEFOURCC('Y','4',0,16),   MEDIASUBTYPE_LAV_RAWVIDEO, VideoInfo::CS_YUVA444P16, 0,             "YUVA444P16",   2, 8,       16,    4,     64},
	// RGB packed
	{BI_RGB,                     MEDIASUBTYPE_RGB24,        VideoInfo::CS_BGR24,      0,             "RGB24",        3, 2,       8,     1,     24},
	{BI_RGB,                     MEDIASUBTYPE_RGB32,        VideoInfo::CS_BGR32,      pfCompatBGR32, "RGB32",        4, 2,       8,     1,     32},
	{BI_RGB,                     MEDIASUBTYPE_ARGB32,       0,                        0,             "ARGB32",       4, 2,       8,     1,     32},
	{MAKEFOURCC('B','G','R',48), MEDIASUBTYPE_RGB48,        VideoInfo::CS_BGR48,      0,             "RGB48",        6, 2,       16,    1,     48},
	{MAKEFOURCC('B','R','A',64), MEDIASUBTYPE_ARGB64,       VideoInfo::CS_BGR64,      0,             "ARGB64",       8, 2,       16,    1,     64},
	// RGB planar
	{MAKEFOURCC('G','3',0,8),    MEDIASUBTYPE_LAV_RAWVIDEO, VideoInfo::CS_RGBP,       pfRGB24,       "RGBP8",        1, 6,       8,     3,     24},
	{MAKEFOURCC('G','3',0,16),   MEDIASUBTYPE_LAV_RAWVIDEO, VideoInfo::CS_RGBP16,     pfRGB48,       "RGBP16",       2, 6,       16,    3,     48},
	// RGB planar whith alpha
	{MAKEFOURCC('G','4',0,8),    MEDIASUBTYPE_LAV_RAWVIDEO, VideoInfo::CS_RGBAP,      0,             "RGBAP8",       1, 8,       8,     4,     32},
	{MAKEFOURCC('G','4',0,16),   MEDIASUBTYPE_LAV_RAWVIDEO, VideoInfo::CS_RGBAP16,    0,             "RGBAP16",      2, 8,       16,    4,     64},
	// grayscale
	{FCC('Y800'),                MEDIASUBTYPE_Y800,         VideoInfo::CS_Y8,         pfGray8,       "Y8",           1, 2,       8,     1,     8},
	{MAKEFOURCC('Y','1',0,16),   MEDIASUBTYPE_Y16,          VideoInfo::CS_Y16,        pfGray16,      "Y16",          2, 2,       16,    1,     16},
};

const FmtParams_t& GetFormatParamsAviSynth(const int asFormat)
{
	for (const auto& f : s_FormatTable) {
		if (f.ASformat == asFormat) {
			return f;
		}
	}
	return s_FormatTable[0];
}

const FmtParams_t& GetFormatParamsVapourSynth(const int vsFormat)
{
	for (const auto& f : s_FormatTable) {
		if (f.VSformat == vsFormat) {
			return f;
		}
	}
	return s_FormatTable[0];
}
