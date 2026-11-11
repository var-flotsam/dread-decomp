// Unit 1126: whether the tracked target is within range of the body (or the controller is idle enough).
// Names and layouts are placeholders.
#include <math.h>

#include "game/CEnemyMovement.h"
#include "types.h"

bool fn_71007080dc(void* self);

struct UnkTrackEntry_b260 {
    u8 unk_0x0[0x1a];
    s16 field_0x1a;
    u8 unk_0x1c[0x1a];
    s16 field_0x36;
    u8 unk_0x38[0xe8];
};
static_assert(sizeof(UnkTrackEntry_b260) == 0x120, "entry");

struct UnkTrackWorld_b260 {
    u8 unk_0x0[0x12ad4];
    UnkTrackEntry_b260 entries[1];  // 0x12ad4
};

struct UnkTargetSlot_b260 {
    u32 index;       // 0x37fc8
    f32 position[3]; // 0x37fcc
    u8 unk_0x10[0x14];
    u8 field_0x37fdc;
};

struct UnkTrackBody_b260 {
    u8 unk_0x0[0x44];
    f32 x;
    u8 unk_0x48[0xc];
    f32 y;
    u8 unk_0x58[0xc];
    f32 z;
};

struct UnkTrackOwner_b260 {
    u8 unk_0x0[0x218];
    void* controller;  // 0x218
};

struct UnkTrackSelf_b260 {
    u8 unk_0x0[0x10];
    UnkTrackBody_b260* body;  // 0x10 (its +0x218 is the controller)
    u8* world;  // 0x18
};

bool fn_7100704a1c(UnkTrackSelf_b260* self, f32 range, f32 offset) {
    void* controller = reinterpret_cast<UnkTrackOwner_b260*>(self->body)->controller;
    if (controller && fn_71010cc3a4(controller) == 3) {
        if (offset + 0.1f >= 0.0f && offset + -0.1f <= 0.0f)
            return true;
    } else {
        if (!fn_71007080dc(self))
            return false;
        self->world[0x37fdc] = 1;
        return true;
    }
    u32* slot = reinterpret_cast<u32*>(self->world + 0x37fc8);
    u32 index = slot[0];
    if (index == (u32)-1)
        return false;
    UnkTrackEntry_b260* entry = &reinterpret_cast<UnkTrackWorld_b260*>(self->world)->entries[index];
    if (entry->field_0x1a != -1 && entry->field_0x36 != -1)
        return false;
    f32 px = reinterpret_cast<f32*>(slot)[1], py = reinterpret_cast<f32*>(slot)[2], pz = reinterpret_cast<f32*>(slot)[3];
    UnkTrackBody_b260* body = self->body;
    f32 bx = body->x, by = body->y, bz = body->z;
    f32 dx = px - bx;
    f32 dy = py - by;
    f32 dz = pz - bz;
    return sqrtf(dx * dx + dy * dy + dz * dz) <= range;
}
