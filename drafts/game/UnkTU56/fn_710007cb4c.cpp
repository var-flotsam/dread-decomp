// UnkTU56: formatted writes to a CRntFile, the counterpart of fn_710007cf30's reads ("b" bool, "c"
// char, "f" float, "i"/"u" 32-bit, "o" 64-bit, "s" string, "v" three floats). Returns the bytes
// written. Names and layouts are placeholders.
#include <stdarg.h>
#include <string.h>

#include "base/global/CRntFile.h"
#include "game/UnkTU56.h"
#include "types.h"

using base::global::CRntFile;

struct UnkVec3_cb4c {
    f32 x, y, z;
};

// Writes `size` bytes at the position and moves it on; returns `size` (fn_710007c3f0, inlined).
static inline u64 UnkWriteBytes(CRntFile* file, const void* data, u64 size) {
    fn_710007c514(file, size, 1);
    memcpy(file->data + file->position, data, size);
    file->position += size;
    return size;
}

// Writes one value; returns its size.
template <class T>
static inline u64 UnkWrite(CRntFile* file, T value) {
    return UnkWriteBytes(file, &value, sizeof(T));
}

s64 fn_710007cb4c(void* stream, const char* format, ...) {
    CRntFile* file = static_cast<CRntFile*>(stream);
    va_list args;
    va_start(args, format);
    s64 total = 0;
    for (; *format; ++format) {
        switch (*format) {
        case 'b':
            total += UnkWrite<u8>(file, va_arg(args, int) != 0);
            break;
        case 'c':
            total += UnkWrite<u8>(file, va_arg(args, int));
            break;
        case 'f':
            total += UnkWrite<f32>(file, va_arg(args, f64));
            break;
        case 'i':
            total += UnkWrite<s32>(file, va_arg(args, s32));
            break;
        case 'o':
            total += UnkWrite<u64>(file, va_arg(args, u64));
            break;
        case 's': {
            const char* text = va_arg(args, const char*);
            total += UnkWriteBytes(file, text, strlen(text) + 1);
            break;
        }
        case 'u':
            total += UnkWrite<u32>(file, va_arg(args, u32));
            break;
        case 'v':
            total += UnkWrite<UnkVec3_cb4c>(file, *va_arg(args, const UnkVec3_cb4c*));
            break;
        }
    }
    va_end(args);
    return total;
}
