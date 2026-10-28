#pragma once

// The game's text is char32_t, and it hands it to the C library's wide functions as wchar_t, which is
// 32 bits on the Switch and on Linux. On Windows wchar_t is 16 bits, so these helpers do the same work
// there on the char32_t text itself. Where wchar_t is 32 bits they are the original calls, inlined, so
// the matching build is unchanged.

#include <wchar.h>

#include "types.h"

#if __WCHAR_MAX__ > 0xffff

// Reads a hexadecimal number (swscanf "%x"); false when there is none.
__attribute__((always_inline)) inline bool char32_scan_hex(const char32_t* s, u32* out) {
    return swscanf(reinterpret_cast<const wchar_t*>(s), L"%x", out) == 1;
}

// Reads a float (swscanf "%f"); false when there is none.
__attribute__((always_inline)) inline bool char32_scan_f32(const char32_t* s, f32* out) {
    return swscanf(reinterpret_cast<const wchar_t*>(s), L"%f", out) == 1;
}

// The first place needle occurs in s (wcsstr), or null.
__attribute__((always_inline)) inline const char32_t* char32_strstr(const char32_t* s,
                                                                    const char32_t* needle) {
    return reinterpret_cast<const char32_t*>(
        wcsstr(reinterpret_cast<const wchar_t*>(s), reinterpret_cast<const wchar_t*>(needle)));
}

#else

#include <stdio.h>

// The scans copy the text's leading ASCII into a narrow buffer and scan that with the same format: a
// number is ASCII, and the scan stops at the first character that is not ASCII anyway (one of the
// few non-ASCII wide spaces before the number is where this differs from swscanf). Text longer than
// the buffer is cut, which no number the game writes reaches.
inline void char32_narrow(const char32_t* s, char* buf, u32 size) {
    u32 i = 0;
    for (; i + 1 < size && s[i] != 0 && s[i] < 0x80; i++)
        buf[i] = (char)s[i];
    buf[i] = 0;
}

inline bool char32_scan_hex(const char32_t* s, u32* out) {
    char buf[128];
    char32_narrow(s, buf, sizeof(buf));
    return sscanf(buf, "%x", out) == 1;
}

inline bool char32_scan_f32(const char32_t* s, f32* out) {
    char buf[128];
    char32_narrow(s, buf, sizeof(buf));
    return sscanf(buf, "%f", out) == 1;
}

inline const char32_t* char32_strstr(const char32_t* s, const char32_t* needle) {
    if (needle[0] == 0)
        return s;
    for (; *s != 0; s++) {
        u32 i = 0;
        while (needle[i] != 0 && s[i] == needle[i])
            i++;
        if (needle[i] == 0)
            return s;
    }
    return nullptr;
}

#endif
