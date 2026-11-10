// CCharClassDropItemCrazyEvent's unit (tools/tu_map.py unit 2234): fn_7100cb5004 builds the drop spawn
// settings (the 0x90-byte record CDropComponent passes to its spawn, see
// src/game/CDropComponent/CDropComponent_Spawn_ovb0096.cpp) from the event: plain words, the name, two
// vectors, the item id of the definition at +0x78 (fn_7100cb51a0 of its name) and the kind 2. The record is
// returned in memory; its type is local here. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

void fn_71007c7ddc(void*);
u32 fn_7100cb51a0(const char* name);

// A string id assigned like CStrId; the old one is released in two steps (the original reloads
// `data` and the count after the decrement).
struct UnkDropItemName_ovb0136 {
    UnkStringData* data;
    UnkDropItemName_ovb0136& operator=(const UnkDropItemName_ovb0136& other) {
        if (data != other.data) {
            UnkStringData* d = other.data;
            UnkStringPool* pool = g_unk_7101d47108;
            if (d != &pool->empty)
                ++d->ref_count;
            if (data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0)
                    fn_71000803a4(pool, data);
            }
            data = d;
        }
        return *this;
    }
};

struct UnkDropItemVec3_ovb0136 {
    f32 x, y, z;
    UnkDropItemVec3_ovb0136& operator=(const UnkDropItemVec3_ovb0136& other) {
        u64 xy;
        u32 w;
        __builtin_memcpy(&xy, &other, 8);
        __builtin_memcpy(&w, &other.z, 4);
        __builtin_memcpy(this, &xy, 8);
        __builtin_memcpy(&z, &w, 4);
        return *this;
    }
};

// The drop spawn settings (0x90 bytes; as CDropComponent passes them to its spawn), built by
// fn_71007c7ddc.
struct UnkDropItemParams_ovb0136 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10[7];
    u8 pad_0x2c[8];
    u32 field_0x34[4];
    u8 pad_0x44[0xc];
    u32 field_0x50[3];
    u8 pad_0x5c[4];
    UnkDropItemName_ovb0136 name;      // 0x60
    UnkDropItemVec3_ovb0136 field_0x68;
    UnkDropItemVec3_ovb0136 field_0x74;
    u32 item;                           // 0x80
    u32 kind;                           // 0x84
    u8 field_0x88;
    u8 field_0x89;
    u8 field_0x8a;

    UnkDropItemParams_ovb0136() { fn_71007c7ddc(this); }
};

// An item definition: a counted object with its name at +0x10.
struct UnkDropItemDef_ovb0136 {
    u8 pad_0x0[0x10];
    const char* name;
    u8 pad_0x18[0x2c];
    // A union member: the definition pointer is reloaded after the increment.
    union {
        u32 ref_count;  // 0x44
    };
};

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

UnkDropItemParams_ovb0136 fn_7100cb5004(UnkDropSpawnRequest_ovb0096* request) {
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
