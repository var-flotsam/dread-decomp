// CCharClassRodotukAIComponent's unit (tools/tu_map.py): reflected collection slot 0x128, erase the 0x30-byte
// entry an index iterator names, with the erase inlined: the tail is moved down one entry (memcpy) and the count
// (+0x8) drops. The index goes through a 64-bit byte offset and a pointer difference, as
// src/base/UnkUnit2640/UnkEraseAtIter50_A6_b67.cpp. Names are placeholders.
#include "types.h"

struct UnkEntry_Rodotuk30_A3_b74 {
    u8 unk_0x0[0x30];
};

struct UnkCollection_Rodotuk30_A3_b74 {
    UnkEntry_Rodotuk30_A3_b74* data;
    u32 count;
};

static inline __attribute__((always_inline)) void EraseAt_Rodotuk30_A3_b74(UnkCollection_Rodotuk30_A3_b74* self,
                                                                           u32 index) {
    if (index >= self->count)
        return;
    UnkEntry_Rodotuk30_A3_b74* entry = &self->data[index];
    __builtin_memcpy(entry, entry + 1, (u64)(self->count - index - 1) * sizeof(UnkEntry_Rodotuk30_A3_b74));
    --self->count;
}

// The slot's collection, opaque here as in the reflected tables.
struct UnkCollectionRef_Rodotuk30_A3_b74;

void fn_7100670e64(UnkCollectionRef_Rodotuk30_A3_b74* self, const u32* it) {
    UnkEntry_Rodotuk30_A3_b74* base = reinterpret_cast<UnkEntry_Rodotuk30_A3_b74*>(self);
    u64 offset = (u64)*it * sizeof(UnkEntry_Rodotuk30_A3_b74);
    UnkEntry_Rodotuk30_A3_b74* p = reinterpret_cast<UnkEntry_Rodotuk30_A3_b74*>(reinterpret_cast<char*>(base) + offset);
    EraseAt_Rodotuk30_A3_b74(reinterpret_cast<UnkCollection_Rodotuk30_A3_b74*>(self), (u32)(p - base));
}
