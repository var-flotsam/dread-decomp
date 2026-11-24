// CCharClassDropItemCrazyEvent's unit (tools/tu_map.py unit 2234): fn_7100cb5004 builds the drop spawn
// settings (the 0x90-byte record CDropComponent passes to its spawn, see
// src/game/CDropComponent/CDropComponent_Spawn_ovb0096.cpp) from the event: plain words, the name, two
// vectors, the item id of the definition at +0x78 (fn_7100cb51a0 of its name) and the kind 2. The record is
// returned in memory; its type is local here. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/CCharClassDropItemCrazyEvent.h"
#include "game/SDropProbabilities.h"
#include "game/UnkDropItemDef_ovb0136.h"
#include "game/UnkDropItemName_ovb0136.h"
#include "game/UnkDropItemParams_ovb0136.h"
#include "game/UnkDropItemVec3_ovb0136.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

static inline u32 ItemId(UnkDropItemDef_ovb0136* const& def) {
    if ((u64)def != (u64)&g_unk_7101d47108->empty) {
        ++def->ref_count;
        const char* name = def->name;
        return fn_7100cb51a0(name ? name : g_unk_710162e2ed);
    }
    return 0;
}

struct UnkDropItemCrazyEvent_ovb0136 {
    u8 pad_0x0[0x10];
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u8 pad_0x1c[4];
    u32 field_0x20[7];
    u32 field_0x3c[4];
    u32 field_0x4c[3];
    UnkDropItemName_ovb0136 name;  // 0x58
    UnkDropItemVec3_ovb0136 field_0x60;
    UnkDropItemVec3_ovb0136 field_0x6c;
    UnkDropItemDef_ovb0136* def;  // 0x78
    u8 field_0x80;
    u8 field_0x81;
    u8 field_0x82;
};

struct UnkDropSpawnRequest_ovb0096;

UnkDropItemParams_ovb0136 CCharClassDropItemCrazyEvent_BuildDropSpawnParams(UnkDropSpawnRequest_ovb0096* request) {
    auto* event = reinterpret_cast<UnkDropItemCrazyEvent_ovb0136*>(request);
    UnkDropItemParams_ovb0136 params;
    params.field_0x0 = event->field_0x10;
    params.field_0x4 = event->field_0x14;
    params.field_0x8 = event->field_0x18;
    params.field_0xc = event->field_0x18;
    params.field_0x10[0] = event->field_0x20[0];
    params.field_0x10[1] = event->field_0x20[1];
    params.field_0x10[2] = event->field_0x20[2];
    params.field_0x10[3] = event->field_0x20[3];
    params.field_0x10[4] = event->field_0x20[4];
    params.field_0x10[5] = event->field_0x20[5];
    params.field_0x10[6] = event->field_0x20[6];
    params.field_0x34[0] = event->field_0x3c[0];
    params.field_0x34[1] = event->field_0x3c[1];
    params.field_0x34[2] = event->field_0x3c[2];
    params.field_0x34[3] = event->field_0x3c[3];
    params.field_0x50[0] = event->field_0x4c[0];
    params.field_0x50[1] = event->field_0x4c[1];
    params.field_0x50[2] = event->field_0x4c[2];
    params.name = event->name;
    params.field_0x68 = event->field_0x60;
    params.field_0x74 = event->field_0x6c;
    params.item = ItemId(event->def);
    params.kind = 2;
    params.field_0x88 = event->field_0x80;
    params.field_0x89 = event->field_0x81;
    params.field_0x8a = event->field_0x82;
    return params;
}
