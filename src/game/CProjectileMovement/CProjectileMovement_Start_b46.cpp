// CProjectileMovement unit (tools/tu_map.py unit 1623): after the virtual at slot 0x348, adds the
// movement's three timeline entries to the owner's timeline (+0x2e8) once (two static keys and the
// one the virtual at slot 0x2d8 returns, all with the movement's string id at +0x598), then, when
// the definition's flag (+0xec) is set, moves the owner to the stored position (+0x3b4) and marks
// it moved. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CAIManager/CTunableAIManager.h"
#include "game/CTimelineComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

using base::global::CStrId;

// Static timeline keys, 0x18 bytes each: a pointer to the key's record (its id at +0x30), then two words.
struct UnkTimelineKeyHead_b46a3 {
    u8 pad_0x0[0x30];
    u64 id;  // 0x30
};
struct UnkTimelineKey2_b46a3 {
    UnkTimelineKeyHead_b46a3* head;
    u8 data[0x10];
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
        fn_7100a2cddc(timeline, &g_unk_71727556b8[0], &self->id, 0.0f, false);
        fn_7100a2cddc(timeline, self->vf_0x2d8(), &self->id, 0.0f, false);
        fn_7100a2cddc(timeline, &g_unk_71727556b8[1], &self->id, 0.0f, false);
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

// ---- fn_7100ac6850: far from the player ----

// When the definition is active and the owner is farther from the player than the AI tunables' distance
// (+0x34), makes sure the owner's timeline knows the first static key, adding it if not. Returns whether
// the check applied.
struct UnkPlayerPos_lac6850 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
};

struct UnkFarOwner_lac6850 : UnkPlayerPos_lac6850 {
    u8 pad_0x68[0x280];
    UnkTimelineComponent* timeline;  // 0x2e8
};

struct UnkFarDefinition_lac6850 {
    u8 pad_0x0[0x20];
    bool active;  // 0x20
};

struct UnkFarMovement_lac6850 {
    u8 pad_0x0[0x30];
    UnkFarOwner_lac6850* owner;  // 0x30
    u8 pad_0x38[0x560];
    CStrId id;  // 0x598
};

struct UnkFarTunable_lac6850 {
    u8 pad_0x0[0x34];
    f32 distance;  // 0x34
};

struct Vec3_lac6850 {
    f32 x, y, z;
};

static inline Vec3_lac6850 Pos(const UnkPlayerPos_lac6850* p) {
    return {p->x, p->y, p->z};
}

static inline Vec3_lac6850 operator-(const Vec3_lac6850& a, const Vec3_lac6850& b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

bool fn_7100ac6850(UnkFarMovement_lac6850* self, UnkFarDefinition_lac6850* def) {
    if (!def->active)
        return false;
    UnkPlayerPos_lac6850* player = reinterpret_cast<UnkPlayerPos_lac6850*>(fn_7100dec788(g_unk_7101d47310));
    if (!player)
        return false;
    UnkFarOwner_lac6850* owner = self->owner;
    Vec3_lac6850 a = Pos(owner);
    Vec3_lac6850 b = Pos(player);
    f32 dx = a.x - b.x;
    f32 dy = a.y - b.y;
    f32 distSq = dx * dx + dy * dy;
    f32 dz = a.z - b.z;
    distSq += dz * dz;
    f32 limit = reinterpret_cast<const UnkFarTunable_lac6850*>(&g_unk_71727bc558)->distance;
    if (!(distSq > limit * limit))
        return false;
    UnkTimelineComponent* timeline = owner->timeline;
    if (timeline) {
        u64 id = g_unk_71727556b8[0].head->id;
        if (!fn_7100a2be94(timeline, &id)) {
            fn_7100a2cddc(timeline, &g_unk_71727556b8[0], &self->id, 0.0f, true);
            return true;
        }
    }
    return true;
}
