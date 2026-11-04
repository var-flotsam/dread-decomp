// CColliderTriggerComponent unit (tools/tu_map.py, unit 1564): adds a 0x48-byte record (copied by fn_7100331c7c)
// to a fixed list of 32 (u32 count at +0x8, records from +0xc) unless one with the same key (first pointer, then
// +0x10, then +0x10) is there. The result code: 0 already there (a function-local static), 1 added, 2 full,
// 3 added and now full. Names and layouts are placeholders.
#include "types.h"

#include <new>

void fn_7100331c7c(void* self, const void* other);

namespace {
struct UnkTriggerKeyInner_A5_b72 {
    u8 pad_0x0[0x10];
    u64 key;
};
struct UnkTriggerKeyOuter_A5_b72 {
    u8 pad_0x0[0x10];
    UnkTriggerKeyInner_A5_b72* inner;
};
}  // namespace

#pragma pack(push, 4)
struct UnkTriggerEntry_A5_b72 {
    UnkTriggerKeyOuter_A5_b72* outer;
    u64 unk_0x8[8];
    UnkTriggerEntry_A5_b72(const UnkTriggerEntry_A5_b72& other) { fn_7100331c7c(this, &other); }
};

struct UnkTriggerEntryList_A5_b72 {
    void* vtable;
    u32 count;
    UnkTriggerEntry_A5_b72 items[32];
};
#pragma pack(pop)
static_assert(sizeof(UnkTriggerEntry_A5_b72) == 0x48, "entry");

struct UnkAddResult_A5_b72 {
    s32 code;
    UnkAddResult_A5_b72(s32 c) : code(c) {}
    UnkAddResult_A5_b72(const UnkAddResult_A5_b72& other) : code(other.code) {}
    // A weak function-local static (guard and value reached through the GOT).
    static const UnkAddResult_A5_b72& Present() {
        static UnkAddResult_A5_b72 s_present(0);
        return s_present;
    }
};

UnkAddResult_A5_b72 fn_7100a4ccf4(UnkTriggerEntryList_A5_b72* self, const UnkTriggerEntry_A5_b72& entry) {
    u32 count = self->count;
    UnkTriggerEntry_A5_b72* begin = self->items;
    u64 bytes = (u64)count * sizeof(UnkTriggerEntry_A5_b72);
    asm("" : "+r"(bytes));
    UnkTriggerEntry_A5_b72* end = reinterpret_cast<UnkTriggerEntry_A5_b72*>(reinterpret_cast<char*>(begin) + bytes);
    for (UnkTriggerEntry_A5_b72* it = begin; it != end; ++it) {
        if (entry.outer->inner->key == it->outer->inner->key)
            return UnkAddResult_A5_b72::Present();
    }
    s32 free = 32 - (s32)count;
    asm("" : "+r"(free));
    if (free > 0) {
        new (end) UnkTriggerEntry_A5_b72(entry);
        ++self->count;
        return UnkAddResult_A5_b72(free == 1 ? 3 : 1);
    }
    return UnkAddResult_A5_b72(2);
}
