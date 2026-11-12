// CProjectileMovement unit: when the movement is active and the owner is farther from the player than a
// tunable distance, makes sure the owner's timeline knows a static key, adding it if not. Returns whether
// the check applied. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

using base::global::CStrId;

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

struct UnkFarKeyHead_lac6850 {
    u8 pad_0x0[0x30];
    u64 id;  // 0x30
};

struct UnkTimelineKey2_b46a3 {
    u8 data[0x18];  // the first word points at an UnkFarKeyHead_lac6850
};

struct UnkFarKey_lac6850 {
    UnkFarKeyHead_lac6850* head;
    u8 data[0x10];
};

struct UnkTunable;
struct UnkPlayer;

extern UnkTimelineKey2_b46a3 g_unk_71727556b8[2] __attribute__((visibility("hidden"), aligned(8)));
extern UnkTunable g_unk_71727bc558;
UnkPlayer* fn_7100dec788(UnkGameManager*);
void fn_7100a2cddc(void* table, void* key, CStrId* id, bool unique, f32 delay);

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
        UnkFarKeyHead_lac6850* head = reinterpret_cast<const UnkFarKey_lac6850*>(g_unk_71727556b8)->head;
        u64 id = head->id;
        if (!fn_7100a2be94(timeline, &id)) {
            fn_7100a2cddc(timeline, &g_unk_71727556b8[0], &self->id, true, 0.0f);
            return true;
        }
    }
    return true;
}
