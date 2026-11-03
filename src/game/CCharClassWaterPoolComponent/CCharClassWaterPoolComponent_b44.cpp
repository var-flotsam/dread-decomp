// A constructor in the CCharClassWaterPoolComponent unit (tools/tu_map.py unit 1396): the 0x40-byte
// object fn_71004076e4 builds at +0x14750 (its base vtable, an empty array with capacity 10, flags
// 0xf and the default allocator, then its own vtable g_unk_7101a33fa0), followed by a cleared word
// and -1 at +0x40. Fields are addressed by offset, as in
// src/game/CCentralUnitAIComponent/fn_71004076e4.cpp. Names and layout are placeholders.
#include "base/UnkVector.h"
#include "types.h"

extern void* g_unk_7101a20998[];  // the object's base vtable
extern void* g_unk_7101a33fa0[];  // its vtable

struct UnkWaterPoolList_b44a2 {
    u8 unk_0x0;
};

template <class T>
static inline T& At(void* base, u64 offset) {
    return *reinterpret_cast<T*>(reinterpret_cast<char*>(base) + offset);
}

void fn_710090bf18(UnkWaterPoolList_b44a2* self) {
    __builtin_memset(&At<u64>(self, 0x10), 0, 0x38);
    At<void**>(self, 0x0) = g_unk_7101a20998 + 2;
    At<u64>(self, 0x8) = 0;
    At<u64>(self, 0x10) = 0;
    At<u32>(self, 0x18) = 10;
    At<u8>(self, 0x1c) = 1;
    At<u32>(self, 0x20) = 0xf;
    base::UnkAllocator* alloc = fn_7100083990();
    At<base::UnkAllocator*>(self, 0x28) = alloc;
    At<u32>(self, 0x30) = 0;
    At<void**>(self, 0x0) = g_unk_7101a33fa0 + 2;
    At<u64>(self, 0x38) = 0;
    At<f32>(self, 0x40) = -1.0f;
}
