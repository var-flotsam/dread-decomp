// nn::util for the port: a random UUID from the host, formatting through vsnprintf, and the gzip
// trailer. Every stub names the boot-path function that depends on it in a TODO.

#include "nn/util.h"

#include <cstdarg>
#include <cstdio>
#include <random>

namespace nn::util {

Uuid GenerateUuid() {
    // TODO(M1): fn_71001325d0 keeps one as the session id; the play report fn_7100ec32c0 and
    // fn_7100afa6ec, fn_7100b033ac, fn_7100b5ddf0 make their own. Version 4 (random), as the
    // console's are.
    std::random_device device;
    Uuid uuid;
    for (u8& b : uuid.data)
        b = u8(device());
    uuid.data[6] = u8((uuid.data[6] & 0x0f) | 0x40);
    uuid.data[8] = u8((uuid.data[8] & 0x3f) | 0x80);
    return uuid;
}

s32 TSNPrintf(char* buffer, u64 size, const char* format, ...) {
    // TODO(M1): the play report fn_7100ec32c0 formats into 0x40-byte buffers.
    va_list args;
    va_start(args, format);
    const int n = std::vsnprintf(buffer, size, format, args);
    va_end(args);
    return n;
}

s32 SNPrintf(char* buffer, u64 size, const char* format, ...) {
    // No caller in the boot slice: fn_7101204810, fn_7101224074, fn_71012259a8.
    va_list args;
    va_start(args, format);
    const int n = std::vsnprintf(buffer, size, format, args);
    va_end(args);
    return n;
}

u64 GetGzipDecompressedSize(const void* data, u64 size) {
    // No caller in the boot slice: fn_7100140990. ISIZE, the trailer's last word, little-endian
    // (the size modulo 2^32, as the format defines it).
    if (size < 18)
        return 0;  // shorter than a gzip header and trailer
    const auto* p = static_cast<const u8*>(data) + size - 4;
    return u64(p[0]) | u64(p[1]) << 8 | u64(p[2]) << 16 | u64(p[3]) << 24;
}

}  // namespace nn::util
