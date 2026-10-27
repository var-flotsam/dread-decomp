// base::global::CRntFile (include/base/global/CRntFile.h), translation unit 56 of data/tu_map.csv:
// the formatted read (fn_710007cf30), the counterpart of the formatted write fn_710007cb4c. Each
// format character reads one value into the pointer the next argument gives: 'I', 'U' and 'o'
// eight bytes, 'f', 'i' and 'u' four, 'b' one, 'c' and 's' a string of up to 512 chars, 'w' a wide
// string of up to 4096 two-byte chars (stored as 4-byte ones); other characters are skipped. The
// file is read from its start. Its read (fn_710007c714) and position change (fn_710007c00c) are in
// src/base/global/CRntFile.cpp; the original inlines them here (bar the 8-byte read), so this file
// has inline copies.
#include <stdarg.h>
#include <string.h>

#include "base/global/CRntFile.h"
#include "types.h"

using base::global::CRntFile;

namespace {

// fn_710007c00c (src/base/global/CRntFile.cpp), inline.
inline bool UnkSeek(CRntFile* file, u64 offset, s32 whence) {
    u8* data = file->data;
    if (!data)
        return false;
    if (whence != 0) {
        u64* base = &file->position;
        if (whence != 1) {
            if (whence != 2)
                return false;
            base = &file->size;
            offset = -1;
        }
        offset = *base + offset;
    }
    u64 size = file->size;
    if (offset >= size) {
        file->position = size;
        return false;
    }
    if ((u64)data + offset > (u64)data + size)
        file->position = size;
    else
        file->position = offset;
    return true;
}

inline u64 UnkElementCount(u64 left, u64 size) {
#if defined(__aarch64__)
    return left / size;
#else
    return size ? left / size : 0;
#endif
}

inline void UnkSwapElements(u8* p, u64 size, u64 count) {
    if (size == 2) {
        for (; count != 0; --count, p += 2) {
            u8 t = p[0];
            p[0] = p[1];
            p[1] = t;
        }
    } else if (size == 8) {
        for (; count != 0; --count, p += 8) {
            for (int j = 0; j < 4; ++j) {
                u8 t = p[j];
                p[j] = p[7 - j];
                p[7 - j] = t;
            }
        }
    } else if (size == 4) {
        for (; count != 0; --count, p += 4) {
            for (int j = 0; j < 2; ++j) {
                u8 t = p[j];
                p[j] = p[3 - j];
                p[3 - j] = t;
            }
        }
    }
}

// fn_710007c714 (src/base/global/CRntFile.cpp), inline.
inline u64 UnkRead(CRntFile* file, void* dest, u64 size, u64 count) {
    u8* data = file->data;
    if (data) {
        u64 position = file->position;
        u64 end = file->size;
        u64 left = end - position;
        if (end > position) {
            u64 bytes = count * size;
            if (bytes + (u64)data + position > (u64)data + end) {
                memcpy(dest, data + position, left);
                if (file->swap)
                    UnkSwapElements(static_cast<u8*>(dest), size, UnkElementCount(left, size));
                file->position += left;
                return left;
            }
            memcpy(dest, data + position, bytes);
            if (file->swap)
                UnkSwapElements(static_cast<u8*>(dest), size, count);
            file->position += bytes;
            return bytes;
        }
    }
    return 0;
}

}  // namespace

// Returns the bytes read.
// NON_MATCHING(minor): register allocation and frame layout (the original keeps the position's
// address, the 0x20 fill and the table base in callee-saved registers, so it saves one more pair and
// its frame is 0x10 larger), the format pointer is advanced without a pre-indexed load, and the
// inline reads test the element count before the swap flag; the same reads in the same order.
s64 fn_710007cf30(void* stream, const char* format, ...) {
    CRntFile* file = static_cast<CRntFile*>(stream);
    UnkSeek(file, 0, 0);
    va_list args;
    va_start(args, format);
    s64 total = 0;
    for (; *format; ++format) {
        switch (*format) {
        case 'I':
        case 'U':
        case 'o':
            total += fn_710007c714(file, va_arg(args, u64*), 8, 1);
            break;
        case 'b':
            total += UnkRead(file, va_arg(args, u8*), 1, 1);
            break;
        case 'c': {
            char* dest = va_arg(args, char*);
            u32 n = 0;
            char c = ' ';
            do {
                UnkRead(file, &c, 1, 1);
                dest[n++] = c;
            } while (n < 0x200 && c != '\0');
            total += n;
            break;
        }
        case 'f':
            total += UnkRead(file, va_arg(args, f32*), 4, 1);
            break;
        case 'i':
            total += UnkRead(file, va_arg(args, s32*), 4, 1);
            break;
        case 's': {
            char* dest = va_arg(args, char*);
            u32 n = 0;
            char c = ' ';
            do {
                UnkRead(file, &c, 1, 1);
                dest[n++] = c;
            } while (n < 0x200 && c != '\0');
            total += n;
            break;
        }
        case 'u':
            total += UnkRead(file, va_arg(args, u32*), 4, 1);
            break;
        case 'w': {
            u32* dest = va_arg(args, u32*);
            u32 n = 0;
            u32 c = ' ';
            do {
                UnkRead(file, &c, 2, 1);
                dest[n++] = c;
            } while (n < 0x1000 && c != 0);
            total += n * 2;
            break;
        }
        }
    }
    va_end(args);
    return total;
}
