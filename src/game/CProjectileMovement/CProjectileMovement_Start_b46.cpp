// CProjectileMovement unit (tools/tu_map.py unit 1623): after the virtual at slot 0x348, adds the
// movement's three timeline entries to the owner's timeline (+0x2e8) once (two static keys and the
// one the virtual at slot 0x2d8 returns, all with the movement's string id at +0x598), then, when
// the definition's flag (+0xec) is set, moves the owner to the stored position (+0x3b4) and marks
// it moved. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

void fn_7100a2cddc(void* table, void* key, CStrId* id, bool unique, f32 delay);

// Static timeline keys, 0x18 bytes each.
struct UnkTimelineKey2_b46a3 {
    u8 data[0x18];
};
extern UnkTimelineKey2_b46a3 g_unk_71727556b8[2] __attribute__((visibility("hidden")));

struct UnkStartOwner_b46a3 {
    u8 pad_0x0[0x44];
    u32 x;  // 0x44
    u8 pad_0x48[0xc];
    u32 y;  // 0x54
    u8 pad_0x58[0xc];
    u32 z;  // 0x64
    u8 pad_0x68[0x280];
    void* timeline;  // 0x2e8
    u8 pad_0x2f0[0xc0];
    bool moved;  // 0x3b0
};

struct UnkStartDefinition_b46a3 {
    u8 pad_0x0[0xec];
    bool move_owner;  // 0xec
};

struct UnkProjectileMovement10_b46a3 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkStartOwner_b46a3* owner;  // 0x30
    u8 pad_0x38[0x37c];
    u32 pos_x, pos_y, pos_z;  // 0x3b4
    u8 pad_0x3c0[0x164];
    bool entries_added;  // 0x524
    u8 pad_0x525[0x73];
    CStrId id;  // 0x598
    u8 pad_0x5a0[0x28];
    UnkStartDefinition_b46a3* definition;  // 0x5c8

    void vf_0x348() { reinterpret_cast<void (*)(UnkProjectileMovement10_b46a3*)>(vtable[0x348 / 8])(this); }
    void* vf_0x2d8() { return reinterpret_cast<void* (*)(UnkProjectileMovement10_b46a3*)>(vtable[0x2d8 / 8])(this); }
};

void fn_7100ac4dc4(UnkProjectileMovement10_b46a3* self) {
    self->vf_0x348();
    void* timeline = self->owner->timeline;
    if (timeline && !self->entries_added) {
        fn_7100a2cddc(timeline, &g_unk_71727556b8[0], &self->id, false, 0.0f);
        fn_7100a2cddc(timeline, self->vf_0x2d8(), &self->id, false, 0.0f);
        fn_7100a2cddc(timeline, &g_unk_71727556b8[1], &self->id, false, 0.0f);
        self->entries_added = true;
    }
    if (self->definition->move_owner) {
        UnkStartOwner_b46a3* owner = self->owner;
        u32 x = self->pos_x, y = self->pos_y, z = self->pos_z;
        owner->x = x;
        owner->y = y;
        owner->z = z;
        owner->moved = true;
    }
}
