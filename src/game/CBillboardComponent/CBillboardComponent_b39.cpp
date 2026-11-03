// CBillboardComponent unit (tools/tu_map.py): whether the billboard's entry (id at +0x48, -1 for none)
// in the manager g_unk_71726da048 (created on first use, as in
// src/game/CFingSwarmControllerComponent/fn_7100579d08.cpp) has the flag at +0x10d of its data clear.
// Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

extern void* g_unk_71726da048;
void fn_7100e1bca4(void* manager);

struct UnkBillboardData_bbc {
    u8 pad_0x0[0x10d];
    bool field_0x10d;
};

struct UnkBillboardEntry_bbc {
    u8 pad_0x0[0x10];
    UnkBillboardData_bbc* data;  // 0x10
};

UnkBillboardEntry_bbc* fn_7100e1caf0(void* manager, u32 id);

struct UnkBillboard_bbc {
    u8 pad_0x0[0x48];
    u32 id;  // 0x48
};

bool fn_71007298d0(UnkBillboard_bbc* self) {
    if (self->id == 0xffffffff)
        return false;
    void* manager = g_unk_71726da048;
    if (!manager) {
        manager = fn_71000839a0(0xe0, 0x3f);
        fn_7100e1bca4(manager);
        g_unk_71726da048 = manager;
    }
    UnkBillboardEntry_bbc* entry = fn_7100e1caf0(manager, self->id);
    if (!entry)
        return false;
    return !entry->data->field_0x10d;
}
