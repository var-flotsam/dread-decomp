// CPickable's unit (tools/tu_map.py unit 2591): sets a pickable up from its key, position and
// parameters, takes a weak reference to its target (counting the pickable on the target), clears
// its owner and name and marks the pickable's attached items. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

struct UnkPickableKey {
    u64 field_0x0;
    u64 field_0x8;
};

struct UnkPickablePos {
    u32 x;
    u32 y;
    u32 z;
};

struct UnkPickableTarget : UnkWeakTarget {
    u8 pad_0x8[0x1a4 - 0x8];
    s32 pickable_count;  // 0x1a4
};

struct UnkPickableItem {
    u8 pad_0x0[0x25];
    u8 field_0x25;
};

struct UnkPickable {
    u8 pad_0x0[0x10];
    UnkPickableKey key;  // 0x10
    UnkPickableItem* items[2];  // 0x20
    u32 item_count;  // 0x30
    bool field_0x34;
    UnkPickablePos pos;  // 0x38
    f32 field_0x44;
    f32 field_0x48;
    f32 field_0x4c;
    f32 field_0x50;
    f32 field_0x54;
    f32 field_0x58;
    f32 field_0x5c;
    bool field_0x60;
    f32 field_0x64;
    f32 field_0x68;
    f32 field_0x6c;
    f32 field_0x70;
    bool field_0x74;
    u8 pad_0x75;
    bool field_0x76;
    f32 field_0x78;
    u8 pad_0x7c[0x94 - 0x7c];
    bool field_0x94;
    u32 field_0x98;
    bool field_0x9c;
    f32 field_0xa0;
    f32 field_0xa4;
    u8 pad_0xa8;
    u8 field_0xa9;
    bool field_0xaa;
    UnkWeakHandle* owner;  // 0xb0
    CStrId name;  // 0xb8
    u8 pad_0xc0[0xe0 - 0xc0];
    UnkWeakHandle* target;  // 0xe0
};
static_assert(__builtin_offsetof(UnkPickable, field_0x78) == 0x78, "UnkPickable layout");
static_assert(__builtin_offsetof(UnkPickable, field_0xaa) == 0xaa, "UnkPickable layout");
static_assert(__builtin_offsetof(UnkPickable, target) == 0xe0, "UnkPickable layout");

void fn_7100f05ce0(UnkPickable* self, const UnkPickableKey* key, const UnkPickablePos* pos, f32 a,
                   f32 b, f32 c, f32 d, UnkPickableTarget* target, u8 flag_a9, bool flag_aa,
                   f32 e) {
    self->key = *key;
    self->field_0x34 = true;
    self->pos.x = pos->x;
    self->pos.y = pos->y;
    self->pos.z = pos->z;
    self->field_0x44 = a;
    self->field_0x48 = b;
    self->field_0x4c = 0.0f;
    self->field_0x50 = c;
    self->field_0x54 = d;
    self->field_0x6c = 0.0f;
    self->field_0x60 = false;
    self->field_0x5c = 0.0f;
    self->field_0x70 = 0.0f;
    self->field_0x58 = 0.0f;
    self->field_0x68 = 0.0f;
    self->field_0x64 = 0.0f;
    self->field_0x74 = false;
    self->field_0x78 = e;
    self->field_0x94 = false;
    self->field_0x98 = 0;
    self->field_0x9c = false;
    self->field_0xa0 = 1.0f;
    self->field_0xa4 = 1.0f;
    ResetRef(&self->target);
    self->item_count = 0;
    self->field_0xa9 = flag_a9;
    self->field_0xaa = flag_aa;
    if (target) {
        ++target->pickable_count;
        AssignRef(&self->target, GetWeakHandle(target));
    }
    ResetRef(&self->owner);
    self->name = g_unk_7101d470e0;
    self->field_0x76 = true;
    for (u64 i = 0; i < self->item_count; ++i)
        self->items[i]->field_0x25 = 1;
}
