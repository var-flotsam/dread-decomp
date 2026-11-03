#pragma once
// A string id built in place from a literal whose length and hash the caller computed at compile
// time (fn_71000406dc). The hash is fn_7100001570's: the reflected CRC-64 (ECMA polynomial) with an
// all-ones initial value and no final xor.
#include "base/global/CStrId.h"
#include "types.h"

void fn_71000406dc(base::global::CStrId* self, const char* str, u64 length, u64 hash, bool flag);

constexpr u64 StrIdHash(const char* str, u64 length) {
    u64 crc = ~0ull;
    for (u64 i = 0; i < length; ++i) {
        crc ^= static_cast<u8>(str[i]);
        for (int bit = 0; bit < 8; ++bit)
            crc = (crc & 1) ? (crc >> 1) ^ 0xC96C5795D7870F42ull : crc >> 1;
    }
    return crc;
}

template <u64 Hash>
struct UnkConstHash {
    static constexpr u64 value = Hash;
};

// Builds the id at `self` from a literal, the hash folded at compile time.
#define INIT_HASHED_STRID(self, lit) \
    fn_71000406dc((self), (lit), sizeof(lit) - 1, UnkConstHash<StrIdHash(lit, sizeof(lit) - 1)>::value, true)
