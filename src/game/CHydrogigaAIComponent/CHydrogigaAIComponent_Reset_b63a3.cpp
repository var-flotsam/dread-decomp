// CHydrogigaAIComponent's unit (tools/tu_map.py unit 883): fn_71005c437c clears the two counters at
// +0x149c4 and +0x149d0 and, when the owner has a collision container (+0x2a0), passes every 0x58-byte
// entry of the array at +0x149e0 whose collision object (looked up by the entry's id at +0x8) has its
// flag at +0x6a set to fn_71005c11fc. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkCollisionRecord.h"
#include "types.h"

struct UnkHydrogigaEntry_b63a3 {
    u8 pad_0x0[0x8];
    base::global::CStrId id;
    u8 pad_0x10[0x58 - 0x10];
};

struct UnkHydrogigaOwner_b63a3 {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* collision;
};

struct UnkHydrogigaAIReset_b63a3 {
    u8 pad_0x0[0x30];
    UnkHydrogigaOwner_b63a3* owner;
    u8 pad_0x38[0x149c4 - 0x38];
    u32 field_0x149c4;
    u8 pad_0x149c8[0x149d0 - 0x149c8];
    u32 field_0x149d0;
    u8 pad_0x149d4[0x149e0 - 0x149d4];
    UnkHydrogigaEntry_b63a3* entries;
    u8 pad_0x149e8[0x149f0 - 0x149e8];
    u32 count;
};

void fn_71005c11fc(UnkHydrogigaAIReset_b63a3* self, UnkHydrogigaEntry_b63a3* entry, bool on);

void fn_71005c437c(UnkHydrogigaAIReset_b63a3* self) {
    self->field_0x149c4 = 0;
    self->field_0x149d0 = 0;
    UnkCollisionContainer* collision = self->owner->collision;
    if (!collision)
        return;
    u32 count = self->count;
    if (count == 0)
        return;
    UnkHydrogigaEntry_b63a3* entry = self->entries;
    for (UnkHydrogigaEntry_b63a3* end = entry + count; entry != end; ++entry) {
        UnkCollisionObject* object = fn_71008640a4(collision, &entry->id);
        if (object && reinterpret_cast<u8*>(object)[0x6a])
            fn_71005c11fc(self, entry, false);
    }
}
