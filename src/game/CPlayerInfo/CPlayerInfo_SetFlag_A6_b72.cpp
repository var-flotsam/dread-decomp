// CPlayerInfo unit (tools/tu_map.py): sets the flag of the entry whose object's word at +0x10 is the key in a
// packed list (u32 count at +0x90, 0x10-byte entries from +0x94), then recomputes the summary flag at +0x8c as
// whether any entry's flag is set. Names and layouts are placeholders.
#include "types.h"

struct UnkKeyedObj_A6_b72 {
    u8 pad_0x0[0x10];
    void* key;
};

#pragma pack(push, 4)
struct UnkFlagEntry_A6_b72 {
    UnkKeyedObj_A6_b72* obj;
    u32 field_0x8;
    u8 flag;
    u8 pad_0xd[3];
};

struct UnkFlagList_A6_b72 {
    u8 pad_0x0[0x8c];
    u8 any;
    u8 pad_0x8d[3];
    u32 count;
    UnkFlagEntry_A6_b72 entries[1];  // 0x94
};
#pragma pack(pop)

static inline __attribute__((always_inline)) UnkFlagEntry_A6_b72* Find_A6_b72(UnkFlagList_A6_b72* self, void* key) {
    UnkFlagEntry_A6_b72* it = self->entries;
    u64 bytes = (u64)self->count * sizeof(UnkFlagEntry_A6_b72);
    asm("" : "+r"(bytes));
    if (bytes == 0)
        return it;
    UnkFlagEntry_A6_b72* end = (UnkFlagEntry_A6_b72*)((char*)it + bytes);
    for (; bytes; ++it, bytes -= sizeof(UnkFlagEntry_A6_b72)) {
        void* k = it->obj ? it->obj->key : nullptr;
        if (k == key)
            return it;
    }
    return end;
}

void fn_7100f0de24(UnkFlagList_A6_b72* self, void* key, bool flag) {
    *reinterpret_cast<char*>(&Find_A6_b72(self, key)->flag) = flag;
    if (flag) {
        self->any = true;
        return;
    }
    u64 bytes = (u64)self->count * sizeof(UnkFlagEntry_A6_b72);
    asm("" : "+r"(bytes));
    for (UnkFlagEntry_A6_b72 *it = self->entries, *end = (UnkFlagEntry_A6_b72*)((char*)it + bytes); it != end; ++it)
        if ((self->any = *reinterpret_cast<char*>(&it->flag)))
            break;
}
