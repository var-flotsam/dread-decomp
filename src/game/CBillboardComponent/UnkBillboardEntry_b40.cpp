// The unit after CBillboardComponent's (tools/tu_map.py unit 1161, no class hint) holds the 0x148-byte
// entries CBillboardComponent keeps at +0x50 (their destructor fn_7100745b24 is in it). fn_7100745e74
// moves an entry to a position (relative to the actor at +0x8 when there is one), and unless asked not
// to, moves each item of its list at +0xb8 back by the same offset. Names and layouts are placeholders.
#include "game/UnkActor.h"
#include "types.h"

struct UnkVec3_be {
    f32 x, y, z;
};

struct UnkItem_be {
    u8 pad_0x0[0x2c];
    UnkVec3_be offset;
};

struct UnkBillboardEntry_be {
    u8 pad_0x0[0x8];
    UnkActor* actor;
    u8 pad_0x10[0x20];
    UnkVec3_be pos;  // 0x30
    u8 pad_0x3c[0x7c];
    UnkItem_be** items;  // 0xb8
    u32 count;  // 0xc0
};

void fn_7100745e74(UnkBillboardEntry_be* self, const UnkVec3_be* target, bool keep_items) {
    UnkVec3_be d;
    if (UnkActor* actor = self->actor) {
        UnkVec3_be a = {actor->field_0x44, actor->field_0x54, actor->field_0x64};
        d.x = target->x - a.x;
        d.y = target->y - a.y;
        d.z = target->z - a.z;
    } else {
        d = *target;
    }
    f32 dx = d.x - self->pos.x;
    f32 dy = d.y - self->pos.y;
    f32 dz = d.z - self->pos.z;
    self->pos.x = self->pos.x + dx;
    self->pos.y = self->pos.y + dy;
    self->pos.z = self->pos.z + dz;
    if (keep_items)
        return;
    for (UnkItem_be **it = self->items, **end = self->items + self->count; it != end; ++it) {
        UnkItem_be* item = *it;
        item->offset.x = item->offset.x - dx;
        item->offset.y = item->offset.y - dy;
        item->offset.z = item->offset.z - dz;
    }
}
