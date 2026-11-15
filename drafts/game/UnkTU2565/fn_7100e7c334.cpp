// UnkTU2565: constructor of a large table object: zeroed, an empty string id, a vtable, a block of 0xff
// bytes with a pointer to it and a few counters. Names and layouts are placeholders.
#include <cstring>

#include "base/global/CStrId.h"
#include "game/UnkTU2563.h"
#include "types.h"

using base::global::CStrId;

struct UnkTableTail_e7c334 {
    u64 field_0x0;
    u64 field_0x8;
    u8* slots;      // 0x10
    u32 field_0x18;
};

struct UnkTable_e7c334 {
    CStrId id;                // 0x0
    u64 field_0x8;
    u64 field_0x10;
    void** vtable;            // 0x18
    u8 slots[0x5f38];         // 0x20
    UnkTableTail_e7c334 tail; // 0x5f58
};

void fn_7100e7c334(UnkTable_e7c334* self) {
    UnkTableTail_e7c334* tail = &self->tail;
    memset(self, 0, 0x5f78);
    self->id.data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->id.data = &g_unk_7101d47108->empty;
    self->field_0x8 = 0;
    self->field_0x10 = 0;
    self->vtable = g_unk_7101aa9d38 + 2;
    tail->field_0x18 = 0x17cd;
    tail->field_0x0 = 0x17ce0002f9b8;
    tail->field_0x8 = 0x2f9b8;
    tail->slots = self->slots;
    memset(self->slots, 0xff, 0x5f38);
}
