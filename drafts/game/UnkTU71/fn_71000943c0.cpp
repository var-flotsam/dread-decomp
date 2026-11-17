// Unit 71 of tools/tu_map.py: the value of one base64 character, from a lazily built table indexed by
// character. Names and layouts are placeholders.
#include "base/UnkAtExit.h"
#include "base/global/CRntVector.h"
#include "types.h"

using base::global::CRntVector;

extern "C" int __cxa_guard_acquire(u64* guard);
extern "C" void __cxa_guard_release(u64* guard);

void fn_7100094798(void* vector);

// The function's static table and its guard, as the compiler placed them.
struct UnkDecodeStatics_u71 {
    u64 guard;
    CRntVector<u8> table;  // 0x8
};
__attribute__((visibility("hidden"))) extern UnkDecodeStatics_u71 g_unk_7101d5a1a8;

static inline CRntVector<u8>& DecodeTable_u71() {
    UnkDecodeStatics_u71& s = g_unk_7101d5a1a8;
    if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(&s.guard), __ATOMIC_ACQUIRE) & 1), 0)) {
        if (__cxa_guard_acquire(&s.guard)) {
            s.table.count = 0;
            s.table.capacity = 0;
            s.table.data = nullptr;
            s.table.step = 10;
            s.table.field_0x14 = true;
            s.table.flags = 0xf;
            s.table.allocator = fn_7100083990();
            fn_7100000250(fn_7100094798, &s.table, g_unk_7101a19000);
            __cxa_guard_release(&s.guard);
        }
    }
    return s.table;
}

// The 6-bit value of base64 character `c`; 0 for the padding '='.
u8 fn_71000943c0(u8 c) {
    CRntVector<u8>& table = DecodeTable_u71();
    if (table.count == 0) {
        const char* alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
        table.Reserve(256);
        for (u32 i = 0; i < 64; ++i)
            table.GrowAt(static_cast<u8>(alphabet[i])) = i;
    }
    if (c == '=')
        return 0;
    return table.GrowAt(c);
}
