// Unit 2661 (CCutscene::CTunableCutscene): sets the 3-float position at +0x4d0 when any component differs by more than 0.001,
// copies it into the target object and calls virtual slot 0xd0 on each listed entry. Names and layouts are placeholders.
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkTarget2661 {
    u8 pad_0x0[0x4a8];
    UnkVec3f pos;  // 0x4a8
};

struct UnkTargetRef2661 {
    u8 pad_0x0[0x10];
    UnkTarget2661* target;  // 0x10
};

struct UnkListed2661 {
    void** vtable;
};

struct UnkEntry2661 {
    u8 pad_0x0[0x18];
    UnkListed2661* listed;  // 0x18
    u8 pad_0x20[0x10];
};

struct UnkOwner2661 {
    u8 pad_0x0[0x150];
    UnkEntry2661* entries;  // 0x150
    u32 count;              // 0x158
    u8 pad_0x15c[0x4d0 - 0x15c];
    UnkVec3f pos;  // 0x4d0
    u8 pad_0x4dc[0x4f8 - 0x4dc];
    UnkTargetRef2661* targetRef;  // 0x4f8
};

void fn_710106bcfc(UnkOwner2661* self, const UnkVec3f* pos) {
    if (UNK_CHANGED(self->pos.x, pos->x) || UNK_CHANGED(self->pos.y, pos->y) || UNK_CHANGED(self->pos.z, pos->z)) {
        self->pos = *pos;
        self->targetRef->target->pos = self->pos;
        UnkEntry2661* it = self->entries;
        UnkEntry2661* end = it + self->count;
        for (; it != end; ++it)
            reinterpret_cast<void (*)(UnkListed2661*, UnkVec3f*, int)>(it->listed->vtable[0xd0 / 8])(it->listed, &self->pos, 1);
    }
}
