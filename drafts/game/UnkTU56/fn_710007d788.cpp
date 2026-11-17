// base::global::CRntFile, translation unit 56: the formatted read from the file's position (no
// seek to the start, unlike fn_710007cf30). Each format character reads one value into the pointer
// the next argument gives. Names and layouts are placeholders.
#include <stdarg.h>
#include <string.h>

#include "base/global/CRntFile.h"
#include "types.h"

using base::global::CRntFile;

namespace {

inline u64 UnkElementCount(u64 left, u64 size) {
#if defined(__aarch64__)
    return left / size;
#else
    return size ? left / size : 0;
#endif
}

// Swaps the bytes of each of `count` elements (at least one) of `size` bytes.
inline void UnkSwapElements(u8* p, u64 size, u64 count) {
    if (size == 2) {
        do {
            u8 t = p[0];
            p[0] = p[1];
            p[1] = t;
            p += 2;
        } while (--count != 0);
    } else if (size == 8) {
        do {
            for (int j = 0; j < 4; ++j) {
                u8 t = p[j];
                p[j] = p[7 - j];
                p[7 - j] = t;
            }
            p += 8;
        } while (--count != 0);
    } else if (size == 4) {
        do {
            for (int j = 0; j < 2; ++j) {
                u8 t = p[j];
                p[j] = p[3 - j];
                p[3 - j] = t;
            }
            p += 4;
        } while (--count != 0);
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
                u64 whole = UnkElementCount(left, size);
                if (whole != 0 && file->swap)
                    UnkSwapElements(static_cast<u8*>(dest), size, whole);
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

// Returns the bytes read ('c' always counts one).
u64 fn_710007d788(CRntFile* file, const char* format, ...) {
    va_list args;
    va_start(args, format);
    u64 total = 0;
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
        case 'c':
            UnkRead(file, va_arg(args, char*), 1, 1);
            total += 1;
            break;
        case 'f':
            total += UnkRead(file, va_arg(args, f32*), 4, 1);
            break;
        case 'i':
            total += UnkRead(file, va_arg(args, s32*), 4, 1);
            break;
        case 's': {
            char* dest = va_arg(args, char*);
            u64 n = 0;
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
